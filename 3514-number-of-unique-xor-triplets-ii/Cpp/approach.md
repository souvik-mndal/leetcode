![Runtime](https://img.shields.io/badge/Runtime-1527%20ms%20(beats%2033.39%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-36.3%20MB%20(beats%2064.65%25)-green?style=for-the-badge)

---

## Problem Explained

You are given an array of positive integers called `nums`. A **XOR triplet** is formed by choosing three indices `i`, `j`, and `k` such that `i <= j <= k`, and calculating the bitwise XOR value of `nums[i] XOR nums[j] XOR nums[k]`.

The condition `i <= j <= k` means you can pick any three numbers from `nums`, and you are allowed to pick the exact same index (and same number) more than once.

Your goal is to find how many **unique** XOR results you can get across all possible triplet choices.

**Example:**
If `nums = [1, 3]`:
- Pick `(1, 1, 1)`: `1 XOR 1 XOR 1 = 1`
- Pick `(1, 1, 3)`: `1 XOR 1 XOR 3 = 3`
- Pick `(1, 3, 3)`: `1 XOR 3 XOR 3 = 1`
- Pick `(3, 3, 3)`: `3 XOR 3 XOR 3 = 3`

The unique XOR values are `1` and `3`. So the answer is `2`.

---

## Intuition

A brute-force solution would check all combinations of three indices. With up to 1500 numbers, checking all triplets would take billions of operations and fail the time limit.

Notice two key properties:
1. **Bitwise XOR properties**: Order does not matter, and XORing a number with itself cancels it out (`x XOR x = 0`).
2. **Small value upper bound**: The numbers in `nums` are at most 1500. The maximum possible XOR value of any combination cannot exceed 2047 (which fits in 11 bits, up to 2048 - 1).

Instead of picking indices, we can think in terms of **reachable XOR values step by step**:
- **Step 1**: Find all reachable values after picking **1** number.
- **Step 2**: Find all reachable values after picking **2** numbers (by XORing every reachable value from Step 1 with every unique number in `nums`).
- **Step 3**: Find all reachable values after picking **3** numbers (by XORing every reachable value from Step 2 with every unique number in `nums`).

Since there are at most 2048 possible XOR values at any point, we can track reachable values using a small boolean array of size 2048.

---

## Approach

Here is how the code works step-by-step:

- `const int MAX_XOR = 2048;`: Defines the maximum possible size for our boolean lookup tables. Since the input numbers are at most 1500, any XOR combination will stay below 2048.
- `vector<bool> present(MAX_XOR, false);`: Creates a boolean array `present` to mark which individual numbers exist in `nums`.
- `for (int x : nums) { present[x] = true; }`: Marks every number found in `nums` as `true`. This automatically handles duplicates because we only care whether a number exists or not.
- `vector<bool> dp(MAX_XOR, false); dp[0] = true;`: Initializes our dynamic programming state array `dp`. We set `dp[0] = true` as a starting point because `0 XOR x = x`.
- `for (int step = 0; step < 3; step++)`: Runs a loop exactly 3 times. Each step represents picking one additional number for our triplet.
- `vector<bool> next(MAX_XOR, false);`: Creates a fresh boolean array `next` to collect all reachable XOR values for the current step.
- `for (int cur = 0; cur < MAX_XOR; cur++) { if (!dp[cur]) continue; ...`: Loops over all possible XOR values. If `cur` was reachable in the previous step, we expand from it.
- `for (int v = 0; v < MAX_XOR; v++) { if (present[v]) { next[cur ^ v] = true; } }`: Combines each reachable value `cur` with every number `v` present in `nums` using bitwise XOR (`cur ^ v`), marking the new value as `true` in `next`.
- `dp = move(next);`: Replaces `dp` with `next` so `dp` now holds all reachable XOR values after the current step.
- `int ans = 0; for (bool ok : dp) { if (ok) ans++; } return ans;`: Counts how many entries in `dp` are `true` after 3 steps and returns the total.

---

## Dry Run

### Case 1: Small array with two elements (`nums = [1, 3]`)

Initial setup: `present[1] = true`, `present[3] = true`, all others `false`. `dp[0] = true`.

| Step | `cur` | `v` | Action | `next` values set to `true` |
|---|---|---|---|---|
| Initial | - | - | Start with base state | `dp[0]` |
| `step = 0` | 0 | 1 | `0 ^ 1 = 1` | `next[1] = true` |
| `step = 0` | 0 | 3 | `0 ^ 3 = 3` | `next[3] = true` |
| End `step 0` | - | - | Update `dp` | `dp[1]`, `dp[3]` |
| `step = 1` | 1 | 1 | `1 ^ 1 = 0` | `next[0] = true` |
| `step = 1` | 1 | 3 | `1 ^ 3 = 2` | `next[2] = true` |
| `step = 1` | 3 | 1 | `3 ^ 1 = 2` | `next[2] = true` |
| `step = 1` | 3 | 3 | `3 ^ 3 = 0` | `next[0] = true` |
| End `step 1` | - | - | Update `dp` | `dp[0]`, `dp[2]` |
| `step = 2` | 0 | 1 | `0 ^ 1 = 1` | `next[1] = true` |
| `step = 2` | 0 | 3 | `0 ^ 3 = 3` | `next[3] = true` |
| `step = 2` | 2 | 1 | `2 ^ 1 = 3` | `next[3] = true` |
| `step = 2` | 2 | 3 | `2 ^ 3 = 1` | `next[1] = true` |
| End `step 2` | - | - | Update `dp` | `dp[1]`, `dp[3]` |

Final count: `dp[1]` and `dp[3]` are `true`. `ans = 2`.

---

### Case 2: Array with four elements (`nums = [6, 7, 8, 9]`)

Initial setup: `present` is `true` for 6, 7, 8, 9. `dp[0] = true`.

| Step | `cur` | `v` values checked | Action | `next` values set to `true` |
|---|---|---|---|---|
| `step = 0` | 0 | 6, 7, 8, 9 | `0 ^ v` | `next` has {6, 7, 8, 9} |
| End `step 0` | - | - | Update `dp` | `dp` has {6, 7, 8, 9} |
| `step = 1` | 6 | 6, 7, 8, 9 | `6 ^ 6=0, 6 ^ 7=1, 6 ^ 8=14, 6 ^ 9=15` | `next` gets {0, 1, 14, 15} |
| `step = 1` | 7, 8, 9 | 6, 7, 8, 9 | Same XOR operations | `next` still {0, 1, 14, 15} |
| End `step 1` | - | - | Update `dp` | `dp` has {0, 1, 14, 15} |
| `step = 2` | 0 | 6, 7, 8, 9 | `0 ^ v` | `next` gets {6, 7, 8, 9} |
| `step = 2` | 1, 14, 15 | 6, 7, 8, 9 | XOR combinations | `next` still {6, 7, 8, 9} |
| End `step 2` | - | - | Update `dp` | `dp` has {6, 7, 8, 9} |

Final count: `dp[6]`, `dp[7]`, `dp[8]`, `dp[9]` are `true`. `ans = 4`.

---

## Time & Space Complexity

- **Time Complexity:** O(N + MAX_XOR * MAX_XOR) where `N` is `nums.length` and `MAX_XOR = 2048`.
  - Populating `present` takes `O(N)` time.
  - The DP runs 3 outer iterations. Inside, `cur` loops 2048 times and `v` loops 2048 times.
  - Total inner loop operations = `3 * 2048 * 2048 = 12,582,912`, which runs in milliseconds.
- **Space Complexity:** O(MAX_XOR) memory.
  - Uses fixed-size boolean vectors (`present`, `dp`, `next`) of size 2048, which takes negligible memory.

### Can this be improved?

**Yes.** In the inner loop, `v` iterates from 0 to 2047, even though most of those values are not present in `nums`.

Instead of checking all 2048 values for `v`, we can collect only the unique numbers present in `nums` into a separate list `unique_nums`. Then we only loop `v` over `unique_nums`.

This reduces the inner loop iterations from 2048 down to `U`, where `U` is the count of unique elements in `nums` (`U <= 1500`).

Here is the modified code snippet:

```cpp
// Collect only unique present numbers
vector<int> unique_nums;
for (int v = 0; v < MAX_XOR; v++) {
    if (present[v]) {
        unique_nums.push_back(v);
    }
}

// In each step, iterate only over unique_nums
for (int step = 0; step < 3; step++) {
    vector<bool> next(MAX_XOR, false);
    for (int cur = 0; cur < MAX_XOR; cur++) {
        if (!dp[cur]) continue;
        for (int v : unique_nums) {
            next[cur ^ v] = true;
        }
    }
    dp = move(next);
}
```

- **Improved Time Complexity:** O(N + MAX_XOR * U), where `U` is the number of unique elements in `nums`.
- **Improved Space Complexity:** O(MAX_XOR) space.
- **Theoretical Limit:** O(N + MAX_XOR * U) is optimal for this dynamic programming approach and achieves top performance.

---

## Edge Cases Handled

- **Single element array (`nums = [5]`):** The single value `5` is picked 3 times: `5 XOR 5 XOR 5 = 5`. `present[5] = true`, step 1 reaches 5, step 2 reaches 0, step 3 reaches 5. Answer is 1.
- **Large number of duplicate elements (`nums = [7, 7, 7, 7, 7]`):** Duplicate values are deduplicated by the boolean `present` array, avoiding redundant calculations.
- **Max constraint values (`nums[i] = 1500`):** 1500 in binary is `10111011100` (11 bits). The maximum possible XOR result is less than 2048, so no index out-of-bounds error occurs.
- **Array with all distinct numbers:** Handles up to 1500 unique elements without memory overhead because states are bounded by 2048.
