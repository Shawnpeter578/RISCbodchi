#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>

#define MAX_PROCESSES 100
#define MAX_HISTORY 20000

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
} Process;

typedef struct {
    int pid; /* PID 0 means CPU idle */
    int start;
    int finish;
} Execution;

typedef struct {
    int completion;
    int turnaround;
    int waiting;
    int response;
} Metrics;

typedef struct {
    char name[32];
    double avgWaiting;
    double avgTurnaround;
    double avgResponse;
    double cpuUtilization;
} Comparison;

extern Process processes[MAX_PROCESSES];
extern int processCount;
extern Comparison comparisonResults[6];
extern int comparisonCount;
extern const char *currentAlgorithmName;

int readInt(const char *prompt, int *value);
void inputProcesses(void);
void displayProcesses(void);
void addExecution(Execution history[], int *count, int pid, int start, int finish);
void printGanttChart(const Execution history[], int count);
void printMetrics(const Metrics metrics[]);
void runFCFS(void);
void runSJF(void);
void runSRTF(void);
void runPriorityNP(void);
void runPriorityP(void);
void runRoundRobin(void);
void compareAlgorithms(void);

#endif
