![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.9%20MB%20(beats%2075.47%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a game with a single pile of `n` stones. Alice always takes the first turn. 

On any turn, a player must remove a **square number** of stones from the pile (such as 1, 4, 9, 16, and so on). A player cannot remove 0 stones.

The game ends when a player cannot make a valid move because 0 stones are left. The player who cannot move loses the game.

Assuming both Alice and Bob play perfectly to win, you need to return `true` if Alice wins, or `false` if Bob wins.

For example, if `n = 2`:
* Alice can only take 1 stone (since 1 is the only square number less than or equal to 2). That leaves 1 stone.
* Bob then takes the last 1 stone (since 1 is a square number). That leaves 0 stones.
* Alice has no moves left, so Alice loses. The answer is `false`.

---

## Intuition

This problem uses game theory and **Dynamic Programming** (storing smaller answers to solve bigger ones).

In games like this, every position (number of stones) is either a **winning state** or a **losing state**:
* A position is **losing** if every valid move leads to a position where the next player wins.
* A position is **winning** if there is *at least one* valid move that leaves the opponent in a **losing** position.

Instead of working backward from `n` to see what moves we can make, this solution works forward:
1. Start with 0 stones, which is a known **losing state** (if it's your turn with 0 stones, you lose).
2. Whenever we find a state `i` that is **losing**, we know that adding any square number `j * j` to `i` creates a state `i + j * j` where the current player can win! Why? Because from `i + j * j`, the player can take `j * j` stones and force their opponent into the losing state `i`.
3. So, every time we spot a losing state `i`, we mark all states `i + j * j` as winning states.
4. As soon as we find out that our target state `n` is a winning state, we can stop immediately and return `true`.

---

## Approach

Here is how the code executes step-by-step:

* `std::vector<bool> dp(n + 1, false);`: Creates a list of boolean values from index `0` to `n`, initialized to `false`. Here, `dp[x]` represents whether state `x` is a winning state. Index `0` is naturally `false` because 0 stones mean a loss.
* `for (int i = 0; i <= n; i++)`: Iterates through every stone count `i` from `0` up to `n`.
* `if (!dp[i])`: Checks if state `i` is a losing state (`false`). We only care about pushing moves from losing states, because forcing an opponent into a losing state is how you win.
* `for (int j = 1; i + j * j <= n; j++)`: Loops through every possible square move `j * j` (1, 4, 9, 16...) that keeps the target position `i + j * j` within our array bounds (`<= n`).
* `dp[i + j * j] = true;`: Marks state `i + j * j` as `true` (winning), because a player at `i + j * j` can subtract `j * j` to force the opponent into the losing state `i`.
* `if (dp[n])`: Checks if our target value `n` has already been confirmed as a winning state.
* `return true;`: Instantly returns `true` if state `n` is winning, stopping early to save time.
* `return false;`: If the loops finish and `dp[n]` remains `false`, state `n` is losing for Alice, so it returns `false`.

---

## Dry Run

### Case 1: Small non-square number (`n = 2`)

Initial state: `dp = [false, false, false]` (indices 0, 1, 2)

| Step | `i` | `!dp[i]`? | `j` | `i + j * j` | `dp` Array State | Action / Decision |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | Yes (`dp[0]=F`) | 1 | 1 | `[F, T, F]` | `dp[1]` becomes `true` |
| 2 | 0 | Yes (`dp[0]=F`) | 2 | 4 | `[F, T, F]` | `0 + 4 > 2`, inner loop ends |
| 3 | 1 | No (`dp[1]=T`) | - | - | `[F, T, F]` | Skip (state 1 is winning) |
| 4 | 2 | Yes (`dp[2]=F`) | 1 | 3 | `[F, T, F]` | `2 + 1 > 2`, inner loop ends |

Loop ends. `dp[2]` is `false`. Returns `false`.

---

### Case 2: Perfect square input (`n = 4`)

Initial state: `dp = [false, false, false, false, false]` (indices 0 to 4)

| Step | `i` | `!dp[i]`? | `j` | `i + j * j` | `dp` Array State | Action / Decision |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | Yes (`dp[0]=F`) | 1 | 1 | `[F, T, F, F, F]` | `dp[1]` becomes `true` |
| 2 | 0 | Yes (`dp[0]=F`) | 2 | 4 | `[F, T, F, F, T]` | `dp[4]` becomes `true` |
| 3 | 0 | - | - | 4 | `[F, T, F, F, T]` | `dp[4]` is `true`, `if (dp[n])` triggers! |

Returns `true` immediately.

---

## Time & Space Complexity

* **Time Complexity:** **O(n * sqrt(n))** in the worst case. 
  For each losing state `i`, the inner loop runs for values of `j` where `j * j <= n - i`. The number of choices for `j` is bounded by `sqrt(n)`. Summing this over all numbers up to `n` gives a worst-case time bound of `O(n * sqrt(n))`. The early return `if (dp[n]) return true;` makes it much faster on average in practice.
* **Space Complexity:** **O(n)** — We store a single boolean vector `dp` of size `n + 1`.

### Is this optimal?
**Yes, this algorithm is optimal.** 

In game theory problems without predictable mathematical patterns (like Nim-sum), each game state must be built up from lower states. Calculating up to state `n` requires checking square steps, leading directly to `O(n * sqrt(n))` time and `O(n)` space. No faster theoretical lower bound exists for solving this game for general values of `n`.

---

## Edge Cases Handled

* **Base Case (`n = 1`):** Minimum input allowed by constraints (`1 <= n`). On the very first iteration (`i = 0`, `j = 1`), `dp[1]` is set to `true`, and the function immediately returns `true`.
* **Perfect Square Inputs (`n = 1, 4, 9, 16...`):** On step `i = 0`, the inner loop tries `j = sqrt(n)` and directly sets `dp[n] = true`. The function finishes in a single outer loop step.
* **Maximum Constraint (`n = 10^5`):** The total operations will be around `10^5 * sqrt(10^5)`, which is roughly `3.16 * 10^7` operations. This comfortably finishes well within standard time limits (usually around `10^8` operations per second).
