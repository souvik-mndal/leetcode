![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2040.95%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-21.8%20MB%20(beats%2078.75%25)-green?style=for-the-badge)

---

## Problem Explained

The goal is to check whether a given 9 x 9 Sudoku board follows basic Sudoku placement rules.

A valid Sudoku board must meet three simple conditions:
1. Each **row** must contain the digits `1` through `9` without repeats.
2. Each **column** must contain the digits `1` through `9` without repeats.
3. Each of the nine 3 x 3 **sub-boxes** (also called 3x3 grids) must contain the digits `1` through `9` without repeats.

**Important Note:** 
- The board does **not** need to be completely filled. Empty spaces are represented by the character `'.'`.
- You only check the numbers that are already filled in.
- The board does not need to be solvable; it just cannot break any of the three rules in its current state.

### Example
If row 0 already has a `'5'`, placing another `'5'` anywhere in row 0 makes the entire board **invalid** (`false`).


## Intuition

As you walk through the board cell by cell, you need an instant way to ask three questions for every filled digit you see:
1. *Have I seen this digit in this row before?*
2. *Have I seen this digit in this column before?*
3. *Have I seen this digit in this 3 x 3 sub-box before?*

Instead of searching backwards through the row, column, or sub-box every time (which would be slow), you keep three boolean "checklists" (lookup grids). 

The trickiest part is grouping cells into their correct 3 x 3 sub-box. There are 9 sub-boxes on a board, numbered 0 to 8:
- Top row of boxes: index 0, 1, 2
- Middle row of boxes: index 3, 4, 5
- Bottom row of boxes: index 6, 7, 8

You can map any cell at row `i` and column `j` to its 3 x 3 box index using this formula:
`boxIndex = (i / 3) * 3 + (j / 3)`

Integer division truncates the remainder (for example, row 5 divided by 3 gives 1), neatly grouping rows 0-2, 3-5, and 6-8 together.


## Approach

Here is the step-by-step logic of the solution:

* `bool rows[9][9] = {false}; bool cols[9][9] = {false}; bool boxes[9][9] = {false};` — Creates three 2D tracking grids set to `false`. For example, `rows[i][num]` being `true` means row `i` has already seen number `num`.
* `for (int i = 0; i < 9; i++)` — Loops through each row index `i` from 0 to 8.
* `for (int j = 0; j < 9; j++)` — Loops through each column index `j` from 0 to 8.
* `if (board[i][j] != '.')` — Ignores empty cells (`'.'`) and only validates filled digits.
* `int num = board[i][j] - '1';` — Converts character digits `'1'` through `'9'` into zero-based array indices `0` through `8`. Subtracting character `'1'` turns `'1'` into 0 and `'9'` into 8.
* `int boxIndex = (i / 3) * 3 + (j / 3);` — Calculates which 3 x 3 sub-box (from 0 to 8) the current cell belongs to.
* `if (rows[i][num] || cols[j][num] || boxes[boxIndex][num]) return false;` — Checks if `num` has already been seen in the current row, column, or box. If it has, the board is invalid, so return `false` immediately.
* `rows[i][num] = cols[j][num] = boxes[boxIndex][num] = true;` — Marks `num` as seen across all three tracking structures for future checks.
* `return true;` — If every cell is inspected without finding any rule violations, return `true`.


## Dry Run

### Case 1: Valid Partially-Filled Board (Example 1)

Here is a trace of processing the first few filled cells of a valid board:

| `i` | `j` | `board[i][j]` | `num` | `boxIndex` | `rows[i][num]` | `cols[j][num]` | `boxes[boxIndex][num]` | Action |
|---|---|---|---|---|---|---|---|---|
| 0 | 0 | `'5'` | 4 | 0 | false | false | false | Mark all three as `true`. Valid. |
| 0 | 1 | `'3'` | 2 | 0 | false | false | false | Mark all three as `true`. Valid. |
| 0 | 4 | `'7'` | 6 | 1 | false | false | false | Mark all three as `true`. Valid. |
| 1 | 0 | `'6'` | 5 | 0 | false | false | false | Mark all three as `true`. Valid. |
| 1 | 3 | `'1'` | 0 | 1 | false | false | false | Mark all three as `true`. Valid. |
| ... | ... | ... | ... | ... | ... | ... | ... | All remaining cells pass. |
| - | - | - | - | - | - | - | - | **Return `true`** |

---

### Case 2: Invalid Board with Duplicate (Example 2)

This case has `'8'` placed at cell `(0, 0)` and another `'8'` at cell `(3, 0)` in the same column.

| `i` | `j` | `board[i][j]` | `num` | `boxIndex` | `rows[i][num]` | `cols[j][num]` | `boxes[boxIndex][num]` | Action |
|---|---|---|---|---|---|---|---|---|
| 0 | 0 | `'8'` | 7 | 0 | false | false | false | Mark `rows[0][7]`, `cols[0][7]`, `boxes[0][7]` as `true`. |
| 0 | 1 | `'3'` | 2 | 0 | false | false | false | Mark all three as `true`. Valid. |
| ... | ... | ... | ... | ... | ... | ... | ... | Processing intermediate valid cells... |
| 3 | 0 | `'8'` | 7 | 3 | false | **true** | false | **Conflict found!** `cols[0][7]` is already `true`. |
| - | - | - | - | - | - | - | - | **Return `false` immediately** |


## Time & Space Complexity

- **Time Complexity:** **O(1)** — The board size is fixed at 9 x 9 = 81 cells. The loops run exactly 81 times, doing constant-time O(1) checks and array updates at each step. (If extended to an N x N board, time complexity would be O(N^2)).
- **Space Complexity:** **O(1)** — The three boolean arrays `rows`, `cols`, and `boxes` each take 9 x 9 = 81 booleans, taking a tiny, constant amount of memory.

### Optimization Analysis

This algorithm is already optimal in terms of Big-O complexity. However, we can optimize space and memory efficiency further using **bit manipulation** instead of 2D boolean arrays.

#### How Bit Manipulation Works
An integer has 32 bits, which is more than enough to store 9 flags (bits 0 through 8). 
- We can replace `bool rows[9][9]` with a simple 1D array of integers: `int rows[9] = {0}`.
- To check if number `num` has been seen in row `i`, we check if the `num`-th bit of `rows[i]` is set to `1`: `(rows[i] & (1 << num)) != 0`.
- To mark number `num` as seen, we turn on the `num`-th bit using bitwise OR: `rows[i] |= (1 << num)`.

#### Bitmask Implementation Snippet
```cpp
int rows[9] = {0};
int cols[9] = {0};
int boxes[9] = {0};

for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
        if (board[i][j] != '.') {
            int num = board[i][j] - '1';
            int boxIndex = (i / 3) * 3 + (j / 3);
            int mask = 1 << num; // Create a bit flag for digit 'num'

            // If bit is already 1 in any mask, we found a duplicate
            if ((rows[i] & mask) || (cols[j] & mask) || (boxes[boxIndex] & mask)) {
                return false;
            }

            // Set the bit to 1 in all three bitmasks
            rows[i] |= mask;
            cols[j] |= mask;
            boxes[boxIndex] |= mask;
        }
    }
}
```

- **Improved Complexity:** Still **O(1)** time and space, but reduces memory usage from 243 boolean bytes down to just 27 integers (108 bytes total) with faster bitwise register operations.
- **Theoretical Best:** O(1) time and space for a fixed 9x9 grid. The bitmask solution reaches this theoretical limit.


## Edge Cases Handled

- **Empty Board:** A board filled entirely with `'.'` correctly returns `true` because empty cells are ignored.
- **Sparse Board:** Boards with only one or two numbers return `true` without issues.
- **Same Number in Different Regions:** Placing `'5'` at `(0, 0)` and another `'5'` at `(1, 4)` is valid as long as they do not share a row, column, or 3 x 3 sub-box.
- **Duplicate within 3 x 3 Box:** Detects duplicates in the same sub-box even if they appear in different rows and columns.
- **Unsolvable but Valid Board:** A board that cannot be solved to completion, but currently has no duplicate collisions, correctly returns `true` according to the problem rules.
