![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.90%20MB%20(beats%2090.65%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with a pile of `n` stones, taking turns with Alice going first. 

On any turn, a player can remove any positive square number of stones from the pile. A square number is a number you get by multiplying an integer by itself, like 1 (1 * 1), 4 (2 * 2), 9 (3 * 3), 16 (4 * 4), and so on.

The player who cannot make a move (because 0 stones are left) loses the game.

Assuming both players play with perfect strategy to win, we need to return `true` if Alice wins, and `false` if Bob wins.

**Example:**
* If `n = 2`:
  * Alice can only remove 1 stone (leaving 1 stone). She cannot remove 4 because 4 is greater than 2.
  * Bob then removes 1 stone (leaving 0 stones).
  * Alice has no moves left and loses. So for `n = 2`, the answer is `false`.

---

## Intuition

This problem can be understood through two game positions:
* **Losing Position:** A state where no matter what move you make, the opponent can win. A pile of `0` stones is the ultimate losing position because you cannot make a move at all.
* **Winning Position:** A state where you can make at least one valid move that leaves the opponent in a **Losing Position**.

Instead of checking backwards from a number to see if we can reach a losing state, this code looks forward (**Push Dynamic Programming**):

1. We start with `0` stones, which is known to be a losing position.
2. If we find that `i` stones is a **Losing Position** for whoever starts their turn at `i`, then any position `i + j * j` is a **Winning Position**! Why? Because the player at `i + j * j` can simply take away `j * j` stones and force their opponent into the losing position `i`.
3. So, whenever we confirm a number `i` is a losing position, we immediately mark all positions `i + 1`, `i + 4`, `i + 9`, etc., as winning positions.

---

## Approach

* `std::vector<bool> dp(n + 1, false);`: We create a true/false list of size `n + 1` filled with `false`. Here, `dp[k] = true` means starting with `k` stones is a winning position, and `false` means it is a losing position. Everything defaults to `false` (losing).
* `for (int i = 0; i <= n; i++)`: We iterate through every stone count `i` from `0` up to `n`.
* `if (!dp[i])`: We check if position `i` is confirmed to be a losing position (`false`).
* `for (int j = 1; i + j * j <= n; j++)`: If `i` is losing, we loop through all valid square jumps (`1*1`, `2*2`, `3*3`, etc.) that can land directly on `i`.
* `dp[i + j * j] = true;`: We mark each of those higher states (`i + j * j`) as `true` (winning), because a player at `i + j * j` can take `j * j` stones and leave the opponent with `i` stones (a losing position).
* `if (dp[n]) { return true; }`: This is an early exit optimization. As soon as we discover that our target state `n` is guaranteed to be a winning position, we immediately return `true` without completing the rest of the loops.
* `return false;`: If the loops finish and `dp[n]` is still `false`, it means Alice cannot force a win starting at `n`, so we return `false`.

---

## Dry Run

### Case 1: `n = 2` (Small losing case)

Initial setup: `dp` vector of size 3 initialized to `[false, false, false]`.

| Loop Index `i` | `dp[i]` state | Inner Loop `j` | Calculation (`i + j * j`) | Action Taken | `dp` Array State |
|---|---|---|---|---|---|
| `i = 0` | `false` (Losing) | `j = 1` | `0 + 1 * 1 = 1` | Mark `dp[1] = true` | `[false, true, false]` |
| `i = 0` | `false` (Losing) | `j = 2` | `0 + 2 * 2 = 4` | Exceeds `n = 2`, inner loop ends | `[false, true, false]` |
| `i = 1` | `true` (Winning) | N/A | N/A | Skipped because `dp[1]` is `true` | `[false, true, false]` |
| `i = 2` | `false` (Losing) | `j = 1` | `2 + 1 * 1 = 3` | Exceeds `n = 2`, inner loop ends | `[false, true, false]` |

After loop finishes, `dp[2]` is `false`. Returns `false`.

---

### Case 2: `n = 4` (Perfect square winning case)

Initial setup: `dp` vector of size 5 initialized to `[false, false, false, false, false]`.

| Loop Index `i` | `dp[i]` state | Inner Loop `j` | Calculation (`i + j * j`) | Action Taken | `dp` Array State |
|---|---|---|---|---|---|
| `i = 0` | `false` (Losing) | `j = 1` | `0 + 1 * 1 = 1` | Mark `dp[1] = true` | `[false, true, false, false, false]` |
| `i = 0` | `false` (Losing) | `j = 2` | `0 + 2 * 2 = 4` | Mark `dp[4] = true` | `[false, true, false, false, true]` |
| `i = 0` | `false` (Losing) | N/A | `dp[4]` is now `true` | `dp[n]` is `true`, return early! | Returns `true` |

---

## Time & Space Complexity

* **Time Complexity:** `O(n * sqrt(n))` — The outer loop runs up to `n` times. The inner loop runs `sqrt(n - i)` times only when `dp[i]` is `false`. The sum of `sqrt(k)` for `k` from 1 to `n` gives a total runtime bounded by `O(n * sqrt(n))`.
* **Space Complexity:** `O(n)` — Storing the boolean vector `dp` of size `n + 1`.

### Is this optimal?

**Yes, this code is already optimal.** 

To decide if a game state of size `n` is winning or losing, every state from `1` to `n` must potentially evaluate available square moves up to `sqrt(n)`. There is no known formula or pattern to compute the winner in `O(1)` or `O(sqrt(n))` time for all `n`. Therefore, `O(n * sqrt(n))` time and `O(n)` space is the standard best-possible complexity for this problem.

---

## Edge Cases Handled

* **`n = 1` (Smallest input):** `i = 0` marks `dp[0 + 1*1] = dp[1] = true`, immediately triggering the early exit check `if (dp[1]) return true;`.
* **`n` is a perfect square (e.g., `n = 4, 9, 16`):** On the very first step (`i = 0`), the inner loop tries `j = sqrt(n)` and marks `dp[n] = true` directly, returning `true` almost instantly.
* **Maximum constraint (`n = 10^5`):** Since `10^5 * sqrt(10^5)` is roughly 31.6 million operations, this finishes in less than 20 milliseconds in C++, well within the standard 1-second time limit.
