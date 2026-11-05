# Testing and Verification

## 1. Introduction

Testing is an important part of software development. It helps verify that a program behaves as expected and that its components work correctly together.

The Multi-Process Page Replacement Simulator is tested to verify:

- Correct implementation of page replacement algorithms.
- Correct calculation of page faults and page hits.
- Consistent results across IPC mechanisms.
- Successful communication between process roles.
- Correct execution of valid simulation requests.

This document describes the testing methodology used for the project.

## 2. Testing Objectives

The objectives of testing are:

1. Verify FIFO, LRU, and Optimal page replacement algorithms.
2. Check page fault and page hit calculations.
3. Verify that all IPC mechanisms can transport simulation requests and results.
4. Ensure that identical simulation inputs produce consistent results.
5. Identify unexpected errors during program execution.
6. Verify the overall behavior of the multi-process application.

## 3. Testing Environment

| Component | Description |
|---|---|
| Programming Language | C |
| Compiler | GCC |
| Build System | GNU Make |
| Operating System | Linux / Ubuntu / WSL |
| IPC Mechanisms | POSIX Message Queues, Pipes, Shared Memory, Unix Domain Sockets, FIFO |
| Algorithms | FIFO, LRU, Optimal |

## 4. Testing Approach

Testing is performed at two levels.

### 4.1 Algorithm Testing

Algorithm testing verifies the page replacement logic.

For each test case:

- Provide a page-reference string.
- Select a frame count.
- Run the selected algorithm.
- Record page faults and page hits.
- Compare the results with the expected values.

The test cases are documented in `../tests/test_cases.md`.

Expected values are documented in `../tests/expected_results.md`.

### 4.2 Integration Testing

Integration testing verifies that the components work together.

The following components are tested:

- UI process.
- Core process.
- Logger process.
- IPC implementations.
- Page replacement algorithm module.

The purpose is to verify that simulation requests are transmitted, processed, and returned successfully.

## 5. Algorithm Verification

The three page replacement algorithms are tested using identical workloads.

The standard reference string is:

```text id="w4h8r7"
7 0 1 2 0 3 0 4 2 3 0 3 2
```

Number of frames:

```text id="7r1g9z"
3
```

Expected results:

| Algorithm | Page Faults | Page Hits |
|---|---:|---:|
| FIFO | 10 | 3 |
| LRU | 9 | 4 |
| Optimal | 7 | 6 |

These results are used to verify the algorithm implementation.

## 6. IPC Verification

The simulator supports five IPC mechanisms.

Each mechanism is tested using the same simulation input.

The IPC mechanisms are:

1. POSIX Message Queues.
2. Pipes.
3. Shared Memory.
4. Unix Domain Sockets.
5. FIFO / Named Pipes.

For each IPC mechanism:

- Start the simulator.
- Select the IPC mechanism.
- Select the algorithm.
- Enter the frame count.
- Enter the page-reference string.
- Observe the result.
- Compare the result with the expected value.

The algorithm result should remain consistent across IPC mechanisms.

## 7. Manual Test Procedure

The following commands are used to compile and run the project.

### Step 1: Navigate to the project directory

```bash id="f4k3pz"
cd week4_multi_process_simulator
```

### Step 2: Compile the project

```bash id="k9d3ab"
make clean
make
```

### Step 3: Start the simulator

```bash id="v6m1cx"
./simulator
```

### Step 4: Select an IPC mechanism

Select one of the available IPC options.

### Step 5: Select an algorithm

Select FIFO, LRU, or Optimal.

### Step 6: Enter the number of frames

Enter a valid frame count.

### Step 7: Verify the output

Compare the reported page faults and page hits against the expected values.

## 8. Test Result Recording

The results of manual tests can be recorded using the following table.

| Test Case | IPC Mechanism | Algorithm | Expected Result | Actual Result | Status |
|---|---|---|---|---|---|
| TC-01 | POSIX Message Queues | FIFO | 10 faults, 3 hits | 10 faults, 3 hits | PASS |
| TC-01 | Pipes | FIFO | 10 faults, 3 hits | Pending | Pending |
| TC-01 | Shared Memory | FIFO | 10 faults, 3 hits | Pending | Pending |
| TC-01 | Unix Domain Sockets | FIFO | 10 faults, 3 hits | Pending | Pending |
| TC-01 | FIFO / Named Pipes | FIFO | 10 faults, 3 hits | Pending | Pending |

Update the table as the tests are completed.

## 9. Validation Rules

For a completed simulation:

```text id="w0k4r1"
Page Faults + Page Hits = Total Page References
```

The reported page fault and page hit counts must add up to the number of page references supplied to the simulator.

For the standard reference string, the total number of references is 13.

For FIFO:

```text id="z6a4v1"
10 Page Faults + 3 Page Hits = 13 References
```

## 10. Expected Behavior

A valid simulation request should:

- Be received by the core process.
- Execute the selected algorithm.
- Return a simulation result.
- Display the page fault count.
- Display the page hit count.
- Complete without an unexpected crash.

## 11. Limitations

The testing procedure described here is primarily manual.

The expected results provide a basis for verifying correctness, but they do not replace automated unit tests.

The simulator is an educational prototype, and testing does not establish production-level reliability or exhaustive correctness for every possible input.

## 12. Conclusion

Testing verifies that the page replacement algorithms produce expected results and that the IPC mechanisms successfully support communication between process roles.

By using the same workload across algorithms and IPC mechanisms, the testing process helps identify incorrect algorithm behavior or communication problems.
