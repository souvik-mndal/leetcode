![Runtime](https://img.shields.io/badge/Runtime-10%20ms%20(beats%2053.33%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.6%20MB%20(beats%2091.74%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an **n x n** square board of characters. 

* You start at the bottom-right corner marked with **'S'** (Start).
* Your goal is to reach the top-left corner marked with **'E'** (End).
* From any position, you can only move in three directions: **Up**, **Left**, or **Up-Left (diagonally)**.
* Some board cells contain numbers from **'1'** to **'9'** which give you score points.
* Some cells contain **'X'**, which are obstacles you cannot step on.
* The start **'S'** and end **'E'** give **0** points.

You need to find two things:
1. The **maximum score** you can collect on any valid path from 'S' to 'E'.
2. The **total number of distinct paths** that achieve this exact maximum score.

Since the number of paths can be very large, return the path count modulo **10^9 + 7**. If no valid path exists from 'S' to 'E', return `[0, 0]`.

For example, on this board:
```
E 2 3
2 X 2
1 2 S
```
The maximum score you can get is **7** (path: S -> 2 -> 2 -> 3 -> E gives score 0 + 2 + 2 + 3 + 0 = 7). There is only **1** path that gives this top score, so the output is `[7, 1]`.

---

## Intuition

Moving from 'S' (bottom-right) to 'E' (top-left) using **Up**, **Left**, and **Up-Left** moves is the same as filling a grid from bottom to top and right to left.

For any cell `(i, j)`, a path coming from 'S' can enter it from three places:
1. **Down** cell `(i + 1, j)`
2. **Right** cell `(i, j + 1)`
3. **Down-Right** cell `(i + 1, j + 1)`

To get the maximum score at cell `(i, j)`, we look at these 3 neighbors and take the highest score among them. 

What about path counting? 
* If only one neighbor gives the highest score, cell `(i, j)` inherits that neighbor's path count.
* If two or all three neighbors tie for the highest score, cell `(i, j)` sums up the path counts from all those tied neighbors.

Since we only ever need information from the **current row** and the **row below it**, we do not need a full 2D matrix in memory. Two 1D arrays (one for scores, one for path counts) are enough.

---

## Approach

* `const int MOD = 1000000007;` — Defines the modulo value to prevent large numbers from overflowing standard integer limits.
* `int n = board.size();` — Stores the length of the board sides.
* `vector<int> nextScore(n + 1, -1);` — Stores the best scores for each column in the row directly below us (`i + 1`). `-1` means the cell cannot be reached from 'S'.
* `vector<int> nextWays(n + 1, 0);` — Stores the number of optimal paths reaching each column in the row directly below us.
* `for (int i = n - 1; i >= 0; --i)` — Loops through rows from bottom (`n - 1`) to top (`0`).
* `vector<int> currScore(n + 1, -1);` — Array to compute the best scores for the current row `i`.
* `vector<int> currWays(n + 1, 0);` — Array to compute path counts for the current row `i`.
* `for (int j = n - 1; j >= 0; --j)` — Loops through columns from right (`n - 1`) to left (`0`).
* `char cell = board[i][j];` — Fetches the character at cell `(i, j)`.
* `if (cell == 'X') continue;` — Skips obstacles, leaving their score as `-1` (unreachable) and ways as `0`.
* `if (cell == 'S') { currScore[j] = 0; currWays[j] = 1; continue; }` — Sets the base case for the start position: score starts at 0, and there is 1 way to be at 'S'.
* `int best = max({ nextScore[j], currScore[j + 1], nextScore[j + 1] });` — Finds the highest score among the 3 valid incoming neighbors: Down, Right, and Down-Right.
* `if (best == -1) continue;` — If all 3 neighbors are unreachable, cell `(i, j)` is also unreachable, so we skip it.
* `long long ways = 0;` — Initializes a temporary variable to sum up path counts from all optimal neighbors.
* `if (nextScore[j] == best) { ways += nextWays[j]; }` — Adds path count from the Down neighbor if it offers the maximum score.
* `if (currScore[j + 1] == best) { ways += currWays[j + 1]; }` — Adds path count from the Right neighbor if it offers the maximum score.
* `if (nextScore[j + 1] == best) { ways += nextWays[j + 1]; }` — Adds path count from the Down-Right neighbor if it offers the maximum score.
* `int value = (cell == 'E') ? 0 : cell - '0';` — Converts numeric character to an integer value, treating 'E' as 0 points.
* `currScore[j] = best + value;` — Updates the best score for cell `(i, j)` by adding its digit value to `best`.
* `currWays[j] = ways % MOD;` — Stores total path count for cell `(i, j)` after applying modulo.
* `nextScore = move(currScore); nextWays = move(currWays);` — Shifts current row data to become the "next row" data for the upcoming row loop iteration.
* `if (nextScore[0] == -1) return {0, 0};` — If 'E' at `(0, 0)` was never reached, returns `[0, 0]`.
* `return {nextScore[0], nextWays[0]};` — Returns the final maximum score and total ways stored at 'E'.

---

## Dry Run

### Case 1: Standard board with obstacle (`board = ["E23", "2X2", "12S"]`)

| Row (`i`) | Col (`j`) | `cell` | Neighbors Check (Down, Right, Down-Right) | `best` | `currScore[j]` | `currWays[j]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 2 | 2 | 'S' | None (Start) | - | 0 | 1 | Base case 'S' initialized. |
| 2 | 1 | '2' | Right: score 0, ways 1 | 0 | 2 | 1 | Moves from 'S', collects 2 points. |
| 2 | 0 | '1' | Right: score 2, ways 1 | 2 | 3 | 1 | Moves from (2,1), collects 1 point. |
| 1 | 2 | '2' | Down: score 0, ways 1 | 0 | 2 | 1 | Moves from 'S', collects 2 points. |
| 1 | 1 | 'X' | Obstacle | - | -1 | 0 | Skipped. |
| 1 | 0 | '2' | Down: score 3, ways 1 | 3 | 5 | 1 | Moves from (2,0), collects 2 points. |
| 0 | 2 | '3' | Down: score 2, ways 1 | 2 | 5 | 1 | Moves from (1,2), collects 3 points. |
| 0 | 1 | '2' | Right: score 5, ways 1 | 5 | 7 | 1 | Moves from (0,2), collects 2 points. |
| 0 | 0 | 'E' | Right: score 7, ways 1 | 7 | 7 | 1 | Best path reached 'E' with score 7. |

**Final Output:** `[7, 1]`

---

### Case 2: Multiple paths tie for maximum score (`board = ["E12", "1X1", "21S"]`)

| Row (`i`) | Col (`j`) | `cell` | Neighbors Check (Down, Right, Down-Right) | `best` | `currScore[j]` | `currWays[j]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 2 | 2 | 'S' | None (Start) | - | 0 | 1 | Base case 'S' initialized. |
| 2 | 1 | '1' | Right: score 0, ways 1 | 0 | 1 | 1 | Moves from 'S', score = 1. |
| 2 | 0 | '2' | Right: score 1, ways 1 | 1 | 3 | 1 | Moves from (2,1), score = 3. |
| 1 | 2 | '1' | Down: score 0, ways 1 | 0 | 1 | 1 | Moves from 'S', score = 1. |
| 1 | 1 | 'X' | Obstacle | - | -1 | 0 | Skipped. |
| 1 | 0 | '1' | Down: score 3, ways 1 | 3 | 4 | 1 | Moves from (2,0), score = 4. |
| 0 | 2 | '2' | Down: score 1, ways 1 | 1 | 3 | 1 | Moves from (1,2), score = 3. |
| 0 | 1 | '1' | Right: score 3, ways 1 | 3 | 4 | 1 | Moves from (0,2), score = 4. |
| 0 | 0 | 'E' | Down: score 4, ways 1<br>Right: score 4, ways 1 | 4 | 4 | 2 | **Tie!** Sums ways from Down (1) and Right (1). |

**Final Output:** `[4, 2]`

---

## Time & Space Complexity

* **Time Complexity:** **O(n^2)** — The board contains `n * n` total cells. We visit each cell exactly once, doing constant time **O(1)** work to evaluate its 3 neighbors.
* **Space Complexity:** **O(n)** — We only store two vectors (`currScore`/`currWays` and `nextScore`/`nextWays`) of length `n + 1` at any time, using memory proportional to a single row length rather than the full `n * n` grid.

### Is this optimal?
**Yes, this code is fully optimal.** 
* **Time:** Any algorithm must inspect the grid cells to find valid paths and numbers, making **O(n^2)** the theoretical minimum time complexity.
* **Space:** Since transitions depend on cells in the row below, holding at least one row's worth of information (**O(n)** space) is required.

---

## Edge Cases Handled

* **Unreachable Goal 'E':** If obstacles completely block all paths from 'S' to 'E' (e.g. board `["E11","XXX","11S"]`), cell `(0,0)` remains `-1` and the code correctly returns `[0, 0]`.
* **Minimum Constraints:** Handles the smallest valid board size (`2 x 2`) without index out-of-bounds errors due to using size `n + 1` vectors initialized with safe default values.
* **Tied Maximum Scores:** Correctly combines path counts using `ways += nextWays[...]` when multiple incoming directions yield the exact same best score.
* **Large Path Counts:** Prevents integer overflow by applying `% 1000000007` to path counts at every step.
