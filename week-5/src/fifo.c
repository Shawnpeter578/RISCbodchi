#include "orchestrator.h"

void simulate_fifo(const AlgoInput *in, SimResult *out) {
    if (!out || !in) return;
    memset(out, 0, sizeof(*out));
    out->name = "FIFO"; out->algo = ALGO_FIFO;
    out->frames = in->frame_count; out->total_refs = in->ref_count;

    int frames[MAX_FRAMES], next_victim = 0, hits = 0, faults = 0;
    for (int f = 0; f < out->frames; ++f) frames[f] = EMPTY_FRAME;

    for (int i = 0; i < in->ref_count; ++i) {
        SimStep *s = &out->steps[i];
        s->page = in->refs[i]; s->status = STATUS_FAULT; s->victim_frame = -1;
        
        int slot = -1;
        for (int f = 0; f < out->frames; ++f) if (frames[f] == s->page) { slot = f; break; }

        if (slot >= 0) {
            s->status = STATUS_HIT; ++hits;
        } else {
            ++faults;
            int empty = -1;
            for (int f = 0; f < out->frames; ++f) if (frames[f] == EMPTY_FRAME) { empty = f; break; }
            if (empty >= 0) frames[empty] = s->page;
            else {
                s->victim_frame = next_victim; frames[next_victim] = s->page;
                next_victim = (next_victim + 1) % out->frames;
            }
        }
        s->frame_count = out->frames;
        for (int f = 0; f < out->frames; ++f) s->frames[f] = frames[f];
    }
    out->hits = hits; out->faults = faults; out->step_count = in->ref_count;
}
