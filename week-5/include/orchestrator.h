#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

#define MAX_FRAMES       64
#define MAX_REFERENCES   1024
#define EMPTY_FRAME      (-1)

typedef enum {
    ALGO_FIFO = 1,
    ALGO_LRU  = 2,
    ALGO_OPT  = 3,
    ALGO_ALL  = 4
} AlgorithmChoice;

typedef enum {
    STATUS_FAULT = 0,
    STATUS_HIT   = 1
} PageStatus;

typedef struct {
    int        page;
    int        frames[MAX_FRAMES];
    int        frame_count;
    PageStatus status;
    int        victim_frame;
} SimStep;

typedef struct {
    const char *name;
    AlgorithmChoice algo;
    int  frames;
    int  total_refs;
    int  hits;
    int  faults;
    SimStep steps[MAX_REFERENCES];
    int  step_count;
} SimResult;

typedef struct {
    int            refs[MAX_REFERENCES];
    int            ref_count;
    int            frame_count;
    AlgorithmChoice choice;
} SimInput;

typedef struct {
    int refs[MAX_REFERENCES];
    int ref_count;
    int frame_count;
} AlgoInput;

typedef void (*PageReplacementFn)(const AlgoInput *in, SimResult *out);

#define IPC_PROTOCOL_VERSION 1
#define IPC_MAX_ERROR_LEN    256
#define IPC_MAX_LINE_LEN     65536

typedef enum {
    IPC_OK           =  0,
    IPC_ERR_PARSE    = -1,
    IPC_ERR_INPUT    = -2,
    IPC_ERR_CORE     = -3,
    IPC_ERR_INTERNAL = -4,
    IPC_ERR_QUIT     = -5
} IpcStatus;

typedef enum {
    IPC_CMD_RUN  = 1,
    IPC_CMD_PING = 2,
    IPC_CMD_QUIT = 3
} IpcCommand;

typedef struct {
    IpcCommand cmd;
    SimInput   input;
} IpcRequest;

typedef struct {
    IpcStatus  status;
    int        protocol_version;
    int        result_count;
    int        best_index;
    SimResult  results[3];
    char       error[IPC_MAX_ERROR_LEN];
} IpcResponse;

IpcStatus ipc_server_init(const char *socket_path);
IpcStatus ipc_server_run(void);
void ipc_server_shutdown(void);
IpcStatus ipc_parse_request(const char *line, size_t len, IpcRequest *req);
void ipc_dispatch(const IpcRequest *req, IpcResponse *resp);
size_t ipc_serialize_response(const IpcResponse *resp, char *buf, size_t cap);

int  read_input(SimInput *in);
int  validate_input(const SimInput *in);
void run_simulation(const SimInput *in, SimResult results[], int *result_count);
void print_comparison(const SimResult results[], int result_count);
int  find_best_algorithm(const SimResult results[], int result_count);

void simulate_fifo(const AlgoInput *in, SimResult *out);
void simulate_lru (const AlgoInput *in, SimResult *out);
void simulate_optimal(const AlgoInput *in, SimResult *out);

void print_header(const char *title);
void print_sim_table(const SimResult *r);
void print_stats(const SimResult *r);
void print_comparison_table(const SimResult results[], int count);

void result_init(SimResult *out, const char *name, AlgorithmChoice algo, const AlgoInput *in);

#endif /* ORCHESTRATOR_H */