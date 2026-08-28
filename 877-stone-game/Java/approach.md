![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-43.08%20MB%20(beats%2066.72%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a turn-based game with an even number of stone piles lined up in a row. Each pile contains a positive integer number of stones.

On every turn, a player must take all the stones from either the far-left pile or the far-right pile. This continues until no piles remain. The player with the most total stones at the end wins.

Alice always takes the first turn. The game has two key rules designed into its setup:
1. The total number of piles is **even**.
2. The total sum of all stones across all piles is **odd**.

Because the total sum is odd, a tie is impossible. Assuming both players make the smartest possible moves to maximize their own scores, return `true` if Alice wins, or `false` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
- Alice takes `5` from the front. Piles left: `[3, 4, 5]`.
- Bob takes `3` from the front. Piles left: `[4, 5]`.
- Alice takes `5` from the end. Piles left: `[4]`.
- Bob takes `4`.
- Alice ends up with `5 + 5 = 10` stones, and Bob gets `3 + 4 = 7` stones. Alice wins, so the result is `true`.

---

## Intuition

The key realization is that **Alice can always win, no matter what piles are given**.

Here is why:
Imagine labelling the pile indices from `0` to `N - 1`. Since `N` is an even number, there are an equal number of even-indexed piles (`0, 2, 4, ...`) and odd-indexed piles (`1, 3, 5, ...`).

Before making her first move, Alice can sum up all the stones in even positions, and sum up all the stones in odd positions. Because the grand total of all stones is odd, one of these two sums **must** be strictly larger than the other.

Alice can force the game to give her whichever set of piles she wants:
- If she wants all **even** piles, she takes pile `0` first. This leaves piles `1` through `N - 1`. Notice that both available ends for Bob (pile `1` and pile `N - 1`) are **odd** indices! Bob is forced to take an odd pile, which immediately frees up an even pile for Alice on her next turn.
- If she wants all **odd** piles, she takes pile `N - 1` first. This forces Bob to face even piles on both ends, which frees up an odd pile for Alice on her next turn.

By following this strategy, Alice can guarantee she gets 100% of the piles from whichever index group has the larger sum. Since she picks the larger group, she is guaranteed to score more points than Bob.

Because Alice can always force a win, returning `true` is always correct!

---

## Approach

Here is step-by-step how the code operates:

*   `return true;` - Directly returns `true` without needing any loops, arrays, or mathematical calculations. Because Alice goes first and the game has an even number of piles with an odd total sum of stones, Alice can always choose a strategy that guarantees her total score is strictly higher than Bob's score.

---

## Dry Run

### Typical case: `piles = [5, 3, 4, 5]`

| Step | Expression / Value | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | Input `piles` | Function receives `piles = [5, 3, 4, 5]` | Execution starts |
| 2 | `return true;` | Returns `true` instantly based on guaranteed win strategy | Returns `true` |

### Case with equal ends: `piles = [3, 7, 2, 3]`

| Step | Expression / Value | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | Input `piles` | Function receives `piles = [3, 7, 2, 3]` | Execution starts |
| 2 | `return true;` | Returns `true` instantly based on guaranteed win strategy | Returns `true` |

---

## Time & Space Complexity

**Time Complexity:** O(1) — Constant time. The code executes a single return statement without inspecting the elements of `piles`.

**Space Complexity:** O(1) — Constant space. No extra memory, variables, or data structures are created.

### Is this already optimal?
Yes, this solution is completely optimal. You cannot improve upon O(1) time and O(1) space complexity.

#### Note on alternative Dynamic Programming approach:
If the problem constraints were slightly different (for instance, if `piles.length` could be odd, or if ties were allowed), you would need to use Dynamic Programming (DP) or Minimax to calculate the maximum score difference Alice can achieve.

In a standard DP approach:
- You define `dp[i][j]` as the maximum relative score difference a player can get from subarray `piles[i]` to `piles[j]`.
- The transitions look at taking either `piles[i]` or `piles[j]` and subtracting the optimal response of the opponent on the remaining subarray: `dp[i][j] = max(piles[i] - dp[i + 1][j], piles[j] - dp[i][j - 1])`.
- That DP solution takes **O(N^2) time** and **O(N^2) or O(N) space**.

However, under the exact rules given in this problem (even length array, odd total sum, Alice goes first), the mathematical shortcut makes the DP calculation unnecessary. `return true;` achieves the theoretical absolute best complexity of O(1) time and O(1) space.

---

## Edge Cases Handled

* **Minimum array size (`piles.length = 2`):** With only 2 piles (e.g., `[3, 7]`), Alice simply picks the larger pile on turn 1 and wins instantly. The code correctly returns `true`.
* **Equal values at both ends (e.g., `[3, 7, 2, 3]`):** Alice is not forced into a tie because the total sum of all piles is odd. One set of alternating indices will always hold more stones than the other.
* **Large input sizes (`piles.length = 500`):** Because the time complexity is O(1), the code runs instantaneously even for the maximum allowed constraint size.
