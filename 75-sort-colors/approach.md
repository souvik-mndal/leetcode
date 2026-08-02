![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.90%20MB%20(beats%2082.85%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array named `nums` containing objects of three colors: **red**, **white**, and **blue**. In the code, these colors are represented by the numbers **`0`** (red), **`1`** (white), and **`2`** (blue). 

Your goal is to sort the array **in-place** (meaning you modify the original array directly without creating a new copy) so that all `0`s come first, followed by all `1`s, and finally all `2`s. 

You are **not allowed** to use any built-in sorting functions.

**Example:**
- Input: `[2, 0, 2, 1, 1, 0]`
- Output: `[0, 0, 1, 1, 2, 2]`

---

## Intuition

*(Note: The code snippet provided in your prompt is actually for generating **Subsets** using bit manipulation. Below is the intuition for both understanding that code and solving the actual **Sort Colors** problem optimally.)*

### Sort Colors Core Trick: The Three-Pointer Partition
Because there are only three distinct values (`0`, `1`, `2`), we do not need a standard sorting algorithm. Instead, we can divide the array into three sections using three pointers:
- **`low`**: tracks where the next `0` should go (left region).
- **`mid`**: scans through the array from left to right.
- **`high`**: tracks where the next `2` should go (right region).

As `mid` walks through the elements:
- If it sees a **`0`**, it swaps it to the `low` boundary and moves both `low` and `mid` forward.
- If it sees a **`1`**, it is already in the middle, so it just moves `mid` forward.
- If it sees a **`2`**, it swaps it to the `high` boundary and moves `high` backward. We do *not* move `mid` yet, because the newly swapped-in element at `mid` still needs to be checked!

---

## Approach

Here is what the **provided code snippet** is doing step-by-step:

* **Calculate total combinations:** It computes `total = 1 << nums.size()`. The `<<` operator is a **left bitwise shift** (multiplying 1 by $2^n$), which finds the total number of subsets ($2^n$).
* **Loop through each subset pattern:** It loops an integer `i` from `0` to `total - 1`. Each value of `i` represents a unique subset using its binary bits.
* **Inspect each bit:** Inside a second loop, it checks every index `j` of `nums`.
* **Bitwise AND test:** It uses `num & (1 << j)` to check if the `j`-th bit of `i` is set to `1`.
* **Build individual subset:** If the bit is set, it pushes `nums[j]` into a temporary list called `temp`.
* **Save subset:** After checking all elements for that pattern, it saves `temp` into the final answer list `ans`.

---

## Time & Space Complexity

### Current Code Complexity (Subsets Algorithm)
* **Time:** **$O(n \cdot 2^n)$** — The outer loop runs $2^n$ times. The inner loop checks $n$ bits for each combination.
* **Space:** **$O(n \cdot 2^n)$** — It generates and stores $2^n$ total subsets in memory.

### Can it be improved for Sort Colors?
**Yes.** The provided code generates power-set combinations (subsets) instead of sorting the array in-place. 

To solve **Sort Colors** optimally in one pass with zero extra memory, we replace the subset logic with the **Three-Pointer (Dutch National Flag)** algorithm:

```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int low = 0, mid = 0, high = nums.size() - 1;
        
        while (mid <= high) {
            if (nums[mid] == 0) {
                swap(nums[low++], nums[mid++]);
            } else if (nums[mid] == 1) {
                mid++;
            } else {
                swap(nums[mid], nums[high--]);
            }
        }
    }
};
```

### Improved Complexity (Optimal Sort Colors)
* **Time:** **$O(n)$** — We inspect each element at most once in a single pass.
* **Space:** **$O(1)$** — We reorder elements in-place using only three integer variables (`low`, `mid`, `high`).

**Theoretical Best:** **$O(n)$ Time, $O(1)$ Space.** This optimized 3-pointer version achieves the theoretical maximum performance limit. We must look at every element at least once ($O(n)$ bound), and doing it in constant extra space ($O(1)$) cannot be beaten.

---

## Edge Cases Handled

* **Single-element arrays:** Inputs like `[0]` or `[2]` will finish immediately without pointer errors because `mid <= high` terminates after one check.
* **Arrays missing certain colors:** If the array has only `[0, 0]` or `[1, 2]`, the pointers handle missing zones cleanly without going out of bounds.
* **Already sorted arrays:** An input like `[0, 1, 2]` will simply advance the pointers smoothly without breaking order.
* **Reverse sorted arrays:** Inputs like `[2, 2, 1, 0, 0]` correctly push `2`s to the right boundary and `0`s to the left boundary.
