# Interactive Deadlock Simulation System

## Overview
This is an interactive Operating System simulator that demonstrates how an OS handles process resource allocation and deadlocks. It uses a high-performance C backend to compute matrix operations and algorithms, connected to a Python Tkinter graphical user interface (GUI) for easy interaction.

## Features
* **Banker's Algorithm:** Determines if the system is in a Safe or Unsafe state and generates the safe sequence.
* **Resource Allocation & Requests:** Simulates processes requesting resources with tentative allocation and rollback mechanisms.
* **Deadlock Detection:** Identifies permanently blocked processes using request and allocation matrices.
* **Deadlock Recovery:** Automatically preempts resources by aborting a deadlocked victim process to return the system to a functional state.
* **Resource Allocation Graph (RAG):** Visualizes assignment and request edges between processes and resources.

## Project Structure
The backend logic is modularized into distinct C files, tied together by a Python frontend:
* `common.h` / `state.c`: Manages global matrices, initialization, and memory state.
* `bankers.c`: Implements the Banker's Algorithm for safety checks.
* `resource.c`: Handles resource requests, tentative allocations, and rollbacks.
* `deadlock.c`: Runs deadlock detection and recovery logic.
* `gui.py`: The Tkinter frontend that interfaces with the C shared library via `ctypes`.
* `Makefile`: Compiles the C files into the `libdeadlock.so` shared library.

## Prerequisites
To run this on Linux or WSL, you need a C compiler and Python's GUI library installed.

```bash
sudo apt update
sudo apt install gcc make python3 python3-tk
```
*(Note: If using WSL, ensure your X11 server like WSLg or VcXsrv is running to display the GUI).*

## How to Build and Run

1. **Compile the Backend:**
   Open your terminal in the project directory and run:
   ```bash
   make
   ```
   This compiles the C files and generates a `libdeadlock.so` file.

2. **Launch the GUI:**
   ```bash
   python3 gui.py
   ```

3. **Clean Build Files (Optional):**
   To remove compiled object files and the shared library:
   ```bash
   make clean
   ```

## Usage Flow
Once the GUI opens, follow this sequence to test the simulation:
1. **Init System:** Enter the total number of processes and resource types.
2. **Set Available Vector:** Input the currently available instances of each resource type.
3. **Add Process:** For each process (0 to N-1), enter its Maximum requirement matrix and current Allocation matrix. 
4. **Test Algorithms:** 
   * Click **Run Banker's Algorithm** to verify system safety.
   * Click **Request Resources** to simulate a process asking for more instances.
   * Click **Detect Deadlock** or **Recover from Deadlock** to test error states.
   * Click **Display Need & State** at any time to print the current matrix snapshots to the console.