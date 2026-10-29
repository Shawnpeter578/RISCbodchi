
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
                # Future page references
                future_pages = reference_string[index + 1:]

                def next_use(frame_page):
                    if frame_page not in future_pages:
                        return float("inf")

                    return future_pages.index(frame_page)

                # Find page used farthest in future
                page_to_replace = max(
                    frames,
                    key=next_use
                )

                replace_index = frames.index(page_to_replace)
                frames[replace_index] = page

        history.append({
            "page": page,
            "frames": frames.copy(),
            "status": status
        })

    total = len(reference_string)

    return {
        "algorithm": "Optimal",
        "history": history,
        "hits": hits,
        "faults": faults,
        "hit_ratio": hits / total * 100,
        "fault_ratio": faults / total * 100
    }
