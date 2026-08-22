![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2059.30%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.29%20MB%20(beats%2023.85%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a goal total called `target`. Your task is to find **two different numbers** in the list that add up to `target`. 

Instead of returning the numbers themselves, you must return their **index positions** (their zero-based slot numbers in the list).

For example:
* Given `nums = [2, 7, 11, 15]` and `target = 9`.
* Look for two numbers that add up to 9. Here, 2 and 7 make 9 (`2 + 7 = 9`).
* The number 2 is at index 0, and 7 is at index 1.
* The output is `[0, 1]`.

You can assume there is always exactly one working pair, and you cannot use the same element twice.

---

## Intuition

The naive way to solve this is checking every possible pair of numbers. That requires nested loops and takes a long time as the list grows.

A smarter way is to use a **hash map** (a quick lookup memory table). As you walk through the list item by item, ask yourself: *"What number do I need to reach the target?"* 

That missing piece is calculated as:
`needed_number = target - current_number`

If you check your lookup table and see that `needed_number` was already saved from earlier, you found your match! If not, save the `current_number` and its index into the table, then move to the next number.

---

## Approach

Here is how the code works step-by-step:

* `HashMap<Integer, Integer> map = new HashMap<>();`  
  Create an empty map. It will hold numbers we have seen as keys, and their list indices as values.

* `for (int i = 0; i < nums.length; i++)`  
  Start a loop to visit every element in `nums` one by one, using `i` as the current index position.

* `int remain = target - nums[i];`  
  Calculate `remain`, which is the exact partner value needed so that `nums[i] + remain == target`.

* `if (map.containsKey(remain))`  
  Check if `remain` is already stored in `map` from an earlier loop step.

* `return new int[] {i, map.get(remain)};`  
  If `remain` exists in `map`, we found the pair! Return an array containing current index `i` and the stored index `map.get(remain)`.

* `map.put(nums[i], i);`  
  If `remain` was not found, save the current value `nums[i]` and its index `i` into `map` so future numbers can pair with it.

* `return new int[] {};`  
  Return an empty array as a fallback if no answer is found.

---

## Dry Run

### Case 1: Typical case (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `remain` | `map` state before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 2 | 7 | `{}` | 7 is not in `map`. Put `2: 0` into `map`. |
| 1 | 7 | 2 | `{2: 0}` | 2 IS in `map` at index 0. Return `[1, 0]`. |

---

### Case 2: Duplicate values (`nums = [3, 3]`, `target = 6`)

| `i` | `nums[i]` | `remain` | `map` state before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 3 | 3 | `{}` | 3 is not in `map`. Put `3: 0` into `map`. |
| 1 | 3 | 3 | `{3: 0}` | 3 IS in `map` at index 0. Return `[1, 0]`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We loop through the array of length `n` at most once. Map lookups and insertions take O(1) constant time on average.
* **Space Complexity:** **O(n)** — In the worst case, we store up to `n - 1` elements inside `map` before finding a match.

### Is this optimal?

**Yes, O(n) time and O(n) space is already the theoretical best possible complexity for this problem.**

You must look at every number at least once (O(n) time) because any unvisited number could be part of the solution. You also need space to store seen numbers (O(n) space) to avoid checking all pairs in O(n^2) time.

#### Small performance tweak (Java micro-optimization):
If you want to improve runtime and memory performance stats on LeetCode, you can pre-allocate the initial capacity of `map`. Java's `HashMap` resizes itself dynamically as items are added, which adds small memory and time overhead. Pre-sizing prevents dynamic resizing:

```java
// Pre-size the map to prevent dynamic memory resizing overhead
HashMap<Integer, Integer> map = new HashMap<>(nums.length);
```

This change does not alter the theoretical O(n) complexity, but it speeds up real-world execution.

---

## Edge Cases Handled

* **Duplicate Numbers in Array:** Handles inputs like `nums = [3, 3], target = 6`. The code checks `map.containsKey(remain)` *before* adding `nums[i]` to `map`. This ensures an element does not match with itself at index `i`.
* **Negative Numbers:** Handles inputs like `nums = [-3, 4, 3], target = 0`. Arithmetic operations like `0 - (-3) = 3` work naturally with negative values.
* **Smallest Allowed Input Size:** Handles the minimum constraint size `nums.length == 2` without index out-of-bounds errors.
