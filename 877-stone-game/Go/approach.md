![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-4.01%20MB%20(beats%2052.42%25)-yellow?style=for-the-badge)

---

## Problem Explained
Alice and Bob play a game with piles of stones placed in a single row. The rules are simple:

- There is an **even number of piles**.
- Each pile contains a positive number of stones.
- The **total number of stones** across all piles is an **odd number**. This means there are no ties.
- Alice always goes first.
- On each turn, a player can take all the stones from either the **very first pile** or the **very last pile** remaining in the row.
- The game ends when all piles are taken. The player with the most total stones wins.

Assuming both Alice and Bob play optimally to win, return `true` if Alice wins, or `false` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
- Alice can pick the first pile (`5`), leaving `[3, 4, 5]`.
- No matter what Bob picks next, Alice can make choices that secure her victory.
- Alice ends up with more stones, so the answer is `true`.

## Intuition
The core secret to this problem is a mathematical rule: **Alice can always win, no matter what numbers are in the piles.**

Here is why:
1. **Even number of piles:** Let us number the piles by their index position from left to right: `0, 1, 2, 3, ..., N-1`. Because there are an even number of piles, exactly half are at **even positions** (`0, 2, 4...`) and half are at **odd positions** (`1, 3, 5...`).
2. **Odd total sum:** The sum of all stones in the game is odd. That means the sum of all **even-indexed piles** and the sum of all **odd-indexed piles** cannot be equal. One sum must be larger than the other.
3. **Alice controls the outcome:** Alice can choose to take **all** even-indexed piles or **all** odd-indexed piles:
   - If Alice wants all even piles, she takes pile `0` first. This leaves Bob with choices at odd positions (`1` or `N-1`). No matter which one Bob takes, an even position is exposed for Alice on her next turn.
   - If Alice wants all odd piles, she takes pile `N-1` first. This leaves Bob with choices at even positions (`0` or `N-2`). No matter which one Bob takes, an odd position is exposed for Alice.
4. **Alice picks the bigger total:** Before making her first move, Alice adds up the even piles and odd piles in her head. She simply picks the strategy that guarantees her the larger total.

Because Alice goes first, she can always force this winning outcome. Thus, the answer is always `true`.

## Approach
Here is how the code executes step-by-step:

* `return true;`: Immediately returns `true` because Alice is guaranteed to win every valid game setup by choosing either all even-indexed or all odd-indexed piles.

## Dry Run

### Typical Case
Input array: `piles = [5, 3, 4, 5]`

| `piles` | Action | Result |
| :--- | :--- | :--- |
| `[5, 3, 4, 5]` | Return `true` directly because Alice can always win | `true` |

### Case with Duplicate Values
Input array: `piles = [3, 7, 2, 3]`

| `piles` | Action | Result |
| :--- | :--- | :--- |
| `[3, 7, 2, 3]` | Return `true` directly because Alice can always win | `true` |

## Time & Space Complexity
- **Time Complexity:** **O(1)** — Constant time. The code executes a single return statement without looping through the array.
- **Space Complexity:** **O(1)** — Constant space. The code uses no additional memory or data structures.

**Is this optimal?**
Yes. **O(1)** time and **O(1)** space is the absolute best possible complexity for any algorithm. It cannot be improved further.

## Edge Cases Handled
- **Smallest valid input (2 piles):** Handled correctly. For example, if `piles = [2, 1]`, Alice takes `2` on turn 1 and wins immediately.
- **Large number of piles (up to 500):** Handled instantly in **O(1)** time without looping or running out of memory.
- **Large stone values (up to 500 per pile):** Handled effortlessly because no addition or comparison operations are run at runtime.
