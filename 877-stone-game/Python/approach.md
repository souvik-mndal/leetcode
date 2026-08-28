![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-19.38%20MB%20(beats%2043.85%25)-yellow?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a turn-based strategy game with a row of stone piles.
- The game starts with an **even** number of piles.
- Each pile has a certain number of stones.
- The total sum of all stones across all piles is **odd**, meaning there are no ties.
- Alice always goes first.
- On each turn, a player can only take the pile at the very beginning or at the very end of the remaining row.
- The player who collects the most total stones by the end wins.

Your goal is to return `True` if Alice can guarantee a win assuming both players play perfectly, or `False` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
- Alice takes the first `5`. Piles left: `[3, 4, 5]`.
- Bob takes `3`. Piles left: `[4, 5]`.
- Alice takes `5`. Piles left: `[4]`.
- Bob takes `4`.
Alice ends up with `5 + 5 = 10` stones, and Bob gets `3 + 4 = 7` stones. Alice wins, so the result is `True`.

---

## Intuition

The major key to this problem is realizing that **Alice can always win, no matter what the input is.**

Why? Look at the rules carefully:
1. There are an **even** number of piles.
2. The total number of stones is **odd**.

Because there are an even number of piles, we can split the pile positions into two sets:
- **Even-indexed piles:** position 0, position 2, position 4, etc.
- **Odd-indexed piles:** position 1, position 3, position 5, etc.

Since the total sum of all stones is odd, the sum of even-indexed piles and the sum of odd-indexed piles **cannot be equal**. One set MUST have more total stones than the other.

Before the game starts, Alice can sum up all even-indexed piles and all odd-indexed piles. 
- If even-indexed piles have more stones, Alice takes the first pile (index 0). Bob is now forced to pick between index 1 and the last index (both are odd!). No matter what Bob picks, he exposes an even index for Alice to take next.
- If odd-indexed piles have more stones, Alice takes the last pile (an odd index). Again, Bob is forced to expose odd indices for Alice.

Alice can control the game to take **all** even-indexed piles or **all** odd-indexed piles. Since she goes first, she picks whichever group has more stones and is guaranteed to win every single time!

---

## Approach

Here is the step-by-step logic of the code:

* `return True`: Because Alice always has a strategy to win (by choosing all even-indexed piles or all odd-indexed piles, whichever group has a larger sum), she will never lose. Thus, the program simply returns `True` immediately without needing to simulate the game or calculate dynamic programming tables.

---

## Dry Run

### Case 1: Typical case (`piles = [5, 3, 4, 5]`)

| Step | piles | Action | Return Value |
| :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | Code evaluates `return True` | `True` |

---

### Case 2: Alternate case (`piles = [3, 7, 2, 3]`)

| Step | piles | Action | Return Value |
| :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | Code evaluates `return True` | `True` |

---

## Time & Space Complexity

- **Time:** O(1) — The code executes a single return statement in constant time regardless of the size of the input array.
- **Space:** O(1) — No extra memory or data structures are created.

### Is this optimal?
Yes, this is the theoretical best possible complexity.
- **Time Complexity:** O(1) is the absolute limit because we do not even need to read through the array to know the answer.
- **Space Complexity:** O(1) uses no extra memory.

No further improvements can be made because constant time and space are already optimal.

---

## Edge Cases Handled

- **Smallest valid input size (2 piles):** E.g., `piles = [2, 10]`. Alice picks 10 and wins. The code returns `True`.
- **Large inputs (500 piles):** The array length has no impact on execution speed because the answer is calculated in O(1) time.
- **Piles with identical edge values:** E.g., `piles = [5, 1, 2, 5]`. Alice can still choose the overall winning parity set (odd vs even positions) and win.
- **No ties:** The problem guarantees an odd total sum of stones, ensuring Alice's chosen parity set is strictly greater than Bob's set.
