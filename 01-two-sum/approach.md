![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2066.28%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.08%20MB%20(beats%2012.03%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a single goal number called `target`. Your job is to find two different numbers in the list that add up exactly to `target`. 

Once you find those two numbers, you return their zero-based positions (indices) in the list.

Key rules:
- Every input guarantees **exactly one** valid pair.
- You **cannot** use the exact same element twice (you cannot add a number at index 0 to itself).
- You can return the two indices in any order.

**Example:**
If `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` and `7` add up to `9`. Their position indices are `0` and `1`, so the answer is `[0, 1]`.

---

## Intuition

The naive way is to check every possible pair using two nested loops. That is slow because it takes **O(n^2)** time.

Instead of searching for pairs, think of it backwards. As you look at each number `x` in the array, ask yourself: *"What exact number do I need to reach `target`?"*

The number you need is `remainder = target - x`.

If you keep a "notebook" (a map) of numbers you have already walked past and their position index, you can just check your notebook:
1. Is the `remainder` already in my notebook?
2. **If yes:** You found the pair! Grab the current position and the saved position of the `remainder`.
3. **If no:** Write the current number and its position index in your notebook, then move to the next number.

---

## Approach

Here is how the provided C++ code executes this logic step-by-step:

- **Create a Map:** Create an ordered map `mp` where keys are the numbers from `nums` and values are their array indices.
- **Loop Through Array:** Go through `nums` from left to right using index `i`.
- **Calculate Needed Number:** For the current number `nums[i]`, calculate `rem = target - nums[i]`.
- **Check Map for Match:** Search `mp` to see if `rem` was already saved earlier (`mp.find(rem) != mp.end()`).
- **Return Answer:** If `rem` exists in `mp`, immediately return a vector containing `{i, mp[rem]}`.
- **Save Current Number:** If `rem` is not found, save the current number and index into the map (`mp[nums[i]] = i`).
- **Fallback Return:** Return `{1, 1}` at the end to satisfy the function return type (this line is never reached because a valid pair is guaranteed).

---

## Time & Space Complexity

- **Time Complexity:** **O(n log n)** — The code loops through `nums` once (n steps). In C++, `std::map` is an ordered map built as a red-black tree. Looking up or inserting a value takes **O(log n)** time. Doing this for n items gives **O(n log n)** total time.
- **Space Complexity:** **O(n)** — In the worst case, we store up to n elements in the map before finding the match.

### Can this be improved?

**Yes, it can be improved.** 

We can swap `std::map` for `std::unordered_map`. An `unordered_map` uses a hash table instead of a tree structure. This reduces key lookups and insertions from **O(log n)** down to **O(1)** on average. We can also remove the redundant second `if` check.

Here are the key changed lines:

```cpp
// Change map<int,int> to unordered_map<int,int>
unordered_map<int, int> mp;

for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {i, mp[rem]};
    }
    mp[nums[i]] = i; // Store current number directly
}
```

- **Improved Time Complexity:** **O(n)** average time — Single pass through the array with O(1) hash table lookups.
- **Improved Space Complexity:** **O(n)** — Storing up to n elements in the hash table.
- **Theoretical Best Complexity:** **O(n)** time and **O(n)** space is the theoretical best possible performance for this problem. We must look at each number at least once to know if it forms the sum. The improved `unordered_map` version reaches this theoretical limit.

---

## Edge Cases Handled

- **Duplicate Values in Input:** For an input like `nums = [3, 3]` and `target = 6`, the code works correctly. It checks for `rem = 3` before inserting the second `3` into the map, successfully finding the first `3` stored at index `0`.
- **Negative Numbers:** Works naturally with negative inputs (e.g., `nums = [-3, 4, 3]`, `target = 0`) because basic subtraction `target - nums[i]` naturally accounts for negative signs.
- **Minimum Constraints:** Handles arrays with only 2 elements (the minimum given in constraints) correctly on the very first iteration pair.
- **Large Values:** Values up to 10^9 and -10^9 fit within standard 32-bit signed integers (`int` in C++), avoiding integer overflow during subtraction.
