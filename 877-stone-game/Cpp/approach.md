![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-10.47%20MB%20(beats%2066.15%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with a row of stone piles. The game has three basic rules:
* There is an **even number** of piles.
* The **total number of stones** across all piles is **odd**, which means there can never be a tie.
* Players take turns, starting with Alice. On each turn, a player can take all stones from **either the far left pile or the far right pile**.

The player with the most stones at the end wins. Assuming both Alice and Bob play perfectly to maximize their score, we need to return `true` if Alice wins, and `false` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
* Alice can take the first pile (`5`) or the last pile (`5`).
* No matter what Bob picks next, Alice can make choices that ensure she gets more total stones than Bob.
* Alice wins, so the answer is `true`.


## Intuition

The key realization is that **Alice can always win, no matter what the input is**.

Why is Alice guaranteed to win?
1. The total number of piles is **even**.
2. Think of the piles by their index positions: `0, 1, 2, 3, ..., N-1`.
3. Separate the piles into two groups based on index positions:
   * **Even-indexed piles:** positions `0, 2, 4, ...`
   * **Odd-indexed piles:** positions `1, 3, 5, ...`
4. Since the total sum of all stones is **odd**, the sum of all even-indexed piles and the sum of all odd-indexed piles **cannot be equal**. One group must have more stones than the other.
5. On the very first turn, Alice can choose whether she wants to collect **all even-indexed piles** or **all odd-indexed piles**:
   * If she wants all even-indexed piles, she picks index `0` (even). This forces Bob to choose between index `1` (odd) and index `N-1` (odd).
   * Whichever odd pile Bob takes, he exposes another even-indexed pile for Alice to pick on her next turn!
6. Alice simply calculates which group (even or odd) has a larger total before making her first move, and forces that set for the rest of the game.

Because Alice plays first and can force a strategy that guarantees her the larger sum, she **always wins**. Therefore, we can simply return `true`.


## Approach

Here is the step-by-step breakdown of the solution code:

* `return true;`: Because the constraints guarantee an even number of piles and an odd total sum of stones, Alice can always choose a winning strategy on move one. Thus, the function immediately returns `true` without needing any loops or extra calculations.


## Dry Run

### Case 1: Typical case with four piles

Input: `piles = [5, 3, 4, 5]`

| Step | `piles` | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | Calculate even positions sum: `piles[0] + piles[2] = 5 + 4 = 9`. Calculate odd positions sum: `piles[1] + piles[3] = 3 + 5 = 8`. | Even group sum (`9`) > Odd group sum (`8`). |
| 2 | `[5, 3, 4, 5]` | Alice can force taking the even group (`9` stones) to beat Bob (`8` stones). | Alice is guaranteed to win. |
| 3 | `[5, 3, 4, 5]` | Execute `return true;`. | Return `true`. |

### Case 2: Case with larger odd-indexed sum

Input: `piles = [3, 7, 2, 3]`

| Step | `piles` | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | Calculate even positions sum: `piles[0] + piles[2] = 3 + 2 = 5`. Calculate odd positions sum: `piles[1] + piles[3] = 7 + 3 = 10`. | Odd group sum (`10`) > Even group sum (`5`). |
| 2 | `[3, 7, 2, 3]` | Alice can force taking the odd group (`10` stones) to beat Bob (`5` stones). | Alice is guaranteed to win. |
| 3 | `[3, 7, 2, 3]` | Execute `return true;`. | Return `true`. |


## Time & Space Complexity

**Time:** O(1) — executing a single `return` statement takes constant time.
**Space:** O(1) — no extra memory or data structures are allocated.

### Is this optimal?
Yes, this is already optimal. 

* The time complexity is **O(1)** (constant time).
* The space complexity is **O(1)** (constant space).

You cannot beat O(1) time and space complexity. While this problem can also be solved using Dynamic Programming in O(N^2) time and O(N^2) space to simulate the minimax decision tree, the mathematical trick reduces the entire problem to a direct O(1) return statement.


## Edge Cases Handled

* **Minimum size array (`piles.length == 2`):** Alice simply picks the larger of the two piles and wins immediately.
* **Large arrays (`piles.length == 500`):** Works instantly in O(1) time without any danger of timing out or overflowing memory.
* **Equal pile values at ends (e.g., `piles = [5, 3, 4, 5]`):** The logic holds regardless of individual pile values because the sum of odd positions and even positions can never be equal when the total sum is odd.
* **No tie possible:** Guaranteed by the constraint that the total sum of stones across all piles is always an odd number.
