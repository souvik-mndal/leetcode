![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-19.27%20MB%20(beats%2068.16%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a game with an even number of stone piles arranged in a row. Each pile has a positive number of stones. 

The total number of stones across all piles is odd, which means the game can never end in a tie. 

Alice goes first. On any turn, a player can take all the stones from either the very first pile or the very last pile remaining in the row. The game ends when all piles are taken. The player with the most stones wins.

Your job is to determine if Alice can always win, assuming both Alice and Bob play perfectly to win. If Alice wins, return `True`. Otherwise, return `False`.

For example, if `piles = [5, 3, 4, 5]`:
* Alice takes `5` from the front or back. 
* Bob takes a pile from the ends left over.
* Alice can force a strategy where she ends up with 10 stones while Bob gets 7 stones.
* Since Alice wins, the answer is `True`.

---

## Intuition

At first glance, this looks like a complex Dynamic Programming or Minimax game theory problem. However, there is a simple mathematical mathematical rule that guarantees Alice **always** wins.

Here is the trick:
1. The total number of piles is **even**.
2. Think of the pile positions as alternating between **even positions** (index 0, 2, 4...) and **odd positions** (index 1, 3, 5...).
3. Alice can choose to take **all even-positioned piles** or **all odd-positioned piles**:
   * On her first move, Alice can pick pile `0` (an even index) or pile `N - 1` (an odd index because length `N` is even).
   * If Alice chooses pile `0` (even), the remaining ends are pile `1` (odd) and pile `N - 1` (odd). Bob is forced to pick an odd pile!
   * Once Bob takes an odd pile, one end becomes an even pile again. Alice can pick that even pile.
   * Alice can repeat this to collect every single even pile, or she can use the exact same logic to collect every single odd pile.

Since the total sum of stones is **odd**, the sum of stones in even piles and the sum of stones in odd piles can never be equal. One set must be strictly larger than the other. 

Before the game starts, Alice can compare the total stones in even positions versus odd positions. She picks the larger group on turn 1 and forces Bob into taking the smaller group. 

Because Alice can always force this winning outcome, she **always wins**. The answer is always `True`!

---

## Approach

* `return True`: Returns `True` instantly. Alice always has a guaranteed strategy to win because she can choose to take either all even-indexed piles or all odd-indexed piles, whichever sum is larger.

---

## Dry Run

### Case 1: Typical case with 4 piles (`piles = [5, 3, 4, 5]`)

| Step | piles | Even-Indexed Sum (index 0, 2) | Odd-Indexed Sum (index 1, 3) | Action | Output |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | `5 + 4 = 9` | `3 + 5 = 8` | Compare even vs. odd sums. Even sum is larger (`9 > 8`). | - |
| 2 | `[5, 3, 4, 5]` | `9` | `8` | Alice targets all even piles. She is guaranteed to win. | `True` |

---

### Case 2: Case with larger middle values (`piles = [3, 7, 2, 3]`)

| Step | piles | Even-Indexed Sum (index 0, 2) | Odd-Indexed Sum (index 1, 3) | Action | Output |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | `3 + 2 = 5` | `7 + 3 = 10` | Compare even vs. odd sums. Odd sum is larger (`10 > 5`). | - |
| 2 | `[3, 7, 2, 3]` | `5` | `10` | Alice targets all odd piles. She is guaranteed to win. | `True` |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** — constant time. The code simply returns `True` without looping through or reading the array.
* **Space Complexity:** **O(1)** — constant space. No extra memory or additional variables are allocated.

### Is this optimal?
Yes. **O(1)** time and **O(1)** space is the theoretical best possible performance. You cannot run faster than a single step or use less memory than zero extra memory.

---

## Edge Cases Handled

* **Smallest array size (`piles.length == 2`):** Alice simply picks `max(piles[0], piles[1])` and wins immediately.
* **Large value discrepancies (e.g., `[1, 100, 1, 1]`):** Alice can force taking the odd piles to secure the pile with `100` stones.
* **Duplicate pile values:** Even if values repeat, the sum of all piles is odd, so one index parity (even or odd) will always strictly exceed the other.
