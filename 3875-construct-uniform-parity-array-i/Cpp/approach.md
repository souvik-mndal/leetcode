![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-30.28%20MB%20(beats%2067.80%25)-green?style=for-the-badge)

---

## Problem Explained

You are given an array of unique positive integers named `nums1`. Your goal is to construct a second array named `nums2` of the same length, such that every number in `nums2` has the **same parity** — meaning every number in `nums2` must be **odd**, or every number must be **even**.

For each position `i` in `nums2`, you are allowed to do one of two choices:
1. Set `nums2[i] = nums1[i]` (keep the original number as it is).
2. Set `nums2[i] = nums1[i] - nums1[j]`, where `j` is any other index in the array (`j` cannot equal `i`).

You need to return `true` if it is possible to build such a `nums2` array, and `false` if it is impossible.

For example, if `nums1 = [2, 3]`:
- `nums2[0] = 2 - 3 = -1` (odd)
- `nums2[1] = 3` (odd)
- Both elements are odd, so the answer is `true`.

---

## Intuition

The key insight is that **it is always mathematically possible** to create a valid `nums2` array for any input `nums1`.

To see why, consider basic subtraction rules for odd and even numbers:
- `Even - Odd = Odd`
- `Odd - Odd = Even`
- `Even - Even = Even`

Now, look at the two possible cases for `nums1`:

1. **All numbers in `nums1` are already even:**
   If every number is even, you do not need to subtract anything. Just keep `nums2[i] = nums1[i]` for all elements. Every number in `nums2` will be even.

2. **At least one number in `nums1` is odd:**
   Pick any single odd number from `nums1` and call it `O`. You can make **every** element in `nums2` odd:
   - If `nums1[i]` is already odd, keep it as `nums1[i]` (it stays odd).
   - If `nums1[i]` is even, subtract `O` from it: `even - odd = odd` (it becomes odd).

Since every array either has zero odd numbers or at least one odd number, a valid uniform array can **always** be formed. Therefore, we do not need to do any loops, math, or checks — we can simply return `true` immediately.

---

## Approach

Here is the step-by-step logic of the code:

* `` `return true;` ``: Immediately return `true` without checking `nums1`. Mathematical logic guarantees that a valid uniform parity array can always be constructed regardless of the input values.

---

## Dry Run

### Case 1: Mixed odd and even numbers (`nums1 = [2, 3]`)

| Step | `nums1` | Action | Return Value |
| :--- | :--- | :--- | :--- |
| 1 | `[2, 3]` | Function called. Instantly executes `return true;`. | `true` |

*Mathematical confirmation:* `nums1` has an odd number (`3`). We keep `3` as `3` (odd) and change `2` to `2 - 3 = -1` (odd). Resulting array `[-1, 3]` is all odd.

---

### Case 2: All even numbers (`nums1 = [4, 6]`)

| Step | `nums1` | Action | Return Value |
| :--- | :--- | :--- | :--- |
| 1 | `[4, 6]` | Function called. Instantly executes `return true;`. | `true` |

*Mathematical confirmation:* `nums1` has no odd numbers. We keep `4` as `4` (even) and `6` as `6` (even). Resulting array `[4, 6]` is all even.

---

## Time & Space Complexity

- **Time Complexity:** **O(1)** — constant time. The function executes a single return statement without inspecting any elements of the array.
- **Space Complexity:** **O(1)** — constant space. No memory or data structures are allocated.

This solution is already at the theoretical limit of efficiency. No algorithmic improvement is possible because O(1) time and space is as fast and light as code can execute.

---

## Edge Cases Handled

- **Array with only even numbers:** Handled automatically. Copying elements directly results in an all-even array.
- **Array with only odd numbers:** Handled automatically. Copying elements directly results in an all-odd array.
- **Array with mixed odd and even numbers:** Handled automatically. Subtracting an odd element from even elements turns everything odd.
- **Minimum array length (n = 1):** Handled automatically. A single element is always uniform with itself.
