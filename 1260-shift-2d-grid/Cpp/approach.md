![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-18.2%20MB%20(beats%2064.12%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a grid of numbers arranged in rows and columns. Shifting the grid means taking every number and moving it one spot forward, just like reading words in a book from left to right:

1. A number moves to the right in the same row.
2. If a number is at the very end of a row, it moves to the start of the next row down.
3. If a number is at the bottom-right corner, it wraps around to the top-left corner.

You are given a grid of size `m` rows by `n` columns, and a number `k`. You need to perform this shift process `k` times and return the modified grid.

For example, if the grid is:
```text
[1, 2, 3]
[4, 5, 6]
[7, 8, 9]
```
And `k = 1`, moving every element forward by 1 spot results in:
```text
[9, 1, 2]
[3, 4, 5]
[6, 7, 8]
```

---

## Intuition

If you shift a grid element-by-element `k` times, it takes a lot of repeated work. A clever trick is to view the 2D grid as a single flat 1D list of numbers. 

If you lay out a 3x3 grid into a flat list of length 9, shifting everything right by `k` spots is just moving indices forward by `k`.

To connect 2D coordinates `(row, col)` with 1D index positions:
- **2D to 1D:** `flat_index = row * total_columns + col`
- **1D to 2D:** `row = flat_index / total_columns` and `col = flat_index % total_columns`

Also, if you have 9 total elements and you shift 9 times, the grid ends up back in its original starting position. Therefore, shifting 10 times is identical to shifting 1 time. We can reduce `k` immediately using the remainder operation (modulo): `k = k % total_elements`.

By calculating where each element ends up directly, we can build the answer in a single pass.

---

## Approach

Here is how the code works step-by-step:

- `int m = grid.size();` and `int n = grid[0].size();` — Reads the number of rows (`m`) and columns (`n`) in the grid.
- `int total = m * n;` — Calculates the total number of elements inside the grid.
- `k %= total;` — Uses modulo (remainder of division) to eliminate full complete loops. Shifting `k` times is identical to shifting `k % total` times.
- `vector<vector<int>> ans(m, vector<int>(n));` — Creates a new empty grid named `ans` with the exact same dimensions to store our shifted values.
- `for (int i = 0; i < m; i++)` and `for (int j = 0; j < n; j++)` — Uses two nested loops to visit every cell `(i, j)` in the input grid.
- `int oldIndex = i * n + j;` — Converts the current 2D coordinate `(i, j)` into its corresponding 1D index.
- `int newIndex = (oldIndex + k) % total;` — Adds `k` to find the target 1D index, using `% total` so indices wrap around back to 0 if they go past the end.
- `int newRow = newIndex / n;` — Converts the target 1D index back into its 2D row coordinate.
- `int newCol = newIndex % n;` — Converts the target 1D index back into its 2D column coordinate.
- `ans[newRow][newCol] = grid[i][j];` — Places the original number into its final destination inside `ans`.
- `return ans;` — Returns the fully shifted grid.

---

## Dry Run

### Case 1: Standard shift (`grid = [[1,2,3],[4,5,6],[7,8,9]]`, `k = 1`)

`m = 3`, `n = 3`, `total = 9`. 
`k = 1 % 9 = 1`.

| `i` | `j` | `grid[i][j]` | `oldIndex` | `newIndex` (`+1 % 9`) | `newRow` | `newCol` | Action |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 1 | 0 | 1 | 0 | 1 | Place 1 at `ans[0][1]` |
| 0 | 1 | 2 | 1 | 2 | 0 | 2 | Place 2 at `ans[0][2]` |
| 0 | 2 | 3 | 2 | 3 | 1 | 0 | Place 3 at `ans[1][0]` |
| 1 | 0 | 4 | 3 | 4 | 1 | 1 | Place 4 at `ans[1][1]` |
| 1 | 1 | 5 | 4 | 5 | 1 | 2 | Place 5 at `ans[1][2]` |
| 1 | 2 | 6 | 5 | 6 | 2 | 0 | Place 6 at `ans[2][0]` |
| 2 | 0 | 7 | 6 | 7 | 2 | 1 | Place 7 at `ans[2][1]` |
| 2 | 1 | 8 | 7 | 8 | 2 | 2 | Place 8 at `ans[2][2]` |
| 2 | 2 | 9 | 8 | 0 | 0 | 0 | Place 9 at `ans[0][0]` |

**Result:** `[[9,1,2],[3,4,5],[6,7,8]]`

---

### Case 2: Full wrap-around (`grid = [[1,2,3],[4,5,6],[7,8,9]]`, `k = 9`)

`m = 3`, `n = 3`, `total = 9`.
`k = 9 % 9 = 0`.

| `i` | `j` | `grid[i][j]` | `oldIndex` | `newIndex` (`+0 % 9`) | `newRow` | `newCol` | Action |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 1 | 0 | 0 | 0 | 0 | Place 1 at `ans[0][0]` |
| ... | ... | ... | ... | ... | ... | ... | *All elements land back in original spots* |
| 2 | 2 | 9 | 8 | 8 | 2 | 2 | Place 9 at `ans[2][2]` |

**Result:** `[[1,2,3],[4,5,6],[7,8,9]]`

---

## Time & Space Complexity

- **Time Complexity:** **O(m * n)** — We process each element in the grid exactly once.
- **Space Complexity:** **O(m * n)** — We create a new 2D vector `ans` of dimensions `m` by `n` to store the result.

### Can this be improved?

**Yes, space complexity can be reduced to O(1) auxiliary space** if we are allowed to modify the grid in-place (excluding the return memory).

To shift an array right by `k` positions in-place without extra space, we can use the three-reversal trick:
1. Flatten the perspective and reverse the entire array from index `0` to `total - 1`.
2. Reverse the first `k` elements (indices `0` to `k - 1`).
3. Reverse the remaining elements (indices `k` to `total - 1`).

To implement this on a 2D grid without flattening it into an extra 1D array, we write a helper function that reverses grid elements given 1D start and end indices.

```cpp
// Helper function to reverse a section of a 2D grid in-place using 1D indices
void reverseGrid(vector<vector<int>>& grid, int start, int end, int n) {
    while (start < end) {
        int r1 = start / n, c1 = start % n;
        int r2 = end / n,   c2 = end % n;
        swap(grid[r1][c1], grid[r2][c2]);
        start++;
        end--;
    }
}

// Inside main function:
k %= total;
reverseGrid(grid, 0, total - 1, n); // Step 1: reverse all elements
reverseGrid(grid, 0, k - 1, n);     // Step 2: reverse first k elements
reverseGrid(grid, k, total - 1, n); // Step 3: reverse remaining elements
```

- **Improved Space Complexity:** **O(1)** extra auxiliary space (modifies input grid directly).
- **Theoretical Best Complexity:** **Time: O(m * n)**, **Space: O(1)** extra space. The reversed approach reaches this theoretical optimum.

---

## Edge Cases Handled

- **`k` is 0 or a multiple of `total`:** The modulo operation `k %= total` reduces `k` to `0`, making elements map directly back to their original positions without unnecessary work.
- **`k` is larger than total elements (`k > m * n`):** Handled safely by `k %= total`, keeping calculations strictly within bounds.
- **Single element grid (`1 x 1`):** `total = 1`, `k %= 1` yields `0`. The single element maps to itself without out-of-bounds index errors.
- **Single row or single column (`1 x n` or `m x 1`):** The 1D index mapping formulas handle asymmetric shapes seamlessly without needing special branch checks.
