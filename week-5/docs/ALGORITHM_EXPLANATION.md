# Algorithm Explanation

This section explains the working principles, implementation logic, and page replacement procedures of the three algorithms used in the Page Replacement Simulator: FIFO, LRU, and Optimal.

## 1. First-In, First-Out (FIFO)

### 1.1 Introduction

First-In, First-Out (FIFO) is a page replacement algorithm that follows the principle of replacing the page that was loaded into memory earliest.

The algorithm maintains the order in which pages enter the available memory frames. When a new page is requested and all frames are occupied, the page that has remained in memory for the longest time is selected for replacement.

FIFO does not consider how frequently or recently a page has been accessed. Its replacement decision depends entirely on the arrival order of pages.

### 1.2 Working Principle

The FIFO algorithm uses a circular replacement pointer to identify the next frame that should be replaced.

The following steps describe its execution:

1. Initialize all memory frames as empty.
2. Read the next page from the reference string.
3. Check whether the requested page is already present in memory.
4. If the page exists, register a page hit without changing the replacement order.
5. If the page is absent, register a page fault.
6. If an empty frame is available, insert the requested page into that frame.
7. If all frames are occupied, replace the page at the current circular pointer.
8. Move the pointer to the next frame using the modulo operation.
9. Repeat the process until every page reference has been processed.

### 1.3 Implementation in the Project

In this project, FIFO is implemented using an integer array to represent memory frames and a variable named `next_victim` to maintain the replacement position.

The circular pointer ensures that frames are selected in their arrival order. After a replacement, the pointer is incremented and wrapped around when it reaches the frame limit.

A page hit does not modify the pointer because the page's original arrival order remains unchanged.

### 1.4 Advantages

- Simple to understand and implement.
- Requires minimal additional memory.
- Uses a straightforward circular replacement mechanism.
- Provides predictable replacement behavior.

### 1.5 Disadvantages

- Does not consider recent page usage.
- May replace frequently accessed pages.
- Can suffer from Belady's Anomaly, where increasing the number of frames may increase page faults.

---

## 2. Least Recently Used (LRU)

### 2.1 Introduction

Least Recently Used (LRU) is a page replacement algorithm that replaces the page that has not been accessed for the longest period of time.

It is based on the principle of temporal locality, which suggests that pages accessed recently are likely to be accessed again in the near future.

Unlike FIFO, LRU considers the recent usage history of pages when making replacement decisions.

### 2.2 Working Principle

The LRU algorithm maintains information about the most recent access time of every page currently stored in memory.

The following steps describe its execution:

1. Initialize all memory frames as empty.
2. Read the next page from the reference string.
3. Search the memory frames to determine whether the requested page is present.
4. If the page is present, register a page hit and update its last-used information.
5. If the page is absent, register a page fault.
6. If an empty frame is available, load the requested page into it.
7. If all frames are occupied, identify the page with the oldest last-used reference index.
8. Replace that page with the newly requested page.
9. Update the last-used information of the newly loaded page.
10. Continue until all page references have been processed.

### 2.3 Implementation in the Project

In this project, LRU is implemented using two arrays:

- A frame array that stores the pages currently available in memory.
- A timestamp array named `last_used` that stores the reference index at which each page was most recently accessed.

Whenever a page is accessed, its corresponding timestamp is updated.

When a replacement is required, the algorithm searches for the smallest timestamp. The page associated with that timestamp is considered the least recently used page and is selected for replacement.

### 2.4 Advantages

- Considers recent page access history.
- Generally performs better than FIFO for many reference patterns.
- Avoids replacing pages solely because they entered memory earlier.
- Does not exhibit Belady's Anomaly under standard page replacement assumptions.

### 2.5 Disadvantages

- Requires additional memory for maintaining usage information.
- Requires searching through the frames to identify the least recently used page.
- More complex to implement than FIFO.
- Exact LRU tracking can introduce overhead in real operating systems.

---

## 3. Optimal Page Replacement

### 3.1 Introduction

Optimal Page Replacement is a theoretical page replacement algorithm that replaces the page whose next occurrence is farthest in the future.

If a page currently stored in memory will not be requested again, it becomes an ideal candidate for replacement.

The algorithm aims to achieve the minimum possible number of page faults for a given page-reference string and number of memory frames.

It is mainly used as a benchmark to evaluate the performance of other page replacement algorithms.

### 3.2 Working Principle

Unlike FIFO and LRU, Optimal examines the future page-reference sequence before selecting a page for replacement.

The following steps describe its execution:

1. Initialize all memory frames as empty.
2. Read the next page from the reference string.
3. Check whether the requested page is already present in memory.
4. If the page is present, register a page hit.
5. If the page is absent, register a page fault.
6. If an empty frame is available, load the requested page.
7. If all frames are occupied, examine the remaining page-reference sequence.
8. Determine when each currently loaded page will next be requested.
9. Identify the page whose next occurrence is farthest in the future.
10. If a page will never be requested again, treat it as having an infinite next-use distance and prioritize it for replacement.
11. Replace the selected page with the newly requested page.
12. Repeat until all references have been processed.

### 3.3 Implementation in the Project

In this project, Optimal is implemented by examining the remaining page-reference sequence whenever a page fault occurs and all frames are occupied.

The function `next_use()` searches for the next occurrence of a particular page starting from the next reference position.

If the page is found, its next-use index is returned. If it does not appear again, a value greater than the reference count is returned to represent an infinite next-use distance.

The algorithm compares these values for all occupied frames and replaces the page whose next use occurs farthest in the future.

If multiple pages have the same replacement priority, the implementation uses frame order to resolve the tie.

### 3.4 Advantages

- Produces the minimum possible number of page faults for a known reference sequence.
- Provides a theoretical performance benchmark.
- Helps evaluate the effectiveness of practical replacement algorithms.
- Does not exhibit Belady's Anomaly under standard assumptions.

### 3.5 Disadvantages

- Requires knowledge of future page references.
- Cannot generally be implemented as a practical online algorithm because future requests are unknown.
- Requires additional computation to examine future references.

---

## 4. Worked Example: FIFO Page Replacement

To understand how FIFO operates, consider the following page-reference sequence.

**Reference String:**

`7 0 1 2 0 3 0 4 2 3`

**Number of Memory Frames:** 3

Initially, all three memory frames are empty.

### 4.1 Step-by-Step Execution

| Step | Requested Page | Frame 1 | Frame 2 | Frame 3 | Status |
|---:|---:|---:|---:|---:|---|
| 1 | 7 | 7 | - | - | Fault |
| 2 | 0 | 7 | 0 | - | Fault |
| 3 | 1 | 7 | 0 | 1 | Fault |
| 4 | 2 | 2 | 0 | 1 | Fault |
| 5 | 0 | 2 | 0 | 1 | Hit |
| 6 | 3 | 2 | 3 | 1 | Fault |
| 7 | 0 | 2 | 3 | 0 | Fault |
| 8 | 4 | 4 | 3 | 0 | Fault |
| 9 | 2 | 4 | 2 | 0 | Fault |
| 10 | 3 | 4 | 2 | 3 | Fault |

### 4.2 Explanation of Each Step

**Step 1: Request page 7**

All frames are empty. Page 7 is loaded into Frame 1.

Result: Page Fault.

**Step 2: Request page 0**

Page 0 is not present in memory. It is loaded into Frame 2.

Result: Page Fault.

**Step 3: Request page 1**

Page 1 is not present in memory. It is loaded into Frame 3.

Result: Page Fault.

**Step 4: Request page 2**

All frames are occupied. Page 7 was the first page loaded, so FIFO replaces page 7 with page 2.

Result: Page Fault.

**Step 5: Request page 0**

Page 0 is already available in Frame 2.

No replacement is necessary.

Result: Page Hit.

**Step 6: Request page 3**

The next page in FIFO replacement order is page 0. It is replaced by page 3.

Result: Page Fault.

**Step 7: Request page 0**

Page 0 is no longer present in memory. The next victim is page 1, which is replaced by page 0.

Result: Page Fault.

**Step 8: Request page 4**

Page 2 is the next page in replacement order. It is replaced by page 4.

Result: Page Fault.

**Step 9: Request page 2**

Page 3 is the oldest remaining page in memory. It is replaced by page 2.

Result: Page Fault.

**Step 10: Request page 3**

Page 0 is selected for replacement because it is the oldest page currently stored in memory.

Page 3 is loaded into its frame.

Result: Page Fault.

### 4.3 Final FIFO Performance

| Performance Metric | Result |
|---|---:|
| Total Page References | 10 |
| Total Page Hits | 1 |
| Total Page Faults | 9 |
| Hit Ratio | 10% |
| Fault Ratio | 90% |

The example demonstrates that FIFO replaces pages based on their arrival order, regardless of how recently they have been accessed.

---

## 5. Comparative Analysis of the Algorithms

The three algorithms use different strategies to select pages for replacement.

| Feature | FIFO | LRU | Optimal |
|---|---|---|---|
| Replacement basis | Arrival order | Recent usage | Future usage |
| Main data structure | Frame array and circular pointer | Frame and timestamp arrays | Frame array and future search |
| Considers recent usage | No | Yes | No |
| Considers future references | No | No | Yes |
| Implementation difficulty | Low | Medium | Medium |
| Belady's Anomaly | Possible | No | No |
| Practical implementation | Yes | Yes | Generally no |

For the sample reference string with three frames, the calculated results are:

| Algorithm | Page Hits | Page Faults | Hit Ratio | Fault Ratio |
|---|---:|---:|---:|---:|
| FIFO | 1 | 9 | 10% | 90% |
| LRU | 2 | 8 | 20% | 80% |
| Optimal | 4 | 6 | 40% | 60% |

The comparison demonstrates that Optimal generates the fewest page faults for the given reference sequence.

However, the performance of FIFO and LRU depends on the page-access pattern. Neither algorithm is guaranteed to perform better than the other for every input.

## 6. Conclusion

FIFO, LRU, and Optimal represent three different approaches to page replacement in operating systems.

FIFO prioritizes page arrival order, LRU uses past access information, and Optimal uses future reference information to make replacement decisions.

Through their implementation in C, this project demonstrates how these strategies affect memory-frame allocation and page fault frequency.

The comparative results help explain why the choice of page replacement algorithm is important in virtual memory management.

Optimal provides a theoretical benchmark, while FIFO and LRU demonstrate practical replacement strategies that can operate without knowing future page requests.