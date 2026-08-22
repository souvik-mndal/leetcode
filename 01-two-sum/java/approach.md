![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2099.33%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.04%20MB%20(beats%2059.48%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to find two numbers in an array (`nums`) that add up to a specific total (`target`). Once you find those two numbers, you return their position indices (their zero-based index in the array) as a pair.

You are guaranteed that every test case has exactly one valid solution, and you cannot use the exact same element twice (meaning you cannot use the number at index 0 twice to make the target).

**Example:**
If `nums = [2, 7, 11, 15]` and `target = 9`:
* Look at 2 and 7: `2 + 7 = 9`.
* Their position indices in the array are `0` and `1`.
* The answer is `[0, 1]` (or `[1, 0]`).

---

## Intuition

The naive way to solve this is to test every single pair of numbers using two nested loops. That requires checking up to `n * n` combinations, which is slow for large arrays.

The key trick ("aha!" moment) is to change the question we ask at each step:
Instead of asking **"Do any two numbers in this array add up to target?"**, we ask **"I am looking at number X right now. What number do I need to complete the target, and have I already seen it?"**

The needed number is simply `target - current_number` (the remainder).

To check if we have seen that needed number before in constant time, we store numbers we encounter in a **Hash Table** (a lookup map that stores key-value pairs). As we walk through the array once:
1. We calculate the matching number we need (`rem`).
2. We check if `rem` is already in our map.
3. If it is, we immediately return both indices.
4. If it isn't, we add our current number and its index to the map so future numbers can find it.

---

## Approach

* `HashMap<Integer, Integer> map = new HashMap<>();` — Creates an empty map to keep track of numbers we have already visited. The key is the number from `nums`, and the value is its index in the array.
* `for (int i = 0; i < nums.length; i++)` — Loops through the `nums` array from the first element to the last, using `i` as the current position index.
* `int rem = target - nums[i];` — Calculates `rem` (the remaining amount needed), which is the exact number required to add to `nums[i]` to equal `target`.
* `if (map.containsKey(rem))` — Checks whether the needed number `rem` was already saved into our lookup map during an earlier loop iteration.
* `return new int[] {i, map.get(rem)};` — Runs if `rem` is found in the map. It returns an array containing the current index `i` and the stored index of `rem` retrieved via `map.get(rem)`.
* `map.put(nums[i], i);` — Saves the current number `nums[i]` as the key and its position index `i` as the value in `map` so that subsequent elements can look it up.
* `return new int[] {};` — Returns an empty array as a fallback in case no matching pair is found (this line is required by Java, though the problem guarantees a solution exists).

---

## Dry Run

### Case 1: Standard case with distinct numbers
`nums = [2, 7, 11, 15]`, `target = 9`

| `i` | `nums[i]` | `rem` (target - nums[i]) | `map` contents before check | Action / Output |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `9 - 2 = 7` | `{}` | `7` is not in map. Store `map.put(2, 0)`. |
| `1` | `7` | `9 - 7 = 2` | `{2: 0}` | `2` is found in map at index `0`! Return `[1, 0]`. |

---

### Case 2: Case with duplicate numbers
`nums = [3, 3]`, `target = 6`

| `i` | `nums[i]` | `rem` (target - nums[i]) | `map` contents before check | Action / Output |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `6 - 3 = 3` | `{}` | `3` is not in map. Store `map.put(3, 0)`. |
| `1` | `3` | `6 - 3 = 3` | `{3: 0}` | `3` is found in map at index `0`! Return `[1, 0]`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We iterate through the array of `n` elements at most once. Looking up a key (`containsKey`) and inserting a key (`put`) in a `HashMap` takes **O(1)** (constant time) on average.
* **Space Complexity:** **O(n)** — In the worst case, we might store up to `n - 1` elements in the map before finding the matching pair on the last element.

### Is this solution optimal?
**Yes, this code is already optimal.** 

* **Why Time is optimal:** We must examine each number in `nums` at least once to know if it contributes to the target sum, which means any valid solution requires at least **O(n)** time.
* **Why Space is optimal:** To achieve **O(n)** time in a single pass without repeatedly scanning previous elements, we must store previously seen elements in memory. This requires **O(n)** extra space. 

While you can reduce extra space to **O(1)** by using two nested loops (brute force), doing so degrades time complexity to **O(n^2)**, which fails the problem's follow-up goal. Therefore, **O(n)** time and **O(n)** space is the best overall theoretical balance for an unsorted input array.

---

## Edge Cases Handled

* **Duplicate Numbers in Input:** Inputs like `nums = [3, 3]` with `target = 6`. Because we check `map.containsKey(rem)` *before* adding the current element `map.put(nums[i], i)`, the second `3` safely finds the first `3` already inside the map without overwriting or confusing indices.
* **Negative Numbers:** Handles inputs with negative values, such as `nums = [-3, 4, 3, 90]` and `target = 0`. Arithmetic subtraction (`0 - (-3) = 3`) works identically for negative integers.
* **Minimum Array Length:** Works correctly for the minimum allowed constraint size (`nums.length = 2`).
* **Target Requiring Large Values:** Handles values up to `10^9` or `-10^9` without integer overflow issues, as standard Java standard 32-bit signed integers handle values up to `2 * 10^9`.
