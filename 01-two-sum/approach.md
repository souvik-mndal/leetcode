![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-54.99%20MB%20(beats%2050.11%25)-yellow?style=for-the-badge)

---

## Problem in Plain English

You are given a list of numbers and a **target sum**. Your job is to find **two different numbers** in that list that add up to the target. 

Once you find them, return their **positions** (indices) in the list as a pair, like `[0, 1]`.

*   You cannot use the same element twice (you can't pick position `0` twice).
*   There is always exactly one correct pair in the list.

**Example:**
If the list is `[2, 7, 11, 15]` and the target is `9`:
*   `2 + 7 = 9`
*   `2` is at index `0`, and `7` is at index `1`.
*   Answer: `[0, 1]`

---

## Intuition

The naive way is to compare every number with every other number. That requires two loops and gets slow very fast.

Instead, think of it like this: as you look at each number, ask yourself: **"What missing number do I need to reach the target?"**

*   Missing number = `target - current number`

If you keep a quick-lookup notebook (**hash map**) of the numbers you have already seen and where you saw them, you can check if your missing number is already in the notebook. 

*   If it **is** in the notebook, you found your pair!
*   If it **is not**, write down the current number and its index in your notebook, then move to the next number.

---

## Approach

Here is how the code works step-by-step:

*   **Create a lookup map:** Set up an empty **hash map** (`mp`) to keep track of numbers we have already checked. The key is the number, and the value is its index.
*   **Loop through the array:** Start scanning the list from left to right using a loop index `i`.
*   **Calculate the needed value:** Subtract the current number `nums[i]` from `target`. Store this in `rem` (the remainder needed).
*   **Check the map:** See if `rem` is already stored in our map (`mp.has(rem)`).
    *   **If found:** Return the current index `i` and the saved index from the map (`mp.get(rem)`). You are done!
    *   **If not found:** Add the current number and its position to the map (`mp.set(nums[i], i)`) so future numbers can find it.
*   **Safety return:** Return `[]` at the end as a fallback, though the problem guarantees a solution exists.

---

## Time & Space Complexity

*   **Time Complexity:** **O(n)** — We iterate through the list of `n` elements at most once. Looking up or inserting items into a JavaScript `Map` takes **O(1)** (constant time) on average.
*   **Space Complexity:** **O(n)** — In the worst case, we might store up to `n` items in our map before finding the matching pair.

### Is this optimal?

**Yes, this is already optimal.**

*   **Time:** **O(n)** is the best possible time complexity. You must inspect each element at least once to know if it can form the target sum.
*   **Space:** **O(n)** is required to achieve **O(n)** time. To lower space to **O(1)**, you would have to use two loops (taking **O(n²)** time) or sort the array first (taking **O(n log n)** time, which also ruins the original indices). 

No further improvements can be made to lower the time complexity.

---

## Edge Cases Handled

*   **Duplicate numbers:** Works correctly when the list contains duplicate values (e.g., `nums = [3, 3]`, `target = 6`). The second `3` looks up the first `3` in the map before overwriting it.
*   **Negative numbers:** Subtraction handles negatives seamlessly (e.g., `nums = [-3, 4, 3]`, `target = 0` correctly matches `-3` and `3`).
*   **Minimum array size:** Handles the smallest valid input length of 2 elements without errors.
*   **Target is zero or negative:** Math operations like `target - nums[i]` work identically regardless of sign.
