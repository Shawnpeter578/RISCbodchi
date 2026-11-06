#include "common.h"

int n_p = 0;
int n_r = 0;
int available[MAX_R];
int max_m[MAX_P][MAX_R];
int alloc_m[MAX_P][MAX_R];
int need_m[MAX_P][MAX_R];
int finished[MAX_P]; 

void init_system(int p, int r) {
    n_p = p;
    n_r = r;
    for(int i = 0; i < MAX_P; i++) {
        finished[i] = 0;
        for(int j = 0; j < MAX_R; j++) {
            max_m[i][j] = 0; alloc_m[i][j] = 0; need_m[i][j] = 0;
        }
    }
}

void set_available(int* arr) {
    for(int i = 0; i < n_r; i++) available[i] = arr[i];
}

void set_process_data(int pid, int* max_arr, int* alloc_arr) {
    if(pid >= n_p) return;
    for(int i = 0; i < n_r; i++) {
        max_m[pid][i] = max_arr[i];
        alloc_m[pid][i] = alloc_arr[i];
        need_m[pid][i] = max_m[pid][i] - alloc_m[pid][i];
    }
}

int get_need(int pid, int rid) { return need_m[pid][rid]; }
int get_alloc(int pid, int rid) { return alloc_m[pid][rid]; }
int get_max(int pid, int rid) { return max_m[pid][rid]; }
int get_avail(int rid) { return available[rid]; }
int get_finished(int pid) { return finished[pid]; }
