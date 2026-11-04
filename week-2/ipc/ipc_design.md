# Inter-Process Communication (IPC) Design

## 1. Introduction

Inter-Process Communication (IPC) refers to mechanisms provided by an operating system that allow independent processes to exchange data and coordinate their execution.

In a multi-process application, each process has its own execution context and typically its own address space. As a result, processes cannot directly access each other's ordinary variables.

IPC provides a way for processes to exchange information through controlled communication channels.

The Multi-Process Page Replacement Simulator uses IPC to communicate between the UI process, core process, and logger process.

The simulator supports five IPC mechanisms:

1. POSIX Message Queues
2. Pipes
3. Shared Memory
4. Unix Domain Sockets
5. FIFO / Named Pipes

The objective is to demonstrate the differences between these mechanisms while maintaining consistent page replacement algorithm behavior.

## 2. Why IPC Is Required

The simulator separates user interaction, computation, and logging into different processes.

These processes must exchange information to complete a simulation.

For example:

- The UI process sends the selected algorithm and simulation parameters to the core process.
- The core process receives the request and performs the simulation.
- The core process returns the calculated page faults and page hits.
- The core process sends diagnostic messages to the logger process.
- The logger process receives and displays those messages.

Without IPC, the processes would not be able to exchange the information required to coordinate the simulation.

## 3. POSIX Message Queues

### 3.1 Theory

POSIX Message Queues provide a mechanism for exchanging messages between processes.

Messages are placed into a queue maintained by the operating system.

A sending process places a message into the queue, while a receiving process retrieves the message.

Unlike ordinary pipes, message queues preserve message boundaries.

### 3.2 How It Works

The general communication process is:

1. Create or open a message queue.
2. The sender prepares a message.
3. The sender places the message into the queue.
4. The receiver retrieves the message.
5. The receiver processes the message.
6. The queue is closed and removed when no longer needed.

### 3.3 Usage in the Simulator

The POSIX Message Queue implementation provides a communication channel for simulation messages.

The UI process sends simulation requests through the queue.

The core process receives the request, executes the selected algorithm, and returns the result through the appropriate communication channel.

The logger receives diagnostic messages through the IPC abstraction.

### 3.4 Advantages

- Preserves message boundaries.
- Supports communication between independent processes.
- Provides a structured message-based communication model.
- Allows messages to be queued until the receiver retrieves them.

### 3.5 Limitations

- Messages are copied between process and kernel-managed queue storage.
- Queue creation and management require operating system resources.
- Queue size and message size are subject to system limits.

## 4. Pipes

### 4.1 Theory

A pipe is a communication channel that allows data to flow between processes.

A pipe is commonly used for communication between related processes, such as a parent and child process.

Pipes provide a byte stream rather than a structured message queue.

### 4.2 How It Works

A pipe is created by the operating system.

It provides two file descriptors:

- A read end
- A write end

The sending process writes data to the write end.

The receiving process reads data from the read end.

The operating system transfers the data through the pipe buffer.

### 4.3 Usage in the Simulator

The pipe implementation provides communication between the UI, core, and logger process roles.

The UI sends simulation requests to the core process.

The core process sends simulation results back to the UI.

Diagnostic messages are sent to the logger process.

### 4.4 Advantages

- Simple communication model.
- Suitable for communication between related processes.
- Supported directly by POSIX systems.
- Does not require a separate named filesystem object.

### 4.5 Limitations

- Pipes provide byte-stream communication.
- Message boundaries must be handled by the application.
- Ordinary pipes are generally used between related processes.
- Communication channels must be created and coordinated correctly.

## 5. Shared Memory

### 5.1 Theory

Shared Memory allows multiple processes to access a common region of memory.

Instead of sending a separate copy of the data through a communication buffer, processes access the shared memory region directly.

Shared memory can provide efficient communication because the data does not need to be repeatedly copied between processes.

However, it introduces synchronization requirements.

### 5.2 How It Works

The general process is:

1. Create a shared memory object.
2. Allocate or define the shared region.
3. Map the region into the address space of participating processes.
4. Write data into the shared region.
5. Notify the receiving process.
6. The receiver reads the data.
7. Synchronize access to prevent conflicting operations.
8. Unmap and remove the shared memory object when finished.

### 5.3 Synchronization

Multiple processes accessing shared memory can cause race conditions.

A race condition occurs when the result depends on the timing of concurrent operations.

The simulator uses synchronization mechanisms to coordinate access to its shared message slots.

Semaphores are used to signal when a slot is available and when a message has been written.

### 5.4 Usage in the Simulator

The shared memory implementation uses shared memory regions to exchange messages.

The sending process places a message into a shared slot.

The receiving process waits until the message is available, reads it, and signals that the slot can be reused.

### 5.5 Advantages

- Avoids repeatedly copying data through a kernel-managed message buffer.
- Can provide efficient communication for larger data transfers.
- Allows multiple processes to access the same shared region.

### 5.6 Limitations

- Requires synchronization.
- Incorrect synchronization can cause race conditions or deadlocks.
- Shared memory setup and cleanup require careful resource management.
- The application must define how data is organized in the shared region.

## 6. Unix Domain Sockets

### 6.1 Theory

Unix Domain Sockets provide communication between processes on the same machine.

They use the socket programming interface, but unlike network sockets, they do not require communication over an external network.

Unix Domain Sockets can provide bidirectional communication.

### 6.2 How It Works

The general process is:

1. Create a socket.
2. Establish a communication connection.
3. Send data through the socket.
4. Receive data through the socket.
5. Close the socket when communication is complete.

### 6.3 Usage in the Simulator

The simulator uses Unix Domain Sockets to exchange simulation messages between process roles.

The socket-based implementation allows the UI and core processes to send and receive requests and results through a bidirectional communication channel.

The logger can also receive diagnostic messages through the selected IPC mechanism.

### 6.4 Advantages

- Supports bidirectional communication.
- Provides a structured socket-based interface.
- Suitable for communication between processes on the same host.
- Can be extended to client-server style communication.

### 6.5 Limitations

- Requires socket creation and connection management.
- Adds system call and data-transfer overhead.
- Communication setup must be coordinated between processes.

## 7. FIFO / Named Pipes

### 7.1 Theory

A FIFO, also called a named pipe, is a communication channel represented by a filesystem object.

Unlike an ordinary pipe, a FIFO can be opened by unrelated processes that know its path.

A FIFO provides byte-stream communication.

### 7.2 How It Works

The general process is:

1. Create a FIFO using the operating system.
2. Open the FIFO for reading or writing.
3. The sender writes data.
4. The receiver reads data.
5. Close the FIFO.
6. Remove the FIFO when it is no longer needed.

### 7.3 Usage in the Simulator

The FIFO implementation uses named pipes to transfer simulation messages between the process roles.

The UI process sends simulation requests to the core process.

The core process sends results back to the UI.

Diagnostic messages can be delivered to the logger process.

### 7.4 Advantages

- Allows communication between unrelated processes.
- Provides a filesystem-based communication endpoint.
- Simple to use with standard file operations.
- Supported by POSIX systems.

### 7.5 Limitations

- Provides byte-stream rather than message-based communication.
- Message framing must be managed by the application.
- Opening FIFOs can block until the other endpoint is available.
- FIFO creation and cleanup require filesystem coordination.

## 8. IPC Comparison

| IPC Mechanism | Communication Model | Message Boundaries | Bidirectional | Synchronization |
|---|---|---|---|---|
| POSIX Message Queue | Message-based | Preserved | Depends on queue arrangement | Queue operations |
| Pipe | Byte stream | Not preserved | Usually separate pipes | Read/write coordination |
| Shared Memory | Shared region | Application-defined | Yes | Required |
| Unix Domain Socket | Socket-based | Depends on socket type | Yes | Connection and I/O coordination |
| FIFO / Named Pipe | Byte stream | Not preserved | Usually separate FIFOs | Read/write coordination |

## 9. IPC and Page Replacement Algorithms

The IPC mechanism and the page replacement algorithm serve different purposes.

The IPC mechanism transports simulation requests and results.

The page replacement algorithm determines which page should be removed from memory when a page fault occurs and all available frames are occupied.

Therefore, selecting a different IPC mechanism should not change the page fault or page hit counts for the same simulation input.

For example, the following configuration should produce consistent algorithm results across all IPC mechanisms:

- Algorithm: FIFO
- Number of frames: 3
- Reference string: 7 0 1 2 0 3 0 4 2 3 0 3 2

The transport mechanism may affect execution time, but it should not affect the algorithm's logical result.

## 10. Conclusion

The simulator demonstrates five IPC mechanisms and their role in coordinating multiple processes.

Each mechanism has different characteristics in terms of communication model, synchronization requirements, resource management, and potential overhead.

By keeping IPC implementation separate from page replacement logic, the simulator allows communication mechanisms to be compared without changing the underlying algorithm.

This modular design provides an educational demonstration of both operating system communication concepts and virtual memory page replacement algorithms.
