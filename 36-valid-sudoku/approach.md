![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-21.87%20MB%20(beats%2073.16%25)-green?style=for-the-badge)

---

## Problem Explained

The goal is to determine whether a given 9x9 Sudoku board is valid based on its current filled cells. 

A board is valid if:
1. Each row contains the digits `1` to `9` without duplicates.
2. Each column contains the digits `1` to `9` without duplicates.
3. Each of the nine 3x3 sub-boxes contains the digits `1` to `9` without duplicates.

**Important Note:** You do **not** need to solve the board or check if it can be completed. You only need to check if the numbers already placed on the board break any of these three rules. Empty cells are represented by `'.'`.

---

## Intuition

The standard way to check a Sudoku board would be to loop through each row, then loop through each column, and finally loop through each 3x3 sub-box. That requires three separate passes.

Instead, we can do everything in **a single pass** over the 9x9 grid. 

As we look at each cell, we keep track of numbers we have already seen using three collections of boolean flags:
* One for each row.
* One for each column.
* One for each 3x3 sub-box.

The main challenge is mapping a cell at position `(i, j)` to its corresponding 3x3 sub-box index (from 0 to 8). 

Because each sub-box is 3 cells wide and 3 cells tall:
* `i / 3` gives the box's row coordinate (0, 1, or 2).
* `j / 3` gives the box's column coordinate (0, 1, or 2).
* Combining them using `(i / 3) * 3 + (j / 3)` gives a unique box number from 0 to 8.

If we ever encounter a digit that has already been marked as `true` in its row, column, or sub-box, the board is invalid.

---

## Approach

Here is the breakdown of the algorithm:

* `bool rows[9][9] = {false}; bool cols[9][9] = {false}; bool boxes[9][9] = {false};`: Creates three 2D boolean arrays initialized to `false`. For example, `rows[i][num]` will be `true` if digit index `num` has been seen in row `i`.
* `for (int i = 0; i < 9; i++)`: Outer loop to iterate over each row index `i` from 0 to 8.
* `for (int j = 0; j < 9; j++)`: Inner loop to iterate over each column index `j` from 0 to 8.
* `if (board[i][j] != '.')`: Skips empty cells so we only process actual digits.
* `int num = board[i][j] - '1';`: Converts the character digit (e.g., `'1'` through `'9'`) into a 0-based integer index (`0` through `8`) by subtracting the character `'1'`.
* `int boxIndex = (i / 3) * 3 + (j / 3);`: Calculates which of the nine 3x3 sub-boxes the cell `(i, j)` belongs to.
* `if (rows[i][num] || cols[j][num] || boxes[boxIndex][num]) return false;`: Checks if digit `num` was already seen in row `i`, column `j`, or box `boxIndex`. If so, returns `false` immediately.
* `rows[i][num] = cols[j][num] = boxes[boxIndex][num] = true;`: Marks digit `num` as seen in row `i`, column `j`, and box `boxIndex`.
* `return true;`: Returns `true` after checking all cells if no rule violations were found.

---

## Dry Run

### Case 1: Valid Sudoku Board (Example 1)

`board` is partially filled with valid numbers.

| `i` | `j` | `board[i][j]` | `num` | `boxIndex` | Action |
|---|---|---|---|---|---|
| 0 | 0 | `'5'` | 4 | 0 | Not seen yet. Set `rows[0][4]`, `cols[0][4]`, `boxes[0][4]` = `true`. |
| 0 | 1 | `'3'` | 2 | 0 | Not seen yet. Set `rows[0][2]`, `cols[1][2]`, `boxes[0][2]` = `true`. |
| 0 | 4 | `'7'` | 6 | 1 | Not seen yet. Set `rows[0][6]`, `cols[4][6]`, `boxes[1][6]` = `true`. |
| 1 | 0 | `'6'` | 5 | 0 | Not seen yet. Set `rows[1][5]`, `cols[0][5]`, `boxes[0][5]` = `true`. |
| 2 | 2 | `'8'` | 7 | 0 | Not seen yet. Set `rows[2][7]`, `cols[2][7]`, `boxes[0][7]` = `true`. |
| ... | ... | ... | ... | ... | All remaining non-dot cells checked without duplicate conflicts. |
| End | - | - | - | - | Loop finishes. Return `true`. |

---

### Case 2: Invalid Board with Duplicate in 3x3 Box (Example 2)

Same board, but `board[0][0]` is `'8'` and `board[2][2]` is also `'8'`.

| `i` | `j` | `board[i][j]` | `num` | `boxIndex` | Action |
|---|---|---|---|---|---|
| 0 | 0 | `'8'` | 7 | 0 | Not seen yet. Set `rows[0][7]`, `cols[0][7]`, `boxes[0][7]` = `true`. |
| 0 | 1 | `'3'` | 2 | 0 | Not seen yet. Set `rows[0][2]`, `cols[1][2]`, `boxes[0][2]` = `true`. |
| ... | ... | ... | ... | ... | Continues scanning filled cells without issue... |
| 2 | 2 | `'8'` | 7 | 0 | Check: `boxes[0][7]` is already `true`! Box duplicate detected. |
| Return | - | - | - | - | Early return `false`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** — The board is fixed at 9x9 cells (81 total iterations). Processing each cell takes O(1) constant time operations. If generalized to an N x N board, the time complexity would be **O(N^2)**.
* **Space Complexity:** **O(1)** — Fixed memory used by three 9x9 boolean arrays (243 booleans total). If generalized to an N x N board, the space complexity would be **O(N^2)**.

### Optimizing Memory with Bitmasks

While the current time complexity is optimal, we can lower memory usage using **bitmasks** (using integers as bit arrays).

An integer has 32 bits. We only need 9 bits to keep track of digits 1 through 9. Instead of using a 2D boolean array of size 9x9, we can use a 1D array of 9 integers, where each integer's binary bits represent seen numbers.

To check if digit `num` is seen in row `i`:
* Shift `1` to the left by `num` bits: `(1 << num)`.
* Perform bitwise AND with `rows[i]`: `(rows[i] & (1 << num))`. If non-zero, it was seen before.
* To mark it seen, perform bitwise OR: `rows[i] |= (1 << num)`.

```cpp
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int rows[9] = {0};
        int cols[9] = {0};
        int boxes[9] = {0};

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '1';
                    int mask = 1 << num;
                    int boxIndex = (i / 3) * 3 + (j / 3);

                    if ((rows[i] & mask) || (cols[j] & mask) || (boxes[boxIndex] & mask)) {
                        return false;
                    }

                    rows[i] |= mask;
                    cols[j] |= mask;
                    boxes[boxIndex] |= mask;
                }
            }
        }
        return true;
    }
};
```

* **Resulting Complexity:** Still **O(1)** time and **O(1)** space, but reduces memory allocations from 243 bytes down to 27 integers (108 bytes on standard systems) and eliminates array indexing overhead. This achieves the theoretical best possible complexity for this problem.

---

## Edge Cases Handled

* **Empty Board:** Boards filled completely with `'.'` pass successfully and return `true`.
* **Row Duplicates:** Duplicate digits in the same row are detected immediately when checking `rows[i][num]`.
* **Column Duplicates:** Duplicate digits in the same column are detected immediately when checking `cols[j][num]`.
* **Box Duplicates:** Duplicate digits within the same 3x3 sub-box (even if in different rows and columns) are detected when checking `boxes[boxIndex][num]`.
* **Unsolvable Board:** A board that is currently valid under the three basic rules, but impossible to solve fully, correctly returns `true` (as required by the rules).
