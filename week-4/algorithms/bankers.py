from typing import List, Tuple


def calculate_need(allocation: List[List[int]], maximum: List[List[int]]) -> List[List[int]]:
    """Need = Maximum - Allocation."""
    return [
        [maximum[i][j] - allocation[i][j] for j in range(len(maximum[i]))]
        for i in range(len(maximum))
    ]


def safety_check(
    allocation: List[List[int]],
    maximum: List[List[int]],
    available: List[int],
) -> Tuple[bool, List[int], List[int]]:
    """
    Run the Banker's safety algorithm.
    Returns (is_safe, safe_sequence, processes_unable_to_finish).
    """
    n = len(allocation)
    if n == 0:
        return True, [], []

    need = calculate_need(allocation, maximum)
    work = available[:]
    finish = [False] * n
    sequence = []

    while True:
        found = False
        for i in range(n):
            if not finish[i] and all(need[i][j] <= work[j] for j in range(len(work))):
                work = [work[j] + allocation[i][j] for j in range(len(work))]
                finish[i] = True
                sequence.append(i)
                found = True
        if not found:
            break

    unable = [i for i, done in enumerate(finish) if not done]
    return len(unable) == 0, sequence, unable


def try_request(
    process: int,
    request: List[int],
    allocation: List[List[int]],
    maximum: List[List[int]],
    available: List[int],
):
    """
    Try to grant a resource request transactionally using Banker's Algorithm.
    Returns (granted, message, new_allocation, new_available).
    If it cannot be granted, original matrices/vectors are returned unchanged.
    """
    resource_count = len(available)
    need = calculate_need(allocation, maximum)

    if process < 0 or process >= len(allocation):
        return False, "Select a valid process.", allocation, available
    if len(request) != resource_count:
        return False, "Request vector has the wrong number of resource types.", allocation, available
    if any(value < 0 for value in request):
        return False, "Requests cannot be negative.", allocation, available
    if any(request[j] > need[process][j] for j in range(resource_count)):
        return False, "Request exceeds the process's remaining Need.", allocation, available
    if any(request[j] > available[j] for j in range(resource_count)):
        return False, "Request cannot be granted now: insufficient Available resources.", allocation, available

    tentative_allocation = [row[:] for row in allocation]
    tentative_available = available[:]
    for j in range(resource_count):
        tentative_allocation[process][j] += request[j]
        tentative_available[j] -= request[j]

    safe, sequence, unable = safety_check(tentative_allocation, maximum, tentative_available)
    if not safe:
        return (
            False,
            "Request denied: granting it would make the system unsafe. "
            + "Unable to finish: " + ", ".join(f"P{i}" for i in unable),
            allocation,
            available,
        )

    sequence_text = " → ".join(f"P{i}" for i in sequence)
    return (
        True,
        f"Request granted. System remains safe. Safe sequence: {sequence_text}",
        tentative_allocation,
        tentative_available,
    )