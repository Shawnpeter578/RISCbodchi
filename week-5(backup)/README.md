# Week 5: Page Replacement Algorithm Simulator

## Overview

The Page Replacement Algorithm Simulator is a Python GUI application designed to demonstrate and compare three page replacement algorithms used in operating systems:

- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal (OPT)

The application simulates how an operating system manages memory when a process requests pages that may not currently be loaded into available memory frames.

Users can enter a page-reference string, specify the number of available frames, execute the algorithms, and compare their performance through page hits, page faults, and frame contents.

## Features

- Enter a page-reference string.
- Specify the number of available page frames.
- Simulate FIFO, LRU, and Optimal algorithms.
- Compare all three algorithms using the same input.
- View frame contents after each page request.
- Calculate page hit and page fault counts.
- Display page hit and page fault ratios.
- Visualize the behavior of page replacement algorithms.

## Topics Covered

### 1. Virtual Memory

Virtual memory allows an operating system to execute programs without requiring all of their pages to be loaded into physical memory simultaneously.

### 2. Paging

Paging divides a process's virtual memory into fixed-size pages and physical memory into corresponding frames.

When a process requests a page, the operating system checks whether the page is already in memory.

### 3. Page Fault

A page fault occurs when a process requests a page that is not currently loaded into memory.

If all frames are occupied, the operating system may need to replace an existing page to make room for the requested page.

### 4. Page Replacement

Page replacement algorithms determine which page should be removed from memory when a new page must be loaded and no free frame is available.

### 5. Page Hit

A page hit occurs when the requested page is already present in one of the available frames.

### 6. Page Fault Ratio

The page fault ratio is the proportion of page requests that result in a page fault.

`Page Fault Ratio = Page Faults / Total Page Requests`

### 7. Page Hit Ratio

The page hit ratio is the proportion of page requests that find the requested page already in memory.

`Page Hit Ratio = Page Hits / Total Page Requests`

## Algorithms Implemented

### FIFO (First-In, First-Out)

FIFO replaces the page that has been in memory for the longest time.

It follows a queue-based approach. The earliest loaded page is removed first when a replacement is needed.

**Example:**

If the frames contain:

```text
[1, 2, 3]
```

and page `4` is requested, FIFO removes page `1` and loads page `4`.

The frames become:

```text
[2, 3, 4]
```

### LRU (Least Recently Used)

LRU replaces the page that has not been used for the longest time.

It uses the recent history of page references to decide which page should be replaced.

**Example:**

If the frames contain:

```text
[1, 2, 3]
```

and page `2` was used most recently, while page `1` was used least recently, LRU replaces page `1` when a new page is requested.

### Optimal (OPT)

Optimal replaces the page whose next use is farthest in the future.

It produces the minimum possible number of page faults for a given reference string and number of frames.

However, it requires knowledge of future page requests, making it impractical for real operating systems to implement exactly.

It is useful as a theoretical benchmark for comparing other algorithms.

## Requirements

- Python 3.10 or newer recommended.
- Tkinter, included with most standard Windows Python installations.
- Additional dependencies listed in `requirements.txt`, if applicable.

## Setup and Installation

### Step 1: Clone the repository

```bash
git clone https://github.com/Shawnpeter578/RISCbodchi.git
```

### Step 2: Navigate to the Week 5 folder

```bash
cd RISCbodchi/week-5
```

If your Week 5 folder has a different name, navigate to the directory containing `main.py`.

### Step 3: Create a virtual environment

#### Windows

```powershell
py -m venv .venv
```

#### macOS / Linux

```bash
python3 -m venv .venv
```

### Step 4: Activate the virtual environment

#### Windows PowerShell

```powershell
.\.venv\Scripts\Activate.ps1
```

#### macOS / Linux

```bash
source .venv/bin/activate
```

### Step 5: Install dependencies

If the project contains a `requirements.txt` file, run:

```bash
python -m pip install -r requirements.txt
```

If there are no external dependencies, this step can be skipped.

## How to Run the Application

From the project directory, run:

```bash
python main.py
```

The GUI application should open in a separate window.

If your system uses `python3` instead of `python`, run:

```bash
python3 main.py
```

## How to Use the Simulator

### Step 1: Enter the page-reference string

Enter a sequence of page numbers separated by commas.

Example:

```text
1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4
```

### Step 2: Specify the number of frames

For example:

```text
3
```

This means the simulated memory can hold three pages at a time.

### Step 3: Select an algorithm

Choose FIFO, LRU, or Optimal to simulate the page-reference string.

### Step 4: Run the simulation

Start the simulation to observe the page requests and how the algorithm handles them.

### Step 5: Compare the algorithms

Run the comparison feature to evaluate FIFO, LRU, and Optimal using the same input.

Review the page faults, page hits, and frame contents produced by each algorithm.

## Example Input

Page-reference string:

```text
1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4
```

Number of frames:

```text
3
```

This input can be used to compare the behavior of FIFO, LRU, and Optimal.

## Example Interpretation

Suppose a process requests page `1`.

- If page `1` is already in a frame, the request results in a page hit.
- If page `1` is not in memory, the request results in a page fault.
- If there is an empty frame, the page can be loaded without replacing another page.
- If all frames are occupied, the selected algorithm determines which page to replace.

## Project Structure

```text
PageReplacementSimulator/
├── main.py
├── gui.py
├── fifo.py
├── lru.py
├── optimal.py
├── requirements.txt
├── README.md
├── .gitignore
├── screenshots/
└── docs/
```

### Important Files

- `main.py`: Application entry point.
- `gui.py`: GUI implementation.
- `fifo.py`: FIFO page replacement algorithm.
- `lru.py`: LRU page replacement algorithm.
- `optimal.py`: Optimal page replacement algorithm.
- `requirements.txt`: External Python dependencies.
- `screenshots/`: Example screenshots.
- `docs/`: Supporting documentation.

## Troubleshooting

### Python is not recognized

Verify that Python is installed:

```bash
python --version
```

On Windows, you can also use:

```powershell
py --version
```

### Tkinter is not available

On Windows, use a standard Python installation that includes Tcl/Tk.

On Ubuntu or Debian Linux, install Tkinter using:

```bash
sudo apt install python3-tk
```

### The application does not launch

Ensure that you are in the directory containing `main.py`.

Then run:

```bash
python main.py
```

### Missing Python packages

Install the project's dependencies:

```bash
python -m pip install -r requirements.txt
```

## Learning Outcomes

After completing this project, students should be able to:

- Explain virtual memory and paging.
- Understand page faults and page hits.
- Explain why page replacement is required.
- Implement FIFO, LRU, and Optimal algorithms.
- Compare page replacement algorithms using the same reference string.
- Calculate page hit and page fault ratios.
- Understand the trade-offs between practical and theoretical algorithms.
- Develop a Python GUI application for operating-system simulation.

## Conclusion

The Page Replacement Algorithm Simulator provides a practical way to understand memory management in operating systems.

By running the same page-reference string through FIFO, LRU, and Optimal, users can observe how replacement policies affect memory contents and page fault behavior.

The Optimal algorithm provides a theoretical benchmark, while FIFO and LRU demonstrate practical approaches to page replacement.

## Team

Add your team members' names, student IDs, class/section, and instructor here.
