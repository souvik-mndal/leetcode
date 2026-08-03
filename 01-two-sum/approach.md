![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2051.45%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.20%20MB%20(beats%209.52%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single integer called `target`. 

Your goal is to find **two different numbers** in the list that add up to the `target`. Once you find them, you must return their **indices** (their 0-based position numbers in the array).

Key rules:
* There is always **exactly one** valid pair that works.
* You **cannot use the exact same element twice** (you cannot add a number at index 0 to itself).
* You can return the two positions in any order.

**Example:**
* Input: `nums = [2, 7, 11, 15]`, `target = 9`
* Output: `[0, 1]`
* Why: `nums[0]` is 2 and `nums[1]` is 7. Since `2 + 7 = 9`, we return their positions `[0, 1]`.

---

## Intuition

The slow way to solve this is to check every possible pair of numbers using two nested loops. That would take a lot of time as the array gets larger.

Instead, think of it like this: as you walk through the list, ask yourself:
> *"What partner number do I need right now to reach the target?"*

For any number you are looking at, its required partner is:
`needed_number = target - current_number`

If you keep a memory notebook of all the numbers you have already walked past (and where you saw them), you can simply check your notebook: *"Have I already seen `needed_number`?"*

* If **yes**: You found the answer! You take the current position and the saved position from your notebook.
* If **no**: Write the current number and its position in your notebook, then move to the next number.

---

## Approach

Here is step-by-step how the current C++ code works:

* **Create a map**: Set up a C++ `map<int, int>` named `mp`. This acts as our notebook, mapping a number (key) to its position index (value).
* **Loop through the array**: Go through `nums` one by one using a loop variable `i` from index `0` to the end.
* **Calculate the needed partner**: Compute `rem = target - nums[i]`. This `rem` (remainder) is the missing value needed to hit `target`.
* **Check the map**: Look for `rem` in `mp` using `mp.find(rem)`.
  * **If found**: Return a vector containing the current index `i` and the stored index `mp[rem]`.
  * **If not found**: Add the current number and its index to the map (`mp[nums[i]] = i`).
* **Fallback return**: If the loop ends without returning (which won't happen based on problem constraints), return `{1, 1}`.

---

## Time & Space Complexity

### Current Code Complexity
* **Time Complexity:** O(n log n)
  * The code loops through the list of `n` elements once.
  * Inside the loop, it uses `std::map`. In C++, `std::map` is built as a balanced binary search tree. Searching (`find`) and inserting into `std::map` takes **O(log n)** time.
  * Doing an **O(log n)** operation `n` times gives **O(n log n)** overall time.
* **Space Complexity:** O(n)
  * In the worst case, we store up to `n` elements inside our map.

---

### Can it be improved?

**Yes!** We can improve the time complexity from **O(n log n)** down to **O(n)**.

#### The Fix:
Switch from `std::map` (ordered map) to `std::unordered_map` (hash table). 
* `std::map` takes **O(log n)** time per search/insert.
* `std::unordered_map` takes **O(1)** (constant average time) per search/insert.

We can also remove the redundant second `if` check.

```cpp
// Change this line:
// map<int, int> mp;

// To this:
unordered_map<int, int> mp;
```

#### Improved Complexity:
* **Improved Time Complexity:** O(n) average time — searching and inserting in a hash map takes O(1) on average, performed across `n` elements.
* **Improved Space Complexity:** O(n) — storing up to `n` elements in the hash map.

#### Is this optimal?
**Yes.** This O(n) time and O(n) space complexity is the theoretical best possible for this problem:
1. **Time:** You must inspect each number at least once, which takes O(n) time.
2. **Space:** You need a hash table to remember seen numbers for O(1) lookups, requiring O(n) memory.

---

## Edge Cases Handled

* **Duplicate Numbers in Array (e.g., `nums = [3, 3]`, `target = 6`):**
  When the loop reaches the second `3`, it checks if the first `3` is already in the map. It finds it *before* trying to overwrite it in the map. The code returns `[1, 0]` correctly.
* **Negative Numbers (e.g., `nums = [-3, 4, 3]`, `target = 0`):**
  The subtraction `rem = target - nums[i]` works correctly with negative numbers (e.g., `0 - (-3) = 3`).
* **Minimum Array Length (2 elements):**
  The loop runs for index `0` (adds to map) and index `1` (finds match and returns), handling the smallest valid input size smoothly.
* **Target Made of Unsorted Values:**
  Because the map stores everything seen so far regardless of order, the array does not need to be sorted.
