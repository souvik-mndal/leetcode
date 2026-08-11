![Runtime](https://img.shields.io/badge/Runtime-11%20ms%20(beats%2050.58%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.26%20MB%20(beats%2027.42%25)-orange?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a game with a row of stone piles. Each pile contains a specific number of stones given in an array called `piles`. Alice always goes first.

On any player's turn, that player can take all the stones from the first `X` remaining piles, where `X` can be any integer from `1` up to `2 * M`.
* At the start of the game, `M` is set to `1`.
* Every time a player chooses to take `X` piles, `M` is updated for the next turn to be `max(M, X)`.
* Play continues back and forth until all piles are taken.

Both Alice and Bob play with full knowledge and optimal strategy to get as many total stones as possible for themselves. The goal is to return the total number of stones Alice gets at the end of the game.

For example, if `piles = [2, 7, 9, 4, 4]`:
* `M` starts at `1`, so Alice can take `1` or `2` piles.
* If Alice takes `1` pile (`2` stones), `M` remains `max(1, 1) = 1`. Bob can then take `1` or `2` piles.
* By making the best moves throughout, Alice ends up with `10` stones total.

---

## Intuition

This is a two-player zero-sum game, which means whatever stones one player takes are stones the other player loses out on. 

Instead of tracking Alice's and Bob's scores separately, we can solve for the **net score difference**: `(My Score - Opponent's Score)` for whoever is currently taking a turn.

When it is your turn at pile `pos` with variable `m`:
1. You try every valid move (taking `X` piles, from `1` to `2 * m`).
2. For each move, you gain the sum of those `X` piles right now.
3. Then, your opponent gets their turn starting at pile `pos + X` with updated `m = max(m, X)`. Your opponent will also play optimally to maximize *their* score difference over you.
4. Therefore, your total net score gain from this choice is: `(Stones you take now) - (Opponent's optimal net score from the next turn)`.
5. You pick the choice `X` that maximizes this net score gain.

Once we know the maximum net score difference Alice achieves from the very start (`netDiff = Alice - Bob`), and we know the total stones in all piles (`sum = Alice + Bob`), we can solve for Alice's total stones with simple middle-school algebra:
```
Alice = (sum + netDiff) / 2
```

---

## Approach

Here is how the code works step-by-step:

* `memo = vector<vector<int>>(piles.size(), vector<int>(100, -1));`: Creates a 2D grid initialized to `-1` to cache previously computed results. `memo[pos][m]` stores the best net score difference achievable from pile `pos` given multiplier `m`.
* `int netDiff = getScore(piles, 0, 1);`: Calls the recursive function starting at index `0` with `m = 1` to find Alice's final net score advantage over Bob.
* `if(pos == piles.size()) return 0;`: Base case for recursion. If there are no piles left to pick, no additional points can be scored, so return `0`.
* `if(memo[pos][m] != -1) return memo[pos][m];`: Memoization check. If we already calculated the result for this `pos` and `m`, return it immediately.
* `for(int i = pos; i < piles.size() && i < pos + 2 * m; i++)`: Loops through all valid numbers of piles `X` the current player can take, ranging from `1` up to `2 * m`.
* `cur += piles[i];`: Keeps a running total of stones taken in the current turn.
* `best = max(best, cur - getScore(piles, i + 1, max(m, i - pos + 1)));`: Recursively finds the opponent's best net score for the rest of the game, subtracts it from `cur`, and updates `best` with the highest net profit found across all valid choices.
* `return memo[pos][m] = best;`: Saves the maximum net profit into `memo[pos][m]` and returns it.
* `for(int x: piles) sum += x;`: Calculates the grand total sum of all stones across all piles.
* `int a = (netDiff + sum) / 2;`: Uses the algebraic relation `(Alice + Bob) + (Alice - Bob) = 2 * Alice` to compute Alice's exact total stones.
* `return a;`: Returns Alice's total score.

---

## Dry Run

### Case 1: Standard Case (`piles = [2, 7, 9, 4, 4]`)
Total sum of piles = `26`.

| `pos` | `m` | Action / Decision | Return Value (`netDiff`) |
| :--- | :--- | :--- | :--- |
| `4` | `1` | 1 pile left (`[4]`). Must take `X = 1`. Gain: `4 - 0 = 4`. | `4` |
| `3` | `1` | 2 piles left (`[4, 4]`). `X=1`: gain `4 - 4 = 0`. `X=2`: gain `8 - 0 = 8`. Best is `X=2`. | `8` |
| `2` | `1` | 3 piles left (`[9, 4, 4]`). `X=1`: gain `9 - 8 = 1`. `X=2`: gain `13 - 4 = 9`. Best is `X=2`. | `9` |
| `2` | `2` | 3 piles left (`[9, 4, 4]`). `m=2` allows `X` up to `4`. Takes all 3 piles: gain `17 - 0 = 17`. | `17` |
| `1` | `1` | 4 piles left (`[7, 9, 4, 4]`). `X=1`: gain `7 - 9 = -2`. `X=2`: gain `16 - 8 = 8`. Best is `X=2`. | `8` |
| `0` | `1` | Start (`[2, 7, 9, 4, 4]`). `X=1`: gain `2 - 8 = -6`. `X=2`: gain `9 - 17 = -8`. Best is `X=1`. | `-6` |

Calculated Alice score: `a = (-6 + 26) / 2 = 10`. Output: `10`.

---

### Case 2: Multi-step Case (`piles = [1, 2, 3, 4, 5, 100]`)
Total sum of piles = `115`.

| `pos` | `m` | Action / Decision | Return Value (`netDiff`) |
| :--- | :--- | :--- | :--- |
| `5` | `3` | 1 pile left (`[100]`). Takes 1 pile. | `100` |
| `4` | `1` | 2 piles left (`[5, 100]`). `X=1`: gain `5 - 100 = -95`. `X=2`: gain `105 - 0 = 105`. Best is `X=2`. | `105` |
| `3` | `1` | 3 piles left (`[4, 5, 100]`). `X=1`: `4 - 105 = -101`. `X=2`: `9 - 100 = -91`. Best is `X=2`. | `-91` |
| `2` | `1` | 4 piles left (`[3, 4, 5, 100]`). `X=1`: `3 - (-91) = 94`. `X=2`: `7 - 105 = -98`. Best is `X=1`. | `94` |
| `1` | `1` | 5 piles left (`[2, 3, 4, 5, 100]`). `X=1`: `2 - 94 = -92`. `X=2`: `5 - (-91) = 96`. Best is `X=2`. | `96` |
| `0` | `1` | Start (`[1, 2, 3, 4, 5, 100]`). `X=1`: `1 - 96 = -95`. `X=2`: `3 - 94 = 93`. Best is `X=2`. | `93` |

Calculated Alice score: `a = (93 + 115) / 2 = 104`. Output: `104`.

---

## Time & Space Complexity

* **Time Complexity:** O(N^3) — where N is the number of piles. There are O(N^2) total states for `(pos, m)`. For each state, the `for` loop runs up to `2 * m` times (which is bounded by N). Thus, total time is O(N^2 * N) = O(N^3).
* **Space Complexity:** O(N^2) — storing the 2D memo grid (`N x 100`), plus O(N) space for the recursive function call stack.

### Can it be improved?

**Yes.** We can optimize both runtime and implementation clean-up in two main ways:

1. **Suffix Sum Optimization:** Instead of dynamically accumulating `cur += piles[i]` inside the inner loop and computing `(netDiff + sum) / 2` at the end, we can precalculate a **suffix sum array**. The suffix sum `suffixSum[i]` stores the total stones remaining from pile `i` to the end.
   If a player takes $X$ piles from position `pos`, the total stones remaining in the entire game from `pos` onwards is `suffixSum[pos]`. Whatever stones the opponent gets from position `pos + X` is subtracted directly from `suffixSum[pos]` to give the current player's maximum direct score:
   ```
   myScore = suffixSum[pos] - solve(pos + X, max(m, X))
   ```
2. **Early Stopping Shortcut:** If `pos + 2 * m >= N` (meaning you can grab all remaining piles in one single turn), you should just grab all of them immediately without running the loop.

Here is the optimized recursive function snippet:

```cpp
int solve(vector<int>& piles, vector<int>& suffixSum, vector<vector<int>>& memo, int pos, int m) {
    int n = piles.size();
    if (pos >= n) return 0;
    if (pos + 2 * m >= n) return suffixSum[pos]; // Shortcut: take all remaining piles
    if (memo[pos][m] != -1) return memo[pos][m];

    int maxStones = 0;
    for (int x = 1; x <= 2 * m; x++) {
        // Remaining stones minus opponent's best score from next position
        int currentChoice = suffixSum[pos] - solve(piles, suffixSum, memo, pos + x, max(m, x));
        maxStones = max(maxStones, currentChoice);
    }

    return memo[pos][m] = maxStones;
}
```

* **Improved Complexity:**
  * **Time Complexity:** Still O(N^3) asymptotically, but with significantly smaller constant factors due to $O(1)$ suffix sum lookups and early stopping shortcuts when $2M \ge N - pos$.
  * **Space Complexity:** O(N^2) for memoization grid and $O(N)$ for suffix sum array.
* **Theoretical Best Complexity:** **O(N^2)** time and **O(N^2)** space. Dynamic programming state space requires $N \times N$ unique states, and with bottom-up DP transition techniques, each state can be evaluated efficiently.

---

## Edge Cases Handled

* **Single Pile (`piles.length == 1`):** Since $M = 1$ initially, $2M = 2 \ge 1$. Alice can take the entire array on turn 1 and win all stones instantly.
* **Can Take All Remaining Piles ($2M \ge \text{remaining piles}$):** The loop naturally allows a player to grab all leftover piles in a single turn if allowed, leaving $0$ for the opponent.
* **Large Stone Amounts ($10^4$ per pile):** Maximum possible total stones across $100$ piles is $1,000,000$. This comfortably fits within a standard 32-bit signed integer (`int`) without overflow.
