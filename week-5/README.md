
# Page Replacement Algorithm Simulator

A native Windows desktop application written in C that demonstrates and compares three operating-system page replacement algorithms:

- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal (OPT)

The application accepts a page-reference string and a physical-memory frame count. It displays the frame contents after every page request, marks each request as a hit or fault, shows replaced pages, and calculates performance statistics.

## 1. Project objectives

This project demonstrates:

1. Virtual memory and demand paging concepts.
2. How page faults occur when a requested page is absent from memory.
3. How FIFO, LRU, and Optimal choose a victim page.
4. The effect of a limited number of physical frames.
5. How page hits, page faults, and their ratios can be used to compare algorithms.

## 2. Technology

| Component | Technology |
|---|---|
| Language | C (C11) |
| Compiler | MinGW-w64 GCC |
| GUI | Windows Win32 API |
| Table | Windows common-control ListView |
| Build | GCC / Make |
| Platform | Windows 10 or Windows 11 |

No third-party C libraries are required. The GUI uses Windows controls.

## 3. Folder structure

```text
PageReplacementSimulator/
├── src/
│   ├── main.c             # Win32 GUI and event handling
│   ├── simulator.c        # Algorithm dispatcher
│   ├── simulator.h        # Shared data structures and declarations
│   ├── algorithms.c       # FIFO, LRU and Optimal implementations
│   ├── algorithms.h       # Algorithm function declarations
│   ├── parser.c           # Page-reference input parser
│   └── parser.h           # Parser declaration
├── docs/
│   ├── PROJECT_REPORT.md
│   ├── ALGORITHM_EXPLANATION.md
│   └── TEAM_CONTRIBUTION.md
├── tests/
│   └── TEST_CASES.md
├── Makefile
├── README.md
└── .gitignore
```

## 4. Requirements

- Windows 10/11.
- MinGW-w64 GCC installed and available in PATH.
- Optional: VS Code for editing.
- Optional: GNU Make. The project can also be compiled directly with GCC.

Check the compiler:

```bash
gcc --version
```

If the command is not recognized, install MinGW-w64 and add its `bin` directory to the Windows PATH.

## 5. Build and run

Open PowerShell, Command Prompt, or the VS Code terminal in the project root.

### Option A: Compile directly

```bash
gcc -std=c11 -Wall -Wextra -O2 -Isrc src/main.c src/simulator.c src/algorithms.c src/parser.c -o PageReplacementSimulator.exe -mwindows -lcomctl32 -lgdi32
```

Run:

```powershell
.\PageReplacementSimulator.exe
```

### Option B: Use Make

If GNU Make is installed:

```bash
make
```

Then:

```powershell
.\PageReplacementSimulator.exe
```

To remove the executable:

```bash
make clean
```

## 6. Using the application

1. Launch `PageReplacementSimulator.exe`.
2. Enter a page-reference string, for example:

   ```text
   7 0 1 2 0 3 0 4 2 3
   ```

3. Enter the number of frames, for example `3`.
4. Select FIFO, LRU, Optimal, or Compare all.
5. Click **Run Simulation**.
6. Read the step-by-step table and performance summary.

The parser accepts spaces or commas between page numbers. For example, both `7 0 1 2` and `7,0,1,2` are valid.

Limits:
- At least one page reference is required.
- Maximum 500 page references.
- Frame count must be from 1 to 32.
- Page numbers must be non-negative integers.

## 7. Operating-system concepts

### Virtual memory

Virtual memory allows a process to use an address space that is larger than the physical RAM allocated to it. The operating system divides virtual memory into fixed-size pages and physical memory into frames.

A page table maps virtual pages to physical frames. When a process requests a page that is not currently in physical memory, a page fault occurs. The operating system must load the page. If no free frame exists, a page replacement algorithm selects a page to evict.

### Page hit

A page hit occurs when the requested page is already present in one of the available frames. No replacement is needed.

### Page fault

A page fault occurs when the requested page is not present in any frame. The simulator loads it into a free frame or replaces an existing page.

## 8. Algorithms

### FIFO: First-In, First-Out

FIFO replaces the page that has been in memory the longest. The implementation maintains a circular pointer to the next victim frame.

- Easy to implement.
- Low bookkeeping overhead.
- May evict a frequently used page.
- Can exhibit Belady's anomaly, where adding frames can increase faults for some reference strings.

### LRU: Least Recently Used

LRU replaces the page that has not been accessed for the longest time. The implementation records the most recent reference index for each frame.

- Uses recent history as a predictor of future use.
- Often performs better than FIFO.
- Requires tracking usage information.

### Optimal

Optimal replaces the page whose next use is farthest in the future. If a page will never be used again, it is an ideal victim.

- Produces the minimum possible number of page faults for a known reference string.
- Useful as a benchmark.
- Not generally implementable in a real operating system because future page requests are unknown.

## 9. How the code works

### Shared result structure

`SimulationStep` stores one moment in the simulation:

- Requested page.
- Contents of every frame after processing the request.
- Whether the request was a hit.
- Which page was replaced, if any.

`SimulationResult` stores the complete run:

- Algorithm name.
- Reference count.
- Frame count.
- Total hits.
- Total faults.
- Array of simulation steps.

### Input parsing

`parse_references()` reads the text entered in the GUI. It accepts spaces, tabs, and commas as separators, rejects invalid characters and negative values, and enforces the 500-reference limit.

### Algorithm execution

`run_simulation()` validates the common arguments and dispatches execution to the selected algorithm.

Each algorithm:
1. Initializes all frames as empty (`-1`).
2. Reads references from left to right.
3. Checks whether the page is already in memory.
4. Records a hit if it is present.
5. Otherwise records a fault and selects an empty frame or victim.
6. Saves a snapshot of the frames after the request.

### GUI

`main.c` creates the native Windows window and controls. The Run button reads the input controls, invokes the parser, validates the frame count, runs the selected algorithm(s), and fills the ListView table and summary box.

## 10. Performance formulas

Let:
- `R` = total number of page references.
- `H` = total page hits.
- `F` = total page faults.

```text
H + F = R

Hit Ratio (%)  = (H / R) * 100
Fault Ratio (%) = (F / R) * 100
```

The hit ratio and fault ratio should add up to 100%, subject to display rounding.

## 11. Sample input and expected FIFO result

Input:

```text
Reference string: 7 0 1 2 0 3 0 4 2 3
Frames: 3
Algorithm: FIFO
```

Expected frame progression:

| Page | Frame 1 | Frame 2 | Frame 3 | Status |
|---:|---:|---:|---:|---|
| 7 | 7 | - | - | Fault |
| 0 | 7 | 0 | - | Fault |
| 1 | 7 | 0 | 1 | Fault |
| 2 | 2 | 0 | 1 | Fault |
| 0 | 2 | 0 | 1 | Hit |
| 3 | 2 | 3 | 1 | Fault |
| 0 | 2 | 3 | 0 | Fault |
| 4 | 4 | 3 | 0 | Fault |
| 2 | 4 | 2 | 0 | Fault |
| 3 | 4 | 2 | 3 | Fault |

FIFO totals:
- References: 10
- Hits: 1
- Faults: 9
- Hit ratio: 10%
- Fault ratio: 90%

The application calculates all values during execution; it does not hardcode the sample result.

## 12. Complexity analysis

Let `n` be the number of page references and `f` be the number of frames.

| Algorithm | Time complexity in this implementation | Extra working memory |
|---|---:|---:|
| FIFO | O(n × f) | O(f) |
| LRU | O(n × f) | O(f) |
| Optimal | O(n × f × n) worst case | O(f) |

The stored output history uses O(n × f) space per run.

Optimal has the higher time complexity because, on a fault when all frames are full, it scans future references to find the next use of every resident page.

## 13. Testing

See `tests/TEST_CASES.md` for the test plan. Test at least:
- A reference string with repeated pages.
- One frame.
- More frames than unique pages.
- A single reference.
- Invalid input.
- All three algorithms with identical input.

## 14. Limitations and possible extensions

Current version:
- Supports non-negative integer page IDs.
- Supports up to 500 references and 32 frames.
- Simulates one process's page-reference stream.
- Uses offline future knowledge for Optimal.
- Does not model disk I/O latency, dirty bits, page-table permissions, or real hardware memory.

Possible extensions:
- Add a graphical bar chart.
- Add step-by-step Next/Previous controls.
- Add animation for page replacement.
- Export results to CSV.
- Add Clock/Second-Chance and LFU algorithms.
- Add a dark theme and improved styling.

## 15. Team contribution

See `docs/TEAM_CONTRIBUTION.md` for a suggested five-person work split.

## 16. Academic note

This is an educational simulator. It demonstrates page replacement decisions for a supplied reference string; it does not modify the operating system's actual virtual memory manager.
