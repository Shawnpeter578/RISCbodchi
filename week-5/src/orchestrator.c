#include "orchestrator.h"

int validate_input(const SimInput *in) {
    if (!in || in->ref_count < 1 || in->ref_count > MAX_REFERENCES || 
        in->frame_count < 1 || in->frame_count > MAX_FRAMES ||
        (in->choice < ALGO_FIFO || in->choice > ALGO_ALL)) return 0;
    for (int i = 0; i < in->ref_count; i++) if (in->refs[i] < 0) return 0;
    return 1;
}

void run_simulation(const SimInput *in, SimResult results[], int *result_count) {
    if (!result_count) return;
    *result_count = 0;
    if (!in || !results || !validate_input(in)) return;

    AlgoInput algo_in = { .ref_count = in->ref_count, .frame_count = in->frame_count };
    memcpy(algo_in.refs, in->refs, sizeof(int) * in->ref_count);

    if (in->choice == ALGO_ALL) {
        simulate_fifo(&algo_in, &results[0]);
        simulate_lru(&algo_in, &results[1]);
        simulate_optimal(&algo_in, &results[2]);
        *result_count = 3;
    } else {
        if (in->choice == ALGO_FIFO) simulate_fifo(&algo_in, &results[0]);
        else if (in->choice == ALGO_LRU) simulate_lru(&algo_in, &results[0]);
        else if (in->choice == ALGO_OPT) simulate_optimal(&algo_in, &results[0]);
        *result_count = 1;
    }
}

int find_best_algorithm(const SimResult results[], int result_count) {
    if (!results || result_count <= 0) return -1;
    int best = -1;
    for (int i = 0; i < result_count; i++) {
        if (results[i].faults < 0) continue;
        if (best == -1 || results[i].faults < results[best].faults) best = i;
    }
    return best;
}
