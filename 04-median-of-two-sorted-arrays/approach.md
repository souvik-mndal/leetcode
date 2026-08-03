![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.14%20MB%20(beats%2062.05%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have two separate lists of numbers, and both lists are already sorted from smallest to largest. Your job is to find the **median** of all those numbers combined. 

The **median** is the exact middle number when you put everything in order:
- If you have an odd total amount of numbers, the median is the single middle number. For example, if you combine lists into [1, 2, 3], the middle number is 2.
- If you have an even total amount of numbers, there is no single middle number. Instead, you take the two numbers right in the middle and find their average (add them together and divide by 2). For example, if you combine lists into [1, 2, 3, 4], the two middle numbers are 2 and 3, and their average is 2.5.

## Intuition

The most straightforward way to solve this is to merge both sorted lists into one big sorted list, just like when you play cards and combine two sorted piles into one. Once they are combined, you can instantly find the middle index or indices to calculate the median.

The **aha** moment in this specific code is that you do not actually need to create a new massive array in memory to merge them. Because both input arrays are already sorted, you can just walk through them side-by-side using two pointers (one for each array), exactly like the merging step in Merge Sort. As you step through, you count how many items you have processed. The moment your counter hits the exact middle index positions you are looking for (`find1` and `find2`), you grab those values and stop caring about storing the rest. This saves you from building a giant combined array.

## Approach

- **Calculate Total Size and Target Indices:** Find the size of both arrays (`n` and `m`) and add them together to get the total length `t`. 
- **Determine What to Find:** Check if `t` is odd or even. 
  - If `t` is odd, set `find2` to `t / 2` (the middle index) and `find1` to `-1` (meaning there is only one middle value needed).
  - If `t` is even, set `find2` to `t / 2` and `find1` to `t / 2 - 1` (the two side-by-side middle indices).
- **Initialize Pointers and Counters:** Set pointers `i` and `j` to 0 to track our position in `nums1` and `nums2`, and set a counter `cnt` to 0 to track how many total elements we have stepped past.
- **Merge and Count Loop:** Run a loop while both pointers are within their respective array bounds (`i < n` and `j < m`):
  - Compare the current element of `nums1[i]` with `nums2[j]`.
  - Take the smaller element. 
  - Check if our current `cnt` matches `find1` or `find2`. If it does, save that number into `val1` or `val2`.
  - Increment the `cnt` and advance the pointer of the array we just took the element from.
- **Clean Up Leftover Elements:** If one array runs out of elements before the other, use separate loops to finish walking through the remaining elements of `nums1` or `nums2`, continuing to check our target index matches (`find1` and `find2`).
- **Calculate and Return:** 
  - If `find1` is `-1`, return `val2` as a float.
  - Otherwise, return the average of `val1` and `val2` as a double.

## Time & Space Complexity

- **Time:** O(m + n) — We walk through both arrays element by element until we reach the middle index. In the worst case, we touch every single element across both arrays.
- **Space:** O(1) — We only use a few integer variables (`i`, `j`, `cnt`, `val1`, `val2`) regardless of how large the arrays get. We do not create any new data structures.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

No, this is **not** the optimal time complexity for this specific problem. The problem statement explicitly asks for an **O(log(m + n))** runtime complexity. Our current solution runs in **O(m + n)** time, which is linear time, not logarithmic time. 

- **How to optimize:** To reach O(log(m + n)) time, you must stop doing a linear merge step and instead use a **Binary Search** approach (cutting the search space in half at each step) directly on the smaller array to partition both arrays into left and right halves.
- **Optimal Time Complexity:** O(log(min(m, n)))
- **Does the improved version reach it?** Yes, a binary search approach on partitions reaches the theoretical best complexity required by the problem. Our current code passes LeetCode because the constraints are very small (`m + n <= 2000`), meaning an O(m + n) solution is still fast enough to beat 100% of submissions on raw execution speed, even though it misses the theoretical ideal time complexity class.

## Edge Cases Handled

- **One Empty Array:** Handled naturally. If `nums1` or `nums2` has a size of 0, the loop for that array is skipped entirely, and the leftover cleanup loop processes the remaining non-empty array correctly.
- **Odd vs Even Total Length:** The code checks `t % 2 == 1` at the beginning and sets up either a single middle target (`find1 = -1`) or a double middle target (`find1` and `find2`), ensuring both types of lengths calculate the correct median format.
- **Negative Numbers:** Handled properly because standard integer comparisons (`<=`) work seamlessly with negative values.
- **Arrays of Size 1:** Correctly processes tiny inputs (e.g., `nums1 = [1]`, `nums2 = [2]`), hitting the target indices immediately within the first few loop steps.
