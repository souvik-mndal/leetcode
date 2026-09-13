![Runtime](https://img.shields.io/badge/Runtime-7%20ms%20(beats%2096.64%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-13.92%20MB%20(beats%2047.87%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given two square grids filled with only `0`s and `1`s, named `img1` and `img2`. Both grids have the same size, `n x n`. 

Imagine `img1` is drawn on a transparent sheet placed over `img2`. You can slide `img1` in any direction—left, right, up, or down—by any number of steps. You cannot rotate the image. Whenever a part of `img1` slides off the edge of the grid, those numbers are cut off and lost.

Your goal is to slide `img1` to a position where as many `1`s as possible land directly on top of `1`s in `img2`. You need to return the highest possible count of overlapping `1`s.

For example, if `img1` has a `1` at the top-left and `img2` has a `1` at the bottom-right, sliding `img1` down and right lines them up, giving an overlap count of 1.

---

## Intuition

A brute-force way is to try sliding `img1` to every possible position and check the whole grid every time. But that performs a lot of unnecessary work checking zeros.

The clever trick is to focus only on the coordinates of the `1`s. 

If a `1` in `img1` is at coordinate `(a_row, a_col)` and a `1` in `img2` is at coordinate `(b_row, b_col)`, the exact shift needed to line them up is:
- Row shift: `b_row - a_row`
- Column shift: `b_col - a_col`

If multiple pairs of `1`s require the exact same row and column shift, all of those pairs will overlap simultaneously when we apply that single shift! 

So, we just list all `1`s in `img1` and all `1`s in `img2`, calculate the required shift for every possible pair, and find which shift vector occurs most often.

---

## Approach

Here is how the algorithm works step-by-step:

* `int n = img1.size();` — Finds the dimension `n` of the square grid.
* `vector<pair<int,int>> A, B;` — Creates two lists to store the `(row, column)` coordinates of every `1` found in `img1` (list `A`) and `img2` (list `B`).
* `for (int i = 0; i < n; ++i) { for (int j = 0; j < n; ++j) { ... } }` — Loops through every row `i` and column `j` of both grids.
* `if (img1[i][j] == 1) A.emplace_back(i, j);` — Saves the coordinate `(i, j)` into `A` whenever a `1` is found in `img1`.
* `if (img2[i][j] == 1) B.emplace_back(i, j);` — Saves the coordinate `(i, j)` into `B` whenever a `1` is found in `img2`.
* `vector<vector<int>> cnt(2 * n, vector<int>(2 * n, 0));` — Creates a 2D grid of counters. The grid size is `2 * n` by `2 * n` so we can safely store negative shift values by adding `n` as an offset.
* `int best = 0;` — Initializes a variable to track the maximum overlap count found.
* `for (auto& a : A) { for (auto& b : B) { ... } }` — Loops through every `1` coordinate in `A` paired with every `1` coordinate in `B`.
* `int dx = b.first - a.first + n;` — Calculates the row shift needed to move coordinate `a` onto coordinate `b`. We add `n` so the index is always positive.
* `int dy = b.second - a.second + n;` — Calculates the column shift needed to move coordinate `a` onto coordinate `b`. We add `n` so the index is always positive.
* `best = max(best, ++cnt[dx][dy]);` — Increments the count for this specific shift offset, then updates `best` if this shift now has the highest overlap count.
* `return best;` — Returns the maximum overlap found.

---

## Dry Run

### Case 1: Standard 3x3 Grid
`img1 = [[1,1,0],[0,1,0],[0,1,0]]`
`img2 = [[0,0,0],[0,1,1],[0,0,1]]`

Grid size `n = 3`. 
`1` coordinates in `A`: `(0,0)`, `(0,1)`, `(1,1)`, `(2,1)`
`1` coordinates in `B`: `(1,1)`, `(1,2)`, `(2,2)`

| Step | `a` in `A` | `b` in `B` | `dx = b.r - a.r + 3` | `dy = b.c - a.c + 3` | `cnt[dx][dy]` | `best` | Action / Explanation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | (0,0) | (1,1) | 1-0+3 = 4 | 1-0+3 = 4 | 1 | 1 | Shift (1, 1) lines up 1 pair. |
| 2 | (0,0) | (1,2) | 1-0+3 = 4 | 2-0+3 = 5 | 1 | 1 | Shift (1, 2) lines up 1 pair. |
| 3 | (0,0) | (2,2) | 2-0+3 = 5 | 2-0+3 = 5 | 1 | 1 | Shift (2, 2) lines up 1 pair. |
| 4 | (0,1) | (1,1) | 1-0+3 = 4 | 1-1+3 = 3 | 1 | 1 | Shift (1, 0) lines up 1 pair. |
| 5 | (0,1) | (1,2) | 1-0+3 = 4 | 2-1+3 = 4 | 2 | 2 | Shift (1, 1) lines up a 2nd pair! |
| 6 | (0,1) | (2,2) | 2-0+3 = 5 | 2-1+3 = 4 | 1 | 2 | Shift (2, 1) lines up 1 pair. |
| 7 | (1,1) | (1,1) | 1-1+3 = 3 | 1-1+3 = 3 | 1 | 2 | Shift (0, 0) lines up 1 pair. |
| 8 | (1,1) | (1,2) | 1-1+3 = 3 | 2-1+3 = 4 | 1 | 2 | Shift (0, 1) lines up 1 pair. |
| 9 | (1,1) | (2,2) | 2-1+3 = 4 | 2-1+3 = 4 | 3 | 3 | Shift (1, 1) lines up a 3rd pair! |
| 10 | (2,1) | (1,1) | 1-2+3 = 2 | 1-1+3 = 3 | 1 | 3 | Shift (-1, 0) lines up 1 pair. |
| 11 | (2,1) | (1,2) | 1-2+3 = 2 | 2-1+3 = 4 | 1 | 3 | Shift (-1, 1) lines up 1 pair. |
| 12 | (2,1) | (2,2) | 2-2+3 = 3 | 2-1+3 = 4 | 2 | 3 | Shift (0, 1) lines up a 2nd pair. |

**Final Output:** `3`

---

### Case 2: Minimal 1x1 Grid
`img1 = [[1]]`
`img2 = [[1]]`

Grid size `n = 1`.
`1` coordinates in `A`: `(0,0)`
`1` coordinates in `B`: `(0,0)`

| Step | `a` in `A` | `b` in `B` | `dx = b.r - a.r + 1` | `dy = b.c - a.c + 1` | `cnt[dx][dy]` | `best` | Action / Explanation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | (0,0) | (0,0) | 0-0+1 = 1 | 0-0+1 = 1 | 1 | 1 | Zero shift lines up the single 1. |

**Final Output:** `1`

---

## Time & Space Complexity

- **Time Complexity:** 
  - **Worst Case:** `O(n^4)`. Let `L1` be the number of `1`s in `img1` and `L2` be the number of `1`s in `img2`. Extracting coordinates takes `O(n^2)` time. The nested loops run `L1 * L2` times. In the worst case where both grids are entirely filled with `1`s, `L1 = n^2` and `L2 = n^2`, giving `n^2 * n^2 = n^4` operations.
  - **Average/Sparse Case:** `O(n^2 + L1 * L2)`. If the images contain few `1`s, this approach is extremely fast.

- **Space Complexity:** `O(n^2)`. Storing lists `A` and `B` takes `O(L1 + L2)` space (at most `2 * n^2`). The frequency grid `cnt` has a fixed size of `(2 * n) * (2 * n) = 4 * n^2`, which simplifies to `O(n^2)`.

---

### Can We Improve It?

Yes, worst-case time complexity can be improved! 

Because the grid size constraint is very small (`n <= 30`), each row of `30` elements can fit inside a single 32-bit integer (`int`). We can compress each row into a single integer where bit `1` represents a `1` cell and bit `0` represents a `0` cell.

With bitmasking:
1. Converting each row to a bitmask takes `O(n)` per image.
2. We iterate over all `2 * n` possible vertical shifts and `2 * n` possible horizontal shifts.
3. For a given shift, counting overlapping `1`s takes `O(1)` bitwise AND operations per row, summing across `n` rows.

This drops worst-case time complexity to **`O(n^3)`**.

```cpp
int largestOverlap(vector<vector<int>>& img1, vector<vector<int>>& img2) {
    int n = img1.size();
    vector<int> A(n, 0), B(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (img1[i][j]) A[i] |= (1 << j);
            if (img2[i][j]) B[i] |= (1 << j);
        }
    }
    int best = 0;
    for (int row_shift = -n + 1; row_shift < n; ++row_shift) {
        for (int col_shift = -n + 1; col_shift < n; ++col_shift) {
            int current_overlap = 0;
            for (int i = 0; i < n; ++i) {
                int img1_row = i + row_shift;
                if (img1_row < 0 || img1_row >= n) continue;
                if (col_shift >= 0)
                    current_overlap += __builtin_popcount((A[img1_row] << col_shift) & B[i]);
                else
                    current_overlap += __builtin_popcount((A[img1_row] >> (-col_shift)) & B[i]);
            }
            best = max(best, current_overlap);
        }
    }
    return best;
}
```

- `A[i] |= (1 << j)` packs row `i` into a single integer bitmask.
- `__builtin_popcount(...)` counts set bits in `O(1)` time after bit-shifting and combining rows with bitwise AND (`&`).

- **Improved Complexity:** **Time:** `O(n^3)` worst-case. **Space:** `O(n)` to store row bitmasks.
- **Theoretical Limit:** Fast Fourier Transform (FFT) / 2D Convolution can achieve `O(n^2 * log(n))`, but for `n <= 30`, the bitmask `O(n^3)` approach is practically the fastest and easiest to implement.

---

## Edge Cases Handled

* **Grids with No `1`s:** If either matrix contains only zeros, list `A` or `B` remains empty. The nested loop never runs, and the code returns `0` correctly.
* **Size `n = 1`:** Handled cleanly without out-of-bounds errors because array index offsets `+ n` accommodate small board sizes.
* **Negative Shifts:** Sliding an image up or left yields negative shift coordinates (`b - a < 0`). Adding `+ n` shifts all indices into the positive range `[0, 2n - 1]`, preventing invalid matrix indexing.
* **All Ones:** When both matrices are full of `1`s, the zero-shift position `(dx = n, dy = n)` accumulates `n * n` matches, returning `n * n` correctly.
