![Runtime](https://img.shields.io/badge/Runtime-1%20ms%20(beats%2088.09%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-56.8%20MB%20(beats%2058.38%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers (an array) and a target number. Your goal is to find two numbers in that list that add up to the target number. Once you find them, you need to return their positions in the list (their **indices**). 

For example, if your list is `[2, 7, 11, 15]` and your target is `9`, you look at the numbers and see that `2 + 7 = 9`. Their positions are `0` and `1`. So, you return `[0, 1]`. 

You can assume there is always exactly one correct pair, you cannot use the same number twice, and you can return the positions in any order.

---

## Intuition

The naive way to solve this is to check every possible pair of numbers using two nested loops. That would take N * N time (quadratic time), which gets very slow for large lists. 

The "aha" moment is realizing you can solve this in a single pass by flipping the question around. As you look at each number in the list, ask yourself: *"What is the remaining number I need to reach the target?"* 

For example, if your target is `9` and you are currently looking at `7`, your remaining number needed is `2` (because `9 - 7 = 2`). If you keep a memory log of all the numbers you have already seen along with their positions, you can instantly check if `2` is already in your memory log. If it is, you immediately have your answer! If it is not, you just save the current number and its position into your memory log and move on to the next number.

---

## Approach

Here is how the TypeScript code implements this logic step by step:

* `const mp = new Map<number, number>();`: Creates a hash map (a key-value dictionary) named `mp` to store every number you look at as a key, and its index position as the value. This acts as your memory log.
* `for (let i = 0; i < nums.length; i++) {`: Starts a loop that looks at every number in the `nums` list one by one, starting from index `0` up to the end of the list.
* `const rem = target - nums[i];`: Calculates the **rem**aining number needed. It takes the `target` and subtracts the current number (`nums[i]`) from it.
* `if (mp.has(rem)) {`: Checks if your memory log `mp` already contains the needed remaining number.
* `return [i, mp.get(rem)!];`: If the remaining number is found in your memory log, this immediately returns an array containing your current index `i` and the stored index of the remaining number (`mp.get(rem)`).
* `mp.set(nums[i], i);`: If the remaining number is not in your memory log yet, this saves the current number (`nums[i]`) and its index (`i`) into `mp` so future numbers can check against it.
* `return [];`: Returns an empty array if the loop finishes without finding a pair (though the problem constraints guarantee a valid answer always exists).

---

## Dry Run

### Case 1: Typical case
* **Input:** `nums = [2, 7, 11, 15]`, `target = 9`

| i | nums[i] | rem (target - nums[i]) | mp before step | mp.has(rem)? | Action |
|---|---|---|---|---|---|
| 0 | 2 | 7 | empty | false | Save `2` at index `0` into `mp` |
| 1 | 7 | 2 | `2: 0` | true | Found `2` in `mp` at index `0`. Return `[1, 0]` |

### Case 2: Edge case with duplicate values
* **Input:** `nums = [3, 3]`, `target = 6`

| i | nums[i] | rem (target - nums[i]) | mp before step | mp.has(rem)? | Action |
|---|---|---|---|---|---|
| 0 | 3 | 3 | empty | false | Save `3` at index `0` into `mp` |
| 1 | 3 | 3 | `3: 0` | true | Found `3` in `mp` at index `0`. Return `[1, 0]` |

---

## Time & Space Complexity

* **Time:** `O(n)` — We loop through the list of length `n` at most once. Checking the map and inserting into the map takes `O(1)` constant time on average.
* **Space:** `O(n)` — In the worst-case scenario (like the answer being near the very end of the list), we store up to `n` numbers inside our map `mp`.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

No, this code is already optimal. 

Time complexity cannot be reduced below `O(n)` because you generally must inspect every element in the array at least once to ensure you do not miss the target pair. Space complexity cannot be improved below `O(n)` if you want linear time, because you need to remember past elements to avoid a second loop.

---

## Edge Cases Handled

* **Duplicate numbers:** Handled correctly because the map stores numbers by their value. If a duplicate appears later, it checks if its complement exists and can successfully pair with an earlier identical number.
* **Negative numbers:** Handled correctly because basic arithmetic (`target - nums[i]`) works naturally with negative values.
* **Minimum length array:** Handles arrays with exactly `2` elements (the minimum allowed by constraints) because the loop will find the pair on the second iteration at the latest.
