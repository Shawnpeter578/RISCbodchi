#include "scheduler.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int readInt(const char *prompt, int *value) {
    char line[128], extra;
    long parsed;

    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    if (sscanf(line, " %ld %c", &parsed, &extra) != 1 ||
        parsed < INT_MIN || parsed > INT_MAX) {
        return 0;
    }

    *value = (int)parsed;
    return 1;
}

void inputProcesses(void) {
    int n;
    Process temp[MAX_PROCESSES];

    if (!readInt("\nEnter number of processes (1-100): ", &n) ||
        n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes. Existing data unchanged.\n");
        return;
    }

    for (int i = 0; i < n; ++i) {
        temp[i].pid = i + 1;
        printf("\nProcess P%d\n", i + 1);

        if (!readInt("Arrival time (>= 0): ", &temp[i].arrival) ||
            temp[i].arrival < 0) {
            printf("Invalid arrival time. Existing data unchanged.\n");
            return;
        }

        if (!readInt("Burst time (> 0): ", &temp[i].burst) ||
            temp[i].burst <= 0) {
            printf("Invalid burst time. Existing data unchanged.\n");
            return;
        }

        if (!readInt("Priority (smaller number = higher priority): ",
                     &temp[i].priority)) {
            printf("Invalid priority. Existing data unchanged.\n");
            return;
        }
    }

    for (int i = 0; i < n; ++i) processes[i] = temp[i];
    processCount = n;
    printf("\nProcesses saved successfully!\n");
}

void displayProcesses(void) {
    if (processCount == 0) {
        printf("\nNo processes entered yet.\n");
        return;
    }

    printf("\n%-10s %-12s %-10s %-10s\n",
           "Process", "Arrival", "Burst", "Priority");
    printf("------------------------------------------------\n");
    for (int i = 0; i < processCount; ++i) {
        printf("P%-9d %-12d %-10d %-10d\n",
               processes[i].pid, processes[i].arrival,
               processes[i].burst, processes[i].priority);
    }
}

int main(void) {
    int choice;

    for (;;) {
        printf("\n========================================\n");
        printf("     CPU PROCESS SCHEDULING SIMULATOR\n");
        printf("========================================\n");
        printf("1. Enter / replace processes\n");
        printf("2. Display processes\n");
        printf("3. FCFS Scheduling\n");
        printf("4. SJF (Non-preemptive)\n");
        printf("5. SJF (Preemptive / SRTF)\n");
        printf("6. Priority (Non-preemptive)\n");
        printf("7. Priority (Preemptive)\n");
        printf("8. Round Robin Scheduling\n");
        printf("9. Compare All Algorithms\n");
        printf("0. Exit\n");
        printf("----------------------------------------\n");

        if (!readInt("Enter choice: ", &choice)) {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }

        switch (choice) {
            case 1: inputProcesses(); break;
            case 2: displayProcesses(); break;
            case 3: runFCFS(); break;
            case 4: runSJF(); break;
            case 5: runSRTF(); break;
            case 6: runPriorityNP(); break;
            case 7: runPriorityP(); break;
            case 8: runRoundRobin(); break;
            case 9: compareAlgorithms(); break;
            case 0:
                printf("\nExiting simulator. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
}
