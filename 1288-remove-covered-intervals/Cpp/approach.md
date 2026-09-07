![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2042.93%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.9%20MB%20(beats%2078.81%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of intervals. Each interval is like a time block with a start point and an end point. For example, interval [1, 4] starts at 1 and ends at 4. 

An interval is considered "covered" if it sits completely inside another interval. Look at interval [3, 6] and interval [2, 8]. Interval [2, 8] starts at 2 (which is before or at 3) and ends at 8 (which is after or at 6). Because [3, 6] is completely swallowed up by [2, 8], we remove [3, 6]. 

Our job is to count how many intervals are left after we remove all the covered ones.

## Intuition

The main trick to solving this easily is **sorting the intervals properly**. 

If we sort all the intervals by their start points from smallest to largest, we already know the current interval's start point is greater than or equal to the previous ones. But what happens if two intervals start at the exact same place? For example, [1, 5] and [1, 2]. The smaller one [1, 2] is completely covered by the larger one [1, 5]. 

To handle ties neatly, we sort by start point ascending, but if start points are equal, we sort by end point **descending** (largest end point first). 

Once sorted like this, we can just walk through the list from left to right. We only need to track the furthest end point we have seen so far. If a new interval's end point goes beyond that furthest end point, it means this interval is not covered, so we keep it and update our furthest end point. If its end point does not go beyond, it means it is fully swallowed up by a previous interval, so we drop it.

## Approach

Here is how the C++ code implements this logic step by step:

* `ranges::sort(A, {}, [](auto& x) { return pair{x[0], -x[1]}; });`
  This sorts the interval list A. It uses a custom sorting rule: sort primarily by the start point x[0] from smallest to largest. If two intervals share the same start point, it sorts by the end point x[1] multiplied by negative one, which puts the interval with the *largest* end point first.

* `int res = 0, r = 0;`
  This sets up two variables. `res` will count how many intervals remain (not covered). `r` tracks the furthest end point we have encountered so far among the intervals we have processed.

* `for (auto& x : A)`
  This loops through each interval `x` in our sorted list one by one.

* `res += x[1] > r;`
  This checks if the current interval's end point `x[1]` stretches past our record-holder `r`. If it does, `x[1] > r` evaluates to true (which counts as 1), adding 1 to our surviving interval count `res`. If it does not, it adds 0 because the interval is covered.

* `r = max(r, x[1]);`
  This updates our furthest end point record `r` by taking the maximum between our old record `r` and the current interval's end point `x[1]`.

* `return res;`
  This sends back the final count of remaining uncovered intervals.

## Dry Run

### Case 1: Typical case (intervals = [[1,4],[3,6],[2,8]])

After sorting by start ascending and end descending, the list becomes: [[1, 4], [2, 8], [3, 6]].

| `x` (current interval) | `r` (before step) | `x[1] > r` | `res` (updated) | `r` (updated) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| [1, 4] | 0 | 4 > 0 (True) | 1 | 4 | Keep [1, 4], update max end to 4 |
| [2, 8] | 4 | 8 > 4 (True) | 2 | 8 | Keep [2, 8], update max end to 8 |
| [3, 6] | 8 | 6 > 8 (False) | 2 | 8 | [3, 6] is covered by [2, 8], skip it |

Final Output: 2

### Case 2: Overlapping start points (intervals = [[1,4],[2,3]])

After sorting, the list is: [[1, 4], [2, 3]].

| `x` (current interval) | `r` (before step) | `x[1] > r` | `res` (updated) | `r` (updated) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| [1, 4] | 0 | 4 > 0 (True) | 1 | 4 | Keep [1, 4], update max end to 4 |
| [2, 3] | 4 | 3 > 4 (False) | 1 | 4 | [2, 3] ends at 3, which is <= 4. It is covered, skip it |

Final Output: 1

## Time & Space Complexity

**Time:** O(n log n) — The dominant operation is sorting the array of length n. The subsequent for-loop scans through the array in linear time O(n), so the total time is dominated by the sorting step.
**Space:** O(log n) or O(n) depending on the sorting implementation's internal stack usage, but auxiliary space is minimal beyond the input storage.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal possible time complexity. Any algorithm to solve this must at least inspect or order the intervals, and comparison-based sorting has a theoretical minimum lower bound of O(n log n). You cannot beat O(n log n) time for a general sorting-based approach.

## Edge Cases Handled

* **Identical start points:** Handled correctly because the custom sort puts the interval with the longer end point first, ensuring the shorter one is correctly flagged as covered.
* **Completely disjoint intervals:** Handled naturally because `r` updates, and each new interval's start point is further along, but `r` won't swallow them unless their end points are smaller.
* **Single interval:** The loop runs once, `res` becomes 1, and it returns 1 correctly.
