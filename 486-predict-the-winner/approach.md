![Runtime](https://img.shields.io/badge/Runtime-141%20ms%20(beats%208.37%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.58%20MB%20(beats%2085.12%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Two players play a game with an array of numbers. Player 1 goes first. Both players start with a score of 0.

On each turn, a player can pick **either the first or the last** number remaining in the array. That number is removed from the array and added to that player's total score. The game continues until no numbers are left.

Both players play **optimally** — meaning Player 1 wants to maximize their own score, and Player 2 wants to maximize their own score (which minimizes Player 1's score).

Return `true` if Player 1 can score **greater than or equal to** Player 2's score. Return `false` otherwise.

**Example:**
* Input: `nums = [1, 5, 2]`
* Player 1 takes `1` or `2`.
  * If Player 1 takes `2`, remaining array is `[1, 5]`. Player 2 takes `5`. Player 1 gets `1`. Player 1 score = `3`, Player 2 score = `5`. Player 1 loses.
  * If Player 1 takes `1`, remaining array is `[5, 2]`. Player 2 takes `5`. Player 1 gets `2`. Player 1 score = `3`, Player 2 score = `5`. Player 1 loses.
* Output: `false`

---

## Intuition

This problem is about exploring choices. Because both players play perfectly, Player 1 must assume Player 2 will make the best possible choice for themselves on Player 2's turn.

Think of it as a decision tree:
* **Player 1's turn:** Player 1 wants the **maximum** total score. They try taking the left number and the right number, then pick whichever path leads to a higher score.
* **Player 2's turn:** Player 2 wants to win, which means Player 2 will leave Player 1 with the **minimum** possible score from the remaining options.

By simulating all possible moves recursively — maximizing on Player 1's turn and minimizing on Player 2's turn — we find the maximum score Player 1 is guaranteed to get. If that score is at least half of the total points available, Player 1 wins.

---

## Approach

Here is step-by-step how the code works:

* **Step 1: Calculate total points.** Sum all numbers in `nums` to know the total score available in the game.
* **Step 2: Start the game.** Call `getPlayer1Max` starting with the whole array range (`0` to `nums.size() - 1`), Player 1 score at `0`, and setting `turn = true` (Player 1's turn).
* **Step 3: Base case check.** Inside the helper function, if `start > end` (no numbers left to take), return Player 1's accumulated score.
* **Step 4: Handle Player 1's turn (`turn == true`).**
  * Try taking `nums[start]` and recursively call the function for Player 2's turn (`turn = false`).
  * Try taking `nums[end]` and recursively call the function for Player 2's turn (`turn = false`).
  * Return the **`max`** of these two results.
* **Step 5: Handle Player 2's turn (`turn == false`).**
  * Player 2 picks either `start` or `end`. Player 1 gets `0` points on this turn.
  * Recursively evaluate both paths with `turn = true`.
  * Return the **`min`** of these two results, because Player 2 will pick the choice that limits Player 1 the most.
* **Step 6: Check the winning condition.** Compare Player 1's guaranteed score against half the total sum (adjusting slightly for odd vs. even total sums). Return `true` if Player 1 reaches the required target.

---

## Time & Space Complexity

* **Time Complexity:** **$O(2^n)$** — At each step in the array, the function splits into 2 recursive calls without saving work. For an array of length $n$, the decision tree has a depth of $n$, leading to $2^n$ operations.
* **Space Complexity:** **$O(n)$** — The memory space used by the call stack ( recursion stack ) goes up to $n$ levels deep.

### Can this be improved?

**Yes, significantly.** 

The current code re-calculates the exact same subarray ranges over and over. We can store the results of already computed subproblems (**memoization** or **Dynamic Programming**). 

Instead of tracking player scores separately, we can compute the relative score difference (`Player 1 score - Player 2 score`):

```cpp
class Solution {
    int memo[21][21] = {0};
public:
    bool predictTheWinner(vector<int>& nums) {
        return getScoreDiff(nums, 0, nums.size() - 1) >= 0;
    }
    
    int getScoreDiff(vector<int>& nums, int i, int j) {
        if (i == j) return nums[i];
        if (memo[i][j] != 0) return memo[i][j];
        
        int takeLeft = nums[i] - getScoreDiff(nums, i + 1, j);
        int takeRight = nums[j] - getScoreDiff(nums, i, j - 1);
        
        return memo[i][j] = max(takeLeft, takeRight);
    }
};
```

* **Improved Time Complexity:** **$O(n^2)$** — Every subarray range `(i, j)` is solved only once.
* **Improved Space Complexity:** **$O(n^2)$** for the memo table (can be optimized further to **$O(n)$** space using a 1D DP table).
* **Theoretical Best Complexity:** **$O(n^2)$** time and **$O(n)$** space. The dynamic programming approach reaches this theoretical optimal bound.

---

## Edge Cases Handled

* **Single-element array (`nums.size() == 1`):** Automatically works. Player 1 takes the only number and wins immediately.
* **Two-element array (`nums.size() == 2`):** Player 1 picks the larger number and always wins or ties.
* **Ties (Equal total score):** The condition `playerOneMax >= target` uses `>=`. If scores are equal, Player 1 is correctly declared the winner per problem rules.
* **Odd vs. Even sum totals:** The code explicitly checks `sum % 2` to calculate the winning threshold correctly regardless of whether the sum is odd or even.
* **Large value bounds ($nums[i] \le 10^7$):** Since array size $n \le 20$, the maximum possible sum is around $2 \times 10^8$. This comfortably fits within standard 32-bit integer limits without integer overflow.
