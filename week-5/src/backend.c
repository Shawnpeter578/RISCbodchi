#include "orchestrator.h"

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define WEAK __attribute__((weak))

WEAK int read_input(SimInput *in) {
    (void)in;
    fprintf(stderr, "[stub] read_input(): core not implemented.\n");
    return 0;
}

WEAK int validate_input(const SimInput *in) {
    if (!in) return 0;
    if (in->ref_count < 1 || in->ref_count > MAX_REFERENCES) return 0;
    if (in->frame_count < 1 || in->frame_count > MAX_FRAMES)   return 0;
    if (in->choice < ALGO_FIFO || in->choice > ALGO_ALL)       return 0;
    for (int i = 0; i < in->ref_count; ++i)
        if (in->refs[i] < 0) return 0;
    return 1;
}

WEAK void run_simulation(const SimInput *in, SimResult results[],
                         int *result_count) {
    (void)in;
    if (results)      memset(results, 0, sizeof(SimResult) * 3);
    if (result_count) *result_count = 0;
    fprintf(stderr, "[stub] run_simulation(): core not implemented.\n");
}

WEAK int find_best_algorithm(const SimResult results[], int result_count) {
    (void)results; (void)result_count;
    return -1;
}

WEAK void simulate_fifo(const AlgoInput *in, SimResult *out) {
    result_init(out, "FIFO", ALGO_FIFO, in); out->faults = -1;
}
WEAK void simulate_lru(const AlgoInput *in, SimResult *out) {
    result_init(out, "LRU", ALGO_LRU, in); out->faults = -1;
}
WEAK void simulate_optimal(const AlgoInput *in, SimResult *out) {
    result_init(out, "Optimal", ALGO_OPT, in); out->faults = -1;
}

WEAK void print_header(const char *t)            { (void)t; }
WEAK void print_sim_table(const SimResult *r)    { (void)r; }
WEAK void print_stats(const SimResult *r)        { (void)r; }
WEAK void print_comparison_table(const SimResult r[], int n) { (void)r; (void)n; }
WEAK void print_comparison(const SimResult r[], int n)       { (void)r; (void)n; }

WEAK void result_init(SimResult *out, const char *name,
                      AlgorithmChoice algo, const AlgoInput *in) {
    if (!out) return;
    memset(out, 0, sizeof(*out));
    out->name       = name;
    out->algo       = algo;
    out->frames     = in ? in->frame_count : 0;
    out->total_refs = in ? in->ref_count   : 0;
    out->step_count = 0;
}

typedef struct {
    char  *buf;
    size_t cap;
    size_t len;
    int    overflow;
} Obuf;

static void ob_init(Obuf *o, char *buf, size_t cap) {
    o->buf = buf; o->cap = cap; o->len = 0; o->overflow = 0;
    if (cap) buf[0] = '\0';
}

static void ob_putc(Obuf *o, char c) {
    if (o->overflow) return;
    if (o->len + 1 >= o->cap) { o->overflow = 1; return; }
    o->buf[o->len++] = c;
    o->buf[o->len] = '\0';
}

static void ob_puts(Obuf *o, const char *s) {
    while (*s) ob_putc(o, *s++);
}

static void ob_printf(Obuf *o, const char *fmt, ...) {
    if (o->overflow) return;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(o->buf + o->len, o->cap - o->len, fmt, ap);
    va_end(ap);
    if (n < 0) { o->overflow = 1; return; }
    if ((size_t)n >= o->cap - o->len) {
        o->overflow = 1;
        o->len = o->cap - 1;
        o->buf[o->len] = '\0';
        return;
    }
    o->len += (size_t)n;
}

static void ob_json_string(Obuf *o, const char *s) {
    if (!s) { ob_puts(o, "null"); return; }
    ob_putc(o, '"');
    for (; *s; ++s) {
        unsigned char c = (unsigned char)*s;
        switch (c) {
        case '"':  ob_puts(o, "\\\""); break;
        case '\\': ob_puts(o, "\\\\"); break;
        case '\n': ob_puts(o, "\\n");  break;
        case '\r': ob_puts(o, "\\r");  break;
        case '\t': ob_puts(o, "\\t");  break;
        case '\b': ob_puts(o, "\\b");  break;
        case '\f': ob_puts(o, "\\f");  break;
        default:
            if (c < 0x20) ob_printf(o, "\\u%04x", c);
            else          ob_putc(o, (char)c);
        }
    }
    ob_putc(o, '"');
}

typedef struct {
    const char *p;
    const char *end;
} Scanner;

static void sc_init(Scanner *s, const char *line, size_t len) {
    s->p = line;
    s->end = line + len;
}

static void sc_skip_ws(Scanner *s) {
    while (s->p < s->end &&
           (*s->p == ' ' || *s->p == '\t' || *s->p == '\r' || *s->p == '\n'))
        ++s->p;
}

static int sc_peek(const Scanner *s) {
    return s->p < s->end ? (unsigned char)*s->p : -1;
}

static int sc_eat(Scanner *s, char c) {
    sc_skip_ws(s);
    if (s->p < s->end && *s->p == c) { ++s->p; return 1; }
    return 0;
}

static int sc_string(Scanner *s, char *out, size_t cap) {
    sc_skip_ws(s);
    if (s->p >= s->end || *s->p != '"') return 0;
    ++s->p;
    size_t n = 0;
    while (s->p < s->end) {
        char c = *s->p++;
        if (c == '"') { if (cap) out[n < cap ? n : cap - 1] = '\0'; return 1; }
        if (c != '\\') {
            if (n + 1 < cap) out[n] = c;
            ++n;
            continue;
        }
        if (s->p >= s->end) return 0;
        char e = *s->p++;
        switch (e) {
        case '"':  if (n + 1 < cap) out[n] = '"';  ++n; break;
        case '\\': if (n + 1 < cap) out[n] = '\\'; ++n; break;
        case '/':  if (n + 1 < cap) out[n] = '/';  ++n; break;
        case 'b':  if (n + 1 < cap) out[n] = '\b'; ++n; break;
        case 'f':  if (n + 1 < cap) out[n] = '\f'; ++n; break;
        case 'n':  if (n + 1 < cap) out[n] = '\n'; ++n; break;
        case 'r':  if (n + 1 < cap) out[n] = '\r'; ++n; break;
        case 't':  if (n + 1 < cap) out[n] = '\t'; ++n; break;
        case 'u': {
            if (s->end - s->p < 4) return 0;
            int v = 0;
            for (int i = 0; i < 4; ++i) {
                char h = s->p[i];
                int d = (h >= '0' && h <= '9') ? h - '0'
                      : (h >= 'a' && h <= 'f') ? h - 'a' + 10
                      : (h >= 'A' && h <= 'F') ? h - 'A' + 10 : -1;
                if (d < 0) return 0;
                v = (v << 4) | d;
            }
            s->p += 4;
            if (v > 0x7F) return 0;
            if (n + 1 < cap) out[n] = (char)v;
            ++n;
            break;
        }
        default: return 0;
        }
    }
    return 0;
}

static int sc_long(Scanner *s, long *out) {
    sc_skip_ws(s);
    char *end = NULL;
    errno = 0;
    long v = strtol(s->p, &end, 10);
    if (end == s->p || errno == ERANGE) return 0;
    s->p = end;
    *out = v;
    return 1;
}

static int sc_int_array(Scanner *s, int *out, int cap) {
    if (!sc_eat(s, '[')) return -1;
    sc_skip_ws(s);
    if (sc_eat(s, ']')) return 0;
    int n = 0;
    for (;;) {
        long v;
        if (!sc_long(s, &v)) return -1;
        if (n >= cap) return -1;
        out[n++] = (int)v;
        sc_skip_ws(s);
        if (sc_eat(s, ']')) return n;
        if (!sc_eat(s, ',')) return -1;
    }
}

static int sc_skip_value(Scanner *s) {
    sc_skip_ws(s);
    int c = sc_peek(s);
    if (c < 0) return 0;
    if (c == '"') {
        char tmp[2];
        return sc_string(s, tmp, sizeof tmp) || 1;
    }
    if (c == '[' || c == '{') {
        char open = (char)c, close = (open == '[') ? ']' : '}';
        ++s->p;
        int depth = 1;
        while (s->p < s->end && depth) {
            char ch = *s->p;
            if (ch == '"') {
                char tmp[2];
                if (!sc_string(s, tmp, sizeof tmp)) return 0;
                continue;
            }
            if (ch == open)  ++depth;
            if (ch == close) --depth;
            ++s->p;
        }
        return depth == 0;
    }
    while (s->p < s->end && *s->p != ',' && *s->p != ']' && *s->p != '}')
        ++s->p;
    return 1;
}

IpcStatus ipc_parse_request(const char *line, size_t len, IpcRequest *req) {
    if (!line || !req) return IPC_ERR_PARSE;
    memset(req, 0, sizeof(*req));

    Scanner s;
    sc_init(&s, line, len);

    if (!sc_eat(&s, '{')) return IPC_ERR_PARSE;

    int have_cmd = 0;
    char cmd[16] = {0};

    for (;;) {
        sc_skip_ws(&s);
        if (sc_eat(&s, '}')) break;

        char key[32];
        if (!sc_string(&s, key, sizeof key)) return IPC_ERR_PARSE;
        if (!sc_eat(&s, ':')) return IPC_ERR_PARSE;

        if (strcmp(key, "cmd") == 0) {
            if (!sc_string(&s, cmd, sizeof cmd)) return IPC_ERR_PARSE;
            have_cmd = 1;
        } else if (strcmp(key, "refs") == 0) {
            int n = sc_int_array(&s, req->input.refs, MAX_REFERENCES);
            if (n < 0) return IPC_ERR_PARSE;
            req->input.ref_count = n;
        } else if (strcmp(key, "frames") == 0) {
            long v;
            if (!sc_long(&s, &v)) return IPC_ERR_PARSE;
            req->input.frame_count = (int)v;
        } else if (strcmp(key, "algo") == 0) {
            long v;
            if (!sc_long(&s, &v)) return IPC_ERR_PARSE;
            req->input.choice = (AlgorithmChoice)v;
        } else {
            if (!sc_skip_value(&s)) return IPC_ERR_PARSE;
        }

        sc_skip_ws(&s);
        if (sc_eat(&s, ',')) continue;
        if (sc_eat(&s, '}')) break;
        return IPC_ERR_PARSE;
    }

    if (!have_cmd) return IPC_ERR_PARSE;

    if (strcmp(cmd, "run") == 0)       { req->cmd = IPC_CMD_RUN;  return IPC_OK; }
    if (strcmp(cmd, "ping") == 0)      { req->cmd = IPC_CMD_PING; return IPC_OK; }
    if (strcmp(cmd, "quit") == 0)      { req->cmd = IPC_CMD_QUIT; return IPC_ERR_QUIT; }

    return IPC_ERR_PARSE;
}

void ipc_dispatch(const IpcRequest *req, IpcResponse *resp) {
    if (!resp) return;
    memset(resp, 0, sizeof(*resp));
    resp->status = IPC_OK;
    resp->protocol_version = IPC_PROTOCOL_VERSION;
    resp->best_index = -1;

    if (!req) {
        resp->status = IPC_ERR_INTERNAL;
        snprintf(resp->error, sizeof resp->error, "null request");
        return;
    }

    switch (req->cmd) {
    case IPC_CMD_PING:
        return;

    case IPC_CMD_QUIT:
        resp->status = IPC_ERR_QUIT;
        return;

    case IPC_CMD_RUN: {
        if (!validate_input(&req->input)) {
            resp->status = IPC_ERR_INPUT;
            snprintf(resp->error, sizeof resp->error,
                     "invalid input: refs=%d frames=%d algo=%d",
                     req->input.ref_count, req->input.frame_count,
                     (int)req->input.choice);
            return;
        }
        run_simulation(&req->input, resp->results, &resp->result_count);
        if (resp->result_count == 0) {
            resp->status = IPC_ERR_CORE;
            snprintf(resp->error, sizeof resp->error,
                     "simulation core not linked (stubs active)");
            return;
        }
        resp->best_index = find_best_algorithm(resp->results,
                                               resp->result_count);
        return;
    }

    default:
        resp->status = IPC_ERR_INTERNAL;
        snprintf(resp->error, sizeof resp->error,
                 "unknown command %d", (int)req->cmd);
    }
}

static void emit_result(Obuf *o, const SimResult *r, int is_best) {
    ob_puts(o, "{\"name\":");
    ob_json_string(o, r->name ? r->name : "");
    ob_printf(o, ",\"algo\":%d,\"frames\":%d,\"total_refs\":%d,"
                 "\"hits\":%d,\"faults\":%d,\"best\":%s,\"steps\":[",
              (int)r->algo, r->frames, r->total_refs,
              r->hits, r->faults, is_best ? "true" : "false");

    for (int i = 0; i < r->step_count; ++i) {
        const SimStep *s = &r->steps[i];
        if (i) ob_putc(o, ',');
        ob_printf(o, "{\"page\":%d,\"status\":\"%s\",\"victim\":%d,"
                     "\"frames\":[",
                  s->page,
                  s->status == STATUS_HIT ? "HIT" : "FAULT",
                  s->victim_frame);
        for (int f = 0; f < r->frames; ++f) {
            if (f) ob_putc(o, ',');
            if (s->frames[f] == EMPTY_FRAME) ob_puts(o, "null");
            else                             ob_printf(o, "%d", s->frames[f]);
        }
        ob_puts(o, "]}");
    }
    ob_puts(o, "]}");
}

size_t ipc_serialize_response(const IpcResponse *resp,
                              char *buf, size_t cap) {
    if (!resp || !buf || cap == 0) return 0;

    Obuf o;
    ob_init(&o, buf, cap);

    switch (resp->status) {
    case IPC_OK: {
        ob_printf(&o, "{\"ok\":true,\"v\":%d", IPC_PROTOCOL_VERSION);
        if (resp->result_count > 0) {
            ob_printf(&o, ",\"cmd\":\"run\",\"best\":%d,\"results\":[",
                      resp->best_index);
            for (int i = 0; i < resp->result_count; ++i) {
                if (i) ob_putc(&o, ',');
                emit_result(&o, &resp->results[i],
                            i == resp->best_index);
            }
            ob_puts(&o, "]");
        } else {
            ob_puts(&o, ",\"cmd\":\"ping\"");
        }
        ob_puts(&o, "}");
        break;
    }
    default: {
        ob_printf(&o, "{\"ok\":false,\"v\":%d,\"status\":%d,\"error\":",
                  IPC_PROTOCOL_VERSION, (int)resp->status);
        ob_json_string(&o, resp->error[0] ? resp->error : "unknown error");
        ob_puts(&o, "}");
    }
    }

    if (o.overflow) return 0;
    return o.len;
}

static int  g_listen_fd = -1;
static char g_socket_path[108];

IpcStatus ipc_server_init(const char *path) {
    if (!path || !*path) return IPC_ERR_INTERNAL;
    if (strlen(path) >= sizeof g_socket_path) return IPC_ERR_INTERNAL;
    strcpy(g_socket_path, path);

    if (unlink(path) != 0 && errno != ENOENT) {
        fprintf(stderr, "[backend] unlink(%s): %s\n", path, strerror(errno));
        return IPC_ERR_INTERNAL;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "[backend] socket(): %s\n", strerror(errno));
        return IPC_ERR_INTERNAL;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof addr);
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, path, strlen(path) + 1);

    if (bind(fd, (struct sockaddr *)&addr, sizeof addr) != 0) {
        fprintf(stderr, "[backend] bind(%s): %s\n", path, strerror(errno));
        close(fd);
        return IPC_ERR_INTERNAL;
    }
    if (listen(fd, 4) != 0) {
        fprintf(stderr, "[backend] listen(): %s\n", strerror(errno));
        close(fd);
        unlink(path);
        return IPC_ERR_INTERNAL;
    }

    g_listen_fd = fd;
    return IPC_OK;
}

static ssize_t read_line(int fd, char *buf, size_t cap) {
    size_t n = 0;
    for (;;) {
        if (n + 1 >= cap) {
            char sink[256];
            for (;;) {
                ssize_t r = read(fd, sink, sizeof sink);
                if (r <= 0) return -2;
                for (ssize_t i = 0; i < r; ++i)
                    if (sink[i] == '\n') return -2;
            }
        }
        char c;
        ssize_t r = read(fd, &c, 1);
        if (r == 0) return 0;
        if (r < 0) return (errno == EINTR) ? read_line(fd, buf, cap) : -1;
        if (c == '\n') { buf[n] = '\0'; return (ssize_t)n; }
        buf[n++] = c;
    }
}

static int write_all(int fd, const char *buf, size_t len) {
    size_t off = 0;
    while (off < len) {
        ssize_t w = write(fd, buf + off, len - off);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        off += (size_t)w;
    }
    return 0;
}

static IpcStatus serve_one(int fd) {
    char line[IPC_MAX_LINE_LEN];
    char out[IPC_MAX_LINE_LEN * 2];

    for (;;) {
        ssize_t n = read_line(fd, line, sizeof line);
        if (n == 0) {
            fprintf(stderr, "[backend] client disconnected\n");
            return IPC_ERR_INTERNAL;
        }
        if (n == -1) {
            fprintf(stderr, "[backend] read: %s\n", strerror(errno));
            return IPC_ERR_INTERNAL;
        }
        if (n == -2) {
            IpcResponse resp;
            memset(&resp, 0, sizeof resp);
            resp.status = IPC_ERR_PARSE;
            snprintf(resp.error, sizeof resp.error, "request line too long");
            size_t m = ipc_serialize_response(&resp, out, sizeof out);
            if (m) { write_all(fd, out, m); write_all(fd, "\n", 1); }
            continue;
        }

        IpcRequest req;
        IpcStatus st = ipc_parse_request(line, (size_t)n, &req);
        if (st == IPC_ERR_QUIT) {
            IpcResponse resp;
            memset(&resp, 0, sizeof resp);
            resp.status = IPC_OK;
            resp.protocol_version = IPC_PROTOCOL_VERSION;
            size_t m = ipc_serialize_response(&resp, out, sizeof out);
            if (m) { write_all(fd, out, m); write_all(fd, "\n", 1); }
            return IPC_OK;
        }
        if (st != IPC_OK) {
            IpcResponse resp;
            memset(&resp, 0, sizeof resp);
            resp.status = st;
            snprintf(resp.error, sizeof resp.error, "malformed request");
            size_t m = ipc_serialize_response(&resp, out, sizeof out);
            if (m) { write_all(fd, out, m); write_all(fd, "\n", 1); }
            continue;
        }

        IpcResponse resp;
        ipc_dispatch(&req, &resp);
        size_t m = ipc_serialize_response(&resp, out, sizeof out);
        if (m == 0) {
            IpcResponse err;
            memset(&err, 0, sizeof err);
            err.status = IPC_ERR_INTERNAL;
            snprintf(err.error, sizeof err.error,
                     "response exceeded %zu bytes", sizeof out);
            m = ipc_serialize_response(&err, out, sizeof out);
            if (m == 0) return IPC_ERR_INTERNAL;
        }
        if (write_all(fd, out, m) != 0 || write_all(fd, "\n", 1) != 0) {
            fprintf(stderr, "[backend] write: %s\n", strerror(errno));
            return IPC_ERR_INTERNAL;
        }
    }
}

IpcStatus ipc_server_run(void) {
    if (g_listen_fd < 0) return IPC_ERR_INTERNAL;

    fprintf(stderr, "[backend] listening on %s\n", g_socket_path);

    for (;;) {
        int fd = accept(g_listen_fd, NULL, NULL);
        if (fd < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "[backend] accept: %s\n", strerror(errno));
            return IPC_ERR_INTERNAL;
        }

        IpcStatus st = serve_one(fd);
        close(fd);

        if (st == IPC_OK) return IPC_OK;
    }
}

void ipc_server_shutdown(void) {
    if (g_listen_fd >= 0) {
        close(g_listen_fd);
        g_listen_fd = -1;
    }
    if (g_socket_path[0]) {
        unlink(g_socket_path);
        g_socket_path[0] = '\0';
    }
}