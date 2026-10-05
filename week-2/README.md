# RISCbodchi – Week 2
## MultiProcess Architecture

RISCbodchi Week 2 is a simple multiprocess computer architecture simulator written in C. It demonstrates how different processes can work together and communicate using POSIX Message Queues.

### How it works

The project has 3 independent processes:

- UI Process – Takes commands from the user and displays the results.
- Core Process – Acts as the main processing unit. It performs calculations and manages memory, stack, and queue operations.
- Logger Process – Records all operations performed by the Core and saves them in `simulator.log`.

The communication happens through three message queues:

- `/riscbodchi_request` – UI → Core
- `/riscbodchi_response` – Core → UI
- `/riscbodchi_log` – Core → Logger

The UI sends a command to the Core. The Core performs the requested operation, sends the result back to the UI, and also sends a log message to the Logger. core ui

### Supported Commands

CPU Operations
```text
ADD a b
SUB a b
MUL a b
DIV a b
```

Stack Operations
```text
PUSH value
POP
```

Queue Operations
```text
ENQUEUE value
DEQUEUE
```

Memory Operations
```text
STORE address value
LOAD address
```

Exit
```text
exit
```
The Core has a memory of 100 locations, a stack of 100 elements, and a queue of 100 elements. core

### Example
```text
UI > ADD 10 20
CORE: ADD result = 30
```
The Logger will also record:

```text
ADD 10 20 = 30
```

in `simulator.log`. logger

### Running the Project
Compile the three programs:

```bash
gcc logger.c -o logger -lrt
gcc core.c -o core -lrt
gcc ui.c -o ui -lrt
```
Run them in three separate terminals, in this order:

```bash
./logger
./core
./ui
```

The Logger should be started first because the Core connects to it when starting. core

### Main Concept

In simple words:

> UI takes the command → Core does the work → UI gets the answer → Logger records what happened.

This project demonstrates multi-process programming, POSIX message queues, CPU operations, memory management, stacks, queues, and logging in C.
