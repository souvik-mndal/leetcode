![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-12.42%20MB%20(beats%2025.32%25)-orange?style=for-the-badge)

---

## Problem Explained

The **Subsets** problem asks you to take an array of unique integers and find every possible group (or combination) of numbers you can make from it. This collection of all combinations is called the **power set**.

For example, if the input is `nums = [1, 2, 3]`, you can make:
* The **empty set**: `[]` (choosing no numbers)
* **1-element sets**: `[1]`, `[2]`, `[3]`
* **2-element sets**: `[1, 2]`, `[1, 3]`, `[2, 3]`
* **3-element set**: `[1, 2, 3]` (choosing all numbers)

Your output must contain all of these without repeating any subset. The order of the subsets in the output does not matter.

---

## Intuition

To build all subsets, think of each number in the array as a light switch. For every number, you have **two choices**: either turn it ON (**include it**) in your current subset or leave it OFF (**exclude it**). Because each of the $N$ numbers has 2 choices, there will always be $2^N$ total subsets.

> **Note on the provided code:** The C++ code snippet attached in your prompt actually implements the logic for **Merge Sorted Array** (LeetCode 88), which merges two sorted arrays together, rather than generating subsets. 
> 
> Below, we walk through what your pasted code does line-by-line, and then show the correct, optimal **Backtracking** solution designed specifically for the **Subsets** problem.

---

## Approach

Here is how the provided code operates step-by-step:

* **Set up two pointers:** Place pointer `l` at the end of the valid data in `nums1` (index `m - 1`) and pointer `r` at the start of `nums2` (index `0`).
* **Swap out-of-order elements:** Loop while `l` is valid (`>= 0`) and `r` is within bounds (`< n`). Compare `nums1[l]` with `nums2[r]`. If `nums1[l]` is bigger, swap them so the smaller value goes to `nums1`. Move `l` left and `r` right. If `nums1[l]` is smaller or equal, break the loop early because elements are already placed correctly.
* **Copy remaining values:** Fill the back part of `nums1` (from index `m` to `m + n - 1`) with the elements from `nums2`.
* **Sort the combined array:** Call `sort()` on all of `nums1` to put the swapped and copied elements into final sorted order.

---

## Time & Space Complexity

### For the Provided Code:
* **Time:** **O((m + n) log(m + n))** — The swap loop takes at most $O(\min(m, n))$ steps, but calling `sort()` on the full length $m + n$ dominates the runtime.
* **Space:** **O(1)** auxiliary space — Modifications are done directly inside `nums1` without allocating extra array memory.

### Can it be improved?
Yes! The provided code is for merging arrays and uses a sorting step that makes it slower than necessary. More importantly, to solve the actual **Subsets** problem, we should use **Backtracking** (a technique where we build a set step-by-step and undo our choice to try other paths).

Here is the clean, optimal C++ solution for **Subsets**:

```cpp
class Solution {
public:
    void backtrack(int start, vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        // Add the current subset combination to our result
        result.push_back(current);
        
        for (int i = start; i < nums.size(); i++) {
            current.push_back(nums[i]);          // Pick nums[i]
            backtrack(i + 1, nums, current, result); // Move to next elements
            current.pop_back();                 // Backtrack (remove nums[i])
        }
    }

    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, nums, current, result);
        return result;
    }
};
```

### Resulting Complexity for Subsets:
* **Improved Time:** **O(N · 2^N)** — There are $2^N$ total subsets, and copying each subset into the result array takes up to $O(N)$ operations.
* **Improved Space:** **O(N · 2^N)** — Required to store all $2^N$ subsets in the output list. The recursion stack uses **O(N)** extra space.
* **Theoretical Best:** **O(N · 2^N)** is the theoretical best possible complexity because any solution must generate and output all $2^N$ subsets of size up to $N$. The backtracking solution achieves this optimal limit.

---

## Edge Cases Handled

* **Empty second array (`n = 0`) in provided code:** The `while` loop condition `r < n` fails immediately. No elements are moved, and it sorts `nums1` safely.
* **Single-element array (`nums = [0]`) for Subsets:** Correctly outputs `[[], [0]]` by exploring the empty set first and then the set containing `0`.
* **Negative numbers:** Handled seamlessly because subset creation depends entirely on index positions, not integer values.
* **Array size limits ($1 \le N \le 10$):** Since $N$ is small, $2^{10} = 1024$ total subsets are generated, comfortably running well within time and memory constraints.
