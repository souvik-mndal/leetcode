![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2053.10%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-55.2%20MB%20(beats%2044.17%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to find two numbers in an array (`nums`) that add up to a specific given total (`target`). Once you find those two numbers, you return their position numbers (their **indices**) in the array.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` and `7` add up to `9`. Since `2` is at position `0` and `7` is at position `1`, the answer is `[0, 1]`.

You can assume there is always exactly one working pair, and you cannot use the exact same array element twice to make the sum.

---

## Intuition

The simple way to solve this is to check every possible pair of numbers using two loops. But checking every pair takes a lot of time as the array grows larger.

Instead, think of it like this: as you walk through the list, ask yourself, *"What number do I need to reach the target?"*

If the target is `9` and your current number is `2`, you need a `7` (`9 - 2 = 7`). 

To avoid checking the list over and over, you can use a **Map** (a fast lookup dictionary). As you inspect each number, you look in your map to see if the number you *need* was already seen earlier. If it is there, you instantly have your answer! If it is not there, you store the current number and its position in the map, so future numbers can find it.

---

## Approach

Here is how the code works step-by-step:

* `let mp = new Map();`: Creates an empty **Map** named `mp`. This will store numbers we have already checked as keys, and their array index positions as values.
* `for (let i = 0; i < nums.length; i++)`: Starts a loop that steps through the array `nums` one item at a time, using `i` as the current index position.
* `let rem = target - nums[i];`: Calculates `rem` (the remaining amount needed). This is the value that must be added to `nums[i]` to equal `target`.
* `if (mp.has(rem))`: Checks if our needed value `rem` is already saved inside our map `mp`.
* `return [i, mp.get(rem)];`: If `rem` is found in `mp`, we stop immediately and return an array containing the current index `i` and the saved index of `rem` retrieved with `mp.get(rem)`.
* `mp.set(nums[i], i);`: If `rem` is not in `mp`, we save the current number `nums[i]` as the key and its index `i` as the value into `mp` for future steps to find.
* `return [];`: Returns an empty array if no pair is found (this acts as a safety net, though the problem guarantees a solution exists).

---

## Dry Run

### Case 1: Standard List
`nums = [2, 7, 11, 15]`, `target = 9`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` state before check | Action |
|---|---|---|---|---|
| `0` | `2` | `7` | `{}` | `7` is not in `mp`. Save `mp.set(2, 0)`. |
| `1` | `7` | `2` | `{2 => 0}` | `2` is in `mp`! Return `[1, 0]`. |

---

### Case 2: Duplicate Values
`nums = [3, 3]`, `target = 6`

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` state before check | Action |
|---|---|---|---|---|
| `0` | `3` | `3` | `{}` | `3` is not in `mp`. Save `mp.set(3, 0)`. |
| `1` | `3` | `3` | `{3 => 0}` | `3` is in `mp`! Return `[1, 0]`. |

---

## Time & Space Complexity

* **Time Complexity:** O(n) — where n is the number of elements in `nums`. We iterate through the list at most once. Looking up or inserting an item in a JavaScript `Map` takes average O(1) constant time.
* **Space Complexity:** O(n) — in the worst case, we store up to n elements inside our map `mp` if the matching pair is at the very end of the array.

### Is this optimal?

**Yes, this solution is already optimal.**

To find two numbers that sum to a target, you must inspect each number at least once. Therefore, the theoretical minimum time complexity is O(n). Since our single-pass loop runs in O(n) time, it cannot be made faster in terms of time complexity.

While you could reduce extra space to O(1) by using two nested loops, doing so would increase time complexity to O(n^2), which is much slower.

---

## Edge Cases Handled

* **Duplicate values:** For inputs like `nums = [3, 3]` and `target = 6`, the code checks if `rem` exists in `mp` *before* saving the current number. This prevents an element from matching with itself and ensures duplicate numbers at different indices work correctly.
* **Negative numbers:** For inputs with negative numbers like `nums = [-3, 4, 3, 90]` and `target = 0`, standard basic arithmetic (`0 - (-3) = 3`) works without needing special logic.
* **Smallest array size:** Handles arrays with the minimum length of 2 elements seamlessly without index errors.
