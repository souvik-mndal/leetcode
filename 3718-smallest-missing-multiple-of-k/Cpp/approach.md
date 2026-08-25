![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2023.25%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-24.08%20MB%20(beats%2082.33%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the **smallest positive multiple of `k`** that does not appear in the given array `nums`.

A positive multiple of `k` means any number you get by multiplying `k` by a positive integer (1, 2, 3, and so on):
* If `k = 2`, the multiples are `2, 4, 6, 8, 10, ...`
* If `k = 5`, the multiples are `5, 10, 15, 20, ...`

We need to check these multiples in increasing order starting from `k` itself, and return the very first one that is missing from `nums`.

For example, if `nums = [8, 2, 3, 4, 6]` and `k = 2`, the multiples of 2 in order are 2, 4, 6, 8, 10... Since 2, 4, 6, and 8 are all in `nums`, the smallest missing multiple is **10**.

---

## Intuition

To easily spot which multiple is missing, it helps if the numbers are ordered from smallest to largest. 

If we sort `nums` first, we can scan through the array while keeping track of the target multiple we are looking for (starting at `k`).
1. Every time we encounter a number equal to our current target, it means this multiple is present in the array. We then increment our target to the next multiple (`val + k`).
2. If we encounter a number strictly larger than our target, we know our target cannot exist anywhere later in the array (because all remaining numbers will be even larger). So, we can stop immediately and return our target!

---

## Approach

Here is how the code executes step-by-step:

* `sort( nums.begin() , nums.end());` — Sorts the entire `nums` array in ascending (smallest to largest) order. This lets us check numbers sequentially.
* `int val = k;` — Initializes a tracking variable `val` to `k`, representing the smallest positive multiple of `k` we are currently looking for.
* `for( auto it:nums )` — Starts a loop to iterate through each number `it` in the sorted array `nums`.
* `if( it == val )` — Checks if the current array element `it` matches our target multiple `val`.
* `val+=k;` — If matched, advances `val` by `k` to check for the next multiple on subsequent iterations.
* `else if( it > val )` — Checks if the current array element `it` has exceeded our target multiple `val`.
* `return val;` — If `it > val`, it means `val` was never found and cannot appear later because the array is sorted. We return `val` immediately.
* `return val;` — If the loop ends without returning early, it means all multiples up to the current `val` were present. We return `val` as the missing multiple.

---

## Dry Run

### Case 1: Typical case with multiples present (`nums = [8, 2, 3, 4, 6]`, `k = 2`)

After sorting: `nums = [2, 3, 4, 6, 8]`. Initial `val = 2`.

| Step | `it` | `val` | Action |
| :--- | :--- | :--- | :--- |
| 1 | 2 | 2 | `it == val` matches! Update `val` to `2 + 2 = 4`. |
| 2 | 3 | 4 | `3 < 4`, ignore and continue. |
| 3 | 4 | 4 | `it == val` matches! Update `val` to `4 + 2 = 6`. |
| 4 | 6 | 6 | `it == val` matches! Update `val` to `6 + 2 = 8`. |
| 5 | 8 | 8 | `it == val` matches! Update `val` to `8 + 2 = 10`. |
| End | - | 10 | Loop ends. Return `10`. |

---

### Case 2: Early return case (`nums = [1, 4, 7, 10, 15]`, `k = 5`)

After sorting: `nums = [1, 4, 7, 10, 15]`. Initial `val = 5`.

| Step | `it` | `val` | Action |
| :--- | :--- | :--- | :--- |
| 1 | 1 | 5 | `1 < 5`, ignore and continue. |
| 2 | 4 | 5 | `4 < 5`, ignore and continue. |
| 3 | 7 | 5 | `7 > 5` is true! We passed `5` without seeing it. Return `val` (5) immediately. |

---

## Time & Space Complexity

* **Time Complexity:** **O(N log N)** where N is the length of `nums`. Sorting `nums` takes O(N log N) time, and the subsequent `for` loop takes O(N) time.
* **Space Complexity:** **O(1)** auxiliary space (or O(log N) depending on C++ sorting implementation overhead) because we only use a single variable `val` to track the current target.

### Can it be improved?

Yes! We can reduce the time complexity to **O(N)** by using a **hash set** (a data structure that lets us look up numbers in O(1) average time).

Instead of sorting `nums`, we insert all elements into a hash set. Then we check if `k` is in the set, then `2 * k`, then `3 * k`, and so on. Since `nums` has N elements, we will find a missing multiple in at most N + 1 steps.

Here is the key logic replacement:

```cpp
unordered_set<int> numSet(nums.begin(), nums.end());
int val = k;
while (numSet.count(val)) {
    val += k;
}
return val;
```

* `unordered_set<int> numSet(nums.begin(), nums.end());` — Copies array values into a lookup set in O(N) average time.
* `while (numSet.count(val))` — Performs fast O(1) average time lookups for each multiple.
* `val += k;` — Advances to the next candidate multiple whenever the current one is present.

* **Improved Time Complexity:** **O(N)** average time.
* **Improved Space Complexity:** **O(N)** space to store array elements in the set.
* **Theoretical Limit:** **O(N)** time is optimal because we must examine every number in `nums` at least once to know which numbers are present. The hash set approach achieves this theoretical limit.

---

## Edge Cases Handled

* **First multiple missing immediately (`k` not in array):** Handled when an element greater than `k` is encountered or when the loop ends without matching `k` (e.g., `nums = [1, 3, 7]`, `k = 5` returns `5`).
* **All multiples present up to array length:** If every multiple is present, the loop finishes and returns the next multiple after the end (e.g., `nums = [2, 4, 6]`, `k = 2` returns `8`).
* **Duplicate values in `nums`:** Handled properly because once `val` increments, any repeated smaller numbers are less than `val` and ignored (e.g., `nums = [2, 2, 4]`, `k = 2`).
* **Unrelated numbers present:** Numbers that are not multiples of `k` (like odd numbers when `k = 2`) are either smaller than `val` (ignored) or larger than `val` (triggering an early return if `val` was missed).
