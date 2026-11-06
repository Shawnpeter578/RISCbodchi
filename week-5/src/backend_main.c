#include "orchestrator.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int g_stop = 0;

static void on_signal(int sig) {
    (void)sig;
    g_stop = 1;
}

int main(int argc, char **argv) {
    const char *path = NULL;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [socket-path]\n", argv[0]);
        return 2;
    }
    if (argc == 2) path = argv[1];
    if (!path) path = getenv("PAGE_SIM_SOCKET");
    if (!path) path = "./page_sim.sock";

    signal(SIGINT,  on_signal);
    signal(SIGTERM, on_signal);
    signal(SIGPIPE, SIG_IGN);   /* a closed client must not kill us */

    if (ipc_server_init(path) != IPC_OK) {
        fprintf(stderr, "[backend] failed to init socket at %s\n", path);
        return 1;
    }

    IpcStatus st = ipc_server_run();
    ipc_server_shutdown();

    if (st == IPC_OK) return 0;
    return g_stop ? 0 : 1;
}