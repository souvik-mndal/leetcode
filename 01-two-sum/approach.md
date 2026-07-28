![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2099.29%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-47.08%20MB%20(beats%2058.98%25)-yellow?style=for-the-badge)

---

## Problem in Plain English

You are given a list of numbers and a single **target** sum. Your job is to find **two distinct numbers** in that list that add up exactly to the target sum. 

Once you find them, you return their **index positions** (where they sit in the list), not the numbers themselves.

* **Example:** If the list is `[2, 7, 11, 15]` and the target is `9`:
  * `2 + 7 = 9`
  * `2` is at position `0` and `7` is at position `1`.
  * Output: `[0, 1]`

You can assume there is always exactly one correct pair, and you cannot use the exact same position twice.

---

## Intuition

The naive way is to pick every number and test it against every other number. That requires two loops and is slow.

Instead, flip the question as you walk through the list:
> *"I am looking at the number `X`. To reach `target`, what number `Y` do I need? Have I already seen `Y` earlier in the list?"*

The needed number `Y` is just `target - X` (the **remainder**). 

To check if you've seen `Y` before in instant time, you use a **hash table** (a lookup map that stores key-value pairs). As you walk through the list, you keep a running record of every number you've seen so far, along with its index position.

---

## Approach

Here is how the code works line-by-line:

* **Create a lookup map:** Initialize an empty **hash table** (`HashMap`) where keys are the numbers seen so far, and values are their array indices.
* **Loop through the array:** Start at index `0` and move step-by-step through the list.
* **Calculate the missing piece:** For the current number `nums[i]`, figure out what matching number is needed: `rem = target - nums[i]`.
* **Check the map:** Look up if `rem` is already in your hash table.
  * **If found:** You've found your pair! Immediately return an array containing the current index `i` and the stored index `map.get(rem)`.
  * **If not found:** Save the current number and its index into the map (`map.put(nums[i], i)`), then move to the next number.
* **Default return:** Return an empty array if no match is found (though the problem guarantees a solution exists).

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — You loop through the list of `n` elements at most once. Looking up and adding items to a hash map takes **O(1)** (constant average time).
* **Space Complexity:** **O(n)** — In the worst-case scenario, you might store up to `n - 1` numbers in the hash table before finding the matching pair.

### Is this optimal?
**Yes, this is already the most optimal complexity possible.** 

* **Time:** You must inspect each number at least once to know if it forms the sum. Therefore, **O(n)** is the theoretical speed limit.
* **Space:** To achieve **O(n)** time without sorting the original array, you must use **O(n)** extra memory to store previously seen numbers. 

No further time optimizations exist for this problem.

---

## Edge Cases Handled

* **Duplicate Numbers:** If the input is `nums = [3, 3]` and `target = 6`, the code checks if the needed `3` is in the map *before* adding the second `3`. It safely matches the second `3` to the first `3` without overwriting keys.
* **Negative Numbers:** The math `target - nums[i]` handles negative values correctly without extra logic (e.g., if `target = -5` and `nums[i] = -2`, the required remainder is `-3`).
* **Minimum Input Size:** Works seamlessly for the smallest allowed array size of 2 elements.
* **Index Order:** The problem allows indices in any order. The code returns `[current_index, past_index]`, which is fully valid.
