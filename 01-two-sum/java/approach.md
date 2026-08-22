![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2059.28%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.04%20MB%20(beats%2059.48%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given an array (a list) of integers named `nums` and a single integer called `target`. Your goal is to find two numbers inside `nums` that add up to `target`. 

Once you find those two numbers, you return their position numbers (their **indices**) in the list. You cannot use the exact same element twice (meaning you cannot use the number at index 0 twice to make the sum). You are guaranteed that exactly one valid answer exists in the list.

**Example:**
If `nums = [2, 7, 11, 15]` and `target = 9`:
- Look at `2` (index 0) and `7` (index 1).
- `2 + 7 = 9`, which matches `target`.
- You return `[0, 1]` (or `[1, 0]`).

---

## Intuition

The brute-force way is to pick every number and test it against every other number. That requires checking pairs over and over, which is slow.

The key trick (the "aha!" moment) is to change how you look at the problem. Instead of asking "Do any two numbers add up to target?", ask: **"I am standing at `nums[i]`. What specific number do I need to reach `target`?"**

That needed number is `remain = target - nums[i]`.

To find out if you have already seen that needed number, you store every number you visit into a **Hash Table** (a memory lookup table) alongside its index. As you move through the array item by item, you check if `remain` is already sitting in your Hash Table. 
- If it is, you immediately found your pair!
- If it is not, you save your current number and its index into the Hash Table and move to the next item.

---

## Approach

- `HashMap<Integer, Integer> map = new HashMap<>();`: Creates an empty hash map to store each number we visit as a key and its index position as the value.
- `for (int i = 0; i < nums.length; i++)`: Starts a loop that steps through `nums` one element at a time, keeping track of the current position index `i`.
- `int remain = target - nums[i];`: Calculates the exact matching value needed to reach `target` when added to the current number `nums[i]`.
- `if (map.containsKey(remain))`: Checks whether our hash map already contains the needed `remain` value from an earlier step.
- `return new int[] {i, map.get(remain)};`: Executes when the matching value is found. It returns an array containing the current index `i` and the stored index of the matching value retrieved via `map.get(remain)`.
- `map.put(nums[i], i);`: Executes when the match is not found yet. It saves the current number `nums[i]` and its index `i` into `map` so future iterations can look it up.
- `return new int[] {};`: Serves as a fallback return statement required by Java, reaching this point only if no pair was found.

---

## Dry Run

### Case 1: Standard Case (`nums = [2, 7, 11, 15]`, `target = 9`)

| Step (`i`) | `nums[i]` | `remain` (`target - nums[i]`) | `map` contents (before action) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `9 - 2 = 7` | `{}` | `7` is not in `map`. Store `{2: 0}`. |
| `1` | `7` | `9 - 7 = 2` | `{2: 0}` | `2` is in `map`! Return `[1, 0]`. |

---

### Case 2: Array with Duplicate Values (`nums = [3, 3]`, `target = 6`)

| Step (`i`) | `nums[i]` | `remain` (`target - nums[i]`) | `map` contents (before action) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `6 - 3 = 3` | `{}` | `3` is not in `map`. Store `{3: 0}`. |
| `1` | `3` | `6 - 3 = 3` | `{3: 0}` | `3` is in `map`! Return `[1, 0]`. |

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — We iterate through the array of `n` elements at most once. Looking up a key and inserting a key in a `HashMap` takes **O(1)** average time.
- **Space Complexity:** **O(n)** — In the worst case, we might insert up to `n - 1` elements into the hash map before finding the matching pair.

### Can it be improved?

**No, this is already the theoretically optimal complexity for this problem.**

- **Time:** To know if a matching pair exists in an unsorted array, you must inspect at least every element once. Thus, **O(n)** is the absolute lower bound for time complexity.
- **Space:** You can reduce space complexity to **O(1)** by using a two-pointer approach, but that requires sorting the array first. Sorting takes **O(n log n)** time, which worsens the time complexity. Therefore, **O(n)** time with **O(n)** space is the best overall balance.

---

## Edge Cases Handled

- **Duplicate Values (e.g., `nums = [3, 3]`, `target = 6`):** The code safely handles duplicate values because it checks `map.containsKey(remain)` *before* putting the current number into `map`. When visiting the second `3`, the first `3` is already sitting in the map.
- **Negative Numbers (e.g., `nums = [-3, 4, 3]`, `target = 0`):** Basic subtraction (`target - nums[i]`) works cleanly across positive and negative values. For `nums[0] = -3`, `remain = 0 - (-3) = 3`.
- **Large Arrays:** Works efficiently up to the constraint limit (`10^4` elements) because lookup time per element stays constant on average.
- **Solution at the End:** If the valid pair consists of the last two elements in the array, the loop successfully checks all previous elements saved in memory without skipping any.
