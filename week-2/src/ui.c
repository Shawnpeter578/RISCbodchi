#include <ncurses.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define REQUEST_QUEUE  "/riscbodchi_request"
#define RESPONSE_QUEUE "/riscbodchi_response"
#define LOG_QUEUE      "/riscbodchi_log"

#define MSG    256
#define N      100
#define KEEP   200

static int stack[N], sp = 0;
static int cq[N], qf = 0, qr = -1;
static int mem[N], used[N];

typedef struct { char line[KEEP][MSG]; int n; } Buf;
static Buf logbuf, respbuf;

static volatile sig_atomic_t quit_flag = 0;
static void on_int(int s) { (void)s; quit_flag = 1; }

static void buf_add(Buf *b, const char *s) {
    if (b->n == KEEP) {
        memmove(b->line[0], b->line[1], (KEEP - 1) * MSG);
        b->n--;
    }
    snprintf(b->line[b->n++], MSG, "%s", s);
}

static void apply_log(const char *l) {
    int a, b;
    if (sscanf(l, "PUSH %d", &a) == 1)              { if (sp < N) stack[sp++] = a; }
    else if (sscanf(l, "POP -> %d", &a) == 1)       { if (sp > 0) sp--; }
    else if (sscanf(l, "ENQUEUE %d", &a) == 1)      { if (qr < N - 1) cq[++qr] = a; }
    else if (sscanf(l, "DEQUEUE -> %d", &a) == 1)   { if (qf <= qr) qf++; }
    else if (sscanf(l, "STORE memory[%d] = %d", &a, &b) == 2)
        { if (a >= 0 && a < N) { mem[a] = b; used[a] = 1; } }
}

static WINDOW *panel(int y, int x, int h, int w, const char *title) {
    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 2, " %s ", title);
    wattroff(win, A_BOLD);
    return win;
}

static void draw_text(WINDOW *win, Buf *b) {
    int h, w, start, row = 1;
    getmaxyx(win, h, w);
    start = b->n - (h - 2);
    if (start < 0) start = 0;
    for (int i = start; i < b->n; i++)
        mvwprintw(win, row++, 1, "%.*s", w - 2, b->line[i]);
}

static void draw(const char *input, int core_alive) {
    int sh = (LINES - 2) / 2, bh = LINES - 2 - sh;
    int cw = COLS / 3, cw3 = COLS - 2 * cw, lw = COLS / 2;
    WINDOW *win;
    int h, w, row;

    erase();
    attron(A_REVERSE);
    mvprintw(0, 0, " RISC-BODCHI CORE | core: %-8s | help = commands | quit = exit ",
             core_alive ? "running" : "stopped");
    clrtoeol();
    attroff(A_REVERSE);
    refresh();

    win = panel(1, 0, sh, cw, "STACK (top first)");
    getmaxyx(win, h, w); row = 1;
    for (int i = sp - 1; i >= 0 && row < h - 1; i--)
        mvwprintw(win, row++, 1, "%3d | %d", i, stack[i]);
    mvwprintw(win, 0, cw - 12, " %d/%d ", sp, N);
    wrefresh(win); delwin(win);

    win = panel(1, cw, sh, cw, "QUEUE (front first)");
    getmaxyx(win, h, w); row = 1;
    for (int i = qf; i <= qr && row < h - 1; i++)
        mvwprintw(win, row++, 1, "%3d | %d", i - qf, cq[i]);
    mvwprintw(win, 0, cw - 12, " %d/%d ", qr - qf + 1, N);
    wrefresh(win); delwin(win);

    win = panel(1, 2 * cw, sh, cw3, "MEMORY (written cells)");
    getmaxyx(win, h, w); row = 1;
    for (int i = 0; i < N && row < h - 1; i++)
        if (used[i]) mvwprintw(win, row++, 1, "[%2d] = %d", i, mem[i]);
    wrefresh(win); delwin(win);

    win = panel(1 + sh, 0, bh, lw, "LOG");
    draw_text(win, &logbuf);
    wrefresh(win); delwin(win);

    win = panel(1 + sh, lw, bh, COLS - lw, "COMMANDS / RESPONSES");
    draw_text(win, &respbuf);
    wrefresh(win); delwin(win);

    mvprintw(LINES - 1, 0, "risc> %s", input);
    move(LINES - 1, 6 + (int)strlen(input));
    refresh();
}

static void drain(mqd_t q, Buf *b, int is_log) {
    char buf[MSG + 1];
    ssize_t n;
    while ((n = mq_receive(q, buf, MSG, NULL)) >= 0) {
        buf[n] = '\0';
        buf_add(b, buf);
        if (is_log) apply_log(buf);
    }
}

static mqd_t open_retry(const char *name, int flags) {
    for (int i = 0; i < 50; i++) {
        mqd_t q = mq_open(name, flags);
        if (q != (mqd_t)-1) return q;
        usleep(100000);
    }
    return (mqd_t)-1;
}

static void show_help(void) {
    buf_add(&respbuf, "ADD a b | SUBTRACT a b");
    buf_add(&respbuf, "PUSH v | POP");
    buf_add(&respbuf, "ENQUEUE v | DEQUEUE");
    buf_add(&respbuf, "STORE addr v | LOAD addr");
    buf_add(&respbuf, "quit  (shuts the core down)");
}

int main(int argc, char **argv) {
    const char *core_path = argc > 1 ? argv[1] : "./core";
    struct mq_attr attr = { .mq_flags = 0, .mq_maxmsg = 10,
                            .mq_msgsize = MSG, .mq_curmsgs = 0 };

    mq_unlink(REQUEST_QUEUE);
    mq_unlink(RESPONSE_QUEUE);
    mq_unlink(LOG_QUEUE);

    mqd_t log_q = mq_open(LOG_QUEUE, O_CREAT | O_RDONLY | O_NONBLOCK, 0666, &attr);
    if (log_q == (mqd_t)-1) { perror("UI: cannot create log queue"); return 1; }

    pid_t core_pid = fork();
    if (core_pid < 0) { perror("fork"); return 1; }
    if (core_pid == 0) {
        int dn = open("/dev/null", O_WRONLY);
        dup2(dn, STDOUT_FILENO);
        dup2(dn, STDERR_FILENO);
        execl(core_path, core_path, (char *)NULL);
        _exit(127);
    }

    mqd_t req_q  = open_retry(REQUEST_QUEUE,  O_WRONLY | O_NONBLOCK);
    mqd_t resp_q = open_retry(RESPONSE_QUEUE, O_RDONLY | O_NONBLOCK);
    if (req_q == (mqd_t)-1 || resp_q == (mqd_t)-1) {
        fprintf(stderr, "UI: could not connect to core (%s)\n", core_path);
        kill(core_pid, SIGTERM);
        mq_unlink(LOG_QUEUE);
        return 1;
    }

    signal(SIGINT, on_int);
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE); timeout(100);

    char input[MSG] = "";
    int len = 0, core_alive = 1;
    buf_add(&respbuf, "RISC-bodchi UI loaded. Type 'help' for commands.");

    while (!quit_flag) {
        drain(log_q, &logbuf, 1);
        drain(resp_q, &respbuf, 0);
        if (core_alive && waitpid(core_pid, NULL, WNOHANG) == core_pid) {
            core_alive = 0;
            buf_add(&respbuf, "** core process has stopped **");
        }
        draw(input, core_alive);

        int ch = getch();
        if (ch == ERR || ch == KEY_RESIZE) continue;

        if (ch == '\n' || ch == KEY_ENTER) {
            if (len == 0) continue;
            char cmd[MSG];
            snprintf(cmd, sizeof cmd, "%s", input);
            input[0] = '\0'; len = 0;

            if (strcasecmp(cmd, "help") == 0) { show_help(); continue; }
            if (strcasecmp(cmd, "quit") == 0 || strcasecmp(cmd, "exit") == 0) {
                strcpy(cmd, "exit");
                quit_flag = 1;
            } else {
                for (char *p = cmd; *p; p++) *p = toupper((unsigned char)*p);
            }

            char echo[MSG + 4];
            snprintf(echo, sizeof echo, "> %s", cmd);
            buf_add(&respbuf, echo);

            if (!core_alive)
                buf_add(&respbuf, "ERROR: core is not running");
            else if (mq_send(req_q, cmd, strlen(cmd) + 1, 0) == -1)
                buf_add(&respbuf, errno == EAGAIN ? "ERROR: request queue full"
                                                  : "ERROR: send failed");
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (len > 0) input[--len] = '\0';
        } else if (isprint(ch) && len < 120) {
            input[len++] = (char)ch;
            input[len] = '\0';
        }
    }

    if (core_alive) {
        mq_send(req_q, "exit", 5, 0);
        for (int i = 0; i < 20; i++) {
            if (waitpid(core_pid, NULL, WNOHANG) == core_pid) break;
            usleep(100000);
        }
    }

    endwin();
    mq_close(req_q); mq_close(resp_q); mq_close(log_q);
    mq_unlink(LOG_QUEUE);
    return 0;
}
