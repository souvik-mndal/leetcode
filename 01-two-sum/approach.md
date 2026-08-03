![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2051.45%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.11%20MB%20(beats%209.52%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers, like `[2, 7, 11, 15]`, and a goal number called a **target**, like `9`. Your job is to find two numbers in that list that add up to your target. 

Once you find them, you need to return their positions (called **indices**) in the list. For our example, the number `2` is at position `0`, and the number `7` is at position `1`. Since `2 + 7 = 9`, you would return `[0, 1]`. 

You are guaranteed that there is always exactly one correct answer, and you cannot use the exact same number twice.

---

## Intuition

The "aha" moment is realizing you do not need to check every possible pair of numbers using two nested loops, which would be very slow. 

Instead, as you walk through the list one number at a time, you can ask a simple question: *"What is the missing partner number I need to reach my target?"* 

For example, if your target is `9` and you are currently looking at `2`, your missing partner is `7` (because `9 - 2 = 7`). 

As you go, you can store every number you have already seen in a notebook (a **Hash Table**, or in C++, a `map`). Before you save your current number, you check your notebook to see if your missing partner is already waiting there. If it is, you instantly found your pair! If it is not, you write down your current number and its position in your notebook, and move on to the next number.

---

## Approach

* **Initialize a map:** Create an empty `map` called `mp` to act as our notebook. It will store each number we look at as a key, and its position in the list as a value.
* **Loop through the list:** Use a `for` loop to look at every number in `nums` one by one, starting from index `0` up to the end of the list.
* **Calculate the remainder:** For the current number `nums[i]`, find its required partner by subtracting it from the target (`int rem = target - nums[i]`).
* **Check the map:** Look inside `mp` to see if `rem` (the remainder) is already saved there.
* **Return if found:** If `rem` exists in the map, it means we previously saw our partner. We return the current index `i` and the stored index of the partner (`mp[rem]`) as our answer.
* **Save if not found:** If `rem` is not in the map yet, save our current number and its index into the map (`mp[nums[i]] = i`) so future numbers can check against it.
* **Fallback return:** Include a safety return statement (`return {1,1};`) at the end of the function to satisfy C++ syntax, even though the problem guarantees a solution will always be found inside the loop.

---

## Time & Space Complexity

* **Time:** O(n log n) — using a C++ `map` which is a self-balancing binary search tree. Every lookup and insertion takes O(log n) time, and we do this for all `n` elements in the array.
* **Space:** O(n) — storing the seen numbers and their indices inside the `map`. In the worst case, we might store almost every element before finding the match.

### Can this be improved?

Yes! The time complexity can be improved from O(n log n) down to **O(n)** time. 

The current code uses `map`, which orders its keys and takes logarithmic time for lookups. If we switch to an **unordered_map** instead, lookups and insertions happen in O(1) average time (constant time) because it uses a hash table.

Here is the key line to change at the top of your function:
```cpp
unordered_map<int, int> mp;
```

* **Resulting improved complexity:** **Time:** O(n) average. **Space:** O(n).
* **Is this the theoretical best possible complexity?** Yes. We must look at each of the `n` elements at least once to find the answer, so an O(n) time complexity is the absolute best possible limit for this problem.

---

## Edge Cases Handled

* **Negative numbers:** The logic uses standard subtraction (`target - nums[i]`). If your target is `-5` and your current number is `-2`, the remainder becomes `-3`. This handles negative numbers seamlessly.
* **Duplicate numbers:** The code checks the map *before* adding the current number. For an input like `[3, 3]` and a target of `6`:
  * At index `0`, number `3`, remainder is `3`. Map is empty. We add `mp[3] = 0`.
  * At index `1`, number `3`, remainder is `3`. We check the map, find `3` is already there at index `0`, and immediately return `[1, 0]`. It correctly avoids using the same element twice inappropriately.
* **Minimum length array:** The constraints guarantee that `nums` will have at least `2` elements, so the loop will always run at least once and find a valid pair before finishing.
