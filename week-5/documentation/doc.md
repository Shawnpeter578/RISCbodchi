# PROJECT DOCUMENTATION
# SIMULATION AND COMPARATIVE ANALYSIS OF PAGE REPLACEMENT ALGORITHMS

**Subject:** Operating Systems  
**Project Type:** Mini Project  
**Programming Language:** C (C11)  
**Graphical Interface:** Windows Win32 API  
**Compiler:** MinGW-w64 GCC  
**Platform:** Windows 10 / Windows 11

---

# TABLE OF CONTENTS

1. Abstract
2. Introduction
3. Problem Statement
4. Project Objectives
5. Scope of the Project
6. Theoretical Background
7. System Requirements
8. System Architecture
9. Functional Requirements
10. Non-Functional Requirements
11. Algorithms Implemented
12. Algorithm Working Examples
13. Software Design and Module Description
14. Data Structures Used
15. Program Execution Workflow
16. Graphical User Interface
17. Input Validation and Error Handling
18. Performance Analysis
19. Testing and Validation
20. Sample Results
21. Advantages
22. Limitations
23. Applications
24. Future Enhancements
25. Team Responsibilities
26. Conclusion
27. References

---

# 1. ABSTRACT

Memory management is one of the most important responsibilities of an operating system. Modern operating systems use virtual memory to allow processes to execute even when their entire memory requirements cannot fit into physical RAM.

Virtual memory is divided into fixed-size units called pages, while physical memory is divided into units called frames. When a process requests a page that is not currently available in physical memory, a page fault occurs. If all available frames are occupied, the operating system must decide which existing page should be removed to accommodate the newly requested page.

This project, titled **Simulation and Comparative Analysis of Page Replacement Algorithms**, implements and demonstrates three fundamental page replacement algorithms:

- First-In, First-Out (FIFO)
- Least Recently Used (LRU)
- Optimal Page Replacement

The application is developed using the C programming language and the Windows Win32 API. It provides a graphical user interface through which users can enter a page-reference string, specify the number of available memory frames, and select an algorithm for simulation.

The application displays the contents of memory frames after every page request, identifies page hits and page faults, records replaced pages, and calculates performance statistics.

The simulator also allows users to execute all three algorithms using identical inputs and compare their performance.

The main purpose of this project is to provide a practical understanding of virtual memory management and demonstrate how different page replacement strategies influence memory utilization and page fault frequency.

# 2. INTRODUCTION

An operating system manages computer hardware and provides services to application programs. One of its important responsibilities is managing main memory.

Physical memory (RAM) is limited. However, applications may require more memory than the amount of physical memory currently available.

To solve this problem, operating systems use a technique called **virtual memory**.

Virtual memory provides each process with a virtual address space. The operating system divides this address space into pages and divides physical memory into frames.

When a process requests a page, the operating system checks whether that page is already loaded into physical memory.

There are two possible situations:

**Page Hit:** The requested page is already present in memory.

**Page Fault:** The requested page is not present in memory and must be loaded.

When a page fault occurs and free memory frames are available, the operating system loads the requested page into an empty frame.

When all frames are occupied, the operating system must select an existing page for removal. This decision is made using a page replacement algorithm.

Different algorithms select different pages for replacement. Their decisions influence the total number of page faults generated during program execution.

This project implements three commonly studied page replacement algorithms and compares their behavior through simulation.

# 3. PROBLEM STATEMENT

Design and develop a graphical simulation application that demonstrates the working of FIFO, LRU, and Optimal page replacement algorithms.

The application should accept a page-reference string and a specified number of memory frames as input.

It should process page requests sequentially, identify page hits and page faults, update memory frame contents, and display the state of memory after each request.

The application should calculate the total number of hits, faults, hit ratio, and fault ratio for each algorithm.

Finally, it should provide a comparative summary that allows users to evaluate the performance of different page replacement techniques.

# 4. PROJECT OBJECTIVES

The primary objectives are:

1. To understand virtual memory and paging concepts.
2. To study the need for page replacement algorithms.
3. To implement FIFO page replacement using C.
4. To implement LRU page replacement using C.
5. To implement Optimal page replacement using C.
6. To simulate memory frame allocation.
7. To identify and count page hits and page faults.
8. To calculate hit ratios and fault ratios.
9. To compare the performance of different algorithms.
10. To provide an easy-to-use graphical interface.
11. To understand the practical importance of memory management in operating systems.

# 5. SCOPE OF THE PROJECT

The project focuses on simulating page replacement decisions for a single page-reference sequence.

The application supports:

- User-defined page-reference sequences.
- Configurable memory frame counts.
- Individual execution of FIFO, LRU, and Optimal.
- Simultaneous comparison of all three algorithms.
- Step-by-step memory frame visualization.
- Page hit and page fault identification.
- Replacement tracking.
- Performance calculations.
- Input validation.

The simulator is designed for educational purposes and does not modify the actual memory management mechanisms of the operating system.

# 6. THEORETICAL BACKGROUND

## 6.1 Virtual Memory

Virtual memory is a memory management technique that allows processes to use a virtual address space independent of the physical RAM available.

It allows the operating system to load only the required portions of a program into physical memory.

The remaining pages can stay outside physical memory until they are needed.

## 6.2 Paging

Paging is a memory management scheme in which:

- Virtual memory is divided into fixed-size blocks called pages.
- Physical memory is divided into fixed-size blocks called frames.
- Pages are loaded into available frames when required.

Pages and frames have the same size.

A page table maintains the mapping between virtual pages and physical frames.

## 6.3 Page Reference String

A page-reference string is a sequence of page numbers representing the order in which a process requests pages.

Example:

`7 0 1 2 0 3 0 4 2 3`

Each number represents a page requested by the process.

The simulator processes these requests sequentially.

## 6.4 Page Hit

A page hit occurs when the requested page is already available in one of the memory frames.

No page replacement is necessary.

## 6.5 Page Fault

A page fault occurs when the requested page is not available in physical memory.

The operating system must load the requested page.

If all frames are occupied, one of the existing pages must be replaced.

## 6.6 Page Replacement

Page replacement is the process of removing an existing page from physical memory to create space for a newly requested page.

The page selected for removal is called the victim page.

The selection of this page depends on the replacement algorithm.

## 6.7 Belady's Anomaly

Belady's anomaly is a phenomenon in which increasing the number of available memory frames can increase the number of page faults.

It can occur with FIFO page replacement.

Stack algorithms such as LRU and Optimal do not exhibit Belady's anomaly under the standard assumptions of page replacement simulation.

# 7. SYSTEM REQUIREMENTS

## 7.1 Hardware Requirements

| Component | Minimum Requirement |
|---|---|
| Processor | Dual-core processor |
| RAM | 4 GB |
| Storage | 100 MB free space |
| Display | 1366 × 768 recommended |
| System | Windows-compatible computer |

## 7.2 Software Requirements

| Component | Technology |
|---|---|
| Operating System | Windows 10 / 11 |
| Programming Language | C |
| Language Standard | C11 |
| Compiler | MinGW-w64 GCC |
| GUI Framework | Windows Win32 API |
| Editor | Visual Studio Code |
| Build Tool | GCC / GNU Make |
| Data Structures | Arrays and counters |

# 8. SYSTEM ARCHITECTURE

The application follows a modular architecture.

Each module performs a specific responsibility, making the application easier to maintain, test, and extend.

## 8.1 Architecture Overview

The application consists of the following layers:

**1. Presentation Layer**

Provides the graphical user interface and accepts user input.

**2. Input Processing Layer**

Validates the page-reference string and frame count.

**3. Simulation Layer**

Receives validated inputs and executes the selected page replacement algorithm.

**4. Algorithm Layer**

Contains independent implementations of FIFO, LRU, and Optimal.

**5. Result Processing Layer**

Stores simulation steps, calculates performance statistics, and prepares comparison results.

**6. Output Layer**

Displays the simulation table and performance summary.

## 8.2 Architecture Diagram

User Input → GUI → Input Parser → Simulation Controller → Selected Algorithm → Simulation Results → Performance Summary → GUI Display

The application processes the same input independently for each algorithm when comparison mode is selected.

# 9. FUNCTIONAL REQUIREMENTS

The system must:

1. Accept a page-reference string.
2. Accept the number of memory frames.
3. Allow selection of an individual algorithm or all algorithms.
4. Validate user inputs.
5. Initialize empty memory frames.
6. Process page references sequentially.
7. Identify page hits.
8. Identify page faults.
9. Perform page replacement when required.
10. Record frame contents after each request.
11. Calculate total hits and faults.
12. Calculate hit and fault ratios.
13. Display algorithm comparison results.
14. Allow the user to clear previous input and results.

# 10. NON-FUNCTIONAL REQUIREMENTS

**Accuracy:** All algorithms must correctly identify page hits, page faults, and replacement decisions.

**Usability:** The application should provide a simple and understandable graphical interface.

**Efficiency:** The application should process reference strings of reasonable length efficiently.

**Maintainability:** Different components should be separated into independent modules.

**Portability:** The source code should be compatible with MinGW-w64 GCC on Windows.

**Reliability:** Invalid inputs should be handled without crashing the application.

# 11. ALGORITHMS IMPLEMENTED

## 11.1 FIFO (First-In, First-Out)

FIFO is one of the simplest page replacement algorithms.

It replaces the page that entered memory first.

The algorithm follows the principle that the page that has remained in memory the longest should be replaced first.

### Working

1. Initialize all frames as empty.
2. Read the next page reference.
3. Check whether the page is already present.
4. If present, record a page hit.
5. If absent, record a page fault.
6. If an empty frame exists, load the page into it.
7. Otherwise, replace the oldest loaded page.
8. Move the replacement pointer to the next frame.
9. Repeat until all references are processed.

### Data Structure

FIFO uses an array of frames and a circular replacement pointer.

The pointer indicates the frame that should be replaced next.

### Advantages

- Simple to understand.
- Easy to implement.
- Low bookkeeping overhead.

### Disadvantages

- May replace frequently used pages.
- Does not consider recent page usage.
- Can suffer from Belady's anomaly.

## 11.2 LRU (Least Recently Used)

LRU replaces the page that has not been accessed for the longest period of time.

The algorithm assumes that pages used recently are more likely to be used again in the near future.

### Working

1. Initialize all frames as empty.
2. Read the next page reference.
3. Check whether the page is already present.
4. If present, record a hit and update its last-used information.
5. If absent, record a fault.
6. If an empty frame exists, load the page.
7. Otherwise, identify the page with the oldest last-used index.
8. Replace that page with the requested page.
9. Update its last-used index.
10. Continue processing the remaining references.

### Data Structure

LRU uses:

- An array to store memory frames.
- Another array to store the last-used reference index of each frame.

### Advantages

- Uses recent access history.
- Often performs better than FIFO.
- Does not exhibit Belady's anomaly.

### Disadvantages

- Requires additional tracking information.
- More complex than FIFO.
- Exact LRU implementations in real systems can have overhead.

## 11.3 Optimal Page Replacement

Optimal page replacement replaces the page whose next use is farthest in the future.

If a page will never be used again, it is an ideal replacement candidate.

### Working

1. Initialize all frames as empty.
2. Read the next page reference.
3. Check whether the page is already present.
4. If present, record a hit.
5. If absent, record a fault.
6. If an empty frame exists, load the requested page.
7. Otherwise, examine the remaining reference string.
8. Determine the next occurrence of each page currently in memory.
9. Replace the page whose next occurrence is farthest away.
10. If a page will never be used again, prioritize it for replacement.
11. Repeat until all references are processed.

### Data Structure

Optimal uses:

- An array for memory frames.
- A search through future page references.

### Advantages

- Produces the minimum possible page fault count for a known reference string.
- Provides a benchmark for evaluating other algorithms.
- Helps demonstrate the theoretical best-case replacement strategy.

### Disadvantages

- Requires knowledge of future page references.
- Cannot generally be implemented as a practical online replacement strategy.
- Requires additional computation to examine future references.

# 12. ALGORITHM WORKING EXAMPLES

Consider the following input:

**Page-reference string:**

`7 0 1 2 0 3 0 4 2 3`

**Number of frames:** 3

## 12.1 FIFO Simulation

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

Total page references = 10

Total hits = 1

Total faults = 9

## 12.2 LRU Simulation

| Page | Frame 1 | Frame 2 | Frame 3 | Status |
|---:|---:|---:|---:|---|
| 7 | 7 | - | - | Fault |
| 0 | 7 | 0 | - | Fault |
| 1 | 7 | 0 | 1 | Fault |
| 2 | 2 | 0 | 1 | Fault |
| 0 | 2 | 0 | 1 | Hit |
| 3 | 2 | 0 | 3 | Fault |
| 0 | 2 | 0 | 3 | Hit |
| 4 | 4 | 0 | 3 | Fault |
| 2 | 4 | 0 | 2 | Fault |
| 3 | 4 | 3 | 2 | Fault |

Total page references = 10

Total hits = 2

Total faults = 8

## 12.3 Optimal Simulation

| Page | Frame 1 | Frame 2 | Frame 3 | Status |
|---:|---:|---:|---:|---|
| 7 | 7 | - | - | Fault |
| 0 | 7 | 0 | - | Fault |
| 1 | 7 | 0 | 1 | Fault |
| 2 | 2 | 0 | 1 | Fault |
| 0 | 2 | 0 | 1 | Hit |
| 3 | 2 | 0 | 3 | Fault |
| 0 | 2 | 0 | 3 | Hit |
| 4 | 2 | 4 | 3 | Fault |
| 2 | 2 | 4 | 3 | Hit |
| 3 | 2 | 4 | 3 | Hit |

Total page references = 10

Total hits = 4

Total faults = 6

Note: When multiple pages have the same optimal replacement priority, any valid tie-breaking choice is acceptable. The displayed table uses the implementation's frame-order tie-breaking rule.

# 13. SOFTWARE DESIGN AND MODULE DESCRIPTION

The source code is divided into independent modules.

## 13.1 main.c

This is the main graphical interface module.

Responsibilities:

- Create the main application window.
- Create input controls.
- Create the algorithm selection dropdown.
- Handle button clicks.
- Read user input.
- Call the input parser.
- Execute the selected algorithms.
- Display simulation results.
- Display performance summaries.

The module uses the Windows API to create native controls.

Important Windows functions include:

- `CreateWindowExA()`
- `DefWindowProc()`
- `GetWindowTextA()`
- `SetWindowTextA()`
- `MessageBoxA()`
- `SendMessage()`
- `DispatchMessage()`

## 13.2 parser.c

This module validates and converts the page-reference string.

Responsibilities:

- Read the input string.
- Recognize spaces and commas as separators.
- Convert page numbers into integers.
- Reject invalid characters.
- Reject negative page numbers.
- Reject empty input.
- Enforce the maximum reference limit.

Main function:

`parse_references()`

The function returns success when the input is valid and failure when the input contains errors.

## 13.3 parser.h

Contains the function declaration required by other modules to use the input parser.

## 13.4 algorithms.c

Contains the implementation of all three algorithms:

- `simulate_fifo()`
- `simulate_lru()`
- `simulate_optimal()`

Each function receives the page-reference array, number of references, frame count, and a pointer to a result structure.

Each algorithm maintains its own frame state and records a snapshot after every reference.

## 13.5 algorithms.h

Contains the declarations of the three algorithm functions.

It allows the simulation controller to call the algorithm implementations.

## 13.6 simulator.c

This module acts as the central simulation controller.

Responsibilities:

- Validate simulation arguments.
- Select the appropriate algorithm.
- Call the corresponding algorithm function.
- Return the simulation result.

Main function:

`run_simulation()`

## 13.7 simulator.h

Defines the common data structures and function declarations shared across the application.

### SimulationStep

Stores the state of one page request.

Its fields include:

- `page`: requested page number.
- `frames`: memory frame contents after the request.
- `hit`: indicates whether the request was a hit.
- `replaced_page`: stores the removed page, if replacement occurred.

### SimulationResult

Stores the complete result of one algorithm execution.

Its fields include:

- Algorithm identifier.
- Number of references.
- Number of frames.
- Total hits.
- Total faults.
- Array of simulation steps.

# 14. DATA STRUCTURES USED

## 14.1 Arrays

Arrays are used to store:

- Page-reference sequences.
- Memory frame contents.
- Last-used reference indices.
- Simulation snapshots.

Arrays allow efficient indexed access to the data.

## 14.2 Circular Pointer

FIFO uses a circular pointer to maintain replacement order.

After replacing a page, the pointer moves to the next frame.

The modulo operator ensures that the pointer returns to the first frame after reaching the final frame.

## 14.3 Timestamp Array

LRU uses a timestamp array to store the most recent reference index of each resident page.

The smallest timestamp identifies the least recently used page.

## 14.4 Simulation Result Structure

A structure is used to group all the information associated with one algorithm execution.

This makes it easier for the GUI to retrieve and display the results.

# 15. PROGRAM EXECUTION WORKFLOW

The complete execution process follows these steps:

1. The user launches the application.
2. The application displays the graphical interface.
3. The user enters a page-reference string.
4. The user specifies the number of memory frames.
5. The user selects FIFO, LRU, Optimal, or Compare All.
6. The GUI reads the input values.
7. The parser validates the page-reference string.
8. The frame count is checked.
9. The simulation controller selects the algorithm.
10. The selected algorithm initializes its memory frames.
11. Each page reference is processed sequentially.
12. Hits and faults are identified.
13. Page replacement is performed when necessary.
14. The state of memory is saved after every request.
15. Total hits and faults are calculated.
16. The GUI displays the simulation table.
17. Performance ratios are displayed.
18. If comparison mode is selected, results from all three algorithms are shown.

# 16. GRAPHICAL USER INTERFACE

The application provides a native Windows graphical interface.

The interface contains the following components:

## 16.1 Page Reference Input

Allows users to enter a sequence of page numbers.

Example:

`7 0 1 2 0 3 0 4 2 3`

## 16.2 Frame Count Input

Allows users to specify the number of physical memory frames.

The application supports frame counts from 1 to 32.

## 16.3 Algorithm Selection

A dropdown allows the user to select:

1. FIFO
2. LRU
3. Optimal
4. Compare All

## 16.4 Run Simulation Button

Starts the simulation using the supplied input.

## 16.5 Clear Button

Clears previous input and results and restores the default settings.

## 16.6 Simulation Table

Displays:

- Algorithm name.
- Step number.
- Requested page.
- Frame contents.
- Hit or fault status.
- Replaced page.

## 16.7 Performance Summary

Displays:

- Total page hits.
- Total page faults.
- Hit ratio.
- Fault ratio.

# 17. INPUT VALIDATION AND ERROR HANDLING

Input validation prevents incorrect data from entering the simulation.

The application checks:

| Input | Validation |
|---|---|
| Empty page string | Rejected |
| Negative page numbers | Rejected |
| Non-numeric characters | Rejected |
| More than 500 references | Rejected |
| Zero frames | Rejected |
| Negative frames | Rejected |
| More than 32 frames | Rejected |
| Valid integers | Accepted |
| Space-separated values | Accepted |
| Comma-separated values | Accepted |

When invalid input is detected, the application displays an error message and prevents execution.

# 18. PERFORMANCE ANALYSIS

Performance is measured using page hits, page faults, hit ratio, and fault ratio.

## 18.1 Page Hit Ratio

The page hit ratio represents the percentage of requests that were already available in memory.

Formula:

Hit Ratio = (Total Page Hits / Total References) × 100

## 18.2 Page Fault Ratio

The page fault ratio represents the percentage of requests that were not available in memory.

Formula:

Fault Ratio = (Total Page Faults / Total References) × 100

## 18.3 Relationship

Total Page Hits + Total Page Faults = Total Page References

Hit Ratio + Fault Ratio = 100%

The second equation is subject to rounding when displayed as percentages.

## 18.4 Time Complexity

Let:

- n = number of page references.
- f = number of available memory frames.

| Algorithm | Time Complexity | Additional Working Space |
|---|---|---|
| FIFO | O(n × f) | O(f) |
| LRU | O(n × f) | O(f) |
| Optimal | O(n² × f) worst case | O(f) |

The complete simulation history requires O(n × f) storage.

Optimal requires additional searching through future references to determine which page should be replaced.

# 19. TESTING AND VALIDATION

Testing ensures that the application produces accurate results under different conditions.

## 19.1 Functional Test Cases

| Test | Input | Frames | Expected Behavior |
|---|---|---:|---|
| T01 | 7 0 1 2 0 3 0 4 2 3 | 3 | Correct algorithm comparison |
| T02 | 1 2 1 2 1 2 | 1 | Repeated hits after loading the same page |
| T03 | 1 2 3 | 5 | No replacements required |
| T04 | 9 | 1 | One page fault |
| T05 | 1,1,1,1 | 2 | One fault followed by three hits |
| T06 | Empty input | 3 | Error message displayed |
| T07 | 1 2 x 4 | 3 | Invalid input rejected |
| T08 | 1 2 3 | 0 | Invalid frame count rejected |

## 19.2 Expected Sample Results

For the reference string:

`7 0 1 2 0 3 0 4 2 3`

With three frames:

| Algorithm | Hits | Faults | Hit Ratio | Fault Ratio |
|---|---:|---:|---:|---:|
| FIFO | 1 | 9 | 10% | 90% |
| LRU | 2 | 8 | 20% | 80% |
| Optimal | 4 | 6 | 40% | 60% |

These results demonstrate how replacement policies influence the number of page faults.

# 20. SAMPLE RESULTS AND DISCUSSION

The sample comparison demonstrates that the three algorithms make different replacement decisions.

FIFO generates nine page faults because it replaces pages according to their arrival order.

LRU generates eight page faults because it considers recent page usage.

Optimal generates six page faults because it uses knowledge of future references to select the ideal victim page.

For this particular reference sequence, Optimal performs better than FIFO and LRU.

However, the performance of FIFO and LRU can change depending on the page-reference pattern.

The results should not be generalized to every possible reference string.

# 21. ADVANTAGES

1. Provides a practical demonstration of virtual memory concepts.
2. Implements three important page replacement algorithms.
3. Uses a graphical interface instead of relying only on console output.
4. Displays memory-frame contents after every request.
5. Clearly identifies page hits and page faults.
6. Calculates performance ratios automatically.
7. Supports comparison using identical inputs.
8. Uses modular C programming.
9. Helps students understand the differences between replacement strategies.
10. Can be extended with additional algorithms.

# 22. LIMITATIONS

1. The application supports only FIFO, LRU, and Optimal.
2. The maximum number of page references is limited to 500.
3. The maximum number of memory frames is limited to 32.
4. The simulator handles one reference sequence at a time.
5. Optimal requires knowledge of future references.
6. The application does not simulate actual disk I/O.
7. It does not model dirty pages or write-back operations.
8. It does not model actual page-table or TLB operations.
9. The simulation is educational and does not control real operating-system memory allocation.

# 23. APPLICATIONS

The concepts demonstrated in this project are relevant to:

- Operating-system memory management.
- Virtual memory systems.
- Computer architecture education.
- Memory allocation research.
- Operating-system algorithm analysis.
- Educational software and simulation tools.
- Understanding the impact of memory limitations on application performance.

# 24. FUTURE ENHANCEMENTS

The application can be improved by introducing the following features:

**1. Graphical Charts**

Display bar charts comparing the page faults generated by different algorithms.

**2. Animated Simulation**

Animate pages entering and leaving memory frames.

**3. Step-by-Step Playback**

Provide Next, Previous, Play, and Pause controls.

**4. Additional Algorithms**

Implement Clock, Second Chance, LFU, and MFU algorithms.

**5. CSV Export**

Allow users to export simulation results for additional analysis.

**6. Improved Interface**

Introduce a modern dashboard, improved colors, and visual status indicators.

**7. Multiple Test Scenarios**

Allow users to save and compare multiple reference strings.

# 25. TEAM RESPONSIBILITIES

The project is divided among five members to encourage modular development and collaboration.

| Member | Responsibility | Main Contribution |
|---|---|---|
| Member 1 | FIFO Algorithm | FIFO implementation and testing |
| Member 2 | LRU Algorithm | LRU implementation and testing |
| Member 3 | Optimal Algorithm | Optimal implementation and simulation integration |
| Member 4 | GUI Development | Windows GUI and event handling |
| Member 5 | Input Validation and Documentation | Parser, testing, integration, and documentation |

All members should understand the overall architecture, theoretical concepts, and working of the three algorithms.

# 26. CONCLUSION

The Page Replacement Algorithm Simulator successfully demonstrates the working of three fundamental memory management algorithms: FIFO, LRU, and Optimal.

Through a graphical interface, users can enter page-reference sequences, specify the number of memory frames, and observe how different algorithms manage limited physical memory.

The application identifies page hits and page faults, records memory-frame changes, and calculates performance statistics.

The comparative analysis demonstrates that the choice of page replacement algorithm has a significant influence on memory management performance.

The project provides practical experience in C programming, data structures, modular software development, Windows GUI programming, and operating-system concepts.

Overall, the simulator serves as an educational tool for understanding virtual memory and evaluating page replacement strategies.

# 27. REFERENCES

1. Abraham Silberschatz, Peter B. Galvin, and Greg Gagne, *Operating System Concepts*.
2. William Stallings, *Operating Systems: Internals and Design Principles*.
3. Andrew S. Tanenbaum and Herbert Bos, *Modern Operating Systems*.
4. Microsoft Learn, Windows API documentation: https://learn.microsoft.com/en-us/windows/win32/
5. GCC Documentation: https://gcc.gnu.org/onlinedocs/
6. Operating Systems course materials and lecture notes.

---

**END OF PROJECT DOCUMENTATION**