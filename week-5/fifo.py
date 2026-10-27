from collections import deque


def run(reference_string, frame_count):
    frames = []
    queue = deque()

    history = []
    hits = 0
    faults = 0

    for page in reference_string:

        if page in frames:
            # Page hit
            hits += 1
            status = "HIT"

        else:
            # Page fault
            faults += 1
            status = "FAULT"

            if len(frames) < frame_count:
                # Empty frame available
                frames.append(page)
                queue.append(page)

            else:
                # Remove oldest loaded page
                oldest_page = queue.popleft()

                replace_index = frames.index(oldest_page)
                frames[replace_index] = page

                queue.append(page)

        # Store memory state after every request
        history.append({
            "page": page,
            "frames": frames.copy(),
            "status": status
        })

    total = len(reference_string)

    return {
        "algorithm": "FIFO",
        "history": history,
        "hits": hits,
        "faults": faults,
        "hit_ratio": hits / total * 100,
        "fault_ratio": faults / total * 100
    }