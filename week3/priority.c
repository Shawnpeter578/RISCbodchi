#include "scheduler.h"
#include <limits.h>
#include <stdio.h>

void runPriorityNP(void) {
    if (processCount == 0) { printf("\nEnter processes first!\n"); return; }

    Metrics metrics[MAX_PROCESSES] = {0};
    Execution history[MAX_HISTORY];
    int completed[MAX_PROCESSES] = {0};
    int historyCount = 0, completedCount = 0, time = 0;

    while (completedCount < processCount) {
        int best = -1;
        for (int i = 0; i < processCount; ++i) {
            if (completed[i] || processes[i].arrival > time) continue;
            if (best == -1 ||
                processes[i].priority < processes[best].priority ||
                (processes[i].priority == processes[best].priority &&
                 processes[i].arrival < processes[best].arrival) ||
                (processes[i].priority == processes[best].priority &&
                 processes[i].arrival == processes[best].arrival &&
                 processes[i].pid < processes[best].pid)) best = i;
        }

        if (best == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < processCount; ++i)
                if (!completed[i] && processes[i].arrival < nextArrival)
                    nextArrival = processes[i].arrival;
            addExecution(history, &historyCount, 0, time, nextArrival);
            time = nextArrival;
            continue;
        }

        int start = time;
        int finish = start + processes[best].burst;
        addExecution(history, &historyCount, processes[best].pid, start, finish);
        metrics[best].completion = finish;
        metrics[best].turnaround = finish - processes[best].arrival;
        metrics[best].waiting = metrics[best].turnaround - processes[best].burst;
        metrics[best].response = start - processes[best].arrival;
        time = finish;
        completed[best] = 1;
        completedCount++;
    }

    printf("\n=== PRIORITY (NON-PREEMPTIVE) ===\n");
    printf("Lower priority number = higher priority.\n");
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}

void runPriorityP(void) {
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
                processes[i].priority < processes[best].priority ||
                (processes[i].priority == processes[best].priority &&
                 processes[i].arrival < processes[best].arrival) ||
                (processes[i].priority == processes[best].priority &&
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

    printf("\n===== PRIORITY (PREEMPTIVE) =====\n");
    printf("Lower priority number = higher priority.\n");
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}
