#include "common.h"

int detect_deadlock(int* deadlocked_procs) {
    int work[MAX_R];
    int finish[MAX_P];
    for(int i = 0; i < n_r; i++) work[i] = available[i];
    
    for(int i = 0; i < n_p; i++) {
        finish[i] = finished[i];
        int is_alloc = 0;
        for(int j = 0; j < n_r; j++) if(alloc_m[i][j] > 0) is_alloc = 1;
        if(!is_alloc && !finished[i]) finish[i] = 1; 
    }

    bool changed = true;
    while(changed) {
        changed = false;
        for(int p = 0; p < n_p; p++) {
            if(!finish[p]) {
                bool can_satisfy = true;
                for(int j = 0; j < n_r; j++) {
                    if(need_m[p][j] > work[j]) { can_satisfy = false; break; }
                }
                if(can_satisfy) {
                    for(int j = 0; j < n_r; j++) work[j] += alloc_m[p][j];
                    finish[p] = 1;
                    changed = true;
                }
            }
        }
    }

    int d_count = 0;
    for(int p = 0; p < n_p; p++) {
        if(!finish[p]) deadlocked_procs[d_count++] = p;
    }
    return d_count;
}

int recover_deadlock() {
    int d_procs[MAX_P];
    int count = detect_deadlock(d_procs);
    if(count == 0) return -1;

    int victim = d_procs[0];
    for(int i = 0; i < n_r; i++) {
        available[i] += alloc_m[victim][i];
        alloc_m[victim][i] = 0;
        need_m[victim][i] = 0;
    }
    finished[victim] = 1; 
    return victim;
}
