
import tkinter as tk

import fifo
import lru
import optimal

from gui import PageReplacementGUI


# ---------------- TESTING ----------------

def run_tests():

    references = [7, 0, 1, 2, 0, 3, 0, 4, 2, 3]
    frames = 3

    fifo_result = fifo.run(references, frames)
    lru_result = lru.run(references, frames)
    optimal_result = optimal.run(references, frames)

    # FIFO expected results
    assert fifo_result["faults"] == 9
    assert fifo_result["hits"] == 1

    # LRU expected results
    assert lru_result["faults"] == 8
    assert lru_result["hits"] == 2

    # Optimal expected results
    assert optimal_result["faults"] == 6
    assert optimal_result["hits"] == 4
    print("All algorithm tests passed!")
    print()

    for result in [
        fifo_result,
        lru_result,
        optimal_result
    ]:
        print(
            result["algorithm"],
            "| Hits:", result["hits"],
            "| Faults:", result["faults"],
            "| Hit Ratio:", result["hit_ratio"],
            "| Fault Ratio:", result["fault_ratio"]
        )


# ---------------- APPLICATION ----------------

if __name__ == "__main__":

    # Run algorithm tests
    run_tests()

    # Launch GUI
    root = tk.Tk()

    app = PageReplacementGUI(root)

    root.mainloop()
