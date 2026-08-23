![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2053.67%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-20.49%20MB%20(beats%2041.07%25)-yellow?style=for-the-badge)

---

## Problem Explained

We are given a list of integers called `nums` and a single goal number called `target`. Our job is to find two different positions (indices) in `nums` whose numbers add up to exactly `target`.

We must return the two index positions as a list. The problem guarantees that every test case has exactly one valid answer, and we cannot use the number at the same index position twice.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` (at index 0) and `7` (at index 1) add up to `9`. We return `[0, 1]` (or `[1, 0]`).

## Intuition

The simple way to solve this is to check every possible pair of numbers using two loops. But that requires checking almost every number against every other number, which is slow for large lists.

Instead, think of it this way: when we look at any single number, say `x`, we already know what matching number we need to reach `target`. That needed number is `target - x`. We call this needed value the **remainder**.

If we remember every number we have already seen as we walk through the list, we can just ask: "Have we seen the **remainder** before?"

To remember numbers efficiently, we use a **dictionary** (also called a **hash table** or **hash map**). A dictionary lets us look up previously stored items almost instantly. By saving each number and its index position as we go, we can find our answer in a single walk through the list.

## Approach

* `mp = {}` creates an empty dictionary named `mp` to store each number we visit as a key, mapped to its index position as the value.
* `for i in range(len(nums)):` loops through the list `nums` position by position, using `i` as the current index.
* `rem = target - nums[i]` calculates `rem`, which is the missing number needed to sum up to `target` with the current number `nums[i]`.
* `if rem in mp:` checks if the missing number `rem` has already been seen and stored in our dictionary `mp`.
* `return [i, mp[rem]]` exits immediately and returns a list with the current index `i` and the stored index `mp[rem]` once a matching pair is found.
* `mp[nums[i]] = i` stores the current number `nums[i]` in `mp` with its index `i` so future numbers can look it up, running only if the matching pair was not found yet.
* `return []` acts as a fallback empty list return if no solution is found (though the problem guarantees one valid solution always exists).

## Dry Run

### Standard Case with Distinct Values

Input: `nums = [2, 7, 11, 15]`, `target = 9`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 2 | 7 | `{}` | `7` is not in `mp`. Add `2: 0` to `mp`. |
| 1 | 7 | 2 | `{2: 0}` | `2` is in `mp`! Return `[1, mp[2]]` which is `[1, 0]`. |

### Edge Case with Duplicate Values

Input: `nums = [3, 3]`, `target = 6`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 3 | 3 | `{}` | `3` is not in `mp`. Add `3: 0` to `mp`. |
| 1 | 3 | 3 | `{3: 0}` | `3` is in `mp`! Return `[1, mp[3]]` which is `[1, 0]`. |

## Time & Space Complexity

**Time:** O(n) — We scan through the list of `n` elements at most once. Checking if `rem` is in `mp` and inserting a key into `mp` both take O(1) average time in Python.

**Space:** O(n) — In the worst case, we might store up to `n - 1` elements in the `mp` dictionary before finding the answer.

### Is this solution optimal?

Yes, this code is already optimal. 

To determine if a pair sums to `target`, any algorithm must examine the elements of `nums` at least once. Reading an array of length `n` requires at least O(n) time. Therefore, O(n) time complexity is the theoretical best possible lower bound for this problem, and this single-pass dictionary approach achieves it. No further time complexity improvements are possible.

## Edge Cases Handled

* **Negative numbers:** The math `target - nums[i]` works correctly regardless of whether numbers or targets are positive or negative. For example, if `target` is `-5` and `nums[i]` is `-2`, `rem` becomes `-3`.
* **Duplicate values:** Inputs like `nums = [3, 3]` with `target = 6` work cleanly. The code checks if `rem` is in `mp` *before* adding the current number to `mp`, so it correctly pairs two identical values without matching an element to itself.
* **Smallest array size:** Handles the minimum allowed constraint of `nums.length = 2` without index out-of-bounds errors.
* **Answer at the very end:** If the matching pair consists of the final two elements in a long list, the code runs without issue and returns the correct indices on the final iteration.
