![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2027.99%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.24%20MB%20(beats%2042.96%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given two separate lists of numbers named `nums1` and `nums2`. Both lists are already sorted from smallest to largest. Your goal is to find the **median** of all numbers combined as if you merged both lists into one big sorted list.

The **median** is the exact middle value of a sorted list:
- If the combined list has an **odd** total number of items, the median is the single number right in the middle.
- If the combined list has an **even** total number of items, the median is the average of the two middle numbers.

### Example
- `nums1 = [1, 3]`
- `nums2 = [2]`

If you combine and sort them, you get `[1, 2, 3]`. The total number of items is 3 (odd). The middle number is `2`, so the median is `2.0`.

- `nums1 = [1, 2]`
- `nums2 = [3, 4]`

If you combine and sort them, you get `[1, 2, 3, 4]`. The total number of items is 4 (even). The two middle numbers are `2` and `3`. Their average is `(2 + 3) / 2 = 2.5`.

---

## Intuition

To merge two sorted lists, you normally compare the smallest remaining numbers from each list and pick the smaller one, step by step. 

Instead of building a brand-new combined list in memory (which wastes space), you can just count steps as you move through both lists. Once your step counter reaches the middle position (or positions), you grab those numbers and compute the median.

---

## Approach

Here is how the provided C++ code works step-by-step:

- **Calculate sizes and target positions:**
  - Get the size of `nums1` as `n`, the size of `nums2` as `m`, and total size `t = n + m`.
  - If total size `t` is odd, set `find2 = t / 2` and `find1 = -1` (you only need one middle number at position `find2`).
  - If total size `t` is even, set `find2 = t / 2` and `find1 = t / 2 - 1` (you need two middle numbers at positions `find1` and `find2`).

- **Walk through both arrays with two pointers:**
  - Track pointer `i` for `nums1`, pointer `j` for `nums2`, and step counter `cnt`.
  - Compare `nums1[i]` and `nums2[j]`. Pick whichever is smaller.
  - If the current step `cnt` equals `find1`, store that number in `val1`.
  - If the current step `cnt` equals `find2`, store that number in `val2`.
  - Increment `cnt` and advance the pointer (`i` or `j`) for the list that provided the smaller number.

- **Process remaining elements:**
  - If one list ends before reaching the middle positions, run clean-up loops for the remaining list (`nums1` or `nums2`), continuing to check for `find1` and `find2`.

- **Calculate and return the median:**
  - If `find1` is `-1` (odd total length), return `val2`.
  - Otherwise (even total length), return `(val1 + val2) / 2.0`.

---

## Time & Space Complexity

- **Time:** O(m + n) — The code steps through elements one by one until it reaches the middle index, taking up to `(m + n) / 2` steps in the worst case.
- **Space:** O(1) — Memory footprint is constant because the code only uses a few simple variables (`i`, `j`, `cnt`, `val1`, `val2`) and does not allocate a new array.

### Can it be improved?

**Yes.** The problem statement requests an overall runtime complexity of O(log (m+n)). The current O(m + n) solution does not meet this requirement.

To optimize it, you can use **Binary Search** on the smaller array. Instead of stepping through elements one by one, binary search splits both arrays into left and right halves simultaneously, searching for the exact partition where every element on the left is less than or equal to every element on the right.

Here is the core binary search logic that optimizes the solution:

```cpp
// Ensure nums1 is the smaller array to minimize binary search range
if (nums1.size() > nums2.size()) 
    return findMedianSortedArrays(nums2, nums1);

int n = nums1.size(), m = nums2.size();
int low = 0, high = n;

while (low <= high) {
    int i = (low + high) / 2;             // Partition index in nums1
    int j = (n + m + 1) / 2 - i;          // Partition index in nums2

    int maxLeft1  = (i == 0) ? INT_MIN : nums1[i - 1];
    int minRight1 = (i == n) ? INT_MAX : nums1[i];

    int maxLeft2  = (j == 0) ? INT_MIN : nums2[j - 1];
    int minRight2 = (j == m) ? INT_MAX : nums2[j];

    if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
        if ((n + m) % 2 == 0)
            return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
        else
            return max(maxLeft1, maxLeft2);
    } else if (maxLeft1 > minRight2) {
        high = i - 1; // Move left in nums1
    } else {
        low = i + 1;  // Move right in nums1
    }
}
```

- **Improved Time Complexity:** O(log(min(m, n))) — Perform binary search on the smaller array.
- **Theoretical Best Complexity:** O(log(min(m, n))) — This binary search approach reaches the theoretical limit for this problem.

---

## Edge Cases Handled

- **One array is empty:** If `nums1` or `nums2` has size 0, the main loop `while (i < n && j < m)` is skipped, and the remaining non-empty array loop handles all steps smoothly.
- **Odd vs. Even combined total length:** The logic explicitly adjusts `find1` and `find2` depending on whether `(m + n)` is even or odd.
- **Duplicate elements:** The comparison `nums1[i] <= nums2[j]` safely handles identical numbers across both arrays without getting stuck.
- **Negative numbers:** Because comparison operations (`<=`) work the same way for negative values, negative values cause no issues.
- **Arrays of vastly different sizes:** Handled correctly because each pointer moves independently based on value comparisons.
