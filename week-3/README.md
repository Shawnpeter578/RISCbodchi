# Week 3: CPU Scheduling Simulator

## Overview

Week 3 explores CPU scheduling, one of the fundamental responsibilities of an operating system.

When multiple processes are ready to execute, the operating system must decide which process receives the CPU.

Different scheduling algorithms use different rules to select processes.

This project contains a menu-driven C simulator and a browser-based dashboard for visualizing and comparing CPU scheduling algorithms.

## Learning Objectives

- Understand CPU scheduling.
- Learn the difference between preemptive and non-preemptive scheduling.
- Implement scheduling algorithms.
- Understand process arrival time, burst time, and priority.
- Calculate waiting time, turnaround time, and response time.
- Generate and interpret Gantt charts.
- Compare scheduling algorithms using common workloads.
- Understand scheduling trade-offs.

## 1. What Is CPU Scheduling?

CPU scheduling is the process of selecting a process from the ready queue and allocating the CPU to it.

A computer may have multiple processes waiting for execution.

The CPU scheduler determines which process should execute next.

The goal of scheduling is to use CPU resources effectively while providing acceptable response time and fairness.

## 2. Process Scheduling Terminology

### Process

A program currently executing or waiting to execute.

### Arrival Time (AT)

The time at which a process enters the ready queue.

### Burst Time (BT)

The amount of CPU execution time required by a process.

### Completion Time (CT)

The time at which a process finishes execution.

### Remaining Time

The amount of CPU time still required by a process.

### Priority

A value used to determine the relative importance of a process.

In this project, a lower numeric priority value means a higher priority.

## 3. Preemptive and Non-Preemptive Scheduling

### Non-Preemptive Scheduling

Once a process is selected, it continues executing until it completes or voluntarily gives up the CPU.

Examples:

- FCFS
- Non-preemptive SJF
- Non-preemptive Priority

### Preemptive Scheduling

The scheduler can interrupt a running process and select another process.

Examples:

- SRTF
- Preemptive Priority
- Round Robin

Preemptive scheduling can improve responsiveness but introduces additional context-switching and scheduling overhead in real systems.

## 4. Scheduling Algorithms

### First Come, First Served (FCFS)

FCFS schedules processes in the order they arrive.

The process that arrives first receives the CPU first.

#### Working

1. Processes enter the ready queue.
2. The scheduler selects the earliest-arriving process.
3. The process executes until completion.
4. The next process is selected.

#### Advantages

- Simple to implement.
- Easy to understand.
- Preserves arrival order.

#### Disadvantages

- A long process can delay shorter processes.
- Can produce high average waiting time.
- Poor response time for interactive workloads.

### Shortest Job First (SJF)

SJF selects the ready process with the shortest CPU burst.

In this project, SJF is non-preemptive.

#### Working

1. Identify the processes that have arrived.
2. Select the process with the shortest burst time.
3. Execute it until completion.
4. Repeat.

#### Advantages

- Can minimize average waiting time under ideal assumptions.
- Effective when burst times are known.

#### Disadvantages

- Burst time may not be known in advance.
- Long processes may experience starvation.

### Shortest Remaining Time First (SRTF)

SRTF is the preemptive version of shortest-job scheduling.

The scheduler selects the process with the shortest remaining execution time.

#### Working

1. Select the ready process with the smallest remaining time.
2. Execute it.
3. When a new process arrives, compare its burst time with the running process's remaining time.
4. If the new process has less remaining time, preempt the current process.
5. Continue until all processes complete.

#### Advantages

- Can provide good average waiting time.
- Can improve completion time for short processes.

#### Disadvantages

- Requires frequent scheduling decisions.
- Long processes may starve.

### Priority Scheduling

Priority scheduling selects a process based on its priority value.

The project includes:

- Non-preemptive Priority
- Preemptive Priority

#### Non-preemptive Priority

Once a process starts, it continues until completion.

#### Preemptive Priority

A newly arrived higher-priority process can interrupt a currently running process.

#### Advantages

- Supports priority-based task execution.
- Useful when some tasks are more important than others.

#### Disadvantages

- Low-priority processes may starve.
- Priority assignment can be difficult.

### Round Robin (RR)

Round Robin is a preemptive scheduling algorithm designed for time-sharing systems.

Each process receives a fixed time slice called the time quantum.

#### Working

1. Processes enter the ready queue.
2. The first process receives the CPU for up to one quantum.
3. If it completes, it leaves the queue.
4. If it does not complete, it is placed at the back of the queue.
5. The scheduler selects the next process.
6. The process queue continues until all processes complete.

#### Advantages

- Provides fairness.
- Improves responsiveness.
- Suitable for interactive systems.

#### Disadvantages

- Performance depends on the quantum.
- A very small quantum increases scheduling overhead.
- A very large quantum makes it behave more like FCFS.

## 5. Scheduling Metrics

### Completion Time

The time at which a process finishes.

### Turnaround Time

The total time from arrival to completion.

```text
Turnaround Time = Completion Time - Arrival Time
```

### Waiting Time

The total time a process spends waiting in the ready queue.

For the standard single CPU-burst model:

```text
Waiting Time = Turnaround Time - Burst Time
```

### Response Time

The time from process arrival until its first CPU execution.

```text
Response Time = First CPU Start Time - Arrival Time
```

### Average Waiting Time

```text
Average Waiting Time =
Sum of Waiting Times / Number of Processes
```

### Average Turnaround Time

```text
Average Turnaround Time =
Sum of Turnaround Times / Number of Processes
```

### CPU Utilization

```text
CPU Utilization =
CPU Busy Time / Total Observed Time * 100
```

## 6. Gantt Chart

A Gantt chart represents the order in which processes execute.

Example:

```text
0      4      7      9
|  P1  |  P2  | P3  |
```

This indicates:

- P1 executes from time 0 to 4.
- P2 executes from time 4 to 7.
- P3 executes from time 7 to 9.

Gantt charts make it easier to compare the behavior of different scheduling algorithms.

## 7. Example Workload

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 5 | 1 |
| P2 | 1 | 3 | 2 |
| P3 | 2 | 1 | 3 |

For Round Robin, use a time quantum of 2.

Run this workload through each scheduling algorithm and compare:

- Execution order
- Waiting time
- Turnaround time
- Response time
- CPU timeline

The result may vary depending on the algorithm and its tie-breaking rules.

## 8. Project Structure

The C simulator includes modules for:

- Main menu and program entry point
- Shared declarations and data structures
- Process input and state
- FCFS and SJF
- SRTF
- Priority scheduling
- Round Robin
- Output and metrics
- Algorithm comparison

The project also includes a browser-based dashboard.

> The dashboard runs its own JavaScript scheduling simulation. It is not currently connected to the C executable.

## 9. Requirements

- GCC or a compatible C compiler.
- Windows, Linux, or macOS terminal.
- A modern browser for the dashboard.

## 10. Build and Run

Compile the C simulator from the directory containing the source files.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c state.c output.c fcfs.c srtf.c priority.c round_robin.c compare.c -o simulator
```

Run on Linux or macOS:

```bash
./simulator
```

Run on Windows PowerShell:

```powershell
.\simulator.exe
```

Adjust the compile command if the source filenames differ in your local folder.

## 11. Browser Dashboard

1. Open the dashboard HTML file in a modern browser.
2. Load sample data or enter processes.
3. Edit arrival times, burst times, and priorities.
4. Select a scheduling algorithm.
5. Set the Round Robin time quantum if needed.
6. Run the simulation.
7. Inspect the Gantt-style timeline.
8. Compare the results using tables and charts.

## 12. How to Compare Algorithms

Use the same process workload for each algorithm.

Record:

- Average waiting time
- Average turnaround time
- Response time
- Execution order
- CPU utilization
- Starvation or fairness observations

Different algorithms optimize different objectives.

There is no single scheduling algorithm that is best for every workload.

## 13. Troubleshooting

### GCC is not recognized

Install a C compiler and ensure it is available through PATH.

### Undefined reference errors

Check whether all required source files are included in the compile command.

### Dashboard does not open

Confirm the HTML file exists and open it directly in a browser.

### Unexpected scheduling results

Verify:

- Arrival times
- Burst times
- Priority convention
- Round Robin quantum
- Tie-breaking rules

## Key Takeaways

CPU scheduling determines which ready process receives CPU time.

FCFS prioritizes arrival order.

SJF prioritizes short jobs.

SRTF prioritizes the shortest remaining time.

Priority scheduling selects according to priority.

Round Robin provides time slices to processes in cyclic order.

## Conclusion

Week 3 demonstrates how CPU scheduling algorithms affect system behavior.

By implementing and comparing multiple algorithms, this project helps illustrate the trade-offs between responsiveness, fairness, waiting time, and scheduling efficiency.
