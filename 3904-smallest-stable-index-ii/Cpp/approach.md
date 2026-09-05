![Runtime](https://img.shields.io/badge/Runtime-7%20ms%20(beats%2077.75%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-202.56%20MB%20(beats%2085.02%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have an array of numbers called `nums` and a threshold integer called `k`.

For any index `i` in the array, we calculate its **instability score**:
1. Find the **largest** number from the start of the array up to index `i` (the prefix maximum).
2. Find the **smallest** number from index `i` to the end of the array (the suffix minimum).
3. Subtract the smallest number from the largest number.

An index is called **stable** if its instability score is less than or equal to `k`. 

Your goal is to find the **smallest index** `i` (the first one from left to right) that is stable. If no index is stable, return `-1`.

For example, if `nums = [5, 0, 1, 4]` and `k = 3`:
- At index `3`: The maximum from index `0` to `3` is `5`. The minimum from index `3` to `3` is `4`.
- Instability score = `5 - 4 = 1`.
- Since `1 <= 3`, index `3` is stable, and it is the first index to satisfy the condition. The answer is `3`.

---

## Intuition

To calculate the instability score at index `i`, we need two values:
1. The maximum value from the left up to `i`.
2. The minimum value from `i` to the right.

Finding the maximum from the left as we loop forward is easy — we just keep a running max variable. 

However, finding the minimum to the right requires looking ahead. If we scanned to the end of the array for every index, it would take too long.

The key idea is to do a **pre-pass from right to left** to calculate all suffix minimums in advance and save them in an array called `store`. 

Once `store` is built:
- `store[i]` instantly tells us the minimum value from index `i` to the end of the array.
- We then iterate from left to right, updating a running maximum `maxi`.
- At each index `i`, we compute `maxi - store[i]`. The very first index where this score is less than or equal to `k` is our answer.

---

## Approach

Here is the step-by-step walk-through of the code:

* `int maxi = nums[0];` — Initializes `maxi` with the first element of `nums`. This variable keeps track of the largest number seen so far as we iterate forward.
* `vector<int>store(nums.size());` — Allocates an array `store` of the same length as `nums` to hold the precomputed suffix minimums.
* `int str = nums[nums.size()-1];` — Initializes `str` with the last element of `nums`, which is the minimum value for the last suffix.
* `for(int j=nums.size()-1;j>=0;j--)` — Loops backward from the end of `nums` down to index `0` to populate `store`.
* `str = min(str,nums[j]);` — Updates `str` to be the smallest value seen from index `j` to the end of the array.
* `store[j] = str;` — Stores this suffix minimum into `store[j]`.
* `int ans = INT_MAX;` — Initializes `ans` to a huge number to help with the conditional check.
* `int ind = -1;` — Declares a variable `ind` (unused later, but initialized to -1).
* `for( int i=0 ; i<nums.size() ; i++ )` — Loops forward from index `0` to the end to check each index in order.
* `maxi = max(maxi , nums[i]);` — Updates `maxi` with the current element `nums[i]`.
* `int val = maxi - store[i];` — Calculates the instability score at index `i` by taking `maxi - store[i]`.
* `if( val <= k && val<ans )` — Checks if the instability score is within the limit `k` (and smaller than `ans`, which is true for the first match since `ans` starts at `INT_MAX`).
* `ans = val; return i;` — Saves `val` into `ans` and immediately returns `i`. Because we are iterating from left to right, this index is guaranteed to be the smallest stable index.
* `return -1;` — If the loop finishes without finding any stable index, returns `-1`.

---

## Dry Run

### Case 1: Typical case (`nums = [5, 0, 1, 4]`, `k = 3`)

**Phase 1: Precomputing `store` (backward loop)**
- `j = 3`: `str = min(4, 4) = 4` -> `store[3] = 4`
- `j = 2`: `str = min(4, 1) = 1` -> `store[2] = 1`
- `j = 1`: `str = min(1, 0) = 0` -> `store[1] = 0`
- `j = 0`: `str = min(0, 5) = 0` -> `store[0] = 0`
- Resulting `store` array: `[0, 0, 1, 4]`

**Phase 2: Checking indices (forward loop)**

| `i` | `nums[i]` | `maxi` | `store[i]` | `val = maxi - store[i]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `0` | `5` | `5` | `0` | `5 - 0 = 5` | `5 <= 3` is false. Keep going. |
| `1` | `0` | `5` | `0` | `5 - 0 = 5` | `5 <= 3` is false. Keep going. |
| `2` | `1` | `5` | `1` | `5 - 1 = 4` | `4 <= 3` is false. Keep going. |
| `3` | `4` | `5` | `4` | `5 - 4 = 1` | `1 <= 3` is true! Return `3`. |

**Final Output:** `3`

---

### Case 2: No stable index exists (`nums = [3, 2, 1]`, `k = 1`)

**Phase 1: Precomputing `store` (backward loop)**
- Resulting `store` array: `[1, 1, 1]`

**Phase 2: Checking indices (forward loop)**

| `i` | `nums[i]` | `maxi` | `store[i]` | `val = maxi - store[i]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `3` | `1` | `3 - 1 = 2` | `2 <= 1` is false. Keep going. |
| `1` | `2` | `3` | `1` | `3 - 1 = 2` | `2 <= 1` is false. Keep going. |
| `2` | `1` | `3` | `1` | `3 - 1 = 2` | `2 <= 1` is false. Keep going. |

Loop ends without returning.
**Final Output:** `-1`

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — We make two linear passes over `nums` of length `n`: one backward pass to fill `store`, and one forward pass to check scores.
- **Space Complexity:** **O(n)** — We allocate an extra vector `store` of size `n` to save the suffix minimums.

### Is this optimal?

**Yes, this approach is optimal in both time and space:**
- **Time:** We must look at every element at least once to compute the prefix maximums and suffix minimums, so **O(n)** time is the best possible bound.
- **Space:** We need access to suffix minimums while processing left-to-right. Storing these precomputed values takes **O(n)** space.

*(Note: The `ans` variable and the `val < ans` check in the code are redundant because the function returns immediately on the very first valid index. However, removing them does not change the asymptotic O(n) performance.)*

---

## Edge Cases Handled

- **Single element array (`n = 1`)**: e.g., `nums = [0], k = 0`. The prefix max is `0` and suffix min is `0`. The instability score is `0 - 0 = 0 <= 0`, correctly returning `0`.
- **No stable index exists**: Returns `-1` after trying every index without triggering the return condition.
- **Zero instability bound (`k = 0`)**: Handles cases where the instability score must be exactly zero (i.e., prefix max equals suffix min).
- **Large numbers**: Elements up to `10^9` fit in standard 32-bit signed integers (`int`) without arithmetic overflow during subtraction.
- **Duplicate elements**: Properly updates minimums and maximums when duplicate numbers appear in `nums`.
