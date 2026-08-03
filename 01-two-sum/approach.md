![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.15%20MB%20(beats%2062.05%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of numbers, like [2, 7, 11, 15], and someone gives you a target number, like 9. Your job is to find the positions (indices) of two numbers in that row that add up to your target. 

For example, looking at [2, 7, 11, 15], the numbers 2 and 7 add up to 9. Since 2 is at position 0 and 7 is at position 1, your answer would be [0, 1]. 

You are guaranteed that there is always exactly one solution, you cannot use the exact same number twice, and you can return the two positions in any order.

*(Note: The code snippet provided in the prompt is actually for a completely different problem called "Median of Two Sorted Arrays", but these notes will focus on solving the Two Sum problem described above).*

## Intuition

The most obvious way to solve this is to test every single pair of numbers using two nested loops. That means checking every number against every other number until you find a pair that hits the target. But that takes a long time if the array is huge because your work grows quadratically (O(n^2)).

The "aha" moment is flipping how you think about the math. If you are looking at a number, say 2, and your target is 9, you already know you need a 7 to make it work (because 9 - 2 = 7). 

Instead of searching forward through the rest of the array, what if you keep a memory log (a hash table or dictionary) of the numbers you have **already** seen as you walk through the array? 

As you look at each new number, you simply ask your memory log: "Have I seen the number I need (target minus current number) yet?" If yes, you instantly have your answer! If no, you write down your current number and its position in your memory log and move on. This lets you solve the problem in a single pass through the array.

## Approach

* **Initialize a Hash Map:** Create an empty hash map (in C++, an unordered_map) to store numbers you have visited as keys and their positions (indices) as values.
* **Loop through the array:** Use a for loop to look at each number in the nums array one by one, keeping track of its index.
* **Calculate the complement:** For the current number, subtract it from the target to find the number you need (let us call it the complement).
* **Check the map:** Look inside your hash map to see if that complement is already stored there.
* **Return if found:** If the complement exists in your map, it means you have previously visited it. Return the position of that complement (stored in the map) and your current index.
* **Save if not found:** If the complement is not in your map yet, add the current number and its index to your map, then continue to the next iteration of the loop.

## Time & Space Complexity

* **Time:** O(n) — You iterate through the array of size n just once. Looking up an item in a hash map takes O(1) time on average. Therefore, doing a lookup for each of the n elements results in linear time.
* **Space:** O(n) — In the worst-case scenario (where the answer is at the very end of the array, or no match is found early), you store every single number from the array into the hash map, which takes space proportional to the size of the input.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal possible time complexity for this problem. You must look at the elements in the array at least once to find the solution, which inherently takes O(n) time. A hash map allows us to check for the complement in constant O(1) time, bringing the overall time complexity down from O(n^2) to O(n). No further time improvement is possible.

## Edge Cases Handled

* **Duplicate Numbers:** If the array contains identical numbers (like [3, 3] with target 6), the code handles this safely because it checks the hash map *before* adding the current number. When it sees the second 3, it finds the first 3 already sitting in the map and successfully returns both indices.
* **Negative Numbers:** The logic works seamlessly with negative numbers because standard subtraction (target minus current number) naturally accounts for signs (e.g., target 5 minus -3 equals 8).
* **Minimum Array Length:** Constraints state the array length is at least 2, so the loop will always have enough elements to check without running into out-of-bounds errors or empty array crashes.
