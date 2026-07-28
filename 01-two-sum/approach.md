![Runtime](https://img.shields.io/badge/Runtime-1%20ms%20(beats%2088.19%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-56.76%20MB%20(beats%2057.28%25)-yellow?style=for-the-badge)

---

## Problem in Plain English

You are given a list of numbers and a target total sum. Your job is to find two numbers in that list that add up to the target, and return their position numbers (indices) in the list.

*   You can assume there is always exactly one correct pair.
*   You cannot use the same number position twice.
*   You can return the two positions in any order.

**Example:**
If the list is `[2, 7, 11, 15]` and the target is `9`:
The numbers at position `0` (value `2`) and position `1` (value `7`) add up to `9`.
So, the answer is `[0, 1]`.

---

## Intuition

The brute-force way to solve this is checking every possible pair, but that is too slow.

Instead, ask a simple math question for every number you look at:
*"If my current number is `x`, what partner number do I need to reach the `target`?"*

The needed partner is simply `target - x`.

As you walk through the list, keep a quick-lookup memory notebook (a **Map** or **Hash Table**). Store every number you have seen so far, along with its position. For each new number, check if its missing partner is already written down in your notebook.

---

## Approach

Here is how the code works step-by-step:

*   **Create a lookup map:** Initialize an empty map (`mp`) to store numbers as keys and their position indices as values.
*   **Loop through the array:** Go through the list of numbers one by one using a standard `for` loop.
*   **Calculate the needed partner:** Subtract the current number (`nums[i]`) from the `target` to find the missing remainder (`rem = target - nums[i]`).
*   **Check the map:** Look to see if `rem` is already saved in the map.
    *   **If found:** You matched the pair! Return an array containing the current index `i` and the stored index `mp.get(rem)`.
    *   **If not found:** Save the current number and its index into the map (`mp.set(nums[i], i)`) so future numbers can find it.
*   **Fallback return:** Return an empty array `[]` if no solution is found (though the problem guarantees one will exist).

---

## Time & Space Complexity

*   **Time:** **O(n)** — We loop through the list of $n$ numbers at most once. Looking up and inserting items in a map takes constant time on average (**O(1)**).
*   **Space:** **O(n)** — In the worst case, we might store up to $n$ numbers in our map before finding the matching pair.

### Is this optimal?

**Yes, this code is already fully optimal.**

*   **Time Optimal:** You must look at each number at least once to know if it forms the sum. Therefore, **O(n)** is the absolute fastest possible time.
*   **Space Trade-off:** You could reduce memory to **O(1)** space by using two loops to compare every pair, but that would make time complexity **O(n²)** (much slower). The **O(n)** time and **O(n)** space balance is the optimal theoretical solution for an unsorted list.

---

## Edge Cases Handled

*   **Duplicate Numbers:** Works for inputs like `nums = [3, 3]`, `target = 6`. Because we check the map *before* adding the second `3`, it safely finds the first `3` without self-matching.
*   **Negative Numbers:** Works smoothly with negative values (e.g., `nums = [-3, 4, 3]`, `target = 0`). The subtraction logic (`target - nums[i]`) works correctly regardless of sign.
*   **Minimum Array Size:** Works with the smallest valid input size of 2 elements (e.g., `nums = [1, 2]`).
*   **Target is Zero or Negative:** Handles targets equal to `0` or negative values without special cases because basic arithmetic holds.
