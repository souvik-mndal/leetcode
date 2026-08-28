![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-53.03%20MB%20(beats%2087.78%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a turn-based game using a row of stone piles. The game has strict setup rules:
* There is an **even number** of piles in total.
* Every pile contains a positive number of stones.
* The total count of all stones across all piles is **odd**, which means a draw or tie is impossible.

Alice always goes first. On any turn, a player can pick up and keep all stones from either the **first pile** (far left) or the **last pile** (far right) of the remaining row. 

When all piles are taken, the player with more stones wins. Assuming both Alice and Bob play with perfect strategy, we need to return `true` if Alice wins, and `false` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`, Alice can take the left `5`, leaving `[3, 4, 5]`. Bob must choose between `3` and `5`. No matter what Bob chooses, Alice can make choices that lead to her getting more total stones. So the answer is `true`.

## Intuition

The standard way to solve this game is with Dynamic Programming or Minimax. However, a clever math shortcut lets us bypass all calculations. Alice can actually force a win on turn one without playing out the game!

Here is why:
1. **Even number of piles:** The piles can be split into two color groups based on their index positions: **even-indexed piles** (0, 2, 4...) and **odd-indexed piles** (1, 3, 5...).
2. **Odd total sum:** Because the total number of stones is odd, the sum of all even-indexed piles and the sum of all odd-indexed piles **cannot be equal**. One of these two sums must be strictly greater than the other.
3. **Alice controls the game:** On her very first turn, Alice can choose whether she wants *all* even piles or *all* odd piles.
   * If she wants all even piles, she takes index `0`. This leaves indices `1` and `N-1` (both odd) at the ends. Bob is forced to take an odd index.
   * Whichever odd index Bob takes, it exposes a new even index for Alice to take on her next turn.
   * Alice repeats this to get every single even-indexed pile.
   * If Alice wants all odd piles instead, she starts by taking index `N-1` (which is odd, because total size `N` is even).

Since Alice goes first, she can calculate which set has more stones before making her first move, choose that strategy, and guarantee a win every time. Thus, Alice always wins, so we can directly return `true`.

## Approach

* `return true;`: Immediately return `true` without processing the array, because Alice is mathematically guaranteed to win every game under the given rules.

## Dry Run

### Typical case: `piles = [5, 3, 4, 5]`

* Even-indexed sum (indices 0, 2): 5 + 4 = 9
* Odd-indexed sum (indices 1, 3): 3 + 5 = 8
* Alice targets even piles (9 > 8).

| Step | Action | `piles` | Result |
| :--- | :--- | :--- | :--- |
| 1 | Evaluate game conditions | `[5, 3, 4, 5]` | Alice chooses even-indexed piles (sum 9 vs sum 8) |
| 2 | Execute return statement | `[5, 3, 4, 5]` | Return `true` |

### Case with larger middle values: `piles = [3, 7, 2, 3]`

* Even-indexed sum (indices 0, 2): 3 + 2 = 5
* Odd-indexed sum (indices 1, 3): 7 + 3 = 10
* Alice targets odd piles (10 > 5).

| Step | Action | `piles` | Result |
| :--- | :--- | :--- | :--- |
| 1 | Evaluate game conditions | `[3, 7, 2, 3]` | Alice chooses odd-indexed piles (sum 10 vs sum 5) |
| 2 | Execute return statement | `[3, 7, 2, 3]` | Return `true` |

## Time & Space Complexity

**Time:** O(1) — returning a constant boolean value takes instant, single-step time regardless of how many piles are given.

**Space:** O(1) — no extra variables, arrays, or memory structures are created.

### Is this optimal?

Yes, this code is already fully optimal. The runtime is O(1) time and O(1) space, which is the theoretical lower limit for any program. No further optimization is possible.

## Edge Cases Handled

* **Smallest input size (`piles.length = 2`):** Works correctly. Alice simply picks whichever of the two piles is larger and wins immediately.
* **Duplicate pile values:** Works correctly. Even if multiple piles share the exact same stone count, the total sum is odd, so the sum of odd positions and even positions will never be equal.
* **Large arrays (`piles.length = 500`):** Works instantly in O(1) time without running loops or running out of call stack memory.
