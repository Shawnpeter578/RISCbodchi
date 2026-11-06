RISC-bodchi OS Simulator

An interactive, multi-process Operating System and CPU simulator built in C. This project demonstrates Inter-Process Communication (IPC) using POSIX message queues, process creation using fork() and exec(), and real-time terminal UI rendering using ncurses.

Features

Multi-Processing: Orchestrates three distinct processes running simultaneously (UI, Core, Logger).

Inter-Process Communication: Uses POSIX message queues (mqueue.h) to pass commands and data between the UI, the CPU Core, and the Logger.

CPU Simulation: Simulates basic CPU operations including a Stack (LIFO), a Queue (FIFO), Random Access Memory (RAM), and an Arithmetic Logic Unit (ALU).

Interactive UI: A visual terminal interface built with ncurses that displays live updates of the CPU state, logs, and responses.

Background Logging: A dedicated logger process records all system events in the background to a simulator.log file.

Architecture & Operating System Concepts

This project models how a modern operating system manages separate programs running concurrently without letting them crash into each other.

The Theory: Core OS Concepts

Memory Isolation: In a modern OS like Linux, every process gets its own virtual memory space. The UI process cannot directly read or modify variables in the Core process. If the UI tries to force its way into the Core's memory, the OS triggers a segmentation fault.

Inter-Process Communication (IPC): Because memory is isolated, processes need a secure bridge to talk to each other. This project uses POSIX Message Queues, which act like secure mailboxes managed directly by the OS kernel.

Process Spawning: An OS doesn't just run one massive block of code. It uses a parent-child hierarchy to split workloads. The master launcher acts as the parent, creating exact clones of itself to handle different tasks.

CPU Architecture Simulation:

Stack: Simulates a Last-In-First-Out (LIFO) memory structure using an array and a top pointer.

Queue: Simulates a First-In-First-Out (FIFO) buffer using front and rear pointers.

ALU (Arithmetic Logic Unit): Handles the basic computational math (ADD, SUBTRACT).

RAM (Memory): Simulates random access memory where specific addresses hold specific values (STORE, LOAD).

The Practical: Under the Hood in C

fork() and exec(): In main.c, fork() duplicates the launcher process. The child process then immediately calls exec() to overwrite its own memory with the compiled binary of either the Logger or the UI. The parent process just sits back and uses waitpid() to monitor them.

Message Queues (mqueue.h):

The system creates three distinct queues: /riscbodchi_request (UI to Core), /riscbodchi_response (Core to UI), and /riscbodchi_log (Core to Logger).

They are opened with specific access flags. For example, the Logger opens its queue as O_RDONLY (Read-Only) because it only listens, while the Core opens it as O_WRONLY (Write-Only).

Non-Blocking I/O & Concurrency: The ncurses UI loop in ui.c cannot freeze while waiting for the user to type or the Core to reply. It uses O_NONBLOCK on its message queues and WNOHANG in waitpid(). This ensures the UI constantly refreshes at 10 frames per second (timeout(100)), draining the queues and updating the screen in real-time.

Daemonization (Background Logging): The Logger is designed as a background daemon. When it is launched, main.c redirects its standard output to a physical file (simulator.log) using dup2(). This guarantees the Logger's printf statements don't spill onto the terminal and corrupt the graphical UI.

Graceful Shutdown: When you type quit, the UI sends an exit string to the Core. The Core breaks its infinite loop, cleans up its queues, and shuts down. The master launcher detects this, cleans up the UI, and sends a SIGTERM kill signal to terminate the background Logger, ensuring no zombie processes are left running.

Prerequisites

This project requires a Linux environment (or Windows Subsystem for Linux - WSL) because it relies on POSIX standards for processes and message queues.

You will need the GCC compiler and the ncurses library installed.
On Ubuntu/Debian/WSL, run:

sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev


Project Structure

.
├── Makefile
└── src/
    ├── main.c      # Master launcher that forks the logger and UI
    ├── ui.c        # The ncurses-based interactive interface
    ├── core.c      # The CPU logic and command processor
    └── logger.c    # Listens to IPC logs and writes them to a file


Building the Project

To compile the project, simply run the provided Makefile from the root directory:

make


This will compile the source files and place the binaries in a new build/ folder, as well as generate the master orchestrator binary riscbodchi_sim in the root folder.

To clean up the build files, run:

make clean


Running the Simulator

Launch the simulator using the master binary:

./riscbodchi_sim


Available Commands

Once the UI is running, you can type the following commands at the risc> prompt:

Stack Operations (LIFO)

PUSH <value>: Pushes an integer onto the top of the stack.

POP: Removes and returns the integer at the top of the stack.

Queue Operations (FIFO)

ENQUEUE <value>: Adds an integer to the back of the queue.

DEQUEUE: Removes and returns the integer at the front of the queue.

Memory Management (RAM)

STORE <address> <value>: Stores an integer value at a specific memory address (0-99).

LOAD <address>: Retrieves the value currently stored at the specified memory address.

Arithmetic (ALU)

ADD <a> <b>: Adds two integers and returns the result.

SUBTRACT <a> <b>: Subtracts b from a and returns the result.

System Control

help: Displays a list of available commands in the UI.

quit (or exit): Gracefully shuts down the Core, UI, and Logger processes.

Viewing Logs

After exiting the simulator, you can view the background system logs generated during your session by reading the log file:

cat simulator.log
