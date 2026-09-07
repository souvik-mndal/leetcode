![Runtime](https://img.shields.io/badge/Runtime-36%20ms%20(beats%2060.88%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-43.8%20MB%20(beats%2067.03%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a line of people of different heights, and you want to give each person a rank number based on how tall they are. 

The shortest person gets a rank of 1. The next shortest gets a rank of 2, and so on. If two people are the exact same height, they must share the exact same rank. Also, the ranks must be packed tightly together with no gaps. If the heights are 10, 20, 40, and 30, the sorted order from shortest to tallest is 10, 20, 30, 40. Their ranks will be 1, 2, 3, and 4.

The problem gives us an array of integers called `arr`. We need to return a new array where each original number is replaced by its rank.

## Intuition

To figure out everyone's rank, we first need to know the sorted order of the numbers. If we just sort a copy of the array, the smallest numbers will appear first in order. 

Once the numbers are sorted, we can assign ranks. The very first number gets a rank of 1. As we move down the sorted list, whenever we see a brand new, larger number, we increase the rank by 1. If we see a duplicate number (a number we already gave a rank to), we skip it so it keeps the same rank. 

We can use a **hash table** (a data structure that maps keys to values, allowing us to look up information instantly) to remember each number's rank. Finally, we loop through the original array and swap each number for its lookup rank.

## Approach

- `vector<int> sorted = arr;`: We make a copy of the input array `arr` and name it `sorted` so we can arrange the numbers without losing our original order.
- `sort(sorted.begin(), sorted.end());`: We rearrange the elements in `sorted` from smallest to largest using standard sorting.
- `unordered_map<int, int> rank;`: We create a hash map named `rank` to store pairs of numbers and their assigned rank values.
- `int currentRank = 1;`: We set up a counter variable named `currentRank` starting at 1, which will track what rank to hand out next.
- `for (int num : sorted) {`: We loop through every number in our sorted copy, moving from smallest to largest.
- `if (!rank.count(num)) { rank[num] = currentRank++; }`: We check if the hash map already has an entry for `num`. If it is not there yet, we assign it the current `currentRank` value and then add 1 to `currentRank` for the next unique number. If it is already there, we do nothing.
- `for (int &num : arr) { num = rank[num]; }`: We loop through the original array `arr` using a reference (`&num`) so we can directly change its values, replacing each number with its matching rank from our `rank` map.
- `return arr;`: We return the modified `arr` containing the final ranks.

## Dry Run

### Case 1: Typical case with unique and duplicate numbers
Input: `arr = [40, 10, 20, 30]`

| Step / Loop | Variable State | Action |
| --- | --- | --- |
| Initialization | `sorted = [40, 10, 20, 30]`, `currentRank = 1` | Copy the array and set the starting rank. |
| After Sorting | `sorted = [10, 20, 30, 40]` | Elements are now ordered from smallest to largest. |
| Map building loop | `num = 10`, `rank = {10: 1}`, `currentRank = 2` | 10 is new, gets rank 1. `currentRank` becomes 2. |
| Map building loop | `num = 20`, `rank = {10: 1, 20: 2}`, `currentRank = 3` | 20 is new, gets rank 2. `currentRank` becomes 3. |
| Map building loop | `num = 30`, `rank = {10: 1, 20: 2, 30: 3}`, `currentRank = 4` | 30 is new, gets rank 3. `currentRank` becomes 4. |
| Map building loop | `num = 40`, `rank = {10: 1, 20: 2, 30: 3, 40: 4}`, `currentRank = 5` | 40 is new, gets rank 4. `currentRank` becomes 5. |
| Replacement loop | `arr = [4, 10, 20, 30]` | Replaced 40 with its rank (4). |
| Replacement loop | `arr = [4, 1, 20, 30]` | Replaced 10 with its rank (1). |
| Replacement loop | `arr = [4, 1, 2, 30]` | Replaced 20 with its rank (2). |
| Replacement loop | `arr = [4, 1, 2, 3]` | Replaced 30 with its rank (3). Return `arr`. |

### Case 2: Edge case with identical values
Input: `arr = [100, 100, 100]`

| Step / Loop | Variable State | Action |
| --- | --- | --- |
| Initialization | `sorted = [100, 100, 100]`, `currentRank = 1` | Copy the array and set starting rank. |
| After Sorting | `sorted = [100, 100, 100]` | Array remains the same. |
| Map building loop | `num = 100`, `rank = {100: 1}`, `currentRank = 2` | First 100 gets rank 1. `currentRank` becomes 2. |
| Map building loop | `num = 100`, `rank = {100: 1}`, `currentRank = 2` | Second 100 is already in map. Skipped. |
| Map building loop | `num = 100`, `rank = {100: 1}`, `currentRank = 2` | Third 100 is already in map. Skipped. |
| Replacement loop | `arr = [1, 100, 100]` | Replaced first element. |
| Replacement loop | `arr = [1, 1, 100]` | Replaced second element. |
| Replacement loop | `arr = [1, 1, 1]` | Replaced third element. Return `arr`. |

## Time & Space Complexity

**Time:** O(n log n) — Sorting the array takes O(n log n) time, where n is the number of elements. Building the hash map and replacing elements both take O(n) time. The sorting step is the slowest part, making the overall time O(n log n).
**Space:** O(n) — We create a copy of the array of size n, and our hash map stores up to n unique elements.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

No, this code is already optimal in terms of time and space complexity. 

Because we need to figure out the relative ranking of elements, we fundamentally have to sort the data (or use a similar comparison-based ordering method), which has a theoretical best time limit of O(n log n) for general values. Additionally, we must store the results and map them back, requiring O(n) space. No further algorithmic improvements can reduce the time complexity below O(n log n).

## Edge Cases Handled

- **Duplicate values:** Handled cleanly because the hash map check ensures we only assign a rank to a number the first time we see it, and identical numbers share the lookup result.
- **Negative numbers:** Handled correctly since sorting and hash maps work with negative integers just as easily as positive ones.
- **Empty input array:** If `arr` has a length of 0, the loops do not run, and the function safely returns an empty array.
- **Single element array:** A single number will be copied, sorted, mapped to rank 1, and returned as `[1]` without error.
