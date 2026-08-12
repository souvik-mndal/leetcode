![Runtime](https://img.shields.io/badge/Runtime-67%20ms%20(beats%2046.74%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-149.26%20MB%20(beats%2084.27%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums` and a limit number called `k`. 

Your goal is to find the length of the longest contiguous section (a **subarray**) where no single number appears more than `k` times. A subarray with no number repeated more than `k` times is called **good**.

For example:
* If `nums = [1, 2, 3, 1, 2, 3, 1, 2]` and `k = 2`:
  * The slice `[1, 2, 3, 1, 2, 3]` contains `1` twice, `2` twice, and `3` twice. Every number appears at most `2` times, so this is valid.
  * Its length is `6`. If we added one more element to make it `[1, 2, 3, 1, 2, 3, 1]`, `1` would appear 3 times, breaking the limit of `k = 2`. Thus, `6` is the maximum length.

## Intuition

Think of this like expanding and shrinking a flexible frame (a **sliding window**) over the array.

We start with an empty frame at the beginning of the array. We expand the right edge of the frame one step at a time to include new numbers. As we add numbers, we keep track of how many times each number appears in our frame using a count table (a **hash map**).

If adding a new number causes its count to exceed `k`, our current frame is no longer valid. To fix it, we shrink the frame from the left side, removing numbers one by one until the over-counted number is back down to `k` occurrences. 

Whenever the frame is valid, we check its size and record the largest size seen so far.

## Approach

Here is step-by-step how the algorithm runs:

* `unordered_map<int, int> m;` — Create a hash table named `m` to store the count of each number inside our current sliding window.
* `int i = 0, j = 0, n = nums.size();` — Initialize the left pointer `i` at index 0, the right pointer `j` at index 0, and store the total array size in `n`.
* `int res = 0;` — Initialize `res` to 0 to store the maximum length of a valid subarray found.
* `while (j < n)` — Loop through the array by moving the right pointer `j` from start to end.
* `m[nums[j]]++;` — Include the current number `nums[j]` in the window by increasing its frequency count in `m`.
* `while (m[nums[j]] > k)` — Check if adding `nums[j]` caused its count to exceed the limit `k`. If so, enter a loop to shrink the window from the left until the count is valid again.
* `m[nums[i]]--; i++;` — Inside the shrink loop, decrease the count of the leftmost element `nums[i]` in `m`, then move the left pointer `i` one step to the right.
* `res = max(res, j - i + 1);` — Calculate the current valid window size using `j - i + 1` and update `res` if this window is larger than any previous one.
* `j++;` — Advance the right pointer to examine the next element in the array.
* `return res;` — Return the maximum subarray length found after checking the entire array.

## Dry Run

### Case 1: Alternating values with k = 1
Input: `nums = [1, 2, 1, 2, 1, 2, 1, 2]`, `k = 1`

| `j` | `nums[j]` | `m[nums[j]]` | `i` | `res` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 1 | 1 | 0 | 1 | Added 1. Frequency of 1 is 1 (valid). Window length is 1. |
| 1 | 2 | 1 | 0 | 2 | Added 2. Frequency of 2 is 1 (valid). Window length is 2. |
| 2 | 1 | 2 | 1 | 2 | Added 1. Frequency of 1 becomes 2 (invalid). Shrink: remove `nums[0]` (1), increment `i` to 1. Frequency of 1 drops back to 1. |
| 3 | 2 | 2 | 2 | 2 | Added 2. Frequency of 2 becomes 2 (invalid). Shrink: remove `nums[1]` (2), increment `i` to 2. Frequency of 2 drops back to 1. |
| 4-7 | ... | ... | ... | 2 | Pattern repeats. Maximum valid length remains 2. |

### Case 2: All identical values with k = 4
Input: `nums = [5, 5, 5, 5, 5, 5, 5]`, `k = 4`

| `j` | `nums[j]` | `m[nums[j]]` | `i` | `res` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 5 | 1 | 0 | 1 | Added 5. Count is 1 (valid). |
| 1 | 5 | 2 | 0 | 2 | Added 5. Count is 2 (valid). |
| 2 | 5 | 3 | 0 | 3 | Added 5. Count is 3 (valid). |
| 3 | 5 | 4 | 0 | 4 | Added 5. Count is 4 (valid). |
| 4 | 5 | 5 -> 4 | 1 | 4 | Added 5. Count becomes 5 (>4). Shrink: decrement count, move `i` to 1. Count returns to 4. |
| 5 | 5 | 5 -> 4 | 2 | 4 | Added 5. Count becomes 5 (>4). Shrink: decrement count, move `i` to 2. Count returns to 4. |
| 6 | 5 | 5 -> 4 | 3 | 4 | Added 5. Count becomes 5 (>4). Shrink: decrement count, move `i` to 3. Count returns to 4. |

## Time & Space Complexity

* **Time Complexity:** **O(N)** — Every element in `nums` is processed at most twice: once when `j` advances to expand the window, and at most once when `i` advances to shrink the window. Hash map lookups and updates take O(1) time on average. Thus, total time scales linearly with the size of the array N.
* **Space Complexity:** **O(N)** — In the worst case, all elements in `nums` are unique. The hash map `m` will store up to N distinct key-value pairs.

### Is this solution optimal?

**Yes, the Big-O complexity of this code is already theoretically optimal.**
* **Time:** You must inspect every element at least once to determine if it can be included, making O(N) time the absolute minimum lower bound.
* **Space:** You must track counts for up to N unique elements, requiring O(N) space in the worst case.

#### Performance Optimization Note for C++:
Although the Big-O complexity cannot be improved, the runtime speed in C++ can be improved. The C++ `std::unordered_map` spends time re-allocating memory as it grows. Pre-allocating map capacity using `m.reserve(n)` or using a custom fixed-size array (if value ranges permit) avoids hash table rehashes and significantly speeds up execution in practice.

```cpp
// Minor practical speed optimization:
unordered_map<int, int> m;
m.reserve(n); // Pre-allocates memory to prevent overhead from rehashing
```

## Edge Cases Handled

* **Large values up to 10^9:** Because values inside `nums` can be up to 1,000,000,000, array indexing (like `count[nums[j]]`) would crash or use gigabytes of memory. Using `unordered_map<int, int>` handles arbitrarily large values safely.
* **k = 1:** Works naturally without special logic. The inner loop forces the window to shrink immediately whenever any repeated number enters the window.
* **k is larger than the array length:** The frequency count never exceeds `k`, the inner loop never runs, and the returned answer is simply the full length of the array `n`.
* **All elements are the same:** The inner loop fires consistently after `k` additions, smoothly maintaining a fixed window length of `k` until the array ends.
