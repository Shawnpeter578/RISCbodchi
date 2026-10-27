# Page Replacement Algorithm Simulator

A Python GUI project for demonstrating and comparing three page replacement algorithms used in operating systems:

- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal (OPT)

## Features

- Enter a page-reference string and number of page frames.
- Simulate FIFO, LRU, or Optimal page replacement.
- Compare all three algorithms on the same input.
- View frame contents after each page request.
- Review hit/fault counts and ratios.

## Requirements

- Python 3.10 or newer recommended
- Tkinter (included with most standard Windows Python installations)
- Additional dependencies listed in `requirements.txt`, if applicable

## Run

From the project directory:

```bash
python main.py
```

If the project uses external packages, install them first:

```bash
python -m pip install -r requirements.txt
```

## Input example

Reference string:

```text
1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4
```

Number of frames: `3`

You can map page IDs to objects for a presentation, for example:
1 = Pizza, 2 = Burger, 3 = Fries, 4 = Sushi, 5 = Tacos.

## Project structure

```text
PageReplacementSimulator/
├── main.py
├── gui.py
├── fifo.py
├── lru.py
├── optimal.py
├── requirements.txt
├── README.md
├── .gitignore
├── screenshots/
└── docs/
```

## Screenshots

The `screenshots/` folder contains illustrative example screens for the project documentation. Replace them with screenshots captured from your own running application before final submission if the GUI differs.

## Team

Add your team members' names, student IDs, class/section, and instructor here.

## Notes

This project is an educational simulation. The Optimal algorithm uses knowledge of future page requests and is therefore a theoretical benchmark, not a practical online operating-system policy.
