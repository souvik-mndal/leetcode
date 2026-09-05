![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-50.3%20MB%20(beats%2073.30%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine an 8 x 8 chessboard. You have a bishop sitting at a starting square called `source` and you want to move it to a destination square called `target`. 

A bishop can move as many squares as it wants in a single turn, but only along straight diagonal lines. The board is completely empty, so there are no other pieces blocking the way.

Your job is to find the minimum number of moves needed to get the bishop from `source` to `target`. If it is impossible to reach the target, return `-1`.

For example:
* If the bishop starts at `[8, 1]` and wants to get to `[1, 8]`, it can reach it in **1 move** because both squares sit on the exact same diagonal.
* If it starts at `[1, 1]` and wants to reach `[3, 4]`, it is **impossible** (return `-1`) because a bishop can never leave its starting square color.

---

## Intuition

The key to this problem is understanding three basic math rules about how a bishop moves on a chessboard:

1. **Square Colors (0 or -1 moves):** 
   A chessboard alternates colors like a grid. If you add the row number and column number of a square (`row + col`), an even total means one color and an odd total means the other. Because a bishop moves diagonally, it can *never* change the color of the square it lands on. If `source` and `target` have different color parities, it is impossible to reach (return `-1`). If `source` and `target` are the same square, 0 moves are needed.

2. **Same Diagonal (1 move):**
   * Two squares are on the same diagonal sloping up-right to down-left (anti-diagonal) if `row + col` is equal for both.
   * Two squares are on the same diagonal sloping up-left to down-right (main diagonal) if `row - col` is equal for both.
   If either condition is true, the bishop reaches the target in **1 move**.

3. **Different Diagonal, Same Color (2 moves):**
   If two squares are the same color, their diagonal lines will always cross each other at some intersection square on the board. The bishop can move to that intersection point in 1 move, then turn and move to the target in 1 move. So, any reachable square that is not on the same diagonal takes exactly **2 moves**.

---

## Approach

Here is how the code evaluates the answer step-by-step:

* `if (source == target)`: Checks if the starting position and target position are identical. If they are, return `0` because no moves are needed.
* `if ((source[0] + source[1]) % 2 != (target[0] + target[1]) % 2)`: Checks whether the two squares have different colors by comparing the remainder of `(row + col)` divided by 2. If the remainders do not match, the bishop can never reach the target, so return `-1`.
* `if (source[0] + source[1] == target[0] + target[1] || source[0] - source[1] == target[0] - target[1])`: Checks if the two squares lie on the same diagonal line. `source[0] + source[1] == target[0] + target[1]` checks the anti-diagonal, while `source[0] - source[1] == target[0] - target[1]` checks the main diagonal. If either condition is true, return `1`.
* `return 2;`: If the target shares the same square color but is not on the same diagonal line, it will always take exactly `2` moves to reach it.

---

## Dry Run

### Case 1: Target is on the same diagonal (`source = [8, 1]`, `target = [1, 8]`)

| Step | Code Condition | Evaluated Calculation | Result | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `source == target` | `[8, 1] == [1, 8]` | False | Move to next check |
| 2 | `(source[0] + source[1]) % 2 != (target[0] + target[1]) % 2` | `(8 + 1) % 2 != (1 + 8) % 2` -> `1 != 1` | False | Same color; move to next check |
| 3 | `source[0] + source[1] == target[0] + target[1] \|\| ...` | `8 + 1 == 1 + 8` -> `9 == 9` | True | Return `1` move |

---

### Case 2: Target is same color, different diagonal (`source = [4, 2]`, `target = [1, 3]`)

| Step | Code Condition | Evaluated Calculation | Result | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `source == target` | `[4, 2] == [1, 3]` | False | Move to next check |
| 2 | `(source[0] + source[1]) % 2 != (target[0] + target[1]) % 2` | `(4 + 2) % 2 != (1 + 3) % 2` -> `0 != 0` | False | Same color; move to next check |
| 3 | `source[0] + source[1] == target[0] + target[1] \|\| source[0] - source[1] == target[0] - target[1]` | `6 == 4 \|\| 2 == -2` | False | Not on same diagonal; move to next check |
| 4 | `return 2` | None | True | Return `2` moves |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** (Constant Time). The code performs a fixed number of basic math operations (+, -, %, ==) regardless of where `source` and `target` are located.
* **Space Complexity:** **O(1)** (Constant Space). No extra memory, arrays, or data structures are allocated.

### Can this be improved?

**No, this solution is already fully optimal.** 

Determining the relationship between two fixed points in 2D space requires reading the input coordinates and running direct coordinate math. Constant time O(1) and constant space O(1) are the absolute best theoretical bounds possible for this problem.

---

## Edge Cases Handled

* **Same starting and ending square (`source == target`):** Handled by the first check, returning `0` immediately.
* **Unreachable target of a different color (e.g., `[1, 1]` to `[3, 4]`):** Handled by comparing coordinate sum parities (`(row + col) % 2`), correctly returning `-1`.
* **Negative differences in coordinate math (e.g., `source[0] - source[1]` is negative):** Standard integer subtraction handles negative values properly (e.g., `1 - 3 = -2`), so main-diagonal checks work accurately across all quadrants of the board.
