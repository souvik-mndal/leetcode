![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-10.9%20MB%20(beats%2020.06%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you are a biker going on a road trip. You start at point 0, and at that very beginning, your altitude is 0. 

As you ride, you move from point 0 to point 1, then to point 2, and so on. You are given an array called `gain`. Each number in `gain` tells you the change in your altitude from the previous point. For example, if `gain` has `-5` as its first number, it means you went down by 5 units. If the next number is `1`, you went up by 1 unit from that new spot.

The problem wants you to figure out the highest altitude you ever reach during the entire trip and return that number.

## Intuition

The core trick here is a concept called **prefix sum**—which just means keeping a running total as you go. 

Instead of trying to calculate every altitude from scratch every single time, we can just walk down the `gain` list step by step. We keep track of our current altitude by adding each gain to a running total. At the exact same time, we keep a separate variable to remember the highest altitude we have seen so far. 

If our current altitude goes higher than our previous record, we update our record. Since we only need to look at each gain value once, we can solve this in a single pass through the list.

## Approach

- `int maxi = 0;`: Sets up a variable to track the highest altitude we have reached. We start it at `0` because the biker starts at altitude `0`, and even if all gains are negative, we never drop below `0` as our *highest* peak since we start there.
- `int current = 0;`: Sets up a variable to track our current altitude as we move along the trip, starting at `0`.
- `for( auto &i:gain ):`: Starts a loop that looks at each altitude change one by one from the `gain` array.
- `current += i;`: Adds the current step's gain `i` to our running altitude total, updating our position on the hill.
- `maxi = max( maxi , current );`: Compares our current altitude against our record `maxi` and keeps whichever one is larger.
- `return maxi;`: Sends back the highest altitude we found after checking every number in the gain array.

## Dry Run

### Case 1: Typical mixed gains

Input: `gain = [-5, 1, 5, 0, -7]`

| Loop Item (`i`) | `current` | `maxi` | Action |
| :--- | :--- | :--- | :--- |
| (Start) | `0` | `0` | Initialize starting values |
| `-5` | `-5` | `0` | Add `-5` to current. Max is still `0`. |
| `1` | `-4` | `0` | Add `1` to current (`-5 + 1 = -4`). Max is still `0`. |
| `5` | `1` | `1` | Add `5` to current (`-4 + 5 = 1`). `1` is bigger than `0`, so `maxi` becomes `1`. |
| `0` | `1` | `1` | Add `0` to current. Max remains `1`. |
| `-7` | `-6` | `1` | Add `-7` to current (`1 - 7 = -6`). Max remains `1`. |

*Result:* Returns `1`.

### Case 2: All negative gains

Input: `gain = [-4, -3, -2]`

| Loop Item (`i`) | `current` | `maxi` | Action |
| :--- | :--- | :--- | :--- |
| (Start) | `0` | `0` | Initialize starting values |
| `-4` | `-4` | `0` | Add `-4` to current. Max is still `0`. |
| `-3` | `-7` | `0` | Add `-3` to current. Max is still `0`. |
| `-2` | `-9` | `0` | Add `-2` to current. Max is still `0`. |

*Result:* Returns `0` (since the biker never gets higher than their starting point of 0).

## Time & Space Complexity

- **Time:** `O(n)` — The code uses a single loop that goes through the `gain` array of length `n` exactly once, doing simple math at each step.
- **Space:** `O(1)` — The code only creates two integer variables (`maxi` and `current`) regardless of how large the input array is, meaning it uses a constant amount of extra memory.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this code is already fully optimal. You must look at every element in the array at least once to know its value, which means any solution requires `O(n)` time. Furthermore, because we only need to keep track of our current running sum and our maximum, we do not need to store any extra arrays, achieving the absolute best `O(1)` space complexity. No further improvement is possible.

## Edge Cases Handled

- **All negative gains:** Handled correctly because `maxi` starts at `0`, ensuring that if the biker only goes downhill from the start, the highest altitude returned is correctly `0`.
- **Single element array:** Handled smoothly because the loop will run exactly once, update the current altitude, check it against `maxi`, and return the correct result.
- **Large positive and negative swings:** Handled properly because the running sum tracks cumulative changes accurately without overflowing standard integer limits within the problem's constraints.
