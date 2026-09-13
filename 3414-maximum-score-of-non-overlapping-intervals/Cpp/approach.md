![Runtime](https://img.shields.io/badge/Runtime-673%20ms%20(beats%2078.95%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-271.96%20MB%20(beats%2073.68%25)-green?style=for-the-badge)

---

## Problem Explained

You are given a list of intervals. Each interval has a start point, an end point, a weight (value), and an original index (0, 1, 2, and so on).

Your goal is to pick **at most 4 intervals** such that:
1. **No two chosen intervals overlap.** Two intervals overlap if they share even a single point. For example, if one interval ends at position 5, another interval cannot start at position 5 (it must start at 6 or later).
2. **The total weight sum is maximized.**
3. **If there is a tie** in total weight sum, you must pick the set of intervals whose original indices form the **lexicographically smallest** list. For example, index list `[1, 4]` is smaller than `[2, 3]` because at the first difference, `1` is less than `2`.

Finally, return the list of chosen original indices.

---

## Intuition

This problem combines **Sorting**, **Dynamic Programming (DP)**, and **Binary Search**.

1. **Why Sort by End Time?**
   If we process intervals ordered by their end times from left to right, any decision we make for a new interval only depends on intervals that finished before it started. This turns an unordered selection problem into a clean left-to-right decision process.

2. **The Core DP Idea:**
   At each interval, we have two choices:
   * **Skip it:** Keep the best answer found so far using intervals up to the previous one.
   * **Take it:** Add its weight to the best answer formed by using one fewer interval (`k - 1` intervals) that ended strictly before this interval starts.

3. **Finding Previous Non-Overlapping Intervals Fast:**
   Because all end times are sorted, we can use **Binary Search** (`lower_bound`) to instantly locate the last interval that ends before our current start time.

4. **Handling Ties Lexicographically:**
   Instead of just tracking maximum score numbers in our DP table, each DP cell stores a `Node` holding both the maximum score and the list of original indices. Whenever two choices produce the same weight, we compare their index lists and pick the lexicographically smaller one.

---

## Approach

Here is how the code builds the solution step-by-step:

* `struct Node` — Represents a state storing the total `score` (weight sum), a `vector<int> ids` of original indices, and a `valid` boolean flag to mark initialized states.
* `bool better(const Node& a, const Node& b)` — A helper function that compares two solutions `a` and `b`. It returns true if `a` is valid and has a higher score than `b`, or if scores are equal, if `a.ids` is lexicographically smaller than `b.ids`.
* `vector<array<long long, 4>> a(n);` — Packs each interval's start, end, weight, and original index together so we don't lose the original index after sorting.
* `sort(a.begin(), a.end(), ...)` — Sorts all intervals in ascending order by their end times (`x[1] < y[1]`).
* `vector<long long> ends(n);` — Extracts the sorted end times into a standalone list so we can perform fast binary searches on it.
* `vector<vector<Node>> dp(K + 1, vector<Node>(n + 1));` — Creates a 2D DP table where `dp[k][i]` holds the best `Node` (score and index list) using up to `k` intervals chosen from the first `i` sorted intervals.
* `for (int i = 0; i <= n; ++i) dp[0][i] = Node(0, {});` — Base case setting 0 intervals chosen to have a score of 0 and an empty index list.
* `int p = lower_bound(ends.begin(), ends.begin() + (i - 1), l) - ends.begin();` — Uses binary search to find how many intervals in `ends` finish strictly before the current interval's start time `l`. The index `p` represents the count of such valid non-overlapping intervals.
* `dp[k][i] = dp[k][i - 1];` — Default decision for state `dp[k][i]`: skip the current interval `i` and inherit the best result from `dp[k][i - 1]`.
* `if (dp[k - 1][p].valid)` — Checks if taking the current interval is possible by checking if choosing `k - 1` intervals from the first `p` non-overlapping intervals is valid.
* `Node take(dp[k - 1][p].score + w, std::move(ids));` — Constructs the new candidate solution if we take interval `i`, adding its weight `w` and sorting its combined index list.
* `if (better(take, dp[k][i])) dp[k][i] = std::move(take);` — Replaces the "skip" choice with the "take" choice if taking interval `i` yields a higher score or a lexicographically smaller index list.
* `for (int k = 1; k <= K; ++k) if (better(dp[k][n], ans)) ans = dp[k][n];` — Scans the final column of the DP table across all counts `k` (from 1 to 4) to find the globally optimal set of indices.

---

## Dry Run

### Case 1: Standard Case with Multiple Options
Input: `intervals = [[1,3,2], [4,5,2], [1,5,5], [6,9,3], [6,7,1], [8,9,1]]`

After storing original indices and sorting by end times:
* Sorted Index 1: `[1, 3, 2]`, Orig Index = `0`, End = `3`
* Sorted Index 2: `[4, 5, 2]`, Orig Index = `1`, End = `5`
* Sorted Index 3: `[1, 5, 5]`, Orig Index = `2`, End = `5`
* Sorted Index 4: `[6, 7, 1]`, Orig Index = `4`, End = `7`
* Sorted Index 5: `[6, 9, 3]`, Orig Index = `3`, End = `9`
* Sorted Index 6: `[8, 9, 1]`, Orig Index = `5`, End = `9`

`ends` array: `[3, 5, 5, 7, 9, 9]`

| Step (`i`) | Interval `a[i-1]` | Start (`l`), Weight (`w`), Orig Index (`idx`) | Binary Search `p` | Best Score `dp[k][i]` updates | Action / Decision |
|---|---|---|---|---|---|
| 1 | `[1, 3, 2]` | `l=1`, `w=2`, `idx=0` | `p=0` | `dp[1][1]` = Score 2, `[0]` | Take interval 0 for `k=1`. |
| 2 | `[4, 5, 2]` | `l=4`, `w=2`, `idx=1` | `p=1` (ends `[3]` < 4) | `dp[2][2]` = Score 4, `[0, 1]` | For `k=2`, combine with `dp[1][1]` to get score 2 + 2 = 4. |
| 3 | `[1, 5, 5]` | `l=1`, `w=5`, `idx=2` | `p=0` | `dp[1][3]` = Score 5, `[2]` | Take interval 2 for `k=1`. Score 5 beats previous best score 2. |
| 4 | `[6, 7, 1]` | `l=6`, `w=1`, `idx=4` | `p=3` (ends `[3,5,5]` < 6) | `dp[2][4]` = Score 6, `[2, 4]` | Combine with `dp[1][3]` (score 5) to get total score 6. |
| 5 | `[6, 9, 3]` | `l=6`, `w=3`, `idx=3` | `p=3` (ends `[3,5,5]` < 6) | `dp[2][5]` = Score 8, `[2, 3]` | Combine with `dp[1][3]` (score 5) to get total score 8. Beats score 6. |
| 6 | `[8, 9, 1]` | `l=8`, `w=1`, `idx=5` | `p=4` (ends `[3,5,5,7]` < 8) | `dp[3][6]` = Score 7, `[2, 4, 5]` | Best score remain 8 at `k=2`. |

Final Result: Max score is 8 with indices `[2, 3]`.

---

### Case 2: Simple Disjoint Intervals
Input: `intervals = [[1, 2, 10], [3, 4, 20]]`

Sorted intervals:
* Sorted Index 1: `[1, 2, 10]`, Orig Index = `0`, End = `2`
* Sorted Index 2: `[3, 4, 20]`, Orig Index = `1`, End = `4`

`ends` array: `[2, 4]`

| Step (`i`) | Interval `a[i-1]` | Start (`l`), Weight (`w`), Orig Index (`idx`) | Binary Search `p` | Best Score `dp[k][i]` updates | Action / Decision |
|---|---|---|---|---|---|
| 1 | `[1, 2, 10]` | `l=1`, `w=10`, `idx=0` | `p=0` | `dp[1][1]` = Score 10, `[0]` | Take interval 0 for `k=1`. |
| 2 | `[3, 4, 20]` | `l=3`, `w=20`, `idx=1` | `p=1` (ends `[2]` < 3) | `dp[2][2]` = Score 30, `[0, 1]` | Combine interval 1 with `dp[1][1]` (score 10) to get 30. |

Final Result: Max score is 30 with indices `[0, 1]`.

---

## Time & Space Complexity

- **Time Complexity:** `O(N * log N)` where `N` is the number of intervals.
  - Sorting `N` intervals by end time takes `O(N * log N)` time.
  - The main loop runs `N` times. Inside the loop, `lower_bound` takes `O(log N)` time.
  - The inner loop runs `K = 4` times (a constant). Operations like sorting index lists take `O(K * log K)` time, which is `O(1)` since `K = 4`.
  - Total time simplifies to `O(N * log N)`.

- **Space Complexity:** `O(N)` (or `O(N * K)` with `K = 4`).
  - The DP table has dimensions `5 * (N + 1)`.
  - Each DP cell holds a small list of at most 4 integers.
  - Total space scales linearly with `N`.

### Is this optimal?
**Yes, this solution is optimal.** Because the input intervals are unsorted, any valid algorithm must inspect or sort the intervals, establishing an `O(N * log N)` lower bound on time. The space complexity `O(N)` is also optimal for dynamic programming lookup when binary search requires querying arbitrary past indices `p`.

---

## Edge Cases Handled

* **Touching Boundaries:** If interval A ends at 5 and interval B starts at 5, they overlap. The `lower_bound` binary search specifically searches for end times strictly less than the start time (`end < l`), correctly treating touching boundaries as overlapping.
* **Fewer than 4 Intervals Selected:** The problem allows picking *at most* 4 intervals. The DP base cases and final check (`k` from 1 to 4) handle picking 1, 2, 3, or 4 intervals seamlessly.
* **Equal Weight Ties:** Handled by `better()`, which uses C++ vector comparison (`a.ids < b.ids`) to select the lexicographically smaller set of original indices whenever scores match.
* **Unsorted Input Indices:** Preserved by storing `[start, end, weight, original_index]` before sorting.
* **Large Coordinate Range:** Coordinates up to `10^9` and weights up to `10^9` fit safely in `long long` integers without integer overflow.
