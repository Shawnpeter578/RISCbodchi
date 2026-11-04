
def run(reference_string, frame_count):
    frames = []

    history = []
    hits = 0
    faults = 0

    for index, page in enumerate(reference_string):

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

            else:
                # Pages accessed before current request
                previous_pages = reference_string[:index]

                # Find least recently used page
                lru_page = min(
                    frames,
                    key=lambda p: (
                        len(previous_pages)
                        - 1
                        - previous_pages[::-1].index(p)
                    )
                )

                replace_index = frames.index(lru_page)
                frames[replace_index] = page

        history.append({
            "page": page,
            "frames": frames.copy(),
            "status": status
        })

    total = len(reference_string)

    return {
        "algorithm": "LRU",
        "history": history,
        "hits": hits,
        "faults": faults,
        "hit_ratio": hits / total * 100,
        "fault_ratio": faults / total * 100
    }
