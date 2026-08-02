![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.74%20MB%20(beats%2012.06%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers named `nums`. This array contains only three values: `0` (red), `1` (white), and `2` (blue). 

Your task is to sort the array **in-place** (meaning you modify the original array directly without creating a new array). All `0`s must come first, followed by all `1`s, and then all `2`s. You are not allowed to use any built-in sorting functions.

**Example:**
- **Input:** `[2, 0, 2, 1, 1, 0]`
- **Output:** `[0, 0, 1, 1, 2, 2]`

---

## Intuition

Since there are only three unique values (`0`, `1`, and `2`), we do not need a complex sorting algorithm. Instead, we can use three pointers to divide the array into three sections:

1. A left boundary (`i`) for where the next `0` should go.
2. A right boundary (`j`) for where the next `2` should go.
3. A current reader (`index`) that scans through the array.

The "aha!" moment is how we move numbers to their correct sides:
- Whenever we find a `0`, we swap it to the left side and move forward.
- Whenever we find a `2`, we swap it to the right side. **Crucial trick:** After swapping a `2` to the right, we do *not* advance our current reader yet. Why? Because the number we just swapped from the back into our current position hasn't been checked yet!
- Whenever we find a `1`, we leave it where it is and move forward.

This strategy is famously known as the **Dutch National Flag algorithm**.

---

## Approach

Here is how the code works step-by-step:

* **Initialize pointers:**
  * Set `i = 0` (tracks the slot for `0`s).
  * Set `j = nums.size() - 1` (tracks the slot for `2`s).
  * Set `index = 0` (scans from left to right).

* **Loop until finished:** Continue looping as long as `index <= j`.

* **Case 1: `nums[index]` is `0`**
  * Swap `nums[index]` with `nums[i]`.
  * Move `i` forward by 1 (`i++`).
  * Move `index` forward by 1 (`index++`).

* **Case 2: `nums[index]` is `2`**
  * Swap `nums[index]` with `nums[j]`.
  * Move `j` backward by 1 (`j--`).
  * Keep `index` where it is so the newly swapped number can be evaluated on the next iteration.

* **Case 3: `nums[index]` is `1`**
  * Leave the number as is.
  * Move `index` forward by 1 (`index++`).

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — where `n` is the number of elements in `nums`. We scan through the array in a single pass. Each element is inspected or moved at most once.
- **Space Complexity:** **O(1)** — we only use three integer variables (`i`, `j`, and `index`). No extra memory or new arrays are created.

### Is this optimal?
**Yes, this is already optimal.**
- **Time:** To sort an array, you must look at every element at least once. Therefore, **O(n)** is the absolute fastest possible time.
- **Space:** Modifying the array in-place using **O(1)** extra memory is the best possible space efficiency.

No further improvements in time or space complexity are possible.

---

## Edge Cases Handled

* **Single-element arrays (e.g., `[0]`):** `index` starts equal to `j` (both `0`). The loop runs once, compares or swaps with itself, and terminates cleanly without errors.
* **Arrays with only one unique value (e.g., `[1, 1, 1]` or `[2, 2, 2]`):** The pointers move smoothly across the array without out-of-bounds pointer reads.
* **Already sorted arrays (e.g., `[0, 1, 2]`):** Elements stay in place without unnecessary corruption of order.
* **Reverse sorted arrays (e.g., `[2, 1, 0]`):** The algorithm correctly swaps `2` to the right and `0` to the left in sequence.
* **Arrays containing only two of the three colors (e.g., `[2, 0]`):** The condition `index <= j` prevents `index` from moving past `j` after `j` shrinks.
