# Week 4: Deadlock Lab
## Interactive Deadlock Simulation System

Deadlock Lab is a local, interactive Streamlit application designed to demonstrate operating-system resource allocation, deadlock prevention concepts, safe states, deadlock detection, and recovery.

The application allows users to experiment with process and resource allocation matrices, execute deadlock-related algorithms, and visualize resource allocation through an interactive dashboard.

## Features

- Dashboard displaying process and resource counts, along with the Available vector.
- Editable Allocation, Maximum, and Outstanding Request matrices.
- Automatically calculated Need matrix.
- Banker's safety algorithm with safe sequence detection.
- Transactional resource request checks.
- Multiple-instance deadlock detection.
- Resource Allocation Graph (RAG) visualization.
- Resource release and process termination recovery actions.
- Built-in safe and deliberate-deadlock scenarios.
- Event log for tracking simulation operations.
- Basic algorithm tests.

## Topics Covered

### 1. Deadlock

A deadlock occurs when a group of processes are permanently blocked because each process is waiting for a resource held by another process in the group.

### 2. Resource Allocation

Processes require resources to execute. The simulator models resource allocation using matrices.

### 3. Safe and Unsafe States

A safe state is one in which there exists at least one sequence of process execution that allows every process to finish.

An unsafe state does not guarantee that a deadlock has occurred. It means the system cannot guarantee that all processes can finish.

### 4. Banker's Algorithm

Banker's Algorithm checks whether a resource request can be granted without moving the system into an unsafe state.

### 5. Deadlock Detection

The detection algorithm examines outstanding resource requests and currently available resources to determine whether processes are deadlocked.

### 6. Deadlock Recovery

The simulator demonstrates recovery by terminating a process and releasing its allocated resources.

## How It Works

The simulator maintains the following resource allocation matrices:

| Matrix | Description |
|---|---|
| Allocation | Resources currently assigned to each process |
| Maximum | Maximum resources each process may require |
| Need | Remaining resources each process may require |
| Request | Outstanding resource requests |
| Available | Resources currently available in the system |

The Need matrix is calculated as:

`Need = Maximum - Allocation`

The application uses these matrices to perform safety checks, resource request validation, deadlock detection, and recovery operations.

## Requirements

- Python 3.10 or newer
- pip
- A terminal such as PowerShell, Windows Terminal, or VS Code Terminal
- A web browser

## Setup and Installation

### Step 1: Clone the repository

```bash
git clone https://github.com/Shawnpeter578/RISCbodchi.git
```

### Step 2: Navigate to the Week 4 folder

```bash
cd RISCbodchi/week-4
```

> If the Week 4 folder has a different name in the repository, navigate to the folder containing `app.py` and `requirements.txt`.

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

If PowerShell blocks activation, use the venv Python directly as shown in the alternative run instructions below.

#### macOS / Linux

```bash
source .venv/bin/activate
```

### Step 5: Install dependencies

```bash
python -m pip install -r requirements.txt
```

## How to Run the Application

After installing the dependencies, run:

```bash
python -m streamlit run app.py
```

Streamlit will start the application and display a local URL, normally:

```text
http://localhost:8501
```

Open this URL in your browser to access the Deadlock Lab dashboard.

To stop the application, press:

```text
Ctrl + C
```

in the terminal where Streamlit is running.

## Alternative Run Instructions for Windows

If virtual environment activation is blocked or unavailable, run the following commands from the Week 4 project folder:

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
```

Then launch the app:

```powershell
.\.venv\Scripts\python.exe -m streamlit run app.py
```

## How to Use the Application

### Scenario 1: Explore a Safe State

1. Start the application.
2. Click **Load safe Banker example**.
3. Open the **Overview** section.
4. Run the Banker's safety check.
5. Examine the safe sequence displayed by the algorithm.
6. Open **Resource Graph** to inspect the resource allocation.

### Scenario 2: Explore a Deadlock

1. Click **Load deadlock example**.
2. Run the deadlock detection algorithm.
3. Observe the processes identified as deadlocked.
4. Open the Resource Allocation Graph.
5. Examine the resource allocation and outstanding requests.

### Scenario 3: Recover from Deadlock

1. Load the deadlock example.
2. Open **Requests & Recovery**.
3. Terminate one of the deadlocked processes.
4. Run deadlock detection again.
5. Observe how releasing resources changes the system state.

### Scenario 4: Experiment with Resource Allocation

1. Edit the Allocation matrix.
2. Change the Maximum matrix.
3. Modify the outstanding Request matrix.
4. Observe how the Need and Available vectors change.
5. Run the safety algorithm and deadlock detection again.

## Testing the Algorithms

The project includes basic algorithm tests.

From the Week 4 project folder, run:

```bash
python -m pytest
```

To run the tests with more detailed output:

```bash
python -m pytest -v
```

## Project Structure

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

### Important Files

- `app.py`: Main Streamlit application.
- `requirements.txt`: Python dependencies.
- `algorithms/bankers.py`: Banker's safety algorithm implementation.
- `algorithms/detection.py`: Deadlock detection implementation.
- `components/graph.py`: Resource Allocation Graph visualization.
- `tests/test_algorithms.py`: Tests for the algorithms.

## Learning Outcomes

After completing this project, students should be able to:

- Explain deadlock and resource allocation in operating systems.
- Understand the difference between safe and unsafe states.
- Apply Banker's Algorithm to check system safety.
- Understand deadlock detection with multiple resource instances.
- Interpret a Resource Allocation Graph.
- Demonstrate basic deadlock recovery techniques.
- Use Python and Streamlit to build an interactive operating-system simulator.

## Important Notes

- An unsafe state is not necessarily a deadlock.
- Banker's Algorithm checks whether a resource request can be granted while preserving a safe state.
- Deadlock detection uses the currently recorded outstanding Request matrix.
- The recovery action simulates terminating a process by clearing its allocation, maximum claim, and outstanding request.
- This is an educational simulator, not an actual operating-system resource manager.
