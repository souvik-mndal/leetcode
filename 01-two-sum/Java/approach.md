![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2059.38%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.3%20MB%20(beats%2023.79%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given an array (a list) of integers called `nums` and a target integer called `target`. Your task is to find two numbers in `nums` that add up to equal `target`. 

Once you find these two numbers, you must return their index positions (their 0-based locations in the array), not the numbers themselves.

Key rules to keep in mind:
* You cannot use the exact same array element twice to build the answer.
* Exactly one valid answer exists in every given test case.
* You can return the two index positions in any order.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`:
* We see that `2 + 7 = 9`.
* `2` is at index `0`, and `7` is at index `1`.
* Output is `[0, 1]`.

## Intuition

The brute-force way to solve this is to check every pair of numbers using two nested loops. That takes quadratic time, written as O(n^2), which gets very slow as the list grows.

Instead, we can do this in a single pass using simple math and a lookup table. 

For any number `nums[i]`, we know the exact second number we need to reach `target`. That missing amount is `remain = target - nums[i]`.

Instead of searching through the rest of the array to find `remain`, we use a **Hash Map** (a key-value storage system) to remember numbers we have already seen. As we step through the array:
1. Calculate the required partner (`remain`).
2. Check if `remain` is already stored in our map.
3. If it is, we found our pair! We grab the saved index and finish.
4. If it isn't, save the current number and its index in the map, then move to the next number.

## Approach

Here is how the provided Java code executes this logic line by line:

* `HashMap<Integer, Integer> map = new HashMap<>();` — Creates an empty Hash Map to store numbers as keys and their array indices as values.
* `for (int i = 0; i < nums.length; i++)` — Loops through the `nums` array one element at a time, keeping track of the current index `i`.
* `int remain = target - nums[i];` — Calculates `remain`, which is the exact partner value needed to sum to `target`.
* `if (map.containsKey(remain))` — Checks if the needed partner number `remain` was already saved into the map during an earlier step.
* `return new int[] {i, map.get(remain)};` — If the partner exists in the map, creates and returns an array containing the current index `i` and the partner's index retrieved with `map.get(remain)`.
* `map.put(nums[i], i);` — If the partner is not yet in the map, saves the current element `nums[i]` as the key and its index `i` as the value so future numbers can look it up.
* `return new int[] {};` — Returns an empty array if no pair is found. This line is required so the Java compiler knows the method always returns a result, though the problem guarantees a solution exists.

## Dry Run

### Case 1: Standard Example
Input: `nums = [2, 7, 11, 15]`, `target = 9`

| `i` | `nums[i]` | `remain` (`target - nums[i]`) | `map` contents before step | Action |
|---|---|---|---|---|
| 0 | 2 | 7 | `{}` | `map.containsKey(7)` is false. Run `map.put(2, 0)`. |
| 1 | 7 | 2 | `{2: 0}` | `map.containsKey(2)` is true! Found pair. Return `{1, 0}`. |

### Case 2: Duplicate Values
Input: `nums = [3, 3]`, `target = 6`

| `i` | `nums[i]` | `remain` (`target - nums[i]`) | `map` contents before step | Action |
|---|---|---|---|---|
| 0 | 3 | 3 | `{}` | `map.containsKey(3)` is false. Run `map.put(3, 0)`. |
| 1 | 3 | 3 | `{3: 0}` | `map.containsKey(3)` is true! Found pair. Return `{1, 0}`. |

## Time & Space Complexity

* **Time Complexity:** O(n) — where `n` is the number of elements in `nums`. We visit each number in the array at most once. Hash Map lookups (`containsKey`) and insertions (`put`) run in O(1) average time.
* **Space Complexity:** O(n) — in the worst-case scenario, we store up to `n - 1` elements in the `map` before finding the solution pair.

**Is this already optimal?**
Yes. This is the optimal time complexity. In an unsorted array, we must inspect every element at least once to verify if it forms the solution, which sets a fundamental lower limit of O(n) time. The O(n) space trade-off is necessary to achieve this linear time speed.

## Edge Cases Handled

* **Duplicate Numbers:** If the two numbers that sum to target are identical (like `[3, 3]` for `target = 6`), the code handles them without conflict because it checks `map.containsKey(remain)` *before* putting the current element into `map`.
* **Negative Numbers:** The math `target - nums[i]` works seamlessly for negative targets and values (e.g., `target = -4`, `nums[i] = -1` yields `remain = -3`).
* **Minimum Array Size:** Handles arrays with only 2 elements correctly on the second loop pass.
* **Order of Output:** Returning `{i, map.get(remain)}` yields indices in reverse order of discovery, which is fully valid since the problem allows returning answers in any order.
