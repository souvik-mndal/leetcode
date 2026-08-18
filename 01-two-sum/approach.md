![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2099.33%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.12%20MB%20(beats%2041.18%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to find two numbers in an array (`nums`) that add up to a specific given total (`target`). Once you find those two numbers, you must return their original position numbers (their **indices**), not the values themselves.

For example, if you are given `nums = [2, 7, 11, 15]` and `target = 9`:
- The numbers `2` and `7` add up to `9`.
- Their indices are `0` and `1`.
- You return `[0, 1]` (or `[1, 0]`).

You are guaranteed that every test case has **exactly one solution**, and you cannot use the exact same array element twice to form the sum.

---

## Intuition

The simple way to solve this is to test every possible pair using two loops. That takes **O(n^2)** time because you keep re-checking numbers you already saw.

Instead, ask yourself a question at each step: *"If I am looking at number X right now, what other number do I need to reach the target?"*

That needed number is simply `target - X`. We call this the **remainder** (or complement).

Instead of searching through the rest of the array to find that remainder, we can keep track of every number we have already walked past using a **Hash Table** (a fast lookup structure, like a contact list where a number maps to its index). 

As we walk through the array:
1. We check if our needed remainder is already saved in our hash table.
2. If it is, we found our pair! We immediately return the current index and the saved index.
3. If it is not, we store the current number and its index in our table, then move to the next number.

This lets us find the answer in a single pass through the array.

---

## Approach

Here is how the code executes line by line:

* `HashMap<Integer, Integer> map = new HashMap<>();` — Creates an empty **HashMap** (a lookup table) to store numbers as **keys** and their index positions as **values**.
* `for (int i = 0; i < nums.length; i++)` — Starts a loop to go through the array `nums` one element at a time, keeping track of the current position using index `i`.
* `int rem = target - nums[i];` — Calculates `rem`, which is the exact matching value needed to add to `nums[i]` to hit the `target`.
* `if (map.containsKey(rem))` — Checks if our lookup map already holds the needed value `rem` from an earlier step.
* `return new int[] {i, map.get(rem)};` — If `rem` is found, returns an array containing the current index `i` and the index of `rem` retrieved from `map`.
* `map.put(nums[i], i);` — If `rem` was not found, saves the current number `nums[i]` and its position `i` into `map` so future elements can look for it.
* `return new int[] {};` — Serves as a fallback return statement in case no pair is found (though the problem guarantees a valid answer always exists).

---

## Dry Run

### Case 1: Typical case
`nums = [2, 7, 11, 15]`, `target = 9`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `map` state (Key: Value) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `7` | `{}` | `7` is not in map. Store `map.put(2, 0)`. |
| `1` | `7` | `2` | `{2: 0}` | `2` is in map! Return `{1, map.get(2)}` -> `{1, 0}`. |

---

### Case 2: Edge case with duplicate values
`nums = [3, 3]`, `target = 6`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `map` state (Key: Value) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `3` | `{}` | `3` is not in map. Store `map.put(3, 0)`. |
| `1` | `3` | `3` | `{3: 0}` | `3` is in map! Return `{1, map.get(3)}` -> `{1, 0}`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We loop through the array of length `n` at most once. Hash map lookups (`containsKey`) and insertions (`put`) take **O(1)** (constant) time on average.
* **Space Complexity:** **O(n)** — In the worst case, we might store up to `n - 1` elements in our `map` before finding a match.

### Is this optimal?
**Yes, this solution is already optimal.**

To solve this problem, you must look at each number in `nums` at least once to know if it contributes to the target, which requires at least **O(n)** time. Using a hash table gives us **O(1)** lookup speed, allowing us to hit the theoretical minimum time complexity of **O(n)** with an **O(n)** space tradeoff. No algorithm can perform better than **O(n)** time overall.

---

## Edge Cases Handled

* **Duplicate Numbers in the Array:** Handled naturally (e.g., `nums = [3, 3]`, `target = 6`). The code checks if the complement exists in `map` *before* inserting the current element. Thus, the second `3` successfully matches with the first `3` stored in `map`.
* **Negative Numbers:** Handled properly because subtraction works cleanly with negative values (e.g., `target = -2`, `nums[i] = -6` results in `rem = -2 - (-6) = 4`).
* **Answer at the Extreme Ends:** If the matching numbers are the very first and very last elements in the array, the loop runs through all elements and finds the pair at the final index without issues.
* **Minimum Allowed Input Size:** Handled seamlessly when `nums` has only 2 elements (the minimum size constraint), completing after checking index `1`.
