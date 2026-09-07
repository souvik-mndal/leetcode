![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.4%20MB%20(beats%2018.22%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have a grid (a 2D matrix) filled with numbers. You want to read all the numbers by starting at the top-left corner and traveling around the outer edges in a clockwise loop, like tracing a spiral. Once you complete the outer loop, you step inward and trace the next inner loop, continuing until every single number in the grid has been collected into a 1D list.

For example, given this 3 by 3 matrix:

```text
[ 1, 2, 3 ]
[ 4, 5, 6 ]
[ 7, 8, 9 ]
```

You start at `1`, go right (`1 -> 2 -> 3`), go down (`6 -> 9`), go left (`8 -> 7`), go up (`4`), and finally step inside to get `5`. 

The final result is `[1, 2, 3, 6, 9, 8, 7, 4, 5]`.

---

## Intuition

The key idea is to view the matrix as a set of shrinking boundaries or walls:
* **`sr` (start row):** Top border
* **`er` (end row):** Bottom border
* **`sc` (start col):** Left border
* **`ec` (end col):** Right border

We traverse the matrix in four distinct directions in a fixed sequence:
1. **Left to Right** across the top border (`sr`), then move the top border down (`sr++`).
2. **Top to Bottom** down the right border (`ec`), then move the right border left (`ec--`).
3. **Right to Left** across the bottom border (`er`), then move the bottom border up (`er--`).
4. **Bottom to Top** up the left border (`sc`), then move the left border right (`sc++`).

After each border traversal, we shrink that boundary inward. We repeat this cycle until we have collected all elements in the matrix.

---

## Approach

Here is how the logic works line-by-line:

* `int row=matrix.size();` — Finds the total number of rows in the matrix.
* `int col = matrix[0].size();` — Finds the total number of columns in the matrix.
* `int sr=0,sc=0,er=row-1,ec=col-1;` — Sets up four boundary pointers: `sr` at the top row (0), `sc` at the left column (0), `er` at the bottom row (`row - 1`), and `ec` at the right column (`col - 1`).
* `int tot=row*col;` — Calculates the total count of numbers we need to collect.
* `int count=0;` — Initializes a counter to track how many numbers have been collected so far.
* `vector<int>ans;` — Creates the output array to store elements in spiral order.
* `while( count<tot)` — Keeps the process running until all `tot` elements are placed into `ans`.
* `for( int i=sc;i<=ec && count<tot ; i++)` — Moves **Left to Right** across the current top row `sr`. Pushes each element `matrix[sr][i]` into `ans`, increments `count`, and then `sr++` moves the top border down.
* `for( int i=sr;i<=er && count<tot ; i++)` — Moves **Top to Bottom** down the current right column `ec`. Pushes each element `matrix[i][ec]` into `ans`, increments `count`, and then `ec--` moves the right border left.
* `for( int i=ec;i>=sc && count<tot ; i--)` — Moves **Right to Left** across the current bottom row `er`. Pushes each element `matrix[er][i]` into `ans`, increments `count`, and then `er--` moves the bottom border up.
* `for( int i=er;i>=sr && count<tot ; i--)` — Moves **Bottom to Top** up the current left column `sc`. Pushes each element `matrix[i][sc]` into `ans`, increments `count`, and then `sc++` moves the left border right.

---

## Dry Run

### Case 1: Square Matrix (`3 x 3`)

Input: `matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]`
Initial values: `row = 3`, `col = 3`, `tot = 9`, `count = 0`, `sr = 0`, `sc = 0`, `er = 2`, `ec = 2`.

| Step | Boundaries (`sr`, `er`, `sc`, `ec`) | `count` | `ans` | Action |
|---|---|---|---|---|
| Start | `sr=0, er=2, sc=0, ec=2` | 0 | `[]` | Enter `while(count < 9)`. |
| 1 | `sr=0, er=2, sc=0, ec=2` | 3 | `[1, 2, 3]` | Move Left->Right on row 0. `sr` becomes 1. |
| 2 | `sr=1, er=2, sc=0, ec=2` | 5 | `[1, 2, 3, 6, 9]` | Move Top->Bottom on col 2. `ec` becomes 1. |
| 3 | `sr=1, er=2, sc=0, ec=1` | 7 | `[1, 2, 3, 6, 9, 8, 7]` | Move Right->Left on row 2. `er` becomes 1. |
| 4 | `sr=1, er=1, sc=0, ec=1` | 8 | `[1, 2, 3, 6, 9, 8, 7, 4]` | Move Bottom->Top on col 0. `sc` becomes 1. |
| 5 | `sr=1, er=1, sc=1, ec=1` | 9 | `[1, 2, 3, 6, 9, 8, 7, 4, 5]` | Move Left->Right on row 1. `sr` becomes 2. |
| End | `sr=2, er=1, sc=1, ec=1` | 9 | `[1, 2, 3, 6, 9, 8, 7, 4, 5]` | `count == tot` (9 == 9). Loop ends. |

---

### Case 2: Rectangular Matrix (`3 x 4`)

Input: `matrix = [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]`
Initial values: `row = 3`, `col = 4`, `tot = 12`, `count = 0`, `sr = 0`, `sc = 0`, `er = 2`, `ec = 3`.

| Step | Boundaries (`sr`, `er`, `sc`, `ec`) | `count` | `ans` | Action |
|---|---|---|---|---|
| Start | `sr=0, er=2, sc=0, ec=3` | 0 | `[]` | Enter `while(count < 12)`. |
| 1 | `sr=0, er=2, sc=0, ec=3` | 4 | `[1, 2, 3, 4]` | Move Left->Right on row 0. `sr` becomes 1. |
| 2 | `sr=1, er=2, sc=0, ec=3` | 6 | `[1, 2, 3, 4, 8, 12]` | Move Top->Bottom on col 3. `ec` becomes 2. |
| 3 | `sr=1, er=2, sc=0, ec=2` | 8 | `[1, 2, 3, 4, 8, 12, 11, 10, 9]` | Move Right->Left on row 2. `er` becomes 1. |
| 4 | `sr=1, er=1, sc=0, ec=2` | 9 | `[1, 2, 3, 4, 8, 12, 11, 10, 9, 5]` | Move Bottom->Top on col 0. `sc` becomes 1. |
| 5 | `sr=1, er=1, sc=1, ec=2` | 11 | `[1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7]` | Move Left->Right on row 1. `sr` becomes 2. |
| 6 | `sr=2, er=1, sc=1, ec=2` | 12 | `[1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7]` | Inner loops stop because `count < tot` fails. |
| End | `sr=2, er=1, sc=1, ec=2` | 12 | `[1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7]` | `count == tot` (12 == 12). Loop ends. |

---

## Time & Space Complexity

* **Time Complexity:** **O(m * n)** — where `m` is the number of rows and `n` is the number of columns. The code visits every element in the matrix exactly once to place it into the result vector.
* **Space Complexity:** **O(1)** auxiliary space — excluding the memory needed for the output array `ans`. The code only uses a few integer scalar variables (`row`, `col`, `sr`, `sc`, `er`, `ec`, `tot`, `count`) to track the traversal state.

**Is this code optimal?**
Yes, this approach is already optimal. 

To output every element in a matrix of size `m * n`, any algorithm must inspect all `m * n` elements at least once. Therefore, **O(m * n)** is the theoretical lower bound for time complexity. The space complexity is also optimal at **O(1)** extra memory since no extra dynamic data structures are used.

---

## Edge Cases Handled

* **Single Element (`1 x 1`):** `sr`, `sc`, `er`, and `ec` are all `0`. The first loop collects `matrix[0][0]`, increments `count` to 1, and all subsequent loops skip because `count < tot` becomes false.
* **Single Row (`1 x n`):** Traverses left to right on the only row. The `count < tot` check inside subsequent loops prevents traversing back right-to-left on the same row.
* **Single Column (`m x 1`):** Traverses left to right for 1 element, then top to bottom for the rest of the column. The `count < tot` condition ensures no duplicate scanning occurs.
* **Non-Square Rectangles (`m != n`):** Handled safely because `count < tot` prevents traversing overlapping boundaries once all elements are added.
