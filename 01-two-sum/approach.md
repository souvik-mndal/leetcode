![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2051.49%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.13%20MB%20(beats%209.51%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a single target number called `target`. Your goal is to find two different numbers in the list that add up to `target`. 

Once you find those two numbers, return their **index positions** (where they are located in the list, starting from 0).

**Rules:**
* Exactly one valid pair exists in the list.
* You cannot use the same element position twice.
* You can return the two positions in any order.

**Example:**
* Input: `nums = [2, 7, 11, 15]`, `target = 9`
* Output: `[0, 1]`
* Why? Because `nums[0]` is `2` and `nums[1]` is `7`. `2 + 7 = 9`.

---

## Intuition

The simple way to solve this is to test every single pair of numbers. However, that gets very slow as the list grows.

Instead, ask a different question as you read each number: **"What other number do I need to reach the target?"**

This missing number is called the **remainder** (`target - current_number`). 

If you keep a running memory log of all numbers you have already seen (and where you saw them), you can just check if your missing number is in that memory log. If it is, you immediately have your answer!

---

## Approach

Here is how the code implements this memory log:

* **Create a memory map:** Set up a map data structure (`map<int, int> mp`) to keep track of numbers we have seen as keys, and their array index positions as values.
* **Loop through the array:** Start at the beginning (index `0`) and move right one number at a time.
* **Calculate the missing pair:** For the current number, calculate `rem = target - nums[i]`. This is the exact number needed to reach the target.
* **Check the memory map:** Look up if `rem` is already stored in our map.
* **If found:** Return a pair containing the current index `i` and the stored index of `rem` (`mp[rem]`).
* **If not found:** Save the current number and its index into the map (`mp[nums[i]] = i`) so future numbers can look it up.
* **Repeat:** Move to the next number until the matching pair is found.

---

## Time & Space Complexity

**Current Complexity:**
* **Time:** **O(n log n)** — We loop through all $n$ numbers once. Inside the loop, we search and insert into `std::map`. A standard `map` in C++ uses a balanced tree structure, so searching or inserting takes **O(log n)** time (logarithmic time, meaning work grows slowly as input grows).
* **Space:** **O(n)** — In the worst-case scenario, we store almost every number from the list inside our map.

**Can it be improved?**
Yes. You can swap `std::map` for `std::unordered_map`. An `unordered_map` uses a **hash table** (a data structure that maps keys to values instantly using math), which drops search and insert times from **O(log n)** to **O(1)** (constant time, meaning it takes the same time regardless of size).

Here is the key change:

```cpp
// Change 'map' to 'unordered_map' for faster lookups
unordered_map<int, int> mp;

for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {i, mp[rem]};
    }
    mp[nums[i]] = i; // Simplified insertion
}
```

* **Improved Complexity:** **Time:** **O(n)** on average. **Space:** **O(n)**.
* **Theoretical Best Complexity:** **O(n)** time is the absolute fastest possible because you must look at each number at least once to know if it forms the sum. The improved version with `unordered_map` reaches this theoretical limit.

---

## Edge Cases Handled

* **Duplicate values:** Handles inputs like `nums = [3, 3]` and `target = 6`. The code checks if the needed pair (`3`) is already in the map *before* adding the current `3` to the map. This prevents a number from pairing with itself at the exact same index.
* **Negative numbers:** Handles lists with negative values like `nums = [-3, 4, 3]` and `target = 0`. Simple subtraction (`0 - (-3) = 3`) works correctly for negative values.
* **Minimum list length:** Handles the smallest possible list size of 2 elements (`nums.length >= 2`).
* **Target with large values:** Handles numbers up to 1 billion without integer overflow because standard 32-bit integers can hold calculations up to ~2 billion.
