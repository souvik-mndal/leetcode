![Runtime](https://img.shields.io/badge/Runtime-1051%20ms%20(beats%205.00%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-520.86%20MB%20(beats%205.27%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have two separate lists of numbers, and both lists are already sorted in order from smallest to largest. Your goal is to find the **median** of all those numbers combined. 

What is a median? It is the exact middle number if you put everything into one big sorted list. 
* If the total count of numbers is odd, the median is the single middle number.
* If the total count of numbers is even, the median is the average of the two middle numbers.

For example, if list one is [1, 3] and list two is [2], combining and sorting them gives [1, 2, 3]. The middle number is 2, so the answer is 2.0.

There is a catch. The problem requires a very fast solution running in **O(log (m + n)) time**, which means you are not allowed to just merge the two lists together and scan through them. That slow approach would take too much time. You need a smarter trick.

*(Note: The code snippet provided in the prompt actually solves a completely different problem about finding the longest palindromic substring, but these notes explain how to approach and solve the Median of Two Sorted Arrays problem correctly.)*

---

## Intuition

The "aha" moment for this problem comes from a simple realization: **you do not need to look at every single number to find the middle.** 

Instead of merging the lists, imagine cutting both lists somewhere near their halves. Let us call the cut point in the first list index `i`, and the cut point in the second list index `j`. If we choose `i` and `j` such that the total number of elements on the left side of both cuts equals the total number of elements on the right side, we split our world into a "left half" and a "right half".

If we can adjust `i` and `j` until every number on the left is smaller than or equal to every number on the right, we have successfully found the median boundary! 

Because the lists are already sorted, we can use **Binary Search** (a technique where you repeatedly cut your search range in half) to find the correct split point `i` in logarithmic time. This satisfies the strict speed requirement.

---

## Approach

Here is how the optimal binary search approach works step-by-step:

* **Identify the smaller array:** Always run your binary search on the smaller of the two arrays (let us call its length `m`) to keep the search space as small as possible.
* **Set up the search range:** Set your binary search pointers `low` to 0 and `high` to `m` for the smaller array.
* **Find the partition point:** Inside a loop, calculate the middle partition point `i` for the first array. 
* **Balance the halves:** Calculate the corresponding partition point `j` for the second array so that the combined left side has the exact same number of elements (or one more) as the combined right side.
* **Check boundary values:** Find the maximum value on the left of both partitions (`maxLeft1`, `maxLeft2`) and the minimum value on the right of both partitions (`minRight1`, `minRight2`).
* **Evaluate the cut:** 
  * If `maxLeft1 <= minRight2` and `maxLeft2 <= minRight1`, your cuts are in the exact right place. Calculate the median using these four boundary values and return it.
  * If `maxLeft1 > minRight2`, it means you took too many elements from the first array. Move your binary search range to the left (`high = i - 1`).
  * Otherwise, you took too few elements from the first array. Move your binary search range to the right (`low = i + 1`).

---

## Time & Space Complexity

For the correct binary search solution:

* **Time:** **O(log(min(m, n)))** — Because we perform a binary search exclusively on the smaller array of size `m` (or `n`), cutting the search space in half at each step.
* **Space:** **O(1)** — Because we only store a few pointer variables and do not create any new arrays or data structures.

**Is this the most optimal possible complexity?**
Yes, this is already the theoretical best possible complexity for this problem. You cannot get faster than logarithmic time when searching through sorted structures, and using constant extra space means you are using zero extra memory.

---

## Edge Cases Handled

A robust solution for this problem must handle several tricky boundaries correctly:

* **Empty arrays:** One of the input arrays can have a length of 0 (`m = 0` or `n = 0`). The binary search logic handles this by ensuring partition calculations evaluate safely without crashing.
* **All elements in one array are smaller than the other:** The cuts can shift all the way to the far left or far right of an array (partition indices can equal 0 or the full array length).
* **Even vs. Odd total lengths:** The code correctly accounts for whether the combined total of elements is even (requiring an average of two numbers) or odd (taking a single middle number).
* **Negative numbers:** The constraints allow numbers down to negative one million. Since the logic relies purely on relative comparisons (`<=` and `>=`), negative numbers do not break the partitioning rules.
