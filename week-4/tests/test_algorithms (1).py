from algorithms.bankers import calculate_need, safety_check, try_request
from algorithms.detection import detect_deadlock


def test_classic_banker_example_is_safe():
    allocation = [[0,1,0],[2,0,0],[3,0,2],[2,1,1],[0,0,2]]
    maximum = [[7,5,3],[3,2,2],[9,0,2],[2,2,2],[4,3,3]]
    available = [3,3,2]
    safe, sequence, unable = safety_check(allocation, maximum, available)
    assert safe
    assert sequence == [1,3,4,0,2]
    assert unable == []


def test_need_matrix():
    assert calculate_need([[1,0],[0,1]], [[1,1],[1,1]]) == [[0,1],[1,0]]


def test_detects_two_process_deadlock():
    allocation = [[1,0],[0,1]]
    available = [0,0]
    requests = [[0,1],[1,0]]
    deadlocked, _ = detect_deadlock(allocation, available, requests)
    assert deadlocked == [0,1]


def test_safe_request_is_granted():
    allocation = [[0,1,0],[2,0,0],[3,0,2],[2,1,1],[0,0,2]]
    maximum = [[7,5,3],[3,2,2],[9,0,2],[2,2,2],[4,3,3]]
    granted, _, new_allocation, new_available = try_request(1, [1,0,2], allocation, maximum, [3,3,2])
    assert granted
    assert new_allocation[1] == [3,0,2]
    assert new_available == [2,3,0]
