# Deadlock Lab: Interactive Deadlock Simulation System

A local Streamlit application for exploring operating-system resource allocation, safe states, deadlock detection, and recovery.

## Features

- Dashboard with process/resource counts and Available vector
- Editable Allocation, Maximum, and Outstanding Request matrices
- Automatically calculated Need matrix
- Banker's safety algorithm with safe sequence
- Transactional resource request checks
- Multiple-instance deadlock detection based on outstanding requests
- Resource Allocation Graph (RAG)
- Release resources and terminate-process recovery action
- Built-in safe and deliberate-deadlock scenarios
- Event log
- Basic algorithm tests

## Requirements

- Python 3.10 or newer recommended

## Setup

### Windows (PowerShell or VS Code terminal)

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install -r requirements.txt
python -m streamlit run app.py
```

If PowerShell blocks activation, you can use the venv Python directly:

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe -m streamlit run app.py
```

### macOS / Linux

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
python -m streamlit run app.py
```

Streamlit normally opens `http://localhost:8501` in your browser.

## Suggested demo

1. Click **Load safe Banker example**.
2. Open **Overview** and run Banker's safety check. A safe sequence should appear.
3. Open **Resource Graph** to view the current allocations.
4. Click **Load deadlock example**.
5. Run deadlock detection. P0 and P1 should be identified as deadlocked.
6. Open **Requests & Recovery**, terminate one process, then run detection again.
7. Edit the matrices to experiment with other scenarios.

## Directory layout

```text
deadlock_lab_streamlit/
├── app.py
├── requirements.txt
├── README.md
├── .gitignore
├── algorithms/
│   ├── __init__.py
│   ├── bankers.py
│   └── detection.py
├── components/
│   ├── __init__.py
│   └── graph.py
└── tests/
    └── test_algorithms.py
```

## Test the algorithms

```bash
python -m pytest
```

## Important modeling notes

- `Need = Maximum - Allocation`.
- An unsafe state is not automatically a deadlock.
- Banker's Algorithm checks whether a request can be granted while preserving a safe state.
- Deadlock detection uses the currently recorded outstanding Request matrix. Use the built-in deadlock scenario to demonstrate this explicitly.
- The recovery action simulates terminating a process by clearing its allocation, maximum claim, and outstanding request. A real operating system would need additional process-lifecycle rules.
- This is an educational simulator, not an actual operating-system resource manager.
