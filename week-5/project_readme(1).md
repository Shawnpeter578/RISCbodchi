# Page Replacement Simulator — Team Riscbodchi

A modular, high-performance operating system simulation suite implementing FIFO, LRU, and Optimal page replacement algorithms. Features a C-backed computational engine, automated unit testing, and an interactive Python Tkinter graphical user interface.

## Features
* **Modular C Core**: Implements pure, zero-dependency simulation logic for FIFO, LRU, and Optimal algorithms.
* **Unix-Domain IPC**: Communicates efficiently between the C backend and Python GUI via newline-delimited JSON over Unix sockets.
* **Interactive GUI**: Built using Python's native Tkinter library for step-by-step frame inspection and side-by-side performance comparison.
* **Discovery-Based Testing**: Integrates strict unit testing directly into the Makefile build system.

---

## Project Structure
```text
.
├── Makefile                # Discovery-based build and test automation
├── README.md               # Project guide and instructions
├── gui.py                  # Python Tkinter graphical frontend
├── runit.sh                # Automated launcher script for backend and GUI
├── include/                # Shared header files
│   └── orchestrator.h      # Core data structures and function prototypes
└── src/                    # C Source files
    ├── backend.c           # Unix socket IPC server and JSON protocol
    ├── backend_main.c      # Backend entry point
    ├── display.c           # Console formatting and printing helpers
    ├── fifo.c              # FIFO algorithm implementation
    ├── lru.c               # LRU algorithm implementation
    └── orchestrator.c      # Validation and simulation dispatcher
```

---

## Getting Started & Execution

### Prerequisites
* GCC compiler supporting the C11 standard (`build-essential` on Linux/WSL).
* Python 3 with Tkinter installed.

### Quick Start (Automated Launcher)
To build the project and launch the visual simulator with a single command:
```bash
chmod +x runit.sh
./runit.sh
```

### Manual Build & Test
1. Compile binaries and execute all unit tests:
   ```bash
   make
   ```
2. Start the C backend server:
   ```bash
   make run
   ```
3. In a separate terminal window, launch the Python GUI:
   ```bash
   python3 gui.py
   ```