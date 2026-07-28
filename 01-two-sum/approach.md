![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-20.45%20MB%20(beats%2040.39%25)-yellow?style=for-the-badge)

---

## Problem in Plain English

You are given a list of numbers and a single **target** sum. You need to find **two distinct numbers** in the list that add up to the **target**. 

Once you find those two numbers, return their **position numbers** (called **indices**), not the actual values themselves.

* Example: If `nums = [2, 7, 11, 15]` and `target = 9`:
  * `2 + 7 = 9`
  * `2` is at index `0`, and `7` is at index `1`.
  * Output: `[0, 1]`

**Rules:**
* Exactly one valid answer exists for every test case.
* You cannot use the number at the same index twice to make the sum.

---

## Intuition

The naive way to solve this is checking every possible pair of numbers. That requires two loops and is very slow.

Instead, ask a different question as you walk through the list: **"What number do I need to reach the target, and have I already seen it?"**

For every number you look at, calculate its **remainder**:
$$\text{remainder} = \text{target} - \text{current number}$$

If you save every number you have seen so far in a fast lookup tool (a **dictionary** or **hash table**), you can instantly check if the remainder is already waiting for you. This turns a slow two-loop search into a single fast pass.

---

## Approach

Here is how the code works step-by-step:

* **Create a dictionary:** Initialize an empty dictionary (`mp`) to store numbers we have seen as keys and their index as values.
* **Loop through the array:** Go through the numbers one by one using their index position (`i`).
* **Calculate the remainder:** Subtract the current number (`nums[i]`) from `target` to get the `rem` (remainder) needed.
* **Check the dictionary:** Look if `rem` is already saved in `mp`.
  * **If found:** Return `[i, mp[rem]]` immediately. You found the pair!
  * **If not found:** Save the current number and its index into the dictionary: `mp[nums[i]] = i`.
* **Repeat:** Move to the next number until the pair is found.

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We iterate through the list of $n$ numbers at most once. Looking up values in a dictionary takes **O(1)** (instant) time on average.
* **Space Complexity:** **O(n)** — In the worst case, we store up to $n$ elements inside our dictionary before finding the answer.

### Is this optimal?

**Yes, this code is already fully optimal.** 

* **Time:** You must inspect every element at least once to know if it can form the target sum. Therefore, **O(n)** is the theoretical best possible time complexity.
* **Space:** Using **O(n)** extra memory allows us to achieve **O(n)** time. While you could achieve **O(1)** memory by sorting the array first, sorting takes **O(n log n)** time and loses original index positions. 

No further big-O time or space improvement is possible for this problem.

---

## Edge Cases Handled

* **Duplicate Numbers (e.g., `nums = [3, 3]`, `target = 6`):** The code checks if the remainder exists in the dictionary *before* adding the current number. At the second `3`, it searches for `3`, finds the first `3` in `mp`, and returns `[1, 0]` without confusing the two identical values.
* **Negative Numbers & Zero (e.g., `nums = [-3, 4, 0, 3]`, `target = 0`):** Basic math handles signs automatically (`0 - (-3) = 3`). The lookup works identically for negative numbers and zero.
* **Minimum Sized Input (`nums.length == 2`):** The loop runs twice, correctly evaluating the single pair and returning their indices without index out-of-bounds errors.
