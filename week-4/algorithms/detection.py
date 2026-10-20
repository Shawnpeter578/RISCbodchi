from typing import List, Tuple


def detect_deadlock(
    allocation: List[List[int]],
    available: List[int],
    outstanding_requests: List[List[int]],
) -> Tuple[List[int], List[int]]:
    """
    Multiple-instance deadlock detection algorithm.
    outstanding_requests[i][j] is the current outstanding request by process i
    for resource type j. Returns (deadlocked_processes, completion_sequence).
    """
    process_count = len(allocation)
    resource_count = len(available)
    work = available[:]

    # Processes holding no resources can be considered able to finish.
    finish = [all(value == 0 for value in allocation[i]) for i in range(process_count)]
    sequence = []

    changed = True
    while changed:
        changed = False
        for i in range(process_count):
            if finish[i]:
                continue
            if all(outstanding_requests[i][j] <= work[j] for j in range(resource_count)):
                work = [work[j] + allocation[i][j] for j in range(resource_count)]
                finish[i] = True
                sequence.append(i)
                changed = True

    deadlocked = [i for i, done in enumerate(finish) if not done]
    return deadlocked, sequence
