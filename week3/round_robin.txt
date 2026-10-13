#include "scheduler.h"
#include <stdio.h>

void runRoundRobin(void) {
    if (processCount == 0) { printf("\nEnter processes first!\n"); return; }

    int quantum;
    if (!readInt("Enter time quantum (> 0): ", &quantum) || quantum <= 0) {
        printf("Invalid time quantum.\n");
        return;
    }

    Metrics metrics[MAX_PROCESSES] = {0};
    Execution history[MAX_HISTORY];
    int remaining[MAX_PROCESSES];
    int queue[MAX_PROCESSES + 1];
    int order[MAX_PROCESSES];
    int head = 0, tail = 0, queueCount = 0;
    int historyCount = 0, completedCount = 0, time = 0, nextProcess = 0;

    for (int i = 0; i < processCount; ++i) {
        remaining[i] = processes[i].burst;
        metrics[i].response = -1;
        order[i] = i;
    }

    for (int i = 1; i < processCount; ++i) {
        int key = order[i], j = i - 1;
        while (j >= 0 &&
               (processes[order[j]].arrival > processes[key].arrival ||
                (processes[order[j]].arrival == processes[key].arrival &&
                 processes[order[j]].pid > processes[key].pid))) {
            order[j + 1] = order[j];
            --j;
        }
        order[j + 1] = key;
    }

    while (completedCount < processCount) {
        while (nextProcess < processCount &&
               processes[order[nextProcess]].arrival <= time) {
            queue[tail] = order[nextProcess];
            tail = (tail + 1) % (MAX_PROCESSES + 1);
            queueCount++;
            nextProcess++;
        }

        if (queueCount == 0) {
            int nextArrival = processes[order[nextProcess]].arrival;
            addExecution(history, &historyCount, 0, time, nextArrival);
            time = nextArrival;
            continue;
        }

        int current = queue[head];
        head = (head + 1) % (MAX_PROCESSES + 1);
        queueCount--;

        if (metrics[current].response == -1)
            metrics[current].response = time - processes[current].arrival;

        int runTime = remaining[current] < quantum ? remaining[current] : quantum;
        int start = time;
        int finish = time + runTime;
        addExecution(history, &historyCount, processes[current].pid, start, finish);
        time = finish;
        remaining[current] -= runTime;

        while (nextProcess < processCount &&
               processes[order[nextProcess]].arrival <= time) {
            queue[tail] = order[nextProcess];
            tail = (tail + 1) % (MAX_PROCESSES + 1);
            queueCount++;
            nextProcess++;
        }

        if (remaining[current] > 0) {
            queue[tail] = current;
            tail = (tail + 1) % (MAX_PROCESSES + 1);
            queueCount++;
        } else {
            metrics[current].completion = time;
            metrics[current].turnaround = time - processes[current].arrival;
            metrics[current].waiting = metrics[current].turnaround - processes[current].burst;
            completedCount++;
        }
    }

    printf("\n========== ROUND ROBIN ==========\n");
    printf("Time Quantum: %d\n", quantum);
    printGanttChart(history, historyCount);
    printMetrics(metrics);
}
