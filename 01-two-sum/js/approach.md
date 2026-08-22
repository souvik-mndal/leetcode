![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2053.20%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-55.16%20MB%20(beats%2044.38%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a target integer called `target`. Your task is to find the two numbers in `nums` that add up to `target` and return their index positions (their 0-based locations in the array).

You can assume that every input has exactly one correct pair, and you cannot use the exact same position twice. You can return the two positions in any order.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` and `7` add up to `9`. Since `2` is at position `0` and `7` is at position `1`, the answer is `[0, 1]` (or `[1, 0]`).

---

## Intuition

The brute-force way is to pick every number and test it against every other number using two loops. That is slow because it checks pairs over and over.

Instead, think about what you need while standing at any number. If the current number is `5` and `target` is `9`, you immediately know you need a `4` to make `9`. The missing amount is always `target - current_number`.

The "aha" moment is to remember every number you have seen so far as you walk through the array. You save each number and its index in a **hash map** (a quick lookup store, called `Map` in JavaScript). At each step, you ask: *"Have I already seen my missing partner in my store?"* If yes, you stop immediately and return both positions.

---

## Approach

Here is how the code executes step-by-step:

* `let mp = new Map();`
  Creates an empty map named `mp` to store each number we visit as a key, and its array index as the value.

* `for (let i = 0; i < nums.length; i++)`
  Starts a loop to iterate through the array `nums` from left to right, using `i` as the current index.

* `let rem = target - nums[i];`
  Calculates `rem` (the remaining amount needed). This is the required partner value that must pair with `nums[i]` to reach `target`.

* `if (mp.has(rem))`
  Checks whether the map `mp` already contains `rem`. This lets us know if we previously passed the matching number.

* `return [i, mp.get(rem)];`
  If `rem` is found in `mp`, we have our pair. Returns an array containing the current index `i` and the saved index of the partner retrieved with `mp.get(rem)`.

* `mp.set(nums[i], i);`
  If `rem` was not in `mp`, stores the current number `nums[i]` along with its index `i` into `mp` so future numbers can look it up.

* `return [];`
  Acts as a fallback return value if no pair is found (though the problem guarantees a valid answer exists).

---

## Dry Run

### Case 1: Standard case (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` contents before step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `7` | `{}` | `mp.has(7)` is `false`. Save `2 => 0` into `mp`. |
| `1` | `7` | `2` | `{ 2 => 0 }` | `mp.has(2)` is `true`! Return `[1, 0]`. |

---

### Case 2: Array with duplicate values (`nums = [3, 3]`, `target = 6`)

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | `mp` contents before step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `3` | `{}` | `mp.has(3)` is `false`. Save `3 => 0` into `mp`. |
| `1` | `3` | `3` | `{ 3 => 0 }` | `mp.has(3)` is `true`! Return `[1, 0]`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where **n** is the number of elements in `nums`. We iterate through the array at most once. Looking up and inserting elements in a JavaScript `Map` takes **O(1)** average time.
* **Space Complexity:** **O(n)** — in the worst case, we store up to **n - 1** elements in `mp` before finding the matching pair.

### Is this solution optimal?

**Yes, this code is already optimal.**

* **Time:** You must look at every number at least once to know if it forms the target sum. Therefore, reading the array takes at least **O(n)** time. Our single pass achieves this theoretical minimum.
* **Space:** To break the naive **O(n^2)** time bound without sorting the array, you must trade space for speed by recording past elements in memory. Thus, **O(n)** space is required for an **O(n)** time solution.

---

## Edge Cases Handled

* **Duplicate numbers in input:** Works properly (e.g., `nums = [3, 3]`, `target = 6`). The code checks `mp.has(rem)` *before* writing the current index with `mp.set()`. At index `1`, it successfully finds the first `3` stored at index `0`.
* **Negative numbers:** Handled automatically (e.g., `nums = [-3, 4, 3]`, `target = 0`). Subtraction like `0 - (-3) = 3` works smoothly with standard arithmetic.
* **Minimum sized inputs:** Works on arrays with only 2 elements, which is the smallest valid input size according to constraints.
* **Target requires numbers later in the array:** Works regardless of where the pair is located, as every unseen element is stored for future matches.
