#include "scheduler.h"
#include <stdio.h>

void addExecution(Execution history[], int *count,
                         int pid, int start, int finish) {
    if (finish <= start) return;

    if (*count > 0 &&
        history[*count - 1].pid == pid &&
        history[*count - 1].finish == start) {
        history[*count - 1].finish = finish;
        return;
    }

    if (*count < MAX_HISTORY) {
        history[*count].pid = pid;
        history[*count].start = start;
        history[*count].finish = finish;
        (*count)++;
    }
}

void printGanttChart(const Execution history[], int count) {
    if (count <= 0) {
        printf("No execution history available.\n");
        return;
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < count; ++i) {
        if (history[i].pid == 0) printf("| IDLE ");
        else printf("| P%d ", history[i].pid);
    }
    printf("|\n");

    printf("%d", history[0].start);
    for (int i = 0; i < count; ++i) printf("    %d", history[i].finish);
    printf("\n");
}

void printMetrics(const Metrics metrics[]) {
    double totalWT = 0.0, totalTAT = 0.0, totalRT = 0.0;
    long long busyTime = 0;
    int endTime = 0;

    printf("\n%-10s %-10s %-10s %-10s %-10s\n",
           "Process", "CT", "TAT", "WT", "RT");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < processCount; ++i) {
        printf("P%-9d %-10d %-10d %-10d %-10d\n",
               processes[i].pid, metrics[i].completion,
               metrics[i].turnaround, metrics[i].waiting,
               metrics[i].response);

        totalWT += metrics[i].waiting;
        totalTAT += metrics[i].turnaround;
        totalRT += metrics[i].response;
        busyTime += processes[i].burst;
        if (metrics[i].completion > endTime) endTime = metrics[i].completion;
    }

    printf("\nAverage Waiting Time    : %.2f\n", totalWT / processCount);
    printf("Average Turnaround Time : %.2f\n", totalTAT / processCount);
    printf("Average Response Time   : %.2f\n", totalRT / processCount);

    double utilization = endTime > 0 ? 100.0 * (double)busyTime / endTime : 0.0;
    printf("CPU Utilization         : %.2f%%\n", utilization);
    printf("CPU Idle Time           : %lld\n", (long long)endTime - busyTime);

    if (currentAlgorithmName != NULL && comparisonCount < 6) {
        Comparison *result = &comparisonResults[comparisonCount++];
        snprintf(result->name, sizeof result->name, "%s", currentAlgorithmName);
        result->avgWaiting = totalWT / processCount;
        result->avgTurnaround = totalTAT / processCount;
        result->avgResponse = totalRT / processCount;
        result->cpuUtilization = utilization;
    }
}
