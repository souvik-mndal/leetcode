![Runtime](https://img.shields.io/badge/Runtime-11%20ms%20(beats%2048.12%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.3%20MB%20(beats%2021.14%25)-orange?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with a row of stone piles. Each pile has a certain number of stones. 

Alice goes first. On any turn, a player can take all stones from the first `X` available piles. The rule for `X` is that it must be between `1` and `2 * M` (inclusive). At the start of the game, `M` is set to `1`. 

After a player picks `X` piles, `M` updates to `max(M, X)`. The game continues until no piles are left. Both players play perfectly to get as many stones as possible. The goal is to calculate the total number of stones Alice will get by the end of the game.

For example, if the piles are `[2, 7, 9, 4, 4]`, Alice can take either 1 pile or 2 piles on her first move. Alice plays strategically so that even after Bob makes his optimal moves, she ends up with 10 stones total.


## Intuition

This problem is a turn-based game where both players make optimal choices. To solve it, we use **Dynamic Programming** with **Memoization** (saving past answers so we do not recalculate them).

The core trick is called a **Zero-Sum Game** approach:
Instead of trying to keep track of Alice's score and Bob's score separately during the game, we calculate the **net lead** (my total stones minus opponent's total stones) for whichever player's turn it currently is.

At any step:
1. Current player picks `X` piles, gaining `cur` stones.
2. The remaining game shifts to the other player, who will also play optimally to maximize their own lead.
3. Therefore, the current player's net lead becomes `cur - opponent_lead`.

At the end of the recursion, we get the total net difference between Alice and Bob (`netDiff = Alice - Bob`).
We also know the total sum of all stones in all piles (`sum = Alice + Bob`).

Adding these two equations together gives:
`2 * Alice = netDiff + sum`
`Alice = (netDiff + sum) / 2`

This converts a complex two-player tracking problem into a simple formula.


## Approach

Here is how the code works step-by-step:

* `memo = vector<vector<int>>(piles.size(), vector<int>(100, -1));`: Creates a 2D memory grid filled with `-1`. It stores answers for combinations of index `pos` and game state `m` so we don't recalculate them.
* `int netDiff = getScore(piles, 0, 1);`: Starts the main recursive search at index `0` with initial `m = 1` to find Alice's final relative lead over Bob.
* `if(pos == piles.size()) return 0;`: Checks the base case. If we reached past the last pile, no stones remain, so the score difference is `0`.
* `if(memo[pos][m] != -1) return memo[pos][m];`: Checks if we already solved the game state for `pos` and `m`. If yes, returns the saved result.
* `for(int i = pos; i < piles.size() && i < pos + 2 * m; i++)`: Loops through every valid choice of taking `X` piles, where `X` goes from `1` up to `2 * m`.
* `cur += piles[i];`: Adds the stones from pile `i` to the current turn's total score `cur`.
* `best = max(best, cur - getScore(piles, i + 1, max(m, i - pos + 1)));`: Calculates the net lead for taking `i - pos + 1` piles. It subtracts the opponent's best lead from the remaining piles and keeps the maximum lead found so far in `best`.
* `return memo[pos][m] = best;`: Stores the best net lead for `(pos, m)` inside `memo` and returns it.
* `for(int x: piles) sum+= x;`: Adds up all the stones across every pile in the input array.
* `int a = (netDiff + sum) / 2;`: Applies the algebra formula `Alice = (netDiff + sum) / 2` to figure out Alice's exact total stone count.


## Dry Run

### Example 1: `piles = [2, 7, 9, 4, 4]` (Total Sum = 26)

| State | `pos` | `m` | `X` (Piles Taken) | `cur` | Call / Decision | `best` Return |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 3 | 1 | 2 (piles `[4, 4]`) | 8 | Takes all remaining piles | 8 |
| 2 | 2 | 1 | 2 (piles `[9, 4]`) | 13 | Opponent gets `getScore(4, 2) = 4`. Net: `13 - 4 = 9` | 9 |
| 3 | 1 | 1 | 2 (piles `[7, 9]`) | 16 | Opponent gets `getScore(3, 2) = 8`. Net: `16 - 8 = 8` | 8 |
| 4 | 0 | 1 | 1 (pile `[2]`) | 2 | Opponent gets `getScore(1, 1) = 8`. Net: `2 - 8 = -6` | -6 |
| 5 | 0 | 1 | 2 (pile `[2, 7]`) | 9 | Opponent gets `getScore(2, 2) = 17`. Net: `9 - 17 = -8` | -6 |

* Final `netDiff` = -6.
* `a = (-6 + 26) / 2 = 10`.
* Result: **10**


### Example 2: `piles = [1, 2, 3, 4, 5, 100]` (Total Sum = 115)

| State | `pos` | `m` | `X` (Piles Taken) | `cur` | Call / Decision | `best` Return |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 4 | 2 | 2 (piles `[5, 100]`) | 105 | Takes remaining piles | 105 |
| 2 | 3 | 1 | 2 (piles `[4, 5]`) | 9 | Opponent gets `getScore(5, 2) = 100`. Net: `9 - 100 = -91` | -91 |
| 3 | 2 | 1 | 1 (pile `[3]`) | 3 | Opponent gets `getScore(3, 1) = -91`. Net: `3 - (-91) = 94` | 94 |
| 4 | 1 | 1 | 1 (pile `[2]`) | 2 | Opponent gets `getScore(2, 1) = 94`. Net: `2 - 94 = -92` | -92 |
| 5 | 0 | 1 | 1 (pile `[1]`) | 1 | Opponent gets `getScore(1, 1) = -92`. Net: `1 - (-92) = 93` | 93 |

* Final `netDiff` = 93.
* `a = (93 + 115) / 2 = 104`.
* Result: **104**


## Time & Space Complexity

* **Time Complexity:** **O(N^3)** — There are `N` possible values for `pos` and up to `N` possible values for `m` (where `N` is the length of `piles`). That gives `O(N^2)` dynamic programming states. For each state, the `for` loop runs up to `2 * m` times, which is bounded by `O(N)` steps. Total runtime is `N * N * N = O(N^3)`.
* **Space Complexity:** **O(N^2)** — The `memo` array uses `N * 100` spaces, which is bounded by `O(N^2)` memory. The recursive call stack goes at most `N` levels deep (`O(N)` space).

### Can it be improved?

Yes, the calculation logic can be simplified using **Suffix Sums**. 

Instead of tracking relative score differences (`netDiff`) and solving linear equations at the end, we can directly compute the maximum stones Alice can collect from index `pos` onward. 

If we know the sum of all stones from index `pos` to the end (`suffixSum[pos]`), then taking `X` piles leaves the opponent with `dp(pos + X, max(m, X))` stones. Since all remaining stones equal `suffixSum[pos]`, the current player's score for that move is simply `suffixSum[pos] - dp(pos + X, max(m, X))`.

Here is the key logic change:

```cpp
// Precompute suffix sums
vector<int> suffixSum(n, 0);
suffixSum[n - 1] = piles[n - 1];
for (int i = n - 2; i >= 0; i--) {
    suffixSum[i] = suffixSum[i + 1] + piles[i];
}

// Inside recursion:
int best = 0;
for (int x = 1; x <= 2 * m && pos + x <= n; x++) {
    best = max(best, suffixSum[pos] - getScore(pos + x, max(m, x)));
}
return memo[pos][m] = best;
```

* **Improved Time Complexity:** **O(N^3)** in worst case, but executes faster because it avoids inner loop running totals and algebraic division.
* **Improved Space Complexity:** **O(N^2)** to store DP states.
* **Optimal Complexity:** **O(N^3)** time and **O(N^2)** space is standard for Stone Game II. (Further optimizations using prefix/suffix minima can reduce it to **O(N^2)** time, but `N <= 100` makes O(N^3) extremely fast in practice).


## Edge Cases Handled

* **Small inputs (`piles.length <= 2`):** Alice can take all piles on her first turn because `2 * M = 2 >= piles.size()`. The code correctly evaluates taking all piles in a single turn.
* **Large late values (like `[1, 2, 3, 4, 5, 100]`):** The recursive lookahead ensures Alice plays defensively in early turns so Bob is forced to leave the 100-stone pile for Alice.
* **`M` exceeding array bounds:** The loop condition `i < piles.size()` prevents reading past the end of the array even when `2 * m` grows larger than the remaining piles.
* **Base Case (`pos == piles.size()`):** Handled cleanly at the top of `getScore`, returning `0` score when no piles remain.
