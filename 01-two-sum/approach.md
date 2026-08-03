![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.12%20MB%20(beats%2062.05%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of numbers, like [2, 7, 11, 15], and a goal number, like 9. Your job is to find two numbers in that row that add up to your goal. Once you find them, you need to return their positions (called indices, starting from 0). 

For example, looking at [2, 7, 11, 15], the first number is 2 (at position 0) and the second number is 7 (at position 1). Since 2 + 7 equals 9, your answer is [0, 1]. 

The rules say there is always exactly one correct pair, you cannot use the same number twice, and you can return the answers in any order.

*(Note: The code provided in the prompt actually solves a different problem called "Median of Two Sorted Arrays", but these notes will focus on explaining the classic **Two Sum** problem and its optimal hash table solution as requested by the problem description).*

## Intuition

The naive way to solve this is to check every possible pair of numbers using two nested loops. That means taking the first number and adding it to every other number, then taking the second number and doing the same. That takes O(N^2) time, which is too slow for large lists.

The "aha" moment is this: as you look at any number, you already know your target total. Instead of guessing a second number, you can instantly calculate what number you *need* to reach the target. 

For example, if your target is 9 and you are currently looking at the number 2, you need a 7. 

Instead of searching the whole list again for 7, you can use a **Hash Table** (a data structure in C++ called `unordered_map` that lets you store and look up information instantly). As you walk through the list, you save every number you have seen so far along with its position. Before saving a new number, you just ask the hash table: "Have I already seen the exact number I need to reach my target?" If yes, you instantly have both positions and you are done!

## Approach

* **Create a Hash Table:** Set up an empty hash table to keep track of the numbers you have visited and their positions in the array.
* **Loop Through the Array:** Use a for-loop to look at each number in the `nums` array one by one, keeping track of its index.
* **Calculate the Needed Value:** For the current number, subtract it from the `target` to find the complement (the partner number needed to reach the target).
* **Check the Hash Table:** Look inside your hash table to see if that complement number is already stored there.
* **Return if Found:** If the complement exists in the hash table, you have found your pair. Return the index of the complement and the index of your current number immediately.
* **Store and Continue:** If the complement is not in the hash table yet, add the current number and its index to the hash table, then move on to the next number in the array.

## Time & Space Complexity

* **Time:** O(N) — We loop through the array of size N just once. Looking up items in a hash table takes O(1) constant time on average, so the total time grows directly in proportion to the size of the array.
* **Space:** O(N) — In the worst-case scenario (where the pair is at the very end or doesn't exist until the last elements), we store every number from the array inside our hash table.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal time complexity possible for this problem. You cannot find the answer faster than O(N) because, in the worst case, you must at least look at every element in the array once. Therefore, no further time improvement is possible.

## Edge Cases Handled

* **Negative Numbers:** The logic uses standard subtraction (`target - nums[i]`), which naturally handles negative numbers correctly (e.g., target 5 minus -3 equals 8).
* **Duplicate Numbers:** If the array contains duplicate values (like [3, 3] with target 6), the code checks the hash table *before* saving the current number, preventing a number from accidentally matching with itself at the exact same index.
* **Minimum Array Length:** The constraints state the array has at least 2 elements (`2 <= nums.length`), so there is always a valid pair to find without worrying about empty or single-element inputs crashing the code.
