#include "scheduler.h"
#include <limits.h>
#include <stdio.h>

void compareAlgorithms(void) {
    if (processCount == 0) {
        printf("\nEnter processes first!\n");
        return;
    }

    int quantum;
    if (!readInt("Enter time quantum for Round Robin comparison (> 0): ",
                 &quantum) || quantum <= 0) {
        printf("Invalid time quantum. Comparison cancelled.\n");
        return;
    }

    /*
       The normal Round Robin function asks for a quantum itself.
       Temporarily redirect standard input is unnecessary: use the selected
       quantum through a global setting instead.
    */
    comparisonCount = 0;

    printf("\n========== COMPARING ALL ALGORITHMS ==========\n");
    currentAlgorithmName = "FCFS";
    runFCFS();

    currentAlgorithmName = "SJF Non-preemptive";
    runSJF();

    currentAlgorithmName = "SRTF";
    runSRTF();

    currentAlgorithmName = "Priority Non-preemptive";
    runPriorityNP();

    currentAlgorithmName = "Priority Preemptive";
    runPriorityP();

    /* Compute Round Robin in comparison mode using the already-read quantum. */
    {
        Metrics metrics[MAX_PROCESSES] = {0};
        Execution history[MAX_HISTORY];
        int remaining[MAX_PROCESSES], queue[MAX_PROCESSES + 1];
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
            int start = time, finish = time + runTime;
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

        currentAlgorithmName = "Round Robin";
        printf("\n========== ROUND ROBIN ==========\n");
        printf("Time Quantum: %d\n", quantum);
        printGanttChart(history, historyCount);
        printMetrics(metrics);
    }

    currentAlgorithmName = NULL;

    printf("\n================ COMPARISON SUMMARY ================\n");
    printf("%-25s %10s %10s %10s %12s\n",
           "Algorithm", "Avg WT", "Avg TAT", "Avg RT", "CPU Util %");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < comparisonCount; ++i) {
        printf("%-25s %10.2f %10.2f %10.2f %12.2f\n",
               comparisonResults[i].name,
               comparisonResults[i].avgWaiting,
               comparisonResults[i].avgTurnaround,
               comparisonResults[i].avgResponse,
               comparisonResults[i].cpuUtilization);
    }
    printf("--------------------------------------------------------------------------\n");
}
