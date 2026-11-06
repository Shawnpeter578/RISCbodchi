#include "orchestrator.h"

void simulate_optimal(const AlgoInput *in, SimResult *out) {
    if (!out || !in) return;
    memset(out, 0, sizeof(*out));
    out->name = "Optimal"; out->algo = ALGO_OPT;
    out->frames = in->frame_count; out->total_refs = in->ref_count;

    int frames[MAX_FRAMES], hits = 0, faults = 0;
    for (int i = 0; i < out->frames; i++) frames[i] = EMPTY_FRAME;

    for (int i = 0; i < in->ref_count; i++) {
        SimStep *step = &out->steps[i];
        step->page = in->refs[i]; step->status = STATUS_FAULT; step->victim_frame = -1;

        int slot = -1;
        for (int f = 0; f < out->frames; f++) if (frames[f] == step->page) { slot = f; break; }

        if (slot != -1) {
            step->status = STATUS_HIT; hits++;
        } else {
            faults++;
            int empty = -1;
            for (int f = 0; f < out->frames; f++) if (frames[f] == EMPTY_FRAME) { empty = f; break; }

            if (empty != -1) frames[empty] = step->page;
            else {
                int victim = 0, farthest = -1;
                for (int f = 0; f < out->frames; f++) {
                    int next_use = -1;
                    for (int j = i + 1; j < in->ref_count; j++) {
                        if (in->refs[j] == frames[f]) { next_use = j; break; }
                    }
                    if (next_use == -1) { victim = f; break; }
                    if (next_use > farthest) { farthest = next_use; victim = f; }
                }
                step->victim_frame = victim; frames[victim] = step->page;
            }
        }
        step->frame_count = out->frames;
        for (int f = 0; f < out->frames; f++) step->frames[f] = frames[f];
    }
    out->hits = hits; out->faults = faults; out->step_count = in->ref_count;
}
