![Runtime](https://img.shields.io/badge/Runtime-Unknown%20(beats%20Unknown)-lightgrey?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-Unknown%20(beats%20Unknown)-lightgrey?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a goal number called `target`. Your job is to find the **indices** (the position numbers, starting at `0`) of two numbers in the list that add up to the `target`.

* You cannot use the exact same position twice.
* Every input is guaranteed to have exactly one valid answer.

**Example:**
* **Input:** `nums = [2, 7, 11, 15]`, `target = 9`
* **Output:** `[0, 1]`
* **Why:** The number at index `0` is `2`, and the number at index `1` is `7`. Since $2 + 7 = 9$, we return their positions `[0, 1]`.

---

## Intuition

Instead of checking every possible pair of numbers (which is slow), you can solve this in a single pass using simple math.

As you walk through the list, ask yourself: *"What second number do I need to reach the target?"* 

That needed number is `rem = target - current_number`.

To make lookups fast, keep a memory log (a lookup table) of numbers you have already walked past, along with their positions. For each number:
1. Check if the needed number (`rem`) is already in your memory log.
2. If it is, you found your match!
3. If it is not, save the current number and its position in your log and move to the next number.

---

## Approach

Here is how the given C++ code implements this logic step-by-step:

* **Create a map:** Set up a `std::map` named `mp` to store previously seen numbers as keys and their array positions as values.
* **Loop through the list:** Start a `for` loop from index `i = 0` to the end of `nums`.
* **Calculate the missing piece:** Find the required matching number by calculating `rem = target - nums[i]`.
* **Search the map:** Use `mp.find(rem)` to check if `rem` is already stored in the map.
* **Handle a match:** If `rem` is found in the map, a matching pair exists. (Note: The provided code contains a placeholder return statement, but the logic intends to return the index of `rem` and the current index `i`).
* **Store for later:** If `rem` is not in the map, store the current number and its index in the map using `mp[nums[i]] = i`.
* **Repeat:** Move to the next index until the matching pair is found.

---

## Time & Space Complexity

**Current Complexity:**
* **Time:** **$O(n \log n)$** — The loop runs $n$ times. In C++, `std::map` is built as a balanced binary tree, so searching or inserting takes $O(\log n)$ time per step.
* **Space:** **$O(n)$** — In the worst case, the map stores up to $n$ elements.

### Can this be improved?

**Yes.** We can improve the time complexity from $O(n \log n)$ to **$O(n)$** by replacing `std::map` with `std::unordered_map` (a **hash table**, which offers fast instant lookups). We can also fix the return statement logic.

**Code changes:**
```cpp
// 1. Switch to a hash table for O(1) average lookups
unordered_map<int, int> mp;

// 2. Return the stored index and current index when found
if (mp.find(rem) != mp.end()) {
    return {mp[rem], i};
}
```

* **Improved Time Complexity:** **$O(n)$** on average — Iterating through the list takes $O(n)$ time, and looking up items in a hash table takes $O(1)$ average time.
* **Improved Space Complexity:** **$O(n)$** — To store up to $n$ elements in the hash map.
* **Is this optimal?** **Yes.** $O(n)$ is the theoretical best time complexity for an unsorted list because you must look at each element at least once to find the pair.

---

## Edge Cases Handled

* **Negative Numbers:** Works automatically with negative values (e.g., `nums = [-3, 4, 3]`, `target = 0`) because basic subtraction `target - nums[i]` correctly handles signs.
* **Duplicate Values:** Correctly handles lists with repeated numbers (e.g., `nums = [3, 3]`, `target = 6`). When checking the second `3`, the code looks up the first `3` in the map before overwriting it.
* **Minimum Array Length:** Works for the smallest allowed input size of 2 elements (as defined by constraints).
