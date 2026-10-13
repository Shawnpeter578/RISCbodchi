#include "scheduler.h"

Process processes[MAX_PROCESSES];
int processCount = 0;
Comparison comparisonResults[6];
int comparisonCount = 0;
const char *currentAlgorithmName = NULL;
