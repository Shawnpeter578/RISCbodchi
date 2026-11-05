# Project Report
## Simulation and Comparative Analysis of Page Replacement Algorithms

### Abstract

This project implements a C-based simulator for three page replacement algorithms: First-In, First-Out (FIFO), Least Recently Used (LRU), and Optimal. The application accepts a page-reference string and a frame count, simulates the memory state after each page request, identifies hits and faults, and compares algorithm performance. A native Windows GUI provides input controls, a step-by-step table, and a performance summary.

### 1. Introduction

Modern operating systems use virtual memory to provide processes with virtual address spaces. Physical memory is divided into frames, while virtual memory is divided into pages. When a requested page is not resident in physical memory, a page fault occurs. If all frames are occupied, the operating system must select a page to remove. This project studies three classic replacement strategies.

### 2. Problem statement

To develop a simulator that accepts page references and a frame count, executes FIFO, LRU, and Optimal, records the state of memory after every reference, and compares the resulting page-fault counts.

### 3. Objectives

- Understand virtual memory and paging.
- Implement FIFO, LRU, and Optimal in C.
- Display memory-frame changes after each reference.
- Identify hits and faults.
- Calculate hit and fault ratios.
- Compare algorithm performance through a GUI.

### 4. Methodology

The input parser converts the user's text into an integer array. The simulator initializes empty frames and processes references sequentially. Each algorithm independently chooses a victim when a fault occurs and memory is full. A snapshot is saved after every reference. The GUI displays snapshots and calculated statistics.

### 5. Algorithms

**FIFO:** Replaces the page that entered memory earliest.

**LRU:** Replaces the page whose most recent access occurred farthest in the past.

**Optimal:** Replaces the page whose next access is farthest in the future, or which will not be accessed again.

### 6. Performance metrics

Hit ratio = (page hits / total references) × 100

Fault ratio = (page faults / total references) × 100

The number of hits plus faults equals the number of references.

### 7. Implementation

The program is divided into modules:
- `main.c`: native Windows interface and event handling.
- `parser.c`: validation and conversion of user input.
- `algorithms.c`: FIFO, LRU, and Optimal implementations.
- `simulator.c`: common validation and algorithm dispatch.
- Header files: shared structures and function declarations.

### 8. Testing

The test plan includes normal input, repeated references, one frame, more frames than unique pages, a single reference, malformed input, and comparison mode. Refer to `tests/TEST_CASES.md`.

### 9. Results and discussion

The application calculates the result for each algorithm using the same reference string and frame count. Optimal should never produce more page faults than FIFO or LRU for the same finite reference string and frame count. FIFO and LRU results depend on the access pattern; neither is guaranteed to outperform the other in every case.

### 10. Limitations

The simulator models page replacement decisions, not a complete operating-system memory manager. It does not model real disk operations, dirty pages, page-table permissions, TLB behavior, or concurrent processes. Optimal uses future knowledge and is included as a theoretical benchmark.

### 11. Conclusion

The project demonstrates how different replacement policies respond to the same memory-access sequence. By displaying frame states and comparing hit/fault statistics, it makes the behavior of FIFO, LRU, and Optimal easier to understand.

### 12. Future enhancements

- Animated frame transitions.
- Graphical comparison charts.
- CSV export.
- Additional algorithms such as Clock and LFU.
- Step-by-step playback controls.
