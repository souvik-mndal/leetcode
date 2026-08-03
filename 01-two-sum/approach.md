![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2051.45%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.18%20MB%20(beats%209.52%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers and a target number. Your goal is to find two numbers in that list that add up to the target number. 

Once you find them, you need to return their positions in the list (their indices). 

For example, if your list is `[2, 7, 11, 15]` and your target is `9`, you look at the numbers and see that `2` plus `7` equals `9`. Because `2` is at position `0` and `7` is at position `1`, your answer is `[0, 1]`. 

The rules state that there is always exactly one correct pair, you cannot use the exact same number twice, and you can return the positions in any order.

---

## Intuition

The "aha" moment for this problem is asking yourself: **What am I missing?**

If you look at any number in the list, you can figure out its missing partner right away. For example, if your target is `9` and you are currently looking at `2`, your missing partner (the **remainder**) is `9 - 2`, which is `7`. 

Instead of checking every number against every other number using two loops (which is slow), you can walk through the list just once. As you visit each number, you save it in a notebook (a **map**, which is a container that stores pairs of keys and values) along with its position. 

Before saving a new number, you check your notebook: *Have I already seen the exact number I need to reach the target?* If yes, you instantly have both positions and you are done. If no, you write down the current number and keep moving.

---

## Approach

Here is the step-by-step breakdown of how the code works:

* **Create a notebook:** We set up a map called `mp` to store numbers as keys and their positions (indices) as values.
* **Start a loop:** We use a `for` loop to look at every number in the `nums` list one by one, from index `0` to the end.
* **Calculate the missing piece:** For the current number at index `i`, we subtract it from the target to find the remainder needed (`rem = target - nums[i]`).
* **Check the notebook:** We look inside our map (`mp`) to see if the remainder has already been saved.
* **Return if found:** If the remainder is found in our map, we have our two numbers. We return the current index `i` and the saved position of the remainder `mp[rem]`.
* **Save if not found:** If the remainder is not in our map yet, we save the current number and its index into our map (`mp[nums[i]] = i`) so future numbers can check against it.
* **Fallback return:** At the very end of the function, a fallback `return {1, 1};` is included, though the problem guarantees a solution will always be found before reaching this point.

---

## Time & Space Complexity

* **Time:** O(n log n) — We loop through the list of length `n` once. Inside the loop, we search and insert into a standard `map`. In C++, a standard `map` is built as a balanced binary search tree, meaning every search and insertion takes O(log n) time. Doing this for all `n` elements gives a total time complexity of O(n log n).
* **Space:** O(n) — In the worst-case scenario (where the matching pair is at the very end of the list), we store almost every number from the list into our map, taking up linear memory proportional to the size of the input.

**Can this be improved?**

Yes, it can be made faster. Because we are using a standard `map` (which keeps things sorted and takes O(log n) per operation), we can speed up lookups by switching to an **unordered_map** (a hash table). An unordered map gives us an average lookup and insertion time of O(1) (constant time).

Here is the key change to optimize the code:
```cpp
// Change this line:
// map<int, int> mp;

// To this:
unordered_map<int, int> mp;
```

With this change:
* **Improved Time Complexity:** O(n) on average, because looking up items in a hash table takes O(1) time instead of O(log n).
* **Improved Space Complexity:** Still O(n) to store the elements in the hash table.

**Theoretical limit:** O(n) time is the absolute best possible complexity for this problem because you must look at (or at least consider) every element in the array at least once. Our improved version with the unordered map reaches this theoretical limit.

---

## Edge Cases Handled

* **Duplicate numbers:** If the input is `[3, 3]` and the target is `6`, the first `3` is added to the map. When the loop reaches the second `3`, it checks the map, finds the first `3`, and successfully returns `[1, 0]`.
* **Negative numbers:** The math `target - nums[i]` works correctly with negative numbers because subtracting a negative number properly adds its absolute value.
* **Large arrays and values:** The constraints allow arrays up to `10^4` elements and number values up to `10^9`. The integer types used in C++ handle these ranges without overflowing during simple subtraction.
