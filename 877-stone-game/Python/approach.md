![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-12.46%20MB%20(beats%2038.53%25)-orange?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with an even number of stone piles arranged in a single row. Each pile contains a positive number of stones. 

Here are the rules of the game:
* Alice always goes first.
* On any player's turn, they can take all stones from either the **very first pile** or the **very last pile** remaining in the row.
* The game ends when all piles are taken.
* Whoever has the higher total number of stones wins.
* The total sum of all stones in all piles is guaranteed to be an odd number, which means a tie is impossible.

We need to return `True` if Alice wins assuming both players make the best possible moves, and `False` if Bob wins.

**Example:**
If `piles = [5, 3, 4, 5]`:
* The total number of piles is 4 (an even number).
* The sum of stones is 5 + 3 + 4 + 5 = 17 (an odd number).
* We need to determine if Alice can force a win.

---

## Intuition

The key takeaway is that **Alice will always win every game**. 

Why? Because of two rules given in the constraints:
1. The total number of piles is **even**.
2. The total sum of all stones is **odd**.

Because the number of piles is even, we can color the positions in the row like a chessboard: alternating **even index positions** (0, 2, 4, ...) and **odd index positions** (1, 3, 5, ...).

Since the total stone count is odd, the sum of stones at even positions can never equal the sum of stones at odd positions. One of these two groups must have a higher total than the other.

Because Alice goes first, she can choose to collect **all even-indexed piles** or **all odd-indexed piles**:
* If Alice wants all **even** piles, she starts by picking index `0`. Now Bob is forced to pick between two **odd** indices (index `1` or the last index `N - 1`). No matter what Bob picks, he opens up another even index for Alice to pick on her next turn!
* If Alice wants all **odd** piles, she starts by picking the last index `N - 1` (which is an odd index). Now Bob is forced to pick between two **even** indices. On her next turn, Alice can grab another odd index.

Before the game even starts, Alice can quickly add up the even piles and odd piles, see which group is larger, and force Bob into taking the smaller group. 

Therefore, Alice can always guarantee a win. We do not need to simulate the game at all — returning `True` is always correct!

---

## Approach

Here is how the code executes step-by-step:

* `def stoneGame(self, piles):`: Accepts the array `piles` containing the size of each stone pile.
* `return True`: Immediately returns `True` because Alice can always win by using the even/odd index strategy described above.

---

## Dry Run

### Case 1: Standard four-pile game (`piles = [5, 3, 4, 5]`)

Even-indexed piles (0, 2): `5 + 4 = 9` stones.  
Odd-indexed piles (1, 3): `3 + 5 = 8` stones.  
Alice chooses the even strategy because `9 > 8`.

| Step | `piles` | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | Evaluate winning strategy (Even group = 9, Odd group = 8) | Alice can force a win |
| 2 | `[5, 3, 4, 5]` | Return result | Returns `True` |

---

### Case 2: Game with odd-sum dominant (`piles = [3, 7, 2, 3]`)

Even-indexed piles (0, 2): `3 + 2 = 5` stones.  
Odd-indexed piles (1, 3): `7 + 3 = 10` stones.  
Alice chooses the odd strategy because `10 > 5`.

| Step | `piles` | Action | Result |
| :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | Evaluate winning strategy (Even group = 5, Odd group = 10) | Alice can force a win |
| 2 | `[3, 7, 2, 3]` | Return result | Returns `True` |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** — Constant time. The code executes a single return statement regardless of the length of `piles`.
* **Space Complexity:** **O(1)** — Constant space. No extra memory or variables are allocated.

### Is this already optimal?
**Yes, this is completely optimal.** 
* You cannot run faster than O(1) time or use less memory than O(1) space. 
* Many full game-tree solutions use Dynamic Programming which takes O(N^2) time and O(N^2) space to calculate the exact difference in final scores. However, because the problem only asks *if* Alice wins (a boolean), mathematical logic allows us to answer in O(1) without calculating the score difference.

---

## Edge Cases Handled

* **Minimum array length (`piles.length == 2`):** Alice simply picks `max(piles[0], piles[1])` and wins immediately. The logic holds, returning `True`.
* **Large arrays (`piles.length == 500`):** Because the time complexity is O(1), large inputs run instantly without any performance penalty or stack overflow risk.
* **Duplicate values across piles (e.g., `[3, 7, 2, 3]`):** Works seamlessly because the problem guarantees the total sum across all piles is odd, so the sum of even positions and odd positions can never be equal. There are no ties.
