![Runtime](https://img.shields.io/badge/Runtime-442%20ms%20(beats%2049.73%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-27.48%20MB%20(beats%2073.18%25)-green?style=for-the-badge)

---

## Problem Explained

You are given a row of stones, where each stone has a positive integer value. You play a game with Bob to get the highest score possible.

In every round, you must split the remaining row of stones into two non-empty groups: a **left group** and a **right group**. Bob then adds up the values of the stones in each group:
* If one group has a larger sum than the other, Bob throws away the larger group. You get points equal to the total sum of the smaller group, and the game continues using only that smaller group.
* If both groups have equal sums, Bob lets you choose which group to throw away. You get points equal to the sum of either group, and the game continues with the group you chose to keep.

The game ends when only 1 stone is left. Your goal is to choose your splits carefully to get the maximum total score by the end of the game.

For example, if the stones are `[6, 2, 3, 4, 5, 5]`:
1. Split into `[6, 2, 3]` (sum 11) and `[4, 5, 5]` (sum 14). Bob throws away the right side. You score 11 points, leaving `[6, 2, 3]`.
2. Split `[6, 2, 3]` into `[6]` (sum 6) and `[2, 3]` (sum 5). Bob throws away the left side. You score 5 points (total 16), leaving `[2, 3]`.
3. Split `[2, 3]` into `[2]` (sum 2) and `[3]` (sum 3). Bob throws away the right side. You score 2 points (total 18), leaving `[2]`.
4. Only 1 stone remains. The game ends with a score of 18.

---

## Intuition

This problem can be solved using **Range Dynamic Programming** (Range DP).

At any point, the game is played on a continuous subarray from index `start` to index `end`. To find the best possible score for this subarray, we test every possible position `i` where we can split the subarray into two pieces: `[start...i]` and `[i+1...end]`.

For each split position:
* Calculate the left sum `l` and right sum `r`.
* If `l < r`, Bob keeps the left side. The max score for this choice is `l` plus the best score we can get from sub-range `[start, i]`.
* If `l > r`, Bob keeps the right side. The max score for this choice is `r` plus the best score we can get from sub-range `[i+1, end]`.
* If `l == r`, we try both choices and pick the one that gives the higher score.

Since many split choices reuse the exact same sub-ranges, we store computed results in a 2D table `dp[start][end]` (**memoization**). This prevents recalculating subproblems multiple times.

---

## Approach

Here is the step-by-step breakdown of how the code runs:

* `vector<vector<int>> dp(stoneValue.size(), vector<int>(stoneValue.size(), -1));`: Creates a 2D grid filled with `-1` to store previously calculated maximum scores for sub-range `[start, end]`.
* `return solve(0, stoneValue.size()-1, stoneValue, dp);`: Triggers the recursive function to solve for the entire array from index `0` to the last index.
* `if(start>=end) return 0;`: Base case. If there is only 1 stone (or zero), no further splits are allowed, so 0 points are added.
* `if(dp[start][end]!=-1) return dp[start][end];`: If the maximum score for sub-range `[start, end]` was already computed, returns that saved answer immediately.
* `int l=0;` and `int r=0;`: Initializes variables to keep track of the sum of stones on the left split (`l`) and right split (`r`).
* `for(int i=start;i<=end;i++){ r+=arr[i]; }`: Loops over the range `[start, end]` to calculate the total sum of all stones in this range, storing it in `r`.
* `int ans=0;`: Initializes `ans` to 0, which will hold the best score found among all possible split choices.
* `for(int i=start ; i<end; i++ ){`: Iterates through every valid split index `i` from `start` up to `end - 1`.
* `l+=arr[i];` and `r-=arr[i];`: Moves element `arr[i]` from the right group to the left group, updating `l` and `r` in constant time.
* `if(l<r) ans=max(ans , l+solve(start, i, arr,dp));`: If the left sum is smaller, Bob keeps the left range `[start, i]`. Calculates `l` plus the recursive score of `[start, i]`, updating `ans` if this score is higher.
* `if(l==r) ans=max(ans, max(l+solve(start, i, arr, dp) , r+solve(i+1, end,arr, dp)));`: If both sums are equal, evaluates both keeping the left range `[start, i]` and keeping the right range `[i+1, end]`, taking the maximum score possible.
* `if(l>r) ans=max(ans, r+solve(i+1,end, arr, dp));`: If the right sum is smaller, Bob keeps the right range `[i+1, end]`. Calculates `r` plus the recursive score of `[i+1, end]`, updating `ans` if this score is higher.
* `return dp[start][end]=ans;`: Saves the maximum score for range `[start, end]` in `dp[start][end]` and returns it.

---

## Dry Run

### Case 1: Base Case with 1 Stone
`stoneValue = [4]`

| start | end | l | r | ans | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | - | - | - | `start >= end` is true. Returns 0 immediately. |

---

### Case 2: Multi-Stone Array
`stoneValue = [6, 2, 3, 4, 5, 5]`

Top-level call: `solve(0, 5, arr, dp)`
Initial state: Total sum `r = 25`, `ans = 0`.

| start | end | i | l | r | ans | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 5 | 0 | 6 | 19 | 6 | `l < r`. Check `6 + solve(0,0) = 6 + 0 = 6`. `ans` becomes 6. |
| 0 | 5 | 1 | 8 | 17 | 10 | `l < r`. `solve(0,1)` evaluates `[6,2]` -> returns 2. Check `8 + 2 = 10`. `ans` becomes 10. |
| 0 | 5 | 2 | 11 | 14 | 18 | `l < r`. `solve(0,2)` evaluates `[6,2,3]` -> returns 7. Check `11 + 7 = 18`. `ans` becomes 18. |
| 0 | 5 | 3 | 15 | 10 | 18 | `l > r`. `solve(4,5)` evaluates `[5,5]` -> returns 5. Check `10 + 5 = 15`. `ans` stays 18. |
| 0 | 5 | 4 | 20 | 5 | 18 | `l > r`. `solve(5,5)` evaluates `[5]` -> returns 0. Check `5 + 0 = 5`. `ans` stays 18. |

Returns `dp[0][5] = 18`.

---

## Time & Space Complexity

### Current Complexity
* **Time Complexity:** **O(N^3)** — There are `O(N^2)` subproblems corresponding to all pairs of `(start, end)`. Inside each subproblem, the `for` loop runs up to `N` times to evaluate split points `i`. Total time is `O(N^3)`.
* **Space Complexity:** **O(N^2)** — Storing answers in the `dp` matrix of size `N x N` uses `O(N^2)` space. The recursion stack uses up to `O(N)` auxiliary space.

---

### Optimization Potential

**Can this be improved? Yes.**

We can optimize the runtime from **O(N^3)** to **O(N^2)**.

#### Why the optimization works:
Notice that as the split index `i` moves from `start` to `end`, `l` increases monotonically and `r` decreases monotonically. 
There is a single mid-point index where `l` transitions from being less than `r` to being greater than `r`.

Instead of iterating through every split index `i` in `O(N)` time:
1. We can precalculate prefix sums so any range sum `sum(start, end)` is calculated in `O(1)` time.
2. We maintain auxiliary arrays `maxLeft[start][end]` and `maxRight[start][end]`, which store the maximum possible score achievable for splits within range `[start, end]`.
3. For a given range, we maintain a split pointer `mid` where `sum(start, mid) <= sum(mid + 1, end)`. As `end` moves right, `mid` only moves right, allowing transitions in **O(1)** amortized time.

#### Improved Code Snippet (Key Changes):

```cpp
// Using prefix sums and range dynamic programming in O(N^2)
int stoneGameV(vector<int>& stoneValue) {
    int n = stoneValue.size();
    vector<int> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + stoneValue[i];

    auto getSum = [&](int l, int r) { return pref[r + 1] - pref[l]; };

    vector<vector<int>> dp(n, vector<int>(n, 0));
    vector<vector<int>> maxL(n, vector<int>(n, 0));
    vector<vector<int>> maxR(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        maxL[i][i] = stoneValue[i];
        maxR[i][i] = stoneValue[i];
    }

    for (int len = 2; len <= n; len++) {
        int mid = 0;
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            while (getSum(i, mid) * 2 < getSum(i, j)) mid++;
            
            if (getSum(i, mid) * 2 == getSum(i, j)) {
                dp[i][j] = max(maxL[i][mid], maxR[mid + 1][j]);
            } else {
                int leftVal = (mid > i) ? maxL[i][mid - 1] : 0;
                int rightVal = (mid < j) ? maxR[mid + 1][j] : 0;
                dp[i][j] = max(leftVal, rightVal);
            }

            maxL[i][j] = max(maxL[i][j - 1], dp[i][j] + getSum(i, j));
            maxR[i][j] = max(maxR[i + 1][j], dp[i][j] + getSum(i, j));
        }
    }
    return dp[0][n - 1];
}
```

#### Final Improved Complexity:
* **Improved Time Complexity:** **O(N^2)** — Each subproblem `(i, j)` is updated in `O(1)` time using precomputed prefix maximum arrays and a monotonic split pointer.
* **Improved Space Complexity:** **O(N^2)** — Requires 2D DP arrays of size `N x N`.
* **Theoretical Best Complexity:** **O(N^2)** — Since there are `O(N^2)` distinct sub-ranges that must be evaluated, `O(N^2)` is the absolute best possible lower bound for this problem.

---

## Edge Cases Handled

* **Single Stone (`stoneValue.length == 1`):** Handled immediately by `if(start>=end) return 0;`. Returns `0` as no moves are possible.
* **Equal Split Sums (`l == r`):** Evaluates both options (`l + solve(...)` and `r + solve(...)`) and picks the path that maximizes overall score.
* **All Identical Elements (e.g., `[7, 7, 7, 7, 7, 7, 7]`):** Correctly handles symmetrical splits and branch choices at every recursion step.
* **Large Numbers:** Stone values can be up to `10^6` with array length up to `500`. Total possible sum is `500 * 10^6 = 5 * 10^8`, which safely fits inside a standard signed 32-bit integer (`int`).
