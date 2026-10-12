# CPU Scheduling Simulator

A menu-driven CPU scheduling simulator written in C, with a browser-based performance dashboard for visualizing and comparing scheduling algorithms.

## Features

### C Simulator
- First-Come, First-Served (FCFS)
- Shortest Job First (SJF), non-preemptive
- Shortest Remaining Time First (SRTF), preemptive
- Priority Scheduling, non-preemptive
- Priority Scheduling, preemptive
- Round Robin
- Compare scheduling algorithms
- View and replace process input
- Calculate scheduling metrics and display execution timelines

> Priority convention: a lower numeric priority value means a higher priority.

### Browser Dashboard
- Edit process arrival time, burst time, and priority
- Add or remove processes and load sample data
- Compare scheduling algorithms using charts and tables
- Inspect a Gantt-style CPU timeline and per-process metrics
- Step through scheduling decisions to understand how processes execute
- Runs locally in a modern web browser

**Important:** The browser dashboard simulates scheduling independently in JavaScript. It is not currently connected to the C executable, so its results are calculated by the dashboard itself.

## Project Structure

The C project is organized into multiple files:

```text
.
├── main.c          # Main menu and program entry point
├── scheduler.h     # Shared declarations and data structures
├── state.c         # Shared process state and input handling
├── fcfs.c          # FCFS and non-preemptive SJF
├── srtf.c          # SRTF scheduling
├── priority.c      # Priority scheduling implementations
├── round_robin.c   # Round Robin scheduling
├── output.c        # Scheduling results and metrics
├── compare.c       # Algorithm comparison
├── dashboard.html  # Browser-based visual dashboard (if included)
└── README.md
```

The exact files included can vary depending on which project package you downloaded.

## Requirements

- GCC C compiler
- Windows, Linux, or macOS terminal
- A modern web browser for `dashboard.html`

## Build and Run the C Simulator

Open a terminal in the directory containing the C source files.

### Windows (PowerShell)

Compile:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c state.c output.c fcfs.c srtf.c priority.c round_robin.c compare.c -o simulator.exe
```

Run:

```powershell
.\simulator.exe
```

### Linux / macOS

Compile:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c state.c output.c fcfs.c srtf.c priority.c round_robin.c compare.c -o simulator
```

Run:

```bash
./simulator
```

If your project uses a different set of source filenames, adjust the compile command to match the files present in your folder.

## Open the Browser Dashboard

1. Locate `dashboard.html`.
2. Open it in a modern browser such as Chrome, Firefox, or Edge.
3. Add processes or load the sample workload.
4. Select an algorithm and run the simulation.
5. Use the step-by-step controls to advance through CPU execution, if available in your dashboard version.

No web server or internet connection should be required for the standalone dashboard.

## Process Inputs

Each process typically has these fields:

- **Arrival Time (AT):** The time at which the process becomes ready to run.
- **Burst Time (BT):** The total CPU time the process needs.
- **Priority:** The process's priority value. Lower values represent higher priority in this project.

For Round Robin, configure a positive time quantum. Each process receives CPU time for up to that quantum before it is moved to the back of the ready queue if it still has remaining work.

## Scheduling Metrics

- **Completion Time (CT):** Time at which the process finishes.
- **Turnaround Time (TAT):** `CT - AT`
- **Waiting Time (WT):** `TAT - BT`
- **Response Time (RT):** First CPU start time minus arrival time.
- **CPU Utilization:** Percentage of the measured timeline during which the CPU is busy.

## Example Workload

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 5 | 1 |
| P2 | 1 | 3 | 2 |
| P3 | 2 | 1 | 3 |

For Round Robin, set the time quantum to `2`.

One possible Gantt timeline for this workload is:

```text
0    2    4   5    7   8   9
| P1 | P2 |P3| P1 | P2| P1 |
```

Expected metrics for this workload:

| Process | Completion Time | Turnaround Time | Waiting Time |
|---|---:|---:|---:|
| P1 | 9 | 9 | 4 |
| P2 | 8 | 7 | 4 |
| P3 | 5 | 3 | 2 |
| **Average** | — | **6.33** | **3.33** |

## Team Collaboration Suggestion

For a five-person team, split ownership by module and agree on shared function declarations in `scheduler.h` before making changes.

- **Member 1:** `main.c`, `scheduler.h`, integration and final testing
- **Member 2:** `fcfs.c` and FCFS/SJF validation
- **Member 3:** `srtf.c` and `priority.c`
- **Member 4:** `round_robin.c` and metric verification
- **Member 5:** `compare.c`, test cases, and documentation

Use separate Git branches and meaningful commits. Review and test changes before merging. Avoid having multiple people edit the same file at the same time.

## Troubleshooting

- **`gcc` is not recognized:** Install GCC (for example, through MinGW-w64/MSYS2 on Windows) and ensure it is available on your PATH.
- **Undefined reference errors:** Confirm every required `.c` file is included in the compile command.
- **No dashboard opens:** Open `dashboard.html` directly in your browser and check that the file was extracted from the ZIP.
- **Unexpected results:** Verify arrival times, burst times, the selected algorithm, Round Robin quantum, and priority convention.

## Possible Future Improvements

- Export C simulator results to CSV or JSON
- Connect the browser dashboard to real C simulator output
- Add automated test cases for edge conditions
- Improve visualization and scheduling explanations

## License

Add a license before redistributing this project publicly. If no license is specified, assume that standard copyright applies.
