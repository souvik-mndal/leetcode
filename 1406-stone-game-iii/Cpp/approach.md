![Runtime](https://img.shields.io/badge/Runtime-35%20ms%20(beats%2039.76%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-136.1%20MB%20(beats%2068.21%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine two players, Alice and Bob, playing a game with a row of stone piles. Each pile has a certain point value. Some values can even be negative, representing a penalty. 

Alice goes first. On your turn, you are allowed to take either 1, 2, or 3 piles from the very front of the remaining row. You add the values of the piles you take to your total score. Then, the other player takes a turn. The game keeps going until all the piles are gone. 

Both players want to win, and both play as smartly as possible to maximize their own score. At the end, whoever has the higher total score wins. If both scores are equal, it is a tie. 

We need to look at the starting array of stone values and predict who will win: return `"Alice"`, `"Bob"`, or `"Tie"`.

---

## Intuition

This is a classic **zero-sum game** and **minimax** problem. In a zero-sum game, whatever one player gains, the other player loses out on. Because both players are playing optimally, they aren't just trying to make their own current score as big as possible. They are thinking ahead. 

The "aha" moment is changing how we evaluate a move. Instead of tracking two separate scores (Alice's total minus Bob's total), we track a single value from the current player's perspective: **my maximum score minus the opponent's maximum score from that point onward.**

We work **backwards** from the end of the array (dynamic programming). Imagine we are standing at index `i` looking at the remaining stones. We can choose to take 1, 2, or 3 piles. If we take a subset of piles, we get their `sum`. What is left over for the opponent starting at the next index (`j + 1`) is their best possible outcome (`dp[j + 1]`). 

Therefore, our net gain for choosing that move is `sum - dp[j + 1]`. To play optimally, we want to maximize this difference. By building this backwards to index `0`, `dp[0]` will tell us the final net advantage Alice has over Bob. If `dp[0] > 0`, Alice wins. If `dp[0] < 0`, Bob wins. If it is `0`, it is a tie.

---

## Approach

Here is how the code implements this step-by-step:

* `int n = stoneValue.size();`: We find the total number of stone piles so we know how long our array is and where our loop boundaries lie.
* `vector<int> dp(n + 1, 0);`: We create a dynamic programming array of size `n + 1`, initialized to all zeros. `dp[i]` will store the maximum score difference a player can achieve starting from index `i` to the end of the array. The extra slot at index `n` represents having zero stones left, which naturally evaluates to `0`.
* `for (int i = n - 1; i >= 0; i--) {`: We loop backwards from the very last stone pile (`n - 1`) all the way back to the first pile (`0`). Working backwards is crucial because to know the best outcome at step `i`, we already need to know the best outcomes for the future steps ahead of it.
* `dp[i] = INT_MIN;`: For the current starting position `i`, we temporarily set our best outcome to the smallest possible integer so that any valid calculation will successfully overwrite it.
* `int sum = 0;`: We initialize a running sum to track the total value of the 1, 2, or 3 piles we choose to take on this turn.
* `for (int j = i; j < min(n, i + 3); j++) {`: We loop through up to 3 piles ahead (from `i` up to `i + 2`, stopping if we hit the end of the array `n`).
* `sum += stoneValue[j];`: We add the value of the current pile `stoneValue[j]` to our running turn-sum.
* `dp[i] = max(dp[i], sum - dp[j + 1]);`: We evaluate taking these piles. Our score increases by `sum`, but whatever is left starting at `j + 1` belongs to the opponent (`dp[j + 1]`). We want to maximize our net advantage, so we keep the highest value between our current best and this new choice.
* `if (dp[0] > 0) return "Alice";`: Once the loop finishes, `dp[0]` holds Alice's total optimal net advantage for the whole game. If it is positive, Alice won more points than Bob.
* `if (dp[0] < 0) return "Bob";`: If `dp[0]` is negative, Bob managed a higher score.
* `return "Tie";`: If `dp[0]` is exactly zero, both players ended with identical scores.

---

## Dry Run

### Case 1: Typical case (`stoneValue = [1, 2, 3, 7]`)

| `i` (loop index) | `j` (inner loop) | `sum` (running total) | `j + 1` | `dp[j + 1]` | `sum - dp[j + 1]` | `dp[i]` (updated max) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 3 | 3 | 7 | 4 | 0 | 7 - 0 = 7 | 7 |
| 2 | 2 | 3 | 3 | 7 | 3 - 7 = -4 | -4 |
| 2 | 3 | 3 + 7 = 10 | 4 | 0 | 10 - 0 = 10 | max(-4, 10) = 10 |
| 1 | 1 | 2 | 2 | 10 | 2 - 10 = -8 | -8 |
| 1 | 2 | 2 + 3 = 5 | 3 | 7 | 5 - 7 = -2 | max(-8, -2) = -2 |
| 1 | 3 | 2 + 3 + 7 = 12 | 4 | 0 | 12 - 0 = 12 | max(-2, 12) = 12 |
| 0 | 0 | 1 | 1 | 12 | 1 - 12 = -11 | -11 |
| 0 | 1 | 1 + 2 = 3 | 2 | 10 | 3 - 10 = -7 | max(-11, -7) = -7 |
| 0 | 2 | 1 + 2 + 3 = 6 | 3 | 7 | 6 - 7 = -1 | max(-7, -1) = -1 |

Final result: `dp[0]` is `-1`, which is less than 0. The function returns `"Bob"`.

### Case 2: Edge case with negative numbers (`stoneValue = [1, 2, 3, -9]`)

| `i` (loop index) | `j` (inner loop) | `sum` (running total) | `j + 1` | `dp[j + 1]` | `sum - dp[j + 1]` | `dp[i]` (updated max) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 3 | 3 | -9 | 4 | 0 | -9 - 0 = -9 | -9 |
| 2 | 2 | 3 | 3 | -9 | 3 - (-9) = 12 | 12 |
| 2 | 3 | 3 + (-9) = -6 | 4 | 0 | -6 - 0 = -6 | max(12, -6) = 12 |
| 1 | 1 | 2 | 2 | 12 | 2 - 12 = -10 | -10 |
| 1 | 2 | 2 + 3 = 5 | 3 | -9 | 5 - (-9) = 14 | max(-10, 14) = 14 |
| 1 | 3 | 2 + 3 + (-9) = -4 | 4 | 0 | -4 - 0 = -4 | max(14, -4) = 14 |
| 0 | 0 | 1 | 1 | 14 | 1 - 14 = -13 | -13 |
| 0 | 1 | 1 + 2 = 3 | 2 | 12 | 3 - 12 = -9 | max(-13, -9) = -9 |
| 0 | 2 | 1 + 2 + 3 = 6 | 3 | -9 | 6 - (-9) = 15 | max(-9, 15) = 15 |

Final result: `dp[0]` is `15`, which is greater than 0. The function returns `"Alice"`.

---

## Time & Space Complexity

* **Time:** O(n) — The outer loop runs `n` times (once for each element in `stoneValue`). Inside that loop, the inner loop always runs at most 3 times (`min(n, i + 3)`). Because 3 is a constant, the total number of operations scales linearly with the size of the array.
* **Space:** O(n) — We allocate a vector `dp` of size `n + 1` to store our dynamic programming results.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

This code is already optimal in terms of time complexity (O(n)) because we must examine every element in the array at least once to determine its value. 

However, the space complexity can be improved from O(n) down to O(1) constant space. Notice that when computing `dp[i]`, we only ever look ahead at the next 3 values (`dp[i + 1]`, `dp[i + 2]`, and `dp[i + 3]`). We never need to access elements further down the array once they are past our 3-step window. 

Instead of storing an entire `dp` vector of size `n + 1`, we could keep track of just the last 4 values using a few individual variables or a small fixed-size circular array. 

Here is how you would adjust the state tracking to optimize space:

```cpp
// Instead of a vector of size n + 1, keep a rolling window of the next 3 values
int dp_next1 = 0; // represents dp[i + 1]
int dp_next2 = 0; // represents dp[i + 2]
int dp_next3 = 0; // represents dp[i + 3]

for (int i = n - 1; i >= 0; i--) {
    int current_dp = INT_MIN;
    int sum = 0;
    // Inside the loop, reference dp_next1, dp_next2, dp_next3 based on index offset (j)
    // Then shift your variables down before moving to the next iteration (i--)
}
```

* **Resulting improved complexity after change:** Time stays O(n), but Space drops to O(1).
* **Theoretical best possible complexity:** O(n) time and O(1) space. The O(1) space version reaches the absolute theoretical limit for this problem. That said, O(n) space is extremely lightweight and easily passes all constraints on platforms like LeetCode.

---

## Edge Cases Handled

* **Single element array (size 1):** The loop runs once for `i = 0`, the inner loop looks at just index 0, and it correctly returns the winner based on whether that single value is positive, negative, or zero.
* **Array size 2 or 3:** The `min(n, i + 3)` check safely handles arrays smaller than 3 elements without causing out-of-bounds errors.
* **All negative numbers:** Handled correctly because `INT_MIN` is used as a starting baseline, and negative sums paired with future negative game states are accurately computed through subtraction.
* **Large arrays (up to 5 * 10^4 elements):** The linear O(n) time complexity ensures the solution runs well within standard time limits without timing out.
