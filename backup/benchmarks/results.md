# RISCbodchi Week 4 - Performance Benchmark

## Benchmark Overview

This benchmark measures the performance of the RISCbodchi multi-process
simulator using POSIX message queues.

-   Architecture: UI + Core + Logger
-   IPC: POSIX Message Queues
-   Language: C
-   Environment: Linux / WSL
-   Metrics: Execution time and average time per command

## Benchmark Workload

The benchmark uses these 10 commands repeatedly:

``` text
ADD 10 20
SUB 50 20
MUL 5 6
DIV 100 5
PUSH 50
POP
ENQUEUE 100
DEQUEUE
STORE 10 500
LOAD 10
```

## Multi-Process Benchmark Results

  Workload     Commands   Execution Time   Average / Command
  ---------- ---------- ---------------- -------------------
  Test 1          1,000       0.219057 s         0.219057 ms
  Test 2        100,000       6.628087 s         0.066281 ms

## Additional Runs

  Workload        Commands   Execution Time   Average / Command
  ------------ ----------- ---------------- -------------------
  Additional        10,000       0.855916 s         0.085592 ms
  Additional     1,000,000      64.678365 s         0.064678 ms

## Performance Observations

-   The simulator successfully processed all tested workloads.
-   1,000 commands completed in **0.219057 seconds**.
-   100,000 commands completed in **6.628087 seconds**.
-   Average time per command decreased for larger workloads.
-   Repeated runs can vary because of system load and WSL scheduling.
-   The multi-process architecture introduces IPC overhead because
    commands and responses use POSIX message queues.

## Single-Process vs Multi-Process Comparison

  Metric               Single Process    Multi Process
  ------------------ ---------------- ----------------
  1,000 commands       To be measured       0.219057 s
  100,000 commands     To be measured       6.628087 s
  CPU usage            To be measured   To be measured
  Memory usage         To be measured   To be measured
  IPC overhead                    N/A          Present

**Note:** Run the original single-process simulator with exactly the
same workloads before filling the single-process column. Do not estimate
these values.

## Benchmark Procedure

1.  Start the Logger process.
2.  Start the Core process.
3.  Generate the fixed command workload.
4.  Run `benchmark_client`.
5.  Record command count, execution time, and average time per command.
6.  Repeat the same workloads with the single-process simulator.
7.  Compare the results.

Example:

``` bash
./logger
./core
./benchmark_client
```

## Conclusion

The multi-process RISCbodchi simulator successfully processed workloads
from 1,000 to 1,000,000 commands.

For the two required benchmark sizes:

-   **1,000 commands:** 0.219057 seconds
-   **100,000 commands:** 6.628087 seconds

These results demonstrate that the multi-process simulator can process
large workloads while communicating through POSIX message queues.
