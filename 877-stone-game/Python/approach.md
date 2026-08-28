![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-12.42%20MB%20(beats%2038.53%25)-orange?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a turn-based game using a row of stone piles. Each pile contains a positive number of stones. 

Here are the rules:
* There is always an **even number** of piles in the row.
* The **total sum** of all stones across all piles is an **odd number**. This means there can never be a tie.
* Players take turns, and **Alice goes first**.
* On a player's turn, they can take the entire pile of stones from either the **very left end** or the **very right end** of the current row.
* The game ends when no piles are left. The player with the highest total number of stones wins.

The problem asks us to determine if Alice can win, assuming both Alice and Bob play with perfect strategy. We need to return `True` if Alice wins, or `False` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
Alice starts and can pick either `5` (left) or `5` (right). If she plays smartly, she will always end up with more stones than Bob, so the answer is `True`.

---

## Intuition

The "aha" moment comes from looking at the rules of the game, specifically the constraints on the input:
1. The number of piles is **even**.
2. The total number of stones is **odd**.

Because the total number of piles is even, we can divide the piles into two groups based on their positions (indexes):
* **Group 1 (Even indexes):** index 0, index 2, index 4, ...
* **Group 2 (Odd indexes):** index 1, index 3, index 5, ...

Since the total sum of all stones is odd, the sum of Group 1 and the sum of Group 2 **cannot be equal**. One group must have a larger total sum of stones than the other.

Because Alice goes first, she can **force** the game so that she gets **all** the piles from Group 1, or **all** the piles from Group 2!

How?
* If Group 1 (even indexes) has more stones, Alice picks index 0 (an even index) on her first turn.
* This leaves Bob with choices at index 1 and index `N - 1` (both are odd indexes!).
* Whichever pile Bob takes, he exposes an even index for Alice on her next turn.
* Alice can repeat this to collect every single pile in Group 1.

Since Alice can calculate which group has more stones before making her first move, she can always choose the winning group. Therefore, Alice **always wins**. We can simply return `True`.

---

## Approach

Here is how the simple one-line code operates:

* `return True`: Returns the boolean value `True` immediately. This is correct because Alice is mathematically guaranteed to win every game that meets the problem constraints (even length, odd total sum, optimal play).

---

## Dry Run

Even though the code is a single line, let us trace two examples to see why returning `True` is always correct.

### Case 1: Typical case with small values

Input: `piles = [5, 3, 4, 5]`
* Even-indexed piles (indexes 0, 2): values `5` and `4` (sum = 9)
* Odd-indexed piles (indexes 1, 3): values `3` and `5` (sum = 8)
* Alice sees that Even-indexed sum (9) > Odd-indexed sum (8). Alice will target Even piles.

| Step | Current `piles` | Action | Alice Points | Bob Points |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | Alice picks index 0 (`5`) | 5 | 0 |
| 2 | `[3, 4, 5]` | Bob picks index 3 (`5`) | 5 | 5 |
| 3 | `[3, 4]` | Alice picks index 2 (`4`) | 9 | 5 |
| 4 | `[3]` | Bob picks index 1 (`3`) | 9 | 8 |
| **End** | `[]` | Code executes `return True` | **9 (Wins)** | **8** |

---

### Case 2: Case where odd positions win

Input: `piles = [3, 7, 2, 3]`
* Even-indexed piles (indexes 0, 2): values `3` and `2` (sum = 5)
* Odd-indexed piles (indexes 1, 3): values `7` and `3` (sum = 10)
* Alice sees that Odd-indexed sum (10) > Even-indexed sum (5). Alice will target Odd piles.

| Step | Current `piles` | Action | Alice Points | Bob Points |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | Alice picks index 3 (`3`) | 3 | 0 |
| 2 | `[3, 7, 2]` | Bob picks index 0 (`3`) | 3 | 3 |
| 3 | `[7, 2]` | Alice picks index 1 (`7`) | 10 | 3 |
| 4 | `[2]` | Bob picks index 2 (`2`) | 10 | 5 |
| **End** | `[]` | Code executes `return True` | **10 (Wins)** | **5** |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** — The solution runs in constant time because it instantly returns `True` without looking at or iterating through the `piles` array.
* **Space Complexity:** **O(1)** — The solution uses no extra memory or data structures.

### Is this optimal?
**Yes, this is already the most optimal possible solution.** 
The theoretical best time complexity to answer a decision problem is O(1), and this code reaches it. No further speed or memory improvements are possible.

*(Note: While this problem can also be solved using Dynamic Programming or Minimax in O(N^2) time to calculate score differences, the mathematical shortcut allows us to answer in O(1) time).*

---

## Edge Cases Handled

* **Smallest array size (2 piles):** With `piles = [A, B]`, Alice simply picks whichever number is larger on turn 1 and wins. The code correctly returns `True`.
* **Large arrays (up to 500 piles):** The logic holds regardless of array length as long as the length is even.
* **Duplicate values in piles:** Even if multiple piles have the same number of stones, the total sum across all piles is odd, guaranteeing that one index group strictly beats the other. Ties are impossible.
