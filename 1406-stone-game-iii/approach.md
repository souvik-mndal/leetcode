![Runtime](https://img.shields.io/badge/Runtime-35%20ms%20(beats%2050.74%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-136.14%20MB%20(beats%2076.04%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob are playing a game with a row of stones. Each stone has a point value given in an array called `stoneValue`. 

Here are the rules:
* **Alice always goes first.**
* On any player's turn, they can take **1, 2, or 3 stones** from the front of the remaining row.
* Taking stones adds their values to that player's total score. Point values can be **positive or negative**.
* The game ends when all stones are taken.
* Both players play **optimally**—meaning each player makes the absolute best choices to win (or maximize their score difference over the opponent).

Your goal is to determine who wins:
* Return `"Alice"` if Alice gets a higher score than Bob.
* Return `"Bob"` if Bob gets a higher score than Alice.
* Return `"Tie"` if both players end up with the exact same score.

**Example:**
If `stoneValue = [1, 2, 3, 7]`:
* If Alice takes 1 stone (value `1`), Bob will take 3 stones (`2 + 3 + 7 = 12`) and easily win.
* If Alice takes 3 stones (`1 + 2 + 3 = 6`), Bob takes the last stone (`7`) and wins with 7 points.
* Bob wins regardless of Alice's strategy. So the answer is `"Bob"`.

---

## Intuition

The key insight is to track the **score difference** between the current player and the opponent, rather than tracking two separate scores.

At any point in the game, the player whose turn it is wants to answer one question:  
*"If I choose 1, 2, or 3 stones right now, how many **more** points will I end up with compared to my opponent from this point onward?"*

If you take a set of stones with a total value of `sum`, your opponent goes next. The opponent will then play perfectly to maximize *their* lead over you from the remaining stones. 

So, your net lead over your opponent if you pick those stones is:
$$\text{Your Lead} = \text{Value of stones you take} - \text{Opponent's future lead}$$

Because future decisions affect current ones, we work **backward** from the end of the array to the beginning using **dynamic programming** (a technique where we solve smaller subproblems first and save their results to solve bigger ones). By the time we reach index `0` (the start of the game), we will know Alice's exact maximum relative score lead over Bob.

---

## Approach

Here is how the code executes step-by-step:

* **Step 1: Set up the DP table.**  
  Create an array `dp` of size `n + 1` filled with `0`s.  
  `dp[i]` represents the maximum score difference the player whose turn it is at index `i` can achieve over the other player for the rest of the game.  
  `dp[n] = 0` acts as our starting point (when no stones are left, the score lead is `0`).

* **Step 2: Iterate backward through the stones.**  
  Start a loop from the last stone `i = n - 1` down to the first stone `i = 0`.

* **Step 3: Evaluate taking 1, 2, or 3 stones.**  
  At each index `i`:
  * Set `dp[i]` to a very small number (`INT_MIN`) so any real score will overwrite it.
  * Initialize a running total `sum = 0`.
  * Run an inner loop (`j` from `i` up to `i + 2`, stopping if we run out of stones):
    * Add `stoneValue[j]` to `sum`.
    * Calculate the score difference for this choice: `sum - dp[j + 1]`.
    * Update `dp[i]` to keep the **maximum** difference found across all 3 choices.

* **Step 4: Determine the winner.**  
  Once the loop completes, `dp[0]` holds Alice's max relative score lead over Bob at the start of the game.
  * If `dp[0] > 0`, Alice scores more $\rightarrow$ return `"Alice"`.
  * If `dp[0] < 0`, Bob scores more $\rightarrow$ return `"Bob"`.
  * If `dp[0] == 0`, their scores are equal $\rightarrow$ return `"Tie"`.

---

## Time & Space Complexity

* **Time Complexity:** **$O(n)$**  
  We loop through the array of length $n$ once. In each iteration, the inner loop runs at most 3 times. $3 \times n$ operations simplifies to $O(n)$.

* **Space Complexity:** **$O(n)$**  
  We allocate a dynamic programming array `dp` of size $n + 1$.

### Can Space Complexity Be Improved?

**Yes!** Notice that to calculate `dp[i]`, we only ever look at three future values: `dp[i + 1]`, `dp[i + 2]`, and `dp[i + 3]`. We do not need the full array history. 

By using 4 variables instead of an entire array, we can reduce the space complexity to **$O(1)$**.

Here is the key optimization code snippet:

```cpp
class Solution {
public:
    string stoneGameIII(vector<int>& stoneValue) {
        int n = stoneValue.size();
        // dp0 = dp[i], dp1 = dp[i+1], dp2 = dp[i+2], dp3 = dp[i+3]
        int dp1 = 0, dp2 = 0, dp3 = 0;

        for (int i = n - 1; i >= 0; i--) {
            int dp0 = INT_MIN;
            int sum = 0;

            for (int j = i; j < min(n, i + 3); j++) {
                sum += stoneValue[j];
                int nextDp = (j + 1 - i == 1) ? dp1 : (j + 1 - i == 2) ? dp2 : dp3;
                dp0 = max(dp0, sum - nextDp);
            }

            // Shift variables for the next step backward
            dp3 = dp2;
            dp2 = dp1;
            dp1 = dp0;
        }

        if (dp1 > 0) return "Alice";
        if (dp1 < 0) return "Bob";
        return "Tie";
    }
};
```

* **Improved Space Complexity:** **$O(1)$** — Uses constant memory regardless of input size.
* **Theoretical Best Complexity:** **$O(n)$ Time** and **$O(1)$ Space**. Every stone must be looked at at least once to determine optimal strategy, so $O(n)$ time is mandatory. The space-optimized version reaches the absolute theoretical best.

---

## Edge Cases Handled

* **Negative Stone Values:**  
  Stone values can be as low as `-1000`. The code handles negative values seamlessly because it tracks relative difference (`sum - dp[j + 1]`), and initializing `dp[i] = INT_MIN` prevents negative results from being ignored.
* **Fewer Than 3 Stones Remaining:**  
  When index `i` is near the end (e.g., $i = n - 1$), taking 3 stones is impossible. The inner loop condition `j < min(n, i + 3)` guarantees we never go out of bounds.
* **Single Element Array ($n = 1$):**  
  The code gracefully evaluates just 1 option (taking the single stone), sets `dp[0]` to that stone's value, and correctly picks the winner.
* **Exact Draws:**  
  If optimal play leads to identical scores, `dp[0]` evaluates to `0`, correctly returning `"Tie"`.
