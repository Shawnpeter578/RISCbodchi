#include "common.h"

int request_resources(int pid, int* req) {
    for(int i = 0; i < n_r; i++) {
        if(req[i] > need_m[pid][i]) return -1; // Exceeds need
        if(req[i] > available[i]) return -2;   // Exceeds available
    }
    
    // Tentative allocation
    for(int i = 0; i < n_r; i++) {
        available[i] -= req[i];
        alloc_m[pid][i] += req[i];
        need_m[pid][i] -= req[i];
    }
    
    int dummy_seq[MAX_P];
    if(run_bankers(dummy_seq)) return 1;

    // Rollback if unsafe
    for(int i = 0; i < n_r; i++) {
        available[i] += req[i];
        alloc_m[pid][i] -= req[i];
        need_m[pid][i] += req[i];
    }
    return 0; 
}
