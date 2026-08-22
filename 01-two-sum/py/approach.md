![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-13.15%20MB%20(beats%2053.25%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single integer called `target`. Your task is to find two numbers in `nums` that add up to `target`. 

Instead of returning the numbers themselves, you must return their positions in the list, which are called **indices** (plural of index). Indexing starts at `0`.

There are three key rules to keep in mind:
* Exactly one solution exists for every input.
* You cannot use the same element twice (meaning you cannot pick the number at index `1` twice).
* You can return the two indices in any order.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the answer is `[0, 1]` because the number at index `0` (which is `2`) plus the number at index `1` (which is `7`) equals `9`.

## Intuition

The simple way to solve this is to check every pair of numbers using two loops. You pick a number, then compare it to every other number in the list to see if they add up to `target`. However, checking every pair takes a lot of time as the list grows bigger.

To speed things up, we turn the problem around with algebra. If `a + b = target`, then `b = target - a`. 

As we walk through the list, we can calculate what number `b` (the remainder) we need to reach `target`. If we remember every number we have already seen, we can instantly check if `b` was seen earlier.

To remember numbers quickly, we use a **hash table** (a Python dictionary). A dictionary lets us look up whether a number exists almost instantly. We store each number as a key and its index position as the value.

## Approach

Here is how the solution works step-by-step:

* `mp = {}`: Create an empty dictionary named `mp` (short for map). This will store the numbers we have seen so far as keys, and their index positions as values.
* `for i in range(len(nums)):`: Loop through the list `nums` one element at a time, keeping track of the current index `i`.
* `rem = target - nums[i]`: Calculate `rem`, the remaining value needed to reach `target` when added to the current number `nums[i]`.
* `if rem in mp:`: Check if `rem` is already stored in our dictionary `mp`.
* `return [i, mp[rem]]`: If `rem` is in `mp`, we found our pair! Return a list containing the current index `i` and the index of `rem` saved inside `mp`.
* `mp[nums[i]] = i`: If `rem` is not in `mp`, store the current number `nums[i]` as a key in `mp` with its index `i` as the value, so future numbers can find it.
* `return []`: Return an empty list if no pair is found (this line is a backup, though the problem guarantees a valid answer exists).

## Dry Run

### Case 1: Typical case
Inputs: `nums = [2, 7, 11, 15]`, `target = 9`

| Index `i` | Current Number `nums[i]` | Needed `rem` (`target - nums[i]`) | Is `rem` in `mp`? | Dictionary `mp` State | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `9 - 2 = 7` | No (`7` not in `{}`) | `{2: 0}` | Store `2` at index `0`. Move to next item. |
| `1` | `7` | `9 - 7 = 2` | Yes (`2` is in `{2: 0}`) | `{2: 0}` | Match found! Return `[1, mp[2]]`, which is `[1, 0]`. |

### Case 2: Edge case with duplicate values
Inputs: `nums = [3, 3]`, `target = 6`

| Index `i` | Current Number `nums[i]` | Needed `rem` (`target - nums[i]`) | Is `rem` in `mp`? | Dictionary `mp` State | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `6 - 3 = 3` | No (`3` not in `{}`) | `{3: 0}` | Store `3` at index `0`. Move to next item. |
| `1` | `3` | `6 - 3 = 3` | Yes (`3` is in `{3: 0}`) | `{3: 0}` | Match found! Return `[1, mp[3]]`, which is `[1, 0]`. |

## Time & Space Complexity

* **Time Complexity:** O(n) — where n is the total number of elements in `nums`. We loop through the list at most once. Looking up and saving keys in a dictionary takes O(1) average time per operation.
* **Space Complexity:** O(n) — in the worst case, we might store up to n elements inside the dictionary `mp` before finding a matching pair.

**Is this already optimal?**
Yes, this is already the most optimal complexity possible for this problem.

* **Why time cannot be improved:** To find two numbers that sum to `target` in an unsorted list, you must inspect every element at least once in the worst case. This means any working solution requires at least O(n) time.
* **Why space cannot be improved:** To achieve O(n) time, we trade memory by using a dictionary to store seen elements. If we tried to avoid extra space (O(1) space), we would either need to compare every pair using nested loops (taking O(n^2) time) or sort the array first (taking O(n log n) time, which also loses the original index positions).

## Edge Cases Handled

* **Duplicate values in `nums`:** When given input like `nums = [3, 3]` and `target = 6`, the code works correctly. The check `if rem in mp` happens *before* putting the current element into `mp`. This prevents the first `3` from pairing with itself.
* **Negative numbers:** The math logic `rem = target - nums[i]` works automatically with negative values (for example, if `target = -2` and `nums[i] = -5`, then `rem = -2 - (-5) = 3`).
* **Zero values:** Handles `0` smoothly, whether `target` is `0` or one of the numbers in `nums` is `0`.
* **Solution located at the end:** Works regardless of where the matching pair sits in the array, even if the matching pair is at the very last index.
