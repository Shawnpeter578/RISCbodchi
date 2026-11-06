#include "orchestrator.h"

void print_header(const char *title) {
    printf("\n========================================\n");
    printf(" %s\n", title ? title : "Simulation Results");
    printf("========================================\n");
}

void print_sim_table(const SimResult *r) {
    if (r == NULL) {
        printf("Error: No simulation result available.\n");
        return;
    }
    print_header(r->name ? r->name : "Simulation");
    printf("Algorithm: %s\n", r->name ? r->name : "Unknown");
    printf("Page references: %d\n", r->total_refs);
    printf("Frames: %d\n\n", r->frames);
    printf("%-8s %-10s %-12s %-10s\n", "Step", "Page", "Frames", "Status");

    for (int i = 0; i < r->step_count; i++) {
        const SimStep *s = &r->steps[i];
        printf("%-8d %-10d ", i + 1, s->page);
        for (int j = 0; j < s->frame_count; j++) {
            if (s->frames[j] == EMPTY_FRAME) printf("- ");
            else printf("%d ", s->frames[j]);
        }
        printf("%*s", 12 - (s->frame_count * 2), "");
        printf("%-10s\n", s->status == STATUS_HIT ? "HIT" : "FAULT");
    }
}

void print_stats(const SimResult *r) {
    double hit_rate = 0.0, fault_rate = 0.0;
    if (r == NULL) {
        printf("Error: No statistics available.\n");
        return;
    }
    if (r->total_refs > 0) {
        hit_rate = 100.0 * r->hits / r->total_refs;
        fault_rate = 100.0 * r->faults / r->total_refs;
    }
    print_header("Simulation Statistics");
    printf("Algorithm    : %s\n", r->name ? r->name : "Unknown");
    printf("Total refs   : %d\n", r->total_refs);
    printf("Page hits    : %d\n", r->hits);
    printf("Page faults  : %d\n", r->faults);
    printf("Hit rate     : %.2f%%\n", hit_rate);
    printf("Fault rate   : %.2f%%\n", fault_rate);
}

void print_comparison_table(const SimResult results[], int count) {
    if (results == NULL || count <= 0) {
        printf("No results available for comparison.\n");
        return;
    }
    print_header("Algorithm Comparison");
    printf("%-15s %10s %10s %12s %12s\n", "Algorithm", "Frames", "References", "Hits", "Faults");
    for (int i = 0; i < count; i++) {
        printf("%-15s %10d %10d %12d %12d\n",
               results[i].name ? results[i].name : "Unknown",
               results[i].frames, results[i].total_refs,
               results[i].hits, results[i].faults);
    }
}

void result_init(SimResult *out, const char *name, AlgorithmChoice algo, const AlgoInput *in) {
    if (out == NULL) return;
    memset(out, 0, sizeof(*out));
    out->name = name; out->algo = algo;
    if (in == NULL) return;
    out->frames = in->frame_count; out->total_refs = in->ref_count;
}