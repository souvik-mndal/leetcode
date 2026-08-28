![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-55.57%20MB%20(beats%2049.82%25)-yellow?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with piles of stones placed in a row. Each pile has a specific number of stones. 

The game follows these rules:
* The total number of piles is always an **even number**.
* The total number of stones across all piles is always an **odd number**. This means there are no ties.
* Players take turns, and Alice always goes first.
* On their turn, a player can take the entire pile of stones from either the **far left** or the **far right** end of the row.
* The game ends when all piles are taken. The player with the most stones wins.

Assuming both players play with the best possible strategy to maximize their stone count, return `true` if Alice wins, or `false` if Bob wins.

For example, if the piles are `[5, 3, 4, 5]`:
* Alice goes first and can pick either the left `5` or the right `5`.
* If Alice picks the left `5`, the remaining piles are `[3, 4, 5]`.
* Bob can now pick `3` or `5`. No matter what Bob picks, Alice can make a move on her next turn that secures her enough stones to win.

---

## Intuition

This problem looks like a classic **game theory** or **dynamic programming** problem where you would simulate every possible turn. However, there is a mathematical secret that makes it trivial.

Because there are an **even number of piles**, we can split the piles into two sets based on their position:
1. **Even-indexed piles** (index 0, 2, 4, etc.)
2. **Odd-indexed piles** (index 1, 3, 5, etc.)

Since Alice goes first, she can actually decide whether she gets **all even-indexed piles** or **all odd-indexed piles**:
* If Alice wants all even piles, she picks index `0` on her first turn. This leaves Bob with two odd-indexed piles to choose from (index `1` or index `N-1`).
* Whichever odd pile Bob picks, he opens up another even-indexed pile for Alice on her next turn.
* Alice can repeat this process every turn to collect every single even-indexed pile.
* Alternatively, if Alice picks index `N-1` on her first turn, she can force a path to collect every single odd-indexed pile instead.

Before the game even begins, Alice can calculate the total stones in the even piles and the total stones in the odd piles. Because the total number of stones is odd, one group **must** have more stones than the other. They cannot be equal.

Alice simply chooses the group with more stones and enforces that strategy. Therefore, Alice **always wins**, and the answer is always `true`.

---

## Approach

Here is how the code operates step-by-step:

* `function stoneGame(piles: number[]): boolean` - Accepts the array `piles` of numbers representing the stone counts and specifies that the output will be a boolean (`true` or `false`).
* `return true;` - Immediately returns `true` without scanning the array, because Alice can always guarantee a win by controlling whether she gets all even-indexed piles or all odd-indexed piles.

---

## Dry Run

### Case 1: Typical case (`piles = [5, 3, 4, 5]`)

| Step | `piles` | Even-Indexed Sum (Index 0, 2) | Odd-Indexed Sum (Index 1, 3) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | 5 + 4 = 9 | 3 + 5 = 8 | Alice compares sums before playing. Even sum (9) is greater than Odd sum (8). |
| 2 | `[5, 3, 4, 5]` | 9 | 8 | Alice chooses even-index strategy. Code returns `true` instantly. |

### Case 2: Small row case (`piles = [3, 7, 2, 3]`)

| Step | `piles` | Even-Indexed Sum (Index 0, 2) | Odd-Indexed Sum (Index 1, 3) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | 3 + 2 = 5 | 7 + 3 = 10 | Alice compares sums before playing. Odd sum (10) is greater than Even sum (5). |
| 2 | `[3, 7, 2, 3]` | 5 | 10 | Alice chooses odd-index strategy. Code returns `true` instantly. |

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** — The solution runs in constant time. It performs a single operation and returns immediately regardless of the size of `piles`.
* **Space Complexity:** **O(1)** — The solution uses no additional memory or temporary storage.

### Is this optimal?
**Yes, this is already optimal.** O(1) time and space complexity is the absolute best theoretical efficiency achievable for any algorithm.

*Note on standard Game Theory approach:* If the rules were modified (e.g., an odd number of piles allowed, or equal sums allowed), you would need **Dynamic Programming** which takes **O(N^2)** time and **O(N^2)** space (or **O(N)** space optimized) to calculate max score differences. But given this problem's strict constraints, O(1) is completely correct.

---

## Edge Cases Handled

* **Minimum array length (`piles.length = 2`):** Alice simply picks the larger of the two piles and wins immediately.
* **Large arrays (`piles.length = 500`):** Because execution takes O(1) time, large inputs process instantly without running out of memory or timing out.
* **Varying pile values:** No matter how large or small individual values in `piles` are, the odd-versus-even strategy remains mathematically guaranteed.
* **No ties possible:** The constraints guarantee an odd total sum of stones, ensuring one sum (even-indexed or odd-indexed) is strictly strictly larger than the other.
