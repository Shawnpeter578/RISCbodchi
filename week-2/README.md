# Multi-Process Page Replacement Simulator

A C-based operating systems project that simulates and compares page replacement algorithms using multiple processes and inter-process communication (IPC).

The project demonstrates how operating system concepts such as **process management, memory management, page replacement, and IPC mechanisms** work together in a practical application.

---

## Overview

When a process accesses a page that is not currently loaded into memory, a page fault occurs. If all available memory frames are occupied, the operating system must decide which page to remove to make space for the requested page.

This project implements a multi-process simulator to demonstrate and compare three commonly studied page replacement algorithms:

- **FIFO (First-In, First-Out)**
- **LRU (Least Recently Used)**
- **Optimal Page Replacement**

The simulator accepts a page-reference sequence and frame count, executes the selected algorithm, and reports the number of page faults and page hits.

It also uses multiple IPC mechanisms to demonstrate communication between independent processes.

## Objectives

- Implement and compare FIFO, LRU, and Optimal page replacement algorithms.
- Demonstrate process creation and communication in C.
- Explore different IPC mechanisms available in POSIX systems.
- Separate simulation logic, user interaction, and logging into distinct processes.
- Analyze the performance and behavior of page replacement algorithms.
- Provide a modular, maintainable, and reproducible project.

## Features

- Multi-process architecture using POSIX processes.
- Support for FIFO, LRU, and Optimal page replacement.
- Multiple IPC communication mechanisms.
- Interactive input for frame count and page-reference sequence.
- Page fault and page hit statistics.
- Simulation logging.
- Build automation using a Makefile.
- Documentation covering architecture, IPC, testing, and benchmarking.

## Architecture

The simulator is organized around three primary processes:

| Process | Responsibility |
|---|---|
| UI / Main Process | Accepts user input, initiates simulations, and displays results. |
| Core Process | Executes the selected page replacement algorithm and calculates statistics. |
| Logger Process | Receives simulation messages and records relevant events. |

### Process communication

The processes communicate using IPC mechanisms supported by the project:

- POSIX Message Queues
- Pipes
- Shared Memory
- Unix Domain Sockets
- FIFO (Named Pipes)

Each mechanism demonstrates a different approach to exchanging data between processes.

## Page Replacement Algorithms

### FIFO

First-In, First-Out replaces the page that has been in memory the longest.

### LRU

Least Recently Used replaces the page that has not been accessed for the longest period of time.

### Optimal

Optimal replaces the page whose next use is farthest in the future. It is primarily used as a theoretical benchmark because it requires knowledge of future page references.

## Technologies

- **Language:** C
- **Operating System:** Linux / POSIX-compatible environment
- **Process Management:** `fork()`, `wait()`
- **IPC:** POSIX IPC mechanisms
- **Build System:** GNU Make

## Requirements

Before building the project, ensure that the following are installed:

- GCC or another compatible C compiler
- GNU Make
- Linux or WSL with Ubuntu
- POSIX development libraries

## Build

Clone the repository:

```bash
git clone <repository-url>
cd week4_multi_process_simulator
```

Compile the project:

```bash
make
```

## Run

Launch the simulator:

```bash
./simulator
```

Follow the prompts to enter the number of memory frames, page-reference sequence, and algorithm.

## Example

Consider the following input:

```text
Number of frames: 3
Page reference sequence:
7 0 1 2 0 3 0 4 2 3 0 3 2
```

Expected results:

| Algorithm | Page Faults | Page Hits |
|---|---:|---:|
| FIFO | 10 | 3 |
| LRU | 9 | 4 |
| Optimal | 7 | 6 |

The results illustrate how different replacement strategies can produce different page-fault counts for the same reference sequence.

## Project Structure

```text
week4_multi_process_simulator/
│
├── main.c
├── Makefile
├── README.md
│
├── processes/
│   ├── core.c
│   ├── logger.c
│   └── ...
│
├── algorithms/
│   ├── fifo.c
│   ├── lru.c
│   ├── optimal.c
│   └── ...
│
├── ipc/
│   ├── message_queue.c
│   ├── pipe.c
│   ├── shared_memory.c
│   ├── unix_socket.c
│   └── fifo.c
│
├── tests/
│   ├── README.md
│   └── test_cases.md
│
└── docs/
    ├── architecture.md
    ├── ipc_design.md
    ├── testing.md
    ├── benchmarking.md
    └── performance_analysis.md
```

> The structure above represents the intended organization. Update the file names if your implementation uses a different layout.

## Testing

Testing focuses on verifying the correctness of page replacement algorithms and the reliability of inter-process communication.

Test cases include:

- Page-reference sequences with repeated accesses.
- Sequences that cause frequent page faults.
- Different numbers of memory frames.
- Empty or invalid input.
- Comparison of algorithm results against expected values.
- IPC communication and process termination.

Refer to [`tests/test_cases.md`](tests/test_cases.md) and [`docs/testing.md`](docs/testing.md) for more information.

## Benchmarking and Performance Analysis

The project includes documentation for benchmarking and analyzing the simulator.

The analysis can consider:

- Page fault and page hit counts.
- Execution time of each algorithm.
- IPC communication overhead.
- Differences between algorithmic behavior and total simulation runtime.

Refer to:

- [`docs/benchmarking.md`](docs/benchmarking.md)
- [`docs/performance_analysis.md`](docs/performance_analysis.md)

Benchmark values should be measured on the system where the simulator is run. Results may vary depending on hardware, operating system, compiler, and process scheduling.

## Learning Outcomes

This project provides practical experience with:

- Virtual memory and page replacement.
- Multi-process programming in C.
- POSIX process creation and synchronization.
- Inter-process communication.
- Modular software design.
- Testing and performance analysis.
- Linux development and build workflows.

## Team Contributions

The project is developed collaboratively, with responsibilities divided across IPC implementation and system integration.

| Team Member | Responsibility |
|---|---|
| Member 1 | POSIX Message Queues |
| Member 2 | Pipes |
| Member 3 | Shared Memory |
| Member 4 | Unix Domain Sockets |
| Member 5 | FIFO / Named Pipes and integration |

Update this section with the actual team members and their contributions.

## Limitations

- The Optimal algorithm assumes knowledge of future page references.
- Simulation results depend on the selected reference sequence and frame count.
- Execution time may include process scheduling and IPC overhead, not just algorithm computation.
- The simulator models page replacement behavior and does not reproduce a complete operating system's virtual memory subsystem.

## References

- Operating Systems concepts: virtual memory and page replacement.
- POSIX process and IPC documentation.
- Linux manual pages for `fork()`, `pipe()`, `wait()`, `shm_open()`, `mq_open()`, and Unix domain sockets.

## License

This project was developed for academic and educational purposes.

Add a license here if the project is intended to be distributed under a specific open-source license.
