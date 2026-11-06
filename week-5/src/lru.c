#include "orchestrator.h"

void simulate_lru(const AlgoInput *in, SimResult *out) {
    if (!out || !in) return;
    memset(out, 0, sizeof(*out));
    out->name = "LRU"; out->algo = ALGO_LRU;
    out->frames = in->frame_count; out->total_refs = in->ref_count;

    int frames[MAX_FRAMES], last_used[MAX_FRAMES], hits = 0, faults = 0;
    for (int f = 0; f < out->frames; ++f) { frames[f] = EMPTY_FRAME; last_used[f] = -1; }

    for (int i = 0; i < in->ref_count; ++i) {
        SimStep *step = &out->steps[i];
        step->page = in->refs[i]; step->status = STATUS_FAULT; step->victim_frame = -1;

        int slot = -1;
        for (int f = 0; f < out->frames; ++f) if (frames[f] == step->page) { slot = f; break; }

        if (slot >= 0) {
            step->status = STATUS_HIT; last_used[slot] = i; ++hits;
        } else {
            ++faults;
            int target = -1;
            for (int f = 0; f < out->frames; ++f) if (frames[f] == EMPTY_FRAME) { target = f; break; }
            
            if (target == -1) {
                target = 0;
                for (int f = 1; f < out->frames; ++f) if (last_used[f] < last_used[target]) target = f;
                step->victim_frame = target;
            }
            frames[target] = step->page; last_used[target] = i;
        }
        step->frame_count = out->frames;
        for (int f = 0; f < out->frames; ++f) step->frames[f] = frames[f];
    }
    out->hits = hits; out->faults = faults; out->step_count = in->ref_count;
}
