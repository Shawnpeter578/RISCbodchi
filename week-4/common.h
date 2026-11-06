#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>

#define MAX_P 20
#define MAX_R 20

// Global state variables
extern int n_p;
extern int n_r;
extern int available[MAX_R];
extern int max_m[MAX_P][MAX_R];
extern int alloc_m[MAX_P][MAX_R];
extern int need_m[MAX_P][MAX_R];
extern int finished[MAX_P];

// state.c
void init_system(int p, int r);
void set_available(int* arr);
void set_process_data(int pid, int* max_arr, int* alloc_arr);
int get_need(int pid, int rid);
int get_alloc(int pid, int rid);
int get_max(int pid, int rid);
int get_avail(int rid);
int get_finished(int pid);

// bankers.c
int run_bankers(int* safe_seq);

// resource.c
int request_resources(int pid, int* req);

// deadlock.c
int detect_deadlock(int* deadlocked_procs);
int recover_deadlock();

#endif
