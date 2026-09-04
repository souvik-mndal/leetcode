![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2025.17%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-30.14%20MB%20(beats%2094.42%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

We are given an array of numbers called `nums` and a non-negative integer target called `k`.

For every position (index `i`) in the array, we split the array conceptually into two parts:
- **Left part:** all numbers from index `0` up to index `i`.
- **Right part:** all numbers from index `i` to the last index `n - 1`.

Notice that index `i` belongs to both parts.

For each index `i`, we compute its **instability score**:
`instability score = (largest number in left part) - (smallest number in right part)`

An index is called **stable** if its instability score is less than or equal to `k`. Our task is to find and return the **smallest stable index**. If no index is stable, we return `-1`.

---

## Intuition

The goal is to find the very first index `i` whose instability score does not exceed `k`.

To check index `i`:
- We need the maximum value from `nums[0]` through `nums[i]`. As we move left to right, we can update this running maximum in step-by-step fashion.
- We need the minimum value from `nums[i]` through `nums[n-1]`.
- Subtracting the minimum from the maximum gives the instability score.

Since we want the **smallest** stable index, we check indices in order starting from `0`. The first index that satisfies `score <= k` is immediately our answer.

---

## Approach

Here is how the code works step-by-step:

* `void minimum( vector<int>&arr , int &mini , int j)` — Declares a helper function that scans `arr` starting from index `j` to the end of the array. It updates `mini` to hold the smallest element found in `arr[j..n-1]`.
* `int maxi = nums[0];` — Sets `maxi` to the first element `nums[0]`. This variable tracks the running maximum for the left side `nums[0..i]`.
* `int mini = nums[0];` — Initializes `mini` with `nums[0]`. This will hold the minimum element on the right side `nums[i..n-1]`.
* `int ans = INT_MAX;` — Initializes `ans` to a very large number. It stores the smallest valid instability score seen so far.
* `int ind = -1;` — Initializes `ind` to `-1` to represent the default result if no stable index is found.
* `for( int i=0 ; i<nums.size() ; i++ )` — Loops through every index `i` from `0` to `nums.size() - 1`.
* `maxi = max(maxi , nums[i]);` — Updates `maxi` so it holds the maximum value among elements from index `0` to `i`.
* `mini = nums[i];` — Resets `mini` to `nums[i]` before scanning the right portion.
* `minimum(nums , mini , i);` — Calls the helper function to scan from index `i` to the end of `nums`, updating `mini` to the smallest value in `nums[i..n-1]`.
* `int val = maxi - mini;` — Computes the instability score `val` at index `i` by subtracting `mini` from `maxi`.
* `if( val <= k && val<ans )` — Checks if the score `val` is less than or equal to `k` (and smaller than `ans`).
* `ans = val; return i;` — If the condition holds, updates `ans` and immediately returns `i` as the smallest stable index.
* `return -1;` — Returns `-1` if the loop ends without finding any stable index.

---

## Dry Run

### Case 1: Typical case with a valid stable index
Input: `nums = [5, 0, 1, 4]`, `k = 3`

| `i` | `nums[i]` | `maxi` | `mini` (after `minimum()`) | `val` (`maxi - mini`) | Action / Decision |
|---|---|---|---|---|---|
| 0 | 5 | 5 | 0 | 5 - 0 = 5 | `5 <= 3` is false. Keep searching. |
| 1 | 0 | 5 | 0 | 5 - 0 = 5 | `5 <= 3` is false. Keep searching. |
| 2 | 1 | 5 | 1 | 5 - 1 = 4 | `4 <= 3` is false. Keep searching. |
| 3 | 4 | 5 | 4 | 5 - 4 = 1 | `1 <= 3` is true. **Return index 3**. |

---

### Case 2: No stable index exists
Input: `nums = [3, 2, 1]`, `k = 1`

| `i` | `nums[i]` | `maxi` | `mini` (after `minimum()`) | `val` (`maxi - mini`) | Action / Decision |
|---|---|---|---|---|---|
| 0 | 3 | 3 | 1 | 3 - 1 = 2 | `2 <= 1` is false. Keep searching. |
| 1 | 2 | 3 | 1 | 3 - 1 = 2 | `2 <= 1` is false. Keep searching. |
| 2 | 1 | 3 | 1 | 3 - 1 = 2 | `2 <= 1` is false. Keep searching. |
| - | - | - | - | - | Loop finished. **Return -1**. |

---

## Time & Space Complexity

- **Time Complexity:** O(n^2) — The outer loop runs `n` times. Inside it, the helper function `minimum` scans from index `i` to `n - 1`. Across all iterations, it does around `n * (n + 1) / 2` operations, which simplifies to quadratic time.
- **Space Complexity:** O(1) — The algorithm uses a fixed set of scalar variables (`maxi`, `mini`, `val`, `ans`, `ind`) and creates no extra arrays or dynamic memory.

### Can this be improved?

**Yes!** We can improve the time complexity from **O(n^2)** down to **O(n)**.

#### Why the improvement works:
Instead of re-scanning the right side of the array inside the helper function for every single index `i`, we can precompute the minimum values for all right-hand ranges in a single pass from right to left.

We create an array `suffix_min` of size `n`:
- `suffix_min[n - 1]` is just `nums[n - 1]`.
- For any index `i` from `n - 2` down to `0`, `suffix_min[i]` is `min(nums[i], suffix_min[i + 1])`.

After filling `suffix_min` in **O(n)** time, looking up the minimum of `nums[i..n-1]` takes **O(1)** time during our main left-to-right pass.

#### Optimized Code Snippet:
```cpp
int n = nums.size();
vector<int> suffix_min(n);

// Build suffix minimum array from right to left
suffix_min[n - 1] = nums[n - 1];
for (int i = n - 2; i >= 0; i--) {
    suffix_min[i] = min(nums[i], suffix_min[i + 1]);
}

// Find first stable index from left to right
int maxi = nums[0];
for (int i = 0; i < n; i++) {
    maxi = max(maxi, nums[i]);
    int val = maxi - suffix_min[i];
    if (val <= k) {
        return i;
    }
}
return -1;
```

- `suffix_min[i] = min(nums[i], suffix_min[i + 1]);` — Stores the smallest value from index `i` to `n - 1` without repeating scans.
- `int val = maxi - suffix_min[i];` — Replaces the inner loop helper call with a instant O(1) lookup.

#### Resulting Complexity:
- **Time Complexity:** O(n) — One pass right-to-left to build `suffix_min`, and one pass left-to-right to find the answer.
- **Space Complexity:** O(n) — Requires an extra array of size `n` to hold the suffix minimums.

#### Theoretical Best Complexity:
The theoretical best time complexity is **O(n)** because every element must be inspected at least once to find global minimums/maximums. The optimized prefix/suffix approach reaches this theoretical limit.

---

## Edge Cases Handled

- **Single element array (`n = 1`):** `maxi` and `mini` are both `nums[0]`. `val = nums[0] - nums[0] = 0`. Since `k >= 0`, `val <= k` holds true immediately, correctly returning `0`.
- **All elements identical (e.g., `[2, 2, 2]`, `k = 0`):** `maxi` and `mini` are equal at index `0`, making `val = 0`. Returns `0`.
- **No stable index exists:** Loop completes all iterations without satisfying `val <= k` and returns `-1`.
- **Duplicate minimum or maximum values:** Handled naturally by standard `min` and `max` operations.
