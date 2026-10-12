# CPU Scheduling Simulator: Test Documentation

## 1. Objective

To verify that the CPU Scheduling Simulator correctly executes scheduling algorithms and calculates performance metrics.

## 2. Test Cases

### Test Case 1: FCFS Scheduling

**Input:**

| Process | Arrival Time | Burst Time |
|---|---:|---:|
| P1 | 0 | 4 |
| P2 | 1 | 3 |
| P3 | 2 | 2 |

**Expected Gantt Chart:**

`0 | P1 | 4 | P2 | 7 | P3 | 9`

**Expected Average Waiting Time:** 2.67 units

**Expected Average Turnaround Time:** 5.67 units

### Test Case 2: Round Robin Scheduling

**Input:**

| Process | Arrival Time | Burst Time |
|---|---:|---:|
| P1 | 0 | 5 |
| P2 | 1 | 3 |
| P3 | 2 | 1 |

**Time Quantum:** 2 units

**Expected Gantt Chart:**

`0 | P1 | 2 | P2 | 4 | P3 | 5 | P1 | 7 | P2 | 8 | P1 | 9`

**Expected Average Waiting Time:** 3.33 units

**Expected Average Turnaround Time:** 6.33 units

### 3. Additional Tests

- Verify that the program handles a single process.
- Verify that processes arriving at different times are handled correctly.
- Verify that Round Robin rejects a zero or negative time quantum.
- Verify that preemptive algorithms switch processes when required.
- Verify that invalid menu choices are handled safely.

## 4. Test Results

For each test, record:
- Whether the program ran successfully
- Actual output
- Whether actual output matched expected output
- Any bugs found and fixed

## 5. Conclusion

The simulator is considered tested when all documented test cases have been executed and their results verified.