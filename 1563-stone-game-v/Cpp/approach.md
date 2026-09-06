![Runtime](https://img.shields.io/badge/Runtime-442%20ms%20(beats%2049.33%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-27.5%20MB%20(beats%2069.37%25)-green?style=for-the-badge)

---

## Problem Explained

Alice has a row of stones, where each stone has a point value. 

In every round of the game:
1. Alice splits the remaining row into two non-empty left and right parts.
2. Bob sums up the stone values in both parts.
3. Bob throws away the part with the larger sum. Alice gets points equal to the sum of the smaller part, and the game continues with only that smaller part.
4. If both parts have equal sums, Alice gets to choose which part to keep. She gains points equal to that equal sum and continues playing with her chosen part.

The game ends when only 1 stone remains. Alice starts with 0 points and wants to make split choices that give her the highest total score possible.

For example, if the stones are `[6, 2, 3, 4, 5, 5]`:
* If Alice splits into `[6, 2, 3]` (sum 11) and `[4, 5, 5]` (sum 14), Bob throws away `[4, 5, 5]` because 14 is larger. Alice gains 11 points and continues with `[6, 2, 3]`.

---

## Intuition

This problem can be broken down into sub-problems using a technique called **range dynamic programming**. At any point in the game, the remaining stones form a continuous range from index `start` to index `end`.

Bob's behavior is completely fixed: he always destroys the side with the larger sum. Alice's only control is deciding where to split the range (and which side to keep if both sides have equal sums).

To find the best score for any range `[start, end]`:
* Try every possible split position `i` between `start` and `end - 1`.
* Calculate the left sum `l` and the right sum `r`.
* If `l < r`, Bob keeps the left side. Alice gets `l` points plus the maximum score she can get from the sub-range `[start, i]`.
* If `l > r`, Bob keeps the right side. Alice gets `r` points plus the maximum score she can get from the sub-range `[i + 1, end]`.
* If `l == r`, Alice compares both options and picks the one that leads to a higher total score.

Since many splits will re-examine the same sub-ranges, we store calculated scores in a 2D memory table `dp[start][end]`. This process of saving answers to avoid re-calculation is called **memoization**.

---

## Approach

Here is how the code executes step-by-step:

* `vector<vector<int>> dp(stoneValue.size(), vector<int>(stoneValue.size(), -1));`: Creates a 2D grid initialized to `-1`. It will store the maximum score Alice can earn from any range `[start, end]`.
* `return solve(0, stoneValue.size()-1, stoneValue, dp);`: Triggers the recursive function for the full range of stones, starting at index `0` and ending at index `stoneValue.size() - 1`.
* `if(start>=end) return 0;`: Checks if the range contains 1 stone or fewer. If so, no more splits can be made, so return 0 points.
* `if(dp[start][end]!=-1) return dp[start][end];`: Checks if the maximum score for this range was already calculated. If so, returns the stored result immediately.
* `for(int i=start;i<=end;i++){ r+=arr[i]; }`: Loops through the range to calculate the sum of all elements currently on the right side.
* `for(int i=start ; i<end; i++ )`: Loops through every possible split point `i` in the range. The left row spans `start` to `i`, and the right row spans `i + 1` to `end`.
* `l+=arr[i]; r-=arr[i];`: Moves the stone `arr[i]` from the right sum `r` to the left sum `l`. This updates both section sums without re-calculating them from scratch.
* `if(l<r) ans=max(ans , l+solve(start, i, arr,dp));`: If the left sum is smaller, Bob discards the right side. Alice gains `l` points plus the best score from `solve(start, i, arr, dp)`.
* `if(l==r) ans=max(ans, max(l+solve(start, i, arr, dp) , r+solve(i+1, end,arr, dp)));`: If both sums are equal, Alice tests both keeping the left side and keeping the right side, taking the maximum outcome.
* `if(l>r) ans=max(ans, r+solve(i+1,end, arr, dp));`: If the right sum is smaller, Bob discards the left side. Alice gains `r` points plus the best score from `solve(i+1, end, arr, dp)`.
* `return dp[start][end]=ans;`: Stores the highest score achieved among all split choices into `dp[start][end]` and returns it.

---

## Dry Run

### Case 1: Single Stone (`stoneValue = [4]`)

| `start` | `end` | Action / Condition | Return Value |
| :--- | :--- | :--- | :--- |
| 0 | 0 | `start >= end` (0 >= 0 is true) | Returns `0` immediately |

---

### Case 2: Full Array (`stoneValue = [6, 2, 3, 4, 5, 5]`)

Evaluating the top-level recursive call `solve(0, 5)` where the total sum is 25:

| `i` | `l` | `r` | Decision (`l` vs `r`) | Score Calculation | `ans` Updated |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 6 | 19 | `l < r` (6 < 19) | `6 + solve(0, 0)` = 6 + 0 = 6 | `ans = max(0, 6) = 6` |
| 1 | 8 | 17 | `l < r` (8 < 17) | `8 + solve(0, 1)` = 8 + 2 = 10 | `ans = max(6, 10) = 10` |
| 2 | 11 | 14 | `l < r` (11 < 14) | `11 + solve(0, 2)` = 11 + 7 = 18 | `ans = max(10, 18) = 18` |
| 3 | 15 | 10 | `l > r` (15 > 10) | `10 + solve(4, 5)` = 10 + 5 = 15 | `ans = max(18, 15) = 18` |
| 4 | 20 | 5 | `l > r` (20 > 5) | `5 + solve(5, 5)` = 5 + 0 = 5 | `ans = max(18, 5) = 18` |

Final returned answer for `stoneValue = [6, 2, 3, 4, 5, 5]` is **18**.

---

## Time & Space Complexity

* **Time Complexity:** **O(N^3)**
  There are `O(N^2)` distinct range states defined by `(start, end)`. Inside each recursive call, the code calculates `r` with an `O(N)` loop, and then runs another loop `i` across `O(N)` split points. Thus, each state takes `O(N)` work, leading to `O(N^3)` overall time.
* **Space Complexity:** **O(N^2)**
  The 2D DP table `dp` requires `N * N` entries to store results. The call stack depth takes `O(N)` memory in the worst case.

### Can this be improved?

**Yes.** 

1. **Prefix Sums Optimization (Reduces constant factor):**
   Instead of computing `r` using a loop inside `solve`, we can precompute a **prefix sum** array where `pref[k]` stores the sum of the first `k` elements. Any range sum from index `a` to `b` can then be calculated in constant `O(1)` time as `pref[b + 1] - pref[a]`.

2. **Range Maximum DP Optimization (Reduces complexity to O(N^2)):**
   As index `i` moves right, `l` increases and `r` decreases. There is a pivot point where `l <= r` on the left side and `l > r` on the right side. We can store maximum prefix-based DP values in auxiliary lookup tables. This allows us to query the best split in `O(1)` time instead of looping through all split choices `i`.

Here is the key change to replace the sum loop with a prefix sum lookup:

```cpp
// Precompute prefix sums in stoneGameV:
vector<int> pref(N + 1, 0);
for (int i = 0; i < N; i++) pref[i + 1] = pref[i] + stoneValue[i];

// Inside solve(), range sums are computed instantly in O(1):
int total_sum = pref[end + 1] - pref[start];
int l = 0;
for (int i = start; i < end; i++) {
    l = pref[i + 1] - pref[start];
    int r = total_sum - l;
    // ... rest of decision logic remains same
}
```

* **Line 2-3:** `pref[i + 1] = pref[i] + stoneValue[i]` builds a running total so range sums take `O(1)` operations.
* **Line 6:** `pref[end + 1] - pref[start]` gets the full range sum without running a `for` loop.

* **Improved Complexity with Prefix Sums:** Time stays **O(N^3)** theoretically, but runs roughly 3x faster due to fewer operations per state.
* **Optimal Complexity with Range Max Optimization:** Time becomes **O(N^2)**, and Space remains **O(N^2)**. This reaches the theoretical best performance for this problem.

---

## Edge Cases Handled

* **Single Element Array (`stoneValue = [4]`):**
  The recursion base case `if(start >= end) return 0;` handles this immediately. It returns `0` because no splits are possible with only 1 stone.
* **Equal Partition Sums (`l == r`):**
  When left and right sums match (such as `[7, 7, 7, 7]`), the code branches into both options (`max(l + solve(...), r + solve(...))`) so Alice picks the branch yielding higher future points.
* **Large Input Values:**
  Stone values can be up to 1,000,000, and there can be up to 500 stones. The maximum cumulative score fits safely within standard 32-bit signed integers (up to ~2.1 billion), preventing integer overflow.
