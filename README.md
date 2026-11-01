# RISCbodchi
### Operating Systems and Systems Programming | Weekly Project Collection

A collection of practical projects developed by **RISC BODCHI GANG**, exploring fundamental concepts in operating systems, process management, CPU scheduling, deadlock handling, and memory management.

This repository brings together a series of weekly programming projects designed to bridge the gap between theoretical concepts and practical implementation. Each project focuses on a specific operating-system concept and demonstrates its behavior through simulations, algorithmic implementations, or interactive applications.

The projects are developed as part of a progressive learning journey, with each week introducing new concepts and building a deeper understanding of how operating systems manage processes, resources, and memory.

---

## Table of Contents

- [About the Repository](#about-the-repository)
- [Team](#team)
- [Weekly Projects](#weekly-projects)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [Technologies and Tools](#technologies-and-tools)
- [Learning Objectives](#learning-objectives)
- [Project Documentation](#project-documentation)
- [Acknowledgements](#acknowledgements)

---

## About the Repository

**RISCbodchi** is a collection of weekly academic projects developed by RISC BODCHI GANG.

The repository focuses on implementing and demonstrating key concepts in operating systems through hands-on programming. Rather than treating operating-system concepts as purely theoretical topics, these projects provide practical implementations that allow users to observe, experiment with, and understand how different mechanisms work.

The project collection covers areas such as:

- Inter-process communication and synchronization
- Multi-process architecture and coordination
- CPU scheduling and process execution
- Resource allocation and deadlock handling
- Memory management and page replacement

Each weekly project is organized into its own directory, containing the relevant source code, project files, and documentation.

The individual README files provide detailed explanations of the concepts, project architecture, implementation, setup, execution, and usage.

---

## Team

Developed by **RISC BODCHI GANG**

| Team Member |
|---|
| Sayeeda |
| Grace |
| Vashishta |
| Shawn |
| Joylen |

---

## Weekly Projects

The repository is organized into five weekly projects, each focusing on a different aspect of operating systems.

### Week 1: Inter-Process Communication and Synchronization

Introduces the fundamental concepts of communication and synchronization between processes.

The project explores how independent processes exchange information and coordinate their execution using operating-system mechanisms.

**Key concepts:**
- Inter-Process Communication (IPC)
- Process synchronization
- Shared resources
- Race conditions
- Critical sections
- Synchronization mechanisms

[Explore Week 1 →](week-1/README.md)

---

### Week 2: Multi-Process Architecture Simulator

Focuses on communication and coordination between multiple processes within a simulated system architecture.

The project demonstrates how separate processes can perform different responsibilities while communicating through inter-process communication mechanisms.

**Key concepts:**
- Multi-process architecture
- Process creation and coordination
- Inter-process communication
- Message passing
- Resource sharing
- Process responsibilities

[Explore Week 2 →](week-2/README.md)

---

### Week 3: CPU Scheduling Simulator

Explores how operating systems schedule processes for CPU execution.

The project simulates different CPU scheduling algorithms and compares their behavior using scheduling metrics and execution timelines.

**Key concepts:**
- CPU scheduling
- First-Come, First-Served (FCFS)
- Shortest Job First (SJF)
- Shortest Remaining Time First (SRTF)
- Priority Scheduling
- Round Robin Scheduling
- Waiting time
- Turnaround time
- Response time
- Gantt charts

[Explore Week 3 →](week-3/README.md)

---

### Week 4: Deadlock Lab

An interactive deadlock simulation system designed to explore resource allocation, system safety, deadlock detection, and recovery.

The project provides a visual and interactive environment for experimenting with resource allocation states and understanding how deadlocks occur.

**Key concepts:**
- Deadlocks
- Resource allocation
- Safe and unsafe states
- Banker's Algorithm
- Deadlock detection
- Resource Allocation Graphs
- Deadlock recovery

[Explore Week 4 →](week-4/README.md)

---

### Week 5: Page Replacement Algorithm Simulator

A Python GUI application for demonstrating and comparing page replacement algorithms used in operating systems.

The project simulates memory frame allocation and page replacement, allowing users to observe how different algorithms respond to the same page-reference string.

**Key concepts:**
- Virtual memory
- Paging
- Page faults
- Page hits
- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal Page Replacement
- Page fault and hit ratios

[Explore Week 5 →](week-5/README.md)

---

## Repository Structure

The repository follows a week-based organization, with each project maintained in its own directory.

```text
RISCbodchi/
│
├── README.md
│
├── week-1/
│   └── README.md
│
├── week-2/
│   └── README.md
│
├── week-3/
│   └── README.md
│
├── week-4/
│   └── README.md
│
└── week-5/
    └── README.md
```

Each weekly directory contains the source code, supporting files, and project-specific documentation.

The structure allows projects to be explored, executed, and understood independently.

---

## Getting Started

To explore or run any project in this repository, follow the steps below.

### Prerequisites

The requirements vary depending on the weekly project.

Common tools include:

- Git
- Python
- A terminal or command-line interface
- A code editor such as Visual Studio Code

Some projects may require additional Python packages or other dependencies.

Refer to the individual project's README for its exact requirements.

### Clone the Repository

Clone the repository to your local machine:

```bash
git clone https://github.com/Shawnpeter578/RISCbodchi.git
```

Navigate into the repository:

```bash
cd RISCbodchi
```

### Navigate to a Weekly Project

For example, to explore Week 4:

```bash
cd week-4
```

To explore Week 5:

```bash
cd ../week-5
```

### Run a Project

Each weekly project has its own setup and execution instructions.

Before running a project:

1. Navigate to its directory.
2. Open its README file.
3. Review the prerequisites and installation instructions.
4. Install any required dependencies.
5. Execute the project using the provided commands.

For example, Week 5 can be launched using:

```bash
python main.py
```

Run commands may differ between projects. Always refer to the corresponding weekly README for the exact instructions.

---

## Technologies and Tools

The projects use programming languages, libraries, and development tools appropriate to their implementation.

| Technology | Purpose |
|---|---|
| Python | Implementing algorithms, simulations, and application logic |
| Streamlit | Building interactive web-based simulation interfaces |
| Tkinter | Developing desktop GUI applications |
| Git | Version control and project collaboration |
| GitHub | Source code hosting and project documentation |
| Operating System APIs | Supporting process and system-level operations where applicable |

The specific technologies and dependencies used by each project are documented in its respective README.

---

## Learning Objectives

The overall objective of this project collection is to develop a practical understanding of operating-system fundamentals through implementation and experimentation.

By working through the weekly projects, students explore how operating systems:

- Manage processes and coordinate execution
- Enable communication between independent processes
- Schedule processes for CPU execution
- Allocate and manage shared resources
- Detect and recover from deadlocks
- Manage memory using paging
- Handle page faults and replace memory pages
- Apply algorithms to solve system-level problems

The projects also encourage the development of practical programming skills, including algorithm design, debugging, testing, application development, and technical documentation.

---

## Project Documentation

Each weekly project includes a dedicated README that provides:

- Project overview and objectives
- Explanation of the underlying operating-system concepts
- Description of the implemented algorithms
- Architecture and project structure
- Setup and installation instructions
- Step-by-step execution guide
- Example inputs and outputs
- Testing instructions, where applicable
- Learning outcomes

This documentation is intended to make the projects easier to understand, reproduce, and evaluate.

---

## Acknowledgements

Developed as a collaborative academic project by **RISC BODCHI GANG**.

The repository represents our ongoing exploration of operating-system concepts through practical implementation, experimentation, and collaborative development.

---

<div align="center">

**RISC BODCHI GANG**

*Sayeeda · Grace · Vashishta · Shawn · Joylen*

</div>
