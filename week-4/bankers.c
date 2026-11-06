#include "common.h"

int run_bankers(int* safe_seq) {
    int work[MAX_R];
    int finish[MAX_P];
    for(int i = 0; i < n_r; i++) work[i] = available[i];
    for(int i = 0; i < n_p; i++) finish[i] = finished[i];

    int count = 0;
    while(count < n_p) {
        bool found = false;
        for(int p = 0; p < n_p; p++) {
            if(finish[p] == 0) {
                int j;
                for(j = 0; j < n_r; j++) {
                    if(need_m[p][j] > work[j]) break;
                }
                if(j == n_r) { 
                    for(int k = 0; k < n_r; k++) work[k] += alloc_m[p][k];
                    safe_seq[count++] = p;
                    finish[p] = 1;
                    found = true;
                }
            }
        }
        if(!found) {
            int remaining = 0;
            for(int p = 0; p < n_p; p++) if(finish[p] == 0) remaining++;
            if(remaining == 0) return 1; 
            return 0; // Unsafe state
        }
    }
    return 1;
}
