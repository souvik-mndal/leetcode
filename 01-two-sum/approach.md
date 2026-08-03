![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2018.88%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.24%20MB%20(beats%2042.96%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers called `nums` (like `[2, 7, 11, 15]`) and a specific goal number called `target` (like `9`). 

Your job is to find two numbers in that list that add up to the target number. Once you find them, you need to return their positions (called **indices**, like `0` and `1` because `nums[0] + nums[1]` equals `2 + 7 = 9`).

The rules are:
- There is always **exactly one solution**.
- You cannot use the same number position twice (you can't add an element to itself).
- You can return the answer positions in any order.

*(Note: The code provided actually solves a completely different problem called "Median of Two Sorted Arrays", but these notes will focus on the **Two Sum** problem as requested by the prompt theme and topics.)*

---

## Intuition

If you want to solve this without looking at every single pair of numbers (which would be slow), you can use a **hash table** (a structure that lets you look up values instantly, like a dictionary).

As you walk through the list number by number, ask yourself: *"What number do I need to reach my target?"* 

For example, if your target is `9` and you are currently looking at `2`, you need `7` (`9 - 2 = 7`). 

Instead of searching the whole list for `7`, you can just check your memory: *"Have I seen `7` already?"* If no, you save the number `2` and its position, and move on. If yes, you immediately found your pair! This lets you solve the problem in a single pass through the list.

---

## Approach

- **Initialize a hash table** (in C++, an `unordered_map`) to store numbers you have already visited and their index positions.
- **Loop through the array** `nums` from the first element to the last one.
- **Calculate the complement** for the current number by subtracting it from the `target` (`target - nums[i]`).
- **Check the hash table** to see if that complement already exists inside it.
- **If it exists**, you found your match! Return the stored index of the complement and your current index `i`.
- **If it does not exist**, add the current number and its index to the hash table, then continue to the next loop iteration.

---

## Time & Space Complexity

- **Time:** O(N) — We loop through the array of size N just once. Looking up or inserting an item in a hash table takes O(1) time on average. So N items take N * 1 = O(N) total time.
- **Space:** O(N) — In the worst-case scenario (where the pair is at the very end or doesn't exist until the last elements), we store every single element of the array inside our hash table.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal time complexity possible. To find a pair, you fundamentally have to look at the data, and doing it in a single pass using a hash table reduces the time from O(N^2) (comparing every number to every other number) down to O(N). You cannot go faster than O(N) because you must at least inspect each element once.

---

## Edge Cases Handled

- **Negative numbers:** The logic uses subtraction (`target - nums[i]`), which naturally handles negative numbers correctly (e.g., if target is `-5` and current number is `-2`, the complement is `-3`).
- **Duplicate numbers:** If an array has repeating numbers (like `[3, 3]` with target `6`), the first `3` is checked, not found in the map, and added. When the second `3` is reached, it looks up `3` in the map, finds the first one, and successfully returns both indices without breaking.
- **Minimum constraints:** The array length starts at a minimum of 2 elements, ensuring there is always a valid pair to find without running into empty-array errors.
