![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2066.36%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.23%20MB%20(beats%208.47%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single goal integer called `target`. Your task is to find two distinct numbers in `nums` that add up to `target`, and return their position numbers (their **indices**, starting at 0).

You can assume there is always exactly one correct pair, and you cannot use the exact same position twice.

**Example:**
* **Input:** `nums = [2, 7, 11, 15]`, `target = 9`
* **Output:** `[0, 1]`
* **Why:** The number at index 0 is `2`, and the number at index 1 is `7`. Since `2 + 7 = 9`, we return their positions `[0, 1]`.

---

## Intuition

The naive way is to pick every number and compare it with every other number. That requires checking pairs over and over, which is slow.

Instead, think about it like this: as you walk through the array, if you are looking at a number $X$, you already know the exact partner number you need. That missing partner is `target - X`. 

The "aha!" moment is using a lookup table (a **map** or **hash table**) as a memory bank. As you visit each number, you ask: *"Have I already seen my needed partner in my memory bank?"*
* If **yes**, you found the match! Grab the current position and the stored partner's position.
* If **no**, write the current number and its position into your memory bank, then move to the next number.

---

## Approach

Here is how the code executes this logic step-by-step:

* **Create a map:** Set up an ordered map called `mp` where keys are the numbers seen so far, and values are their array index positions.
* **Loop through the array:** Start at index `i = 0` and inspect each number `nums[i]` one by one.
* **Calculate the missing partner:** Compute `rem = target - nums[i]`. This is the exact number needed to hit `target`.
* **Check memory:** Use `mp.find(rem)` to look up if `rem` was saved in `mp` during an earlier step.
* **Return if found:** If `rem` exists in `mp`, immediately return the current index `i` and the partner's stored index `mp[rem]`.
* **Save for later:** If `rem` is not in `mp`, add the current number and its index to `mp` using `mp[nums[i]] = i`.
* **Fallback:** Return `{1, 1}` at the end as a default safeguard (this line is never reached because a valid answer is guaranteed).

---

## Time & Space Complexity

* **Time Complexity:** **$O(n \log n)$** — The code uses `std::map`, which stores elements in a balanced tree structure. Searching and inserting into a `std::map` takes $O(\log n)$ time. Doing this for $n$ elements gives a total time of $O(n \log n)$.
* **Space Complexity:** **$O(n)$** — In the worst case, we store up to $n$ elements in our map.

### Can this be improved?

**Yes.** We can swap `std::map` (ordered map) for `std::unordered_map` (hash table). 

Searching and inserting in an `unordered_map` takes **$O(1)$ average time** instead of $O(\log n)$ time because it uses direct key hashing rather than traversing a tree.

**Key code change:**
```cpp
// Change this line:
// map<int,int> mp;

// To this:
unordered_map<int, int> mp;
```

* **Improved Time Complexity:** **$O(n)$ average** — We visit each of the $n$ numbers once, and map lookups take $O(1)$ time on average.
* **Improved Space Complexity:** **$O(n)$** — We store up to $n$ numbers in the hash map.
* **Theoretical Best:** **$O(n)$ time and $O(n)$ space** is the absolute theoretical best for this problem because we must inspect each number at least once. The `unordered_map` version reaches this optimal limit.

---

## Edge Cases Handled

* **Duplicate values in the array:** For inputs like `nums = [3, 3]` and `target = 6`, the code works correctly. On the first `3` (index 0), it looks for `3` in `mp` (not found) and stores `mp[3] = 0`. On the second `3` (index 1), it looks for `3`, finds it at index 0, and returns `[1, 0]`.
* **Negative numbers:** Handles negative values correctly (e.g., `nums = [-3, 4, 3]`, `target = 0`), because subtraction `target - nums[i]` handles negative arithmetic seamlessly.
* **Minimum size array:** Works when `nums` contains only 2 elements (the smallest valid input constraint).
* **Large input values:** Numbers up to $10^9$ fit inside standard 32-bit signed integers (`int`), preventing integer overflow during subtraction.
