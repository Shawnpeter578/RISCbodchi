# RISCbodchi Week 4 - Benchmarks

Performance benchmarking for the RISCbodchi multi-process simulator.

## Benchmark Setup

-   **Language:** C
-   **Environment:** Linux / WSL
-   **IPC:** POSIX Message Queues
-   **Processes:** UI, Core, Logger
-   **Workload:** Repeated simulator commands

## Files

  -----------------------------------------------------------------------
  File                                Description
  ----------------------------------- -----------------------------------
  `benchmark_client.c`                Automated client that sends
                                      commands through POSIX message
                                      queues

  `commands.txt`                      Commands used as the benchmark
                                      workload

  `results.md`                        Recorded benchmark results and
                                      performance analysis
  -----------------------------------------------------------------------

## Workload

The benchmark repeatedly executes:

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

The workload was tested with different sizes, including:

-   1,000 commands
-   10,000 commands
-   100,000 commands
-   1,000,000 commands

## Running the Benchmark

### 1. Start the Logger

From the project root:

``` bash
./logger
```

### 2. Start the Core

Open another terminal:

``` bash
./core
```

### 3. Run the Benchmark Client

Open another terminal:

``` bash
cd benchmarks
./benchmark_client
```

The benchmark reports:

-   Number of commands executed
-   Total execution time
-   Average execution time per command

## Example Results

     Commands   Execution Time   Average / Command
  ----------- ---------------- -------------------
        1,000       0.219057 s         0.219057 ms
       10,000       0.855916 s         0.085592 ms
      100,000       6.628087 s         0.066281 ms
    1,000,000      64.678365 s         0.064678 ms

These are measured results from the multi-process benchmark.

## Performance Metrics

The benchmark focuses on:

1.  **Execution Time** - total time required to process the workload.
2.  **Average Time per Command** - execution time divided by the number
    of commands.
3.  **CPU Usage** - to be measured using Linux performance tools.
4.  **Memory Usage** - to be measured using Linux performance tools.
5.  **IPC Overhead** - additional cost introduced by communication
    through POSIX message queues.

## Single-Process Comparison

The final benchmark should run the same workloads on the original
single-process simulator.

This ensures a fair comparison:

``` text
Same commands
     ↓
Same workload size
     ↓
Single-process version
        VS
Multi-process version
```

The single-process results will be added to `results.md` after
measurement.

## Notes

Benchmark results may vary slightly between runs because of system load,
CPU scheduling, and WSL overhead.

The benchmark should therefore use the same machine, workload, and
environment when comparing the two implementations.
