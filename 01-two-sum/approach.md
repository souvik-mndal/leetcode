![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2057.26%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-13.15%20MB%20(beats%2052.70%25)-yellow?style=for-the-badge)

---

## Problem in Plain English

Given a list of numbers and a target number, find two numbers in the list that add up to the target. 

You need to return the **indices** (the position numbers, starting at 0) of those two numbers, not the actual values themselves.

Rules to keep in mind:
* Every test case has **exactly one** valid answer.
* You cannot use the number at the same position twice.
* You can return the two position numbers in any order.

**Example:**
If the list is `[2, 7, 11, 15]` and the target is `9`, the answer is `[0, 1]` because the number at index 0 (`2`) plus the number at index 1 (`7`) equals `9`.

---

## Intuition

The simple way to solve this is to check every possible pair of numbers. However, checking every pair takes a long time as the list grows.

A faster way is to ask a different question as you look at each number: *"What partner number do I need to reach the target?"*

For example, if the target is `9` and you are looking at `2`, your needed partner is `9 - 2 = 7`.

Instead of scanning the whole list to see if `7` exists, you can keep a quick lookup table (a **hash table** or **dictionary**) of every number you have already walked past. As you check each number:
1. Calculate its needed partner (`target - current_number`).
2. Check if you already saw that partner earlier.
3. If yes, you found your answer!
4. If no, save the current number and its position in your table, then move to the next number.

---

## Approach

Here is step-by-step how the code works:

* **Create a lookup table:** Initialize an empty dictionary named `mp`. This will store numbers we have seen as keys, and their list index positions as values.
* **Loop through the list:** Go through `nums` one item at a time, keeping track of the current index `i`.
* **Calculate the missing piece:** Subtract the current number from the target (`rem = target - nums[i]`). This `rem` is the partner number we need.
* **Check the lookup table:** Look up if `rem` is already stored in `mp`.
* **Return if found:** If `rem` exists in `mp`, return a list containing the current index `i` and the stored partner's index `mp[rem]`.
* **Save for later:** If `rem` is not in `mp`, save the current number and its index to the table (`mp[nums[i]] = i`) so future numbers can match with it.
* **Fallback:** Return an empty list `[]` if no pair is found (though the problem guarantees a solution exists).

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We loop through the list of `n` elements at most once. Looking up a key in a dictionary takes **O(1)** (constant) time on average.
* **Space Complexity:** **O(n)** — In the worst case, we might store up to `n` elements in our dictionary before finding a match.

### Is this optimal?

**Yes, this code is already optimal.** 
* You must look at each number at least once to find a solution, which means any algorithm must take at least **O(n)** time.
* You could reduce space to **O(1)** by checking every pair without extra storage, but that would slow down the time complexity to **O(n²)**. 
* Therefore, **O(n)** time and **O(n)** space is the theoretical best balance for an unsorted list, and no further asymptotic speed improvement is possible.

---

## Edge Cases Handled

* **Duplicate numbers:** Handles inputs like `nums = [3, 3]` and `target = 6`. The code checks if the needed partner is already in `mp` *before* inserting the current number. This avoids overwriting the index of the first `3`.
* **Negative numbers:** Works with negative values in `nums` or `target` (such as `nums = [-1, -4]`, `target = -5`). Basic subtraction (`target - nums[i]`) handles negative signs correctly.
* **Smallest array size:** Works on the minimum allowed list length of 2 elements (e.g., `nums = [3, 2]`, `target = 5`).
* **Answer order:** Returns the current index first and the previously seen index second (`[i, mp[rem]]`). The problem allows returning indices in any order, so this works without extra sorting.
