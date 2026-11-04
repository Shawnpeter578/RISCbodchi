# Deadlock Lab
## Interactive Deadlock Simulation and Resource Management System

<p align="center">
  <strong>An Interactive Operating Systems Simulator for Deadlock Avoidance, Detection, and Recovery</strong>
</p>

---

## 1. Project Overview

**Deadlock Lab** is an interactive desktop application designed to demonstrate and simulate deadlock management techniques used in modern operating systems.

The project provides a visual environment in which users can configure processes, define resource types, allocate resources, submit resource requests, and observe how an operating system handles potential deadlocks.

It implements important operating system concepts, including:

- Resource allocation and management
- Process-resource relationships
- Banker's Algorithm
- Safe and unsafe system states
- Deadlock detection
- Deadlock recovery
- Resource Allocation Graph (RAG) exploration
- Dynamic resource requests

Unlike a theoretical explanation or static flowchart, Deadlock Lab allows users to experiment with different resource configurations and observe how the system responds.

The application uses **C for the core simulation logic** and **Python with PySide6 for the graphical user interface**.

The C backend is compiled into a Windows Dynamic Link Library (DLL), which is accessed from Python using `ctypes`.

---

## 2. Problem Statement

In a multiprogramming operating system, multiple processes execute concurrently and compete for limited system resources.

Examples of resources include:

- Printers
- Memory
- Files
- I/O devices
- Database locks
- CPU-related system resources

A process may request resources that are currently held by another process. If processes wait indefinitely for resources held by one another, the system can enter a deadlock.

Deadlocks can cause:

- Processes to stop making progress.
- Resources to remain occupied indefinitely.
- Reduced system throughput.
- Poor resource utilization.
- Applications to become unresponsive.

Understanding how operating systems prevent, avoid, detect, and recover from deadlocks is therefore essential.

**The objective of Deadlock Lab is to simulate these situations and demonstrate how different deadlock management algorithms work in practice.**

---

## 3. Project Objectives

The primary objectives are:

1. Understand the concept of deadlocks in operating systems.
2. Simulate multiple processes competing for shared resources.
3. Implement the Banker's Algorithm for deadlock avoidance.
4. Determine whether a system is in a safe or unsafe state.
5. Generate safe execution sequences.
6. Simulate resource requests and allocation decisions.
7. Detect deadlocks using resource request information.
8. Identify processes involved in deadlocks.
9. Demonstrate deadlock recovery through process termination and resource release.
10. Explore resource allocation relationships.
11. Provide an interactive graphical interface for experimentation.
12. Integrate a C-based simulation engine with a Python GUI.

---

# 4. Technologies Used

| Technology | Purpose |
|---|---|
| C | Core operating system simulation logic |
| Python | GUI application and integration |
| PySide6 | Desktop graphical interface |
| MinGW-w64 GCC | Compile C source code |
| Windows DLL | Expose compiled C functionality |
| Python ctypes | Communicate with the C DLL |
| Git & GitHub | Version control and collaboration |

### Why C and Python?

C is used for the simulation engine because it provides efficient memory management, direct control over data structures, and a good environment for implementing operating system algorithms.

Python is used for the graphical interface because it allows rapid development of interactive desktop applications.

Combining both technologies separates the simulation logic from the user interface.

---

# 5. System Architecture

The application follows a layered architecture.

```text
                  USER
                    |
                    v
          +--------------------+
          |   PySide6 GUI      |
          |                    |
          | Overview           |
          | Configure System   |
          | Resource Matrices  |
          | Simulation         |
          | RAG Explorer       |
          +--------------------+
                    |
                    v
          +--------------------+
          | Python Integration |
          |                    |
          | backend.py         |
          | ctypes             |
          +--------------------+
                    |
                    v
          +--------------------+
          |    C Backend DLL   |
          |                    |
          | System Management |
          | Banker's Algorithm |
          | Deadlock Detection |
          | Recovery           |
          | RAG Operations     |
          +--------------------+
                    |
                    v
          +--------------------+
          | Simulation State   |
          |                    |
          | Allocation         |
          | Maximum            |
          | Need               |
          | Available          |
          | Request            |
          +--------------------+
```

## 5.1 Frontend Layer

The frontend is developed using Python and PySide6.

It is responsible for:

- Displaying the application dashboard.
- Accepting user input.
- Configuring processes and resources.
- Displaying allocation and maximum matrices.
- Triggering simulation operations.
- Displaying algorithm results.
- Showing system status.

The frontend does not independently implement the operating system algorithms. It sends data to the C backend and displays the returned results.

## 5.2 Integration Layer

The integration layer connects Python to the C backend.

It uses Python's `ctypes` library to load and call functions exported by the compiled DLL.

The general communication process is:

1. The user enters data in the GUI.
2. Python validates and prepares the input.
3. Python calls the corresponding C function.
4. The C backend processes the data.
5. The backend returns a result or status message.
6. Python displays the result in the GUI.

## 5.3 Backend Layer

The backend contains the core simulation logic.

It manages:

- Process information.
- Resource information.
- Allocation matrices.
- Maximum requirement matrices.
- Need matrices.
- Available resource vectors.
- Outstanding resource requests.

The backend performs the calculations required by the algorithms.

---

# 6. Project Directory Structure

```text
DeadlockSimulation_Python_C/
│
├── backend/
│   │
│   ├── include/
│   │   └── deadlock_api.h
│   │
│   ├── system.c
│   ├── banker.c
│   ├── deadlock.c
│   ├── rag.c
│   │
│   └── build.bat
│
├── frontend/
│   │
│   ├── main.py
│   ├── dashboard.py
│   └── backend.py
│
├── tests/
│   └── test_scenarios.md
│
├── docs/
│
├── requirements.txt
├── README.md
└── .gitignore
```

### Description of important files

| File | Responsibility |
|---|---|
| `main.py` | Application entry point |
| `dashboard.py` | GUI pages, controls, and result display |
| `backend.py` | Python-to-C communication using ctypes |
| `system.c` | Core system state and simulation operations |
| `banker.c` | Banker's Algorithm module |
| `deadlock.c` | Deadlock detection and recovery module |
| `rag.c` | Resource Allocation Graph operations |
| `deadlock_api.h` | C API declarations |
| `build.bat` | Compiles C files into the DLL |
| `requirements.txt` | Python dependencies |
| `test_scenarios.md` | Sample test cases |

**Implementation note:** In the current starter implementation, much of the C simulation logic is located in `system.c`. The other C source files provide module boundaries and can be expanded as the implementation is modularized. The actual function implementation should be checked in the corresponding source file before making changes.

---

# 7. Theoretical Background

## 7.1 What is a Deadlock?

A deadlock is a situation in which a group of processes becomes permanently blocked because every process is waiting for a resource held by another process in the group.

Consider two processes:

- Process P0 holds Resource R0 and requests R1.
- Process P1 holds Resource R1 and requests R0.

P0 cannot continue until R1 becomes available.

P1 cannot continue until R0 becomes available.

Neither process can proceed.

This creates a deadlock.

### Example

```text
P0 holds R0 and waits for R1.

P1 holds R1 and waits for R0.
```

Dependency:

```text
P0 ----requests----> R1
P1 ----requests----> R0

R0 ----allocated---> P0
R1 ----allocated---> P1
```

Both processes are waiting indefinitely.

## 7.2 Four Necessary Conditions for Deadlock

According to operating system theory, a deadlock can occur only when all four conditions exist simultaneously.

### 1. Mutual Exclusion

At least one resource must be non-shareable.

Only one process can use that resource at a time.

Example: A printer being used by one process.

### 2. Hold and Wait

A process holds one or more resources while waiting for additional resources.

Example: P0 holds a printer but requests a scanner.

### 3. No Preemption

Resources cannot be forcibly taken away from a process. They must be released voluntarily.

### 4. Circular Wait

A circular chain of processes exists where every process waits for a resource held by the next process.

Example:

```text
P0 -> P1 -> P2 -> P0
```

All four conditions are necessary for deadlock.

---

# 8. Resource Management Concepts

The simulator uses several matrices and vectors to represent the current system state.

Assume:

- `n` = Number of processes.
- `m` = Number of resource types.

## 8.1 Allocation Matrix

The Allocation Matrix represents the number of instances of each resource currently allocated to each process.

For example:

```text
Allocation:

       R0  R1  R2

P0      0   1   0
P1      2   0   0
P2      3   0   2
P3      2   1   1
P4      0   0   2
```

Each row represents a process.

Each column represents a resource type.

## 8.2 Maximum Matrix

The Maximum Matrix represents the maximum number of instances of each resource that a process may require.

```text
Maximum:

       R0  R1  R2

P0      7   5   3
P1      3   2   2
P2      9   0   2
P3      2   2   2
P4      4   3   3
```

## 8.3 Need Matrix

The Need Matrix represents the remaining resources each process may still require to complete.

It is calculated using:

```text
Need = Maximum - Allocation
```

For example, if P0 has:

```text
Maximum   = 7 5 3
Allocation = 0 1 0
```

Then:

```text
Need = 7 4 3
```

## 8.4 Available Vector

The Available Vector represents the number of currently available instances of each resource.

It is calculated using:

```text
Available = Total Resources - Allocated Resources
```

For example:

```text
Total     = 10 5 7
Allocated =  7 2 5
Available =  3 3 2
```

## 8.5 Request Matrix

The Request Matrix represents the outstanding resource requests made by processes.

It is particularly important for deadlock detection.

A process may have an outstanding request that cannot currently be satisfied.

---

# 9. Deadlock Avoidance: Banker's Algorithm

The Banker's Algorithm is a deadlock avoidance algorithm introduced by Edsger W. Dijkstra.

Its main purpose is to determine whether granting a resource request will leave the system in a safe state.

The algorithm does not simply check whether resources are available.

It checks whether the system can still complete all processes in some order.

## 9.1 What is a Safe State?

A system is in a safe state if there exists at least one order in which every process can obtain its remaining required resources, finish execution, and release its allocated resources.

This order is called a Safe Sequence.

Example:

```text
P1 -> P3 -> P4 -> P0 -> P2
```

This means:

1. P1 can finish with the currently available resources.
2. P1 releases its allocated resources.
3. P3 can now finish.
4. P3 releases its resources.
5. P4 can finish.
6. P0 can finish.
7. P2 can finish.

Therefore, the system is safe.

## 9.2 What is an Unsafe State?

An unsafe state is a state in which the system cannot establish a complete safe sequence using the Banker's safety test.

An unsafe state does not necessarily mean that a deadlock has already occurred.

It means that the system cannot guarantee safe completion under the declared maximum resource requirements.

## 9.3 Algorithm

The safety algorithm uses:

- `Work`: A temporary vector representing available resources.
- `Finish`: An array indicating whether each process can finish.
- `Need`: Remaining resource requirements.
- `Allocation`: Resources currently held by processes.

### Steps

1. Initialize `Work = Available`.
2. Set every `Finish[i] = false`.
3. Find a process whose `Finish[i]` is false and whose Need is less than or equal to Work.
4. Assume that process finishes.
5. Add its allocated resources back to Work.
6. Set `Finish[i] = true`.
7. Repeat until no more processes can be selected.
8. If all processes can finish, the state is SAFE.
9. Otherwise, the state is UNSAFE.

### Pseudocode

```text
Work = Available

For each process:
    Finish[i] = false

While an unfinished process can execute:

    Find process Pi such that:

        Finish[i] == false

        Need[i] <= Work

    If such process exists:

        Work = Work + Allocation[i]

        Finish[i] = true

        Add Pi to safe sequence

If all processes finish:

    System is SAFE

Else:

    System is UNSAFE
```

## 9.4 Complexity

For `n` processes and `m` resource types, the standard safety algorithm has a worst-case time complexity of approximately:

```text
O(n² × m)
```

This is because the algorithm may repeatedly scan all processes and compare their resource requirements.

---

# 10. Resource Request Algorithm

The simulator also supports resource requests made by individual processes.

Suppose process P1 requests:

```text
Request = 1 0 2
```

The backend checks whether the request can safely be granted.

### Request validation

First, check:

```text
Request <= Need
```

If the request exceeds the process's declared maximum requirement, it must be rejected.

Next, check:

```text
Request <= Available
```

If the requested resources are not currently available, the process must wait.

Finally, temporarily allocate the resources and run the safety algorithm.

If the resulting state is safe, the request can be granted.

If the resulting state is unsafe, the allocation is rolled back and the request is not granted.

### Decision flow

```text
        New Request
             |
             v
     Request <= Need?
        /       \
      No         Yes
      |           |
    Reject        v
             Request <= Available?
                /       \
              No         Yes
              |           |
            Wait          v
                    Temporarily Allocate
                           |
                           v
                    Run Safety Check
                      /         \
                   Safe        Unsafe
                    |             |
                  Grant         Rollback
```

---

# 11. Deadlock Detection

Deadlock detection is different from deadlock avoidance.

Banker's Algorithm attempts to avoid entering unsafe states.

Deadlock detection examines an existing system state to determine whether processes are currently deadlocked.

For systems with multiple instances of resource types, a detection algorithm uses:

- Available vector.
- Allocation matrix.
- Request matrix.

### Detection algorithm

1. Initialize Work using Available.
2. For processes with no allocated resources, initialize Finish to true. For other processes, initialize it to false.
3. Find an unfinished process whose outstanding Request is less than or equal to Work.
4. Assume that process completes.
5. Release its allocated resources back to Work.
6. Mark it finished.
7. Repeat while possible.
8. Any process that remains unfinished is identified as deadlocked by the detection algorithm.

### Pseudocode

```text
Work = Available

For each process Pi:

    If Allocation[i] == 0:

        Finish[i] = true

    Else:

        Finish[i] = false

While an unfinished process can execute:

    Find Pi such that:

        Finish[i] == false

        Request[i] <= Work

    If found:

        Work = Work + Allocation[i]

        Finish[i] = true

After the loop:

    Every unfinished process is deadlocked
```

### Example

Consider two processes and two resource types.

Total resources:

```text
2 2
```

Allocation:

```text
       R0  R1

P0      1   0
P1      0   1
```

Outstanding requests:

```text
       R0  R1

P0      0   1
P1      1   0
```

Available:

```text
1 1
```

P0 requires one instance of R1.

P1 requires one instance of R0.

Neither process can complete under the current outstanding requests.

Expected result:

```text
DEADLOCK DETECTED

Deadlocked Processes:

P0
P1
```

---

# 12. Deadlock Recovery

Detection identifies deadlocked processes, but the system must still recover.

One recovery approach is process termination.

The current simulator demonstrates recovery by terminating the processes identified by its deadlock detector and releasing their allocated resources.

### Recovery procedure

1. Run deadlock detection.
2. Identify the deadlocked processes.
3. Select the processes for termination according to the implemented recovery policy.
4. Remove their allocations from the system.
5. Return released resources to the Available vector.
6. Update the system state.
7. Run deadlock detection again if necessary.

### Example

Before recovery:

```text
P0 holds R0
P1 holds R1

P0 waits for R1
P1 waits for R0
```

After terminating both detected processes:

```text
R0 released
R1 released
```

The resources become available to other processes.

**Implementation limitation:** The current recovery operation terminates all processes identified by the detector rather than selecting the minimum-cost victim. This is a simplified educational recovery strategy.

---

# 13. Resource Allocation Graph (RAG)

A Resource Allocation Graph represents the relationships between processes and resources.

It is a directed graph containing two types of nodes:

- Process nodes.
- Resource nodes.

### Edge types

**Request Edge**

A directed edge from a process to a resource.

```text
P0 -> R1
```

This means P0 is requesting R1.

**Allocation Edge**

A directed edge from a resource to a process.

```text
R0 -> P0
```

This means R0 has been allocated to P0.

### Example

```text
R0 ----> P0 ----> R1
^                  |
|                  |
|                  v
P1 <---------------

```

The graph helps demonstrate resource dependencies and circular waiting relationships.

### Important distinction

A cycle in a resource allocation graph indicates deadlock when each resource type has exactly one instance.

With multiple instances of resource types, a cycle alone is not sufficient to prove deadlock.

The current application includes a RAG exploration section. Its graph representation should be understood according to the actual implementation, which currently provides textual resource-edge information rather than a fully rendered interactive graph.

---

# 14. Practical Application Workflow

The following steps demonstrate how to use Deadlock Lab.

## Step 1: Configure the system

Open the application and select:

**Configure System**

Enter:

- Number of processes.
- Number of resource types.
- Total resource instances.

## Step 2: Enter Allocation Matrix

Enter the number of resource instances currently allocated to each process.

Each row represents a process.

Each column represents a resource type.

## Step 3: Enter Maximum Matrix

Enter the maximum resource requirement for each process.

Make sure:

```text
Allocation <= Maximum
```

for every process and resource type.

## Step 4: Initialize the system

Click the initialization control.

The backend validates the matrices and calculates:

```text
Need = Maximum - Allocation
```

and:

```text
Available = Total - Allocated
```

## Step 5: Run Banker's Algorithm

Navigate to:

**Simulation & Requests**

Click:

**Run Banker's Safety Check**

The application displays whether the system is safe or unsafe and, when safe, displays the safe sequence.

## Step 6: Submit resource requests

Enter:

- Process index.
- Resource request vector.

Click:

**Submit Request**

The backend checks whether the request can be granted without violating the safety condition.

## Step 7: Detect deadlocks

Click:

**Detect deadlock**

The detector checks outstanding requests against available resources and identifies unfinished processes.

## Step 8: Recover from deadlock

If a deadlock is detected, use:

**Recover from deadlock**

The simulator applies its implemented recovery policy and releases resources held by the terminated processes.

## Step 9: Explore the RAG

Open:

**RAG Explorer**

Inspect the resource allocation and request relationships represented by the simulator.

---

# 15. Installation and Execution

## 15.1 Prerequisites

Make sure the following software is installed:

1. Windows 10 or Windows 11.
2. Python 3.10 or a compatible version.
3. MinGW-w64 GCC.
4. Visual Studio Code or another code editor.
5. PowerShell or Windows Terminal.

Check Python:

```powershell
python --version
```

Check GCC:

```powershell
gcc --version
```

Both commands should display their installed versions.

## 15.2 Clone the repository

```powershell
git clone <YOUR_GITHUB_REPOSITORY_URL>
```

Navigate into the project:

```powershell
cd DeadlockSimulation_Python_C
```

If you downloaded the ZIP file instead, extract it and open the extracted project directory.

## 15.3 Install Python dependencies

```powershell
pip install -r requirements.txt
```

The main GUI dependency is PySide6.

## 15.4 Compile the C backend

Navigate to the backend directory:

```powershell
cd backend
```

Run:

```powershell
.\build.bat
```

The build script compiles the C source files and generates:

```text
deadlock.dll
```

Return to the project root:

```powershell
cd ..
```

## 15.5 Run the application

Execute:

```powershell
python frontend/main.py
```

The Deadlock Lab graphical interface should now open.

### Important

If the C backend source code changes, rebuild the DLL:

```powershell
cd backend
.\build.bat
cd ..
```

If only Python frontend code changes, rebuilding the DLL is not required.

---

# 16. Sample Test Scenarios

The following scenarios can be used to test the application.

## Scenario 1: Safe System

Processes:

```text
5
```

Resource types:

```text
3
```

Total resources:

```text
10 5 7
```

Allocation:

```text
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
```

Maximum:

```text
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
```

Expected Available:

```text
3 3 2
```

Expected Need:

```text
7 4 3
1 2 2
6 0 0
0 1 1
4 3 1
```

Expected output:

```text
SAFE STATE

Safe Sequence:

P1 -> P3 -> P4 -> P0 -> P2
```

## Scenario 2: Unsafe System

Processes:

```text
2
```

Resource types:

```text
1
```

Total resources:

```text
3
```

Allocation:

```text
1
1
```

Maximum:

```text
3
2
```

Expected Available:

```text
1
```

Expected result:

```text
UNSAFE STATE
No safe sequence exists.
```

## Scenario 3: Deadlock Detection

Processes:

```text
2
```

Resource types:

```text
2
```

Total resources:

```text
2 2
```

Allocation:

```text
1 0
0 1
```

Request:

```text
0 1
1 0
```

Expected Available:

```text
1 1
```

Expected result:

```text
DEADLOCK DETECTED

P0
P1
```

## Scenario 4: Resource Request

Initialize Scenario 1.

Submit:

```text
Process index: 1

Request vector:

1 0 2
```

Expected result:

```text
REQUEST GRANTED
```

The request can be granted while preserving a safe state.

---

# 17. Advantages of the Project

### 1. Interactive Learning

Students can experiment with different resource allocation configurations instead of relying only on textbook examples.

### 2. Practical Understanding

The application demonstrates how operating system algorithms make resource allocation decisions.

### 3. Separation of Frontend and Backend

The use of Python for the GUI and C for the simulation engine demonstrates modular software architecture.

### 4. Algorithm Visualization

Safe sequences, resource states, and deadlock detection results are presented in an accessible interface.

### 5. Real-Time Experimentation

Users can change resource configurations and submit different requests to observe the results.

### 6. Educational Value

The simulator provides practical exposure to:

- Process synchronization concepts.
- Resource management.
- Deadlock avoidance.
- Deadlock detection.
- Deadlock recovery.
- C and Python interoperability.

### 7. Extensibility

The architecture can be extended with additional algorithms and visualization features.

---

# 18. Limitations and Disadvantages

Although the application demonstrates important operating system concepts, it has certain limitations.

### 1. Simulation Rather Than a Real Operating System

The application models resource allocation. It does not control actual Windows processes, memory allocation, printers, or hardware devices.

### 2. Banker's Algorithm Requires Maximum Claims

The Banker's Algorithm assumes that maximum resource requirements are known in advance.

In real operating systems, future resource requirements may not always be predictable.

### 3. Limited Recovery Strategy

The current recovery strategy terminates all processes identified as deadlocked rather than choosing an optimal process to terminate.

### 4. Limited Resource Allocation Graph Visualization

The current RAG explorer provides textual resource relationships rather than a fully interactive graphical graph.

### 5. No Persistent Simulation Storage

Simulation state is maintained during the application session. It is not designed as a persistent database-backed system.

### 6. Fixed Simulation Limits

The backend uses predefined maximum process and resource capacities. These limits restrict the size of the simulation.

### 7. No Automatic Scheduling of Waiting Requests

A request that cannot currently be granted is not automatically re-executed when resources become available. The user can submit another request manually.

### 8. DLL Dependency

The application requires a compatible compiled DLL. Changes to the C backend require recompilation.

---

# 19. Future Enhancements

The project can be extended in several ways.

| Enhancement | Description |
|---|---|
| Interactive RAG | Render processes and resources as graph nodes with animated edges |
| Step-by-step simulation | Show each iteration of Banker's Algorithm |
| Multiple recovery strategies | Implement victim selection and minimum-cost recovery |
| Process scheduling | Add FCFS, SJF, Round Robin, and priority scheduling |
| Simulation history | Save previous simulation results |
| Export reports | Export resource matrices and results as PDF or CSV |
| Automatic request queue | Retry waiting requests when resources become available |
| Performance analysis | Compare algorithm execution time for different configurations |
| Multi-instance resources | Improve visualization of multiple resource instances |
| Advanced statistics | Display resource utilization and process completion metrics |

---

# 20. Complexity Analysis

Let:

- `n` = Number of processes.
- `m` = Number of resource types.

| Operation | Approximate Time Complexity |
|---|---|
| Calculate Need Matrix | O(n × m) |
| Calculate Available Vector | O(n × m) |
| Banker's Safety Algorithm | O(n² × m) |
| Resource Request Safety Check | O(n² × m) |
| Deadlock Detection | O(n² × m) |

These are theoretical algorithmic bounds. Actual execution time also depends on implementation details and system hardware.

---

# 21. Testing and Validation

The application should be tested using multiple resource configurations.

Recommended test categories:

- Valid safe-state initialization.
- Valid unsafe-state initialization.
- Invalid matrix dimensions.
- Negative resource values.
- Allocation exceeding maximum.
- Total resources smaller than allocated resources.
- Valid resource requests.
- Resource requests exceeding maximum need.
- Resource requests exceeding available resources.
- Requests that produce unsafe states.
- Deadlock detection.
- Deadlock recovery.
- Zero-allocation processes.
- Different process and resource counts.

Testing should verify both the algorithmic result and the GUI response.

---

# 22. Learning Outcomes

By developing this project, students gain practical experience in:

1. Understanding deadlocks and their necessary conditions.
2. Implementing resource management algorithms.
3. Applying Banker's Algorithm.
4. Understanding safe and unsafe system states.
5. Implementing deadlock detection and recovery.
6. Working with matrices and vectors in C.
7. Designing modular software.
8. Building desktop applications with PySide6.
9. Integrating C and Python using `ctypes`.
10. Compiling C code into a Windows DLL using MinGW.
11. Debugging cross-language applications.
12. Working collaboratively using Git.

---

# 23. Team Contribution

The project can be divided among five team members.

| Member | Responsibility |
|---|---|
| Member 1 | Core system and resource management |
| Member 2 | Banker's Algorithm and resource requests |
| Member 3 | Deadlock detection and recovery |
| Member 4 | Python GUI and dashboard |
| Member 5 | Python-C integration, RAG, testing, and documentation |

All team members should coordinate on shared API declarations and integration.

---

# 24. Conclusion

Deadlock Lab demonstrates how operating systems manage competing processes and limited resources.

By combining deadlock avoidance, detection, and recovery in one interactive application, it provides a practical way to understand an important area of operating system design.

The implementation also demonstrates how different programming languages can work together: C handles the simulation engine, while Python provides an accessible graphical interface.

The project transforms abstract operating system algorithms into an interactive environment where users can configure systems, submit requests, observe safe sequences, detect deadlocks, and experiment with recovery.

**Deadlock Lab is designed as both an educational tool and a foundation for exploring more advanced operating system simulation techniques.**

---

## 25. License

This project was developed for academic and educational purposes.

You may adapt and extend it for learning, experimentation, and further development.
