#include "scheduler.h"
#include <limits.h>
#include <stdio.h>

void runFCFS(void) {
    if (processCount == 0) {
        printf("\nEnter processes first!\n");
        return;
    }

    Process sorted[MAX_PROCESSES];
    Metrics metrics[MAX_PROCESSES] = {0};
    Execution history[MAX_HISTORY];
    int historyCount = 0, time = 0;

    for (int i = 0; i < processCount; ++i)
        sorted[i] = processes[i];

    for (int i = 1; i < processCount; ++i) {
        Process key = sorted[i];
        int j = i - 1;

        while (j >= 0 &&
               (sorted[j].arrival > key.arrival ||
                (sorted[j].arrival == key.arrival &&
                 sorted[j].pid > key.pid))) {
            sorted[j + 1] = sorted[j];
            --j;
        }

        sorted[j + 1] = key;
    }

    for (int i = 0; i < processCount; ++i) {
        if (time < sorted[i].arrival) {
            addExecution(history, &historyCount, 0, time,
                         sorted[i].arrival);
            time = sorted[i].arrival;
        }

        int start = time;
        int finish = start + sorted[i].burst;

        addExecution(history, &historyCount, sorted[i].pid,
                     start, finish);

        int p = sorted[i].pid - 1;
        metrics[p].completion = finish;
        metrics[p].turnaround = finish - sorted[i].arrival;
        metrics[p].waiting = metrics[p].turnaround - sorted[i].burst;
        metrics[p].response = start - sorted[i].arrival;

        time = finish;
    }

    printf("\n========== FCFS SCHEDULING ==========\n");
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}

void runSJF(void) {
    if (processCount == 0) {
        printf("\nEnter processes first!\n");
        return;
    }

    Metrics metrics[MAX_PROCESSES] = {0};
    Execution history[MAX_HISTORY];
    int completed[MAX_PROCESSES] = {0};
    int historyCount = 0, time = 0, completedCount = 0;

    while (completedCount < processCount) {
        int best = -1;

        for (int i = 0; i < processCount; ++i) {
            if (completed[i] || processes[i].arrival > time)
                continue;

            if (best == -1 ||
                processes[i].burst < processes[best].burst ||
                (processes[i].burst == processes[best].burst &&
                 processes[i].arrival < processes[best].arrival) ||
                (processes[i].burst == processes[best].burst &&
                 processes[i].arrival == processes[best].arrival &&
                 processes[i].pid < processes[best].pid))
                best = i;
        }

        if (best == -1) {
            int nextArrival = INT_MAX;

            for (int i = 0; i < processCount; ++i) {
                if (!completed[i] &&
                    processes[i].arrival < nextArrival)
                    nextArrival = processes[i].arrival;
            }

            addExecution(history, &historyCount, 0, time, nextArrival);
            time = nextArrival;
            continue;
        }

        int start = time;
        int finish = start + processes[best].burst;

        addExecution(history, &historyCount, processes[best].pid,
                     start, finish);

        metrics[best].completion = finish;
        metrics[best].turnaround = finish - processes[best].arrival;
        metrics[best].waiting =
            metrics[best].turnaround - processes[best].burst;
        metrics[best].response = start - processes[best].arrival;

        time = finish;
        completed[best] = 1;
        completedCount++;
    }

    printf("\n======= SJF NON-PREEMPTIVE =======\n");
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}