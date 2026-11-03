#ifndef W4_IPC_H
#define W4_IPC_H
#include "message.h"

typedef enum {
    IPC_POSIX_MQ=1, IPC_PIPE, IPC_SHM, IPC_SOCKET, IPC_FIFO
} IPCType;

typedef struct {
    int (*ui_to_core_send)(const Message *);
    int (*ui_to_core_recv)(Message *);
    int (*core_to_ui_send)(const Message *);
    int (*core_to_ui_recv)(Message *);
    int (*core_to_log_send)(const Message *);
    int (*logger_recv)(Message *);
    void (*close_all)(void);
} IPCOps;

int ipc_init(IPCType type, IPCOps *ops);
const char *ipc_name(IPCType type);
int ipc_parse(const char *s);
#endif
