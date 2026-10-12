#include "scheduler.h"
#include <stdio.h>

void runSRTF(void) {
    if (processCount == 0) { printf("\nEnter processes first!\n"); return; }

    Metrics metrics[MAX_PROCESSES] = {0};
    Execution history[MAX_HISTORY];
    int remaining[MAX_PROCESSES], completedCount = 0, historyCount = 0, time = 0;

    for (int i = 0; i < processCount; ++i) {
        remaining[i] = processes[i].burst;
        metrics[i].response = -1;
    }

    while (completedCount < processCount) {
        int best = -1;
        for (int i = 0; i < processCount; ++i) {
            if (remaining[i] <= 0 || processes[i].arrival > time) continue;
            if (best == -1 ||
                remaining[i] < remaining[best] ||
                (remaining[i] == remaining[best] &&
                 processes[i].arrival < processes[best].arrival) ||
                (remaining[i] == remaining[best] &&
                 processes[i].arrival == processes[best].arrival &&
                 processes[i].pid < processes[best].pid)) best = i;
        }

        if (best == -1) {
            addExecution(history, &historyCount, 0, time, time + 1);
            time++;
            continue;
        }

        if (metrics[best].response == -1)
            metrics[best].response = time - processes[best].arrival;

        addExecution(history, &historyCount, processes[best].pid, time, time + 1);
        remaining[best]--;
        time++;

        if (remaining[best] == 0) {
            metrics[best].completion = time;
            metrics[best].turnaround = time - processes[best].arrival;
            metrics[best].waiting = metrics[best].turnaround - processes[best].burst;
            completedCount++;
        }
    }

    printf("\n======= SJF PREEMPTIVE (SRTF) =======\n");
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}
