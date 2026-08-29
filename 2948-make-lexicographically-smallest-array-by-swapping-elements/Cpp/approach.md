![Runtime](https://img.shields.io/badge/Runtime-86%20ms%20(beats%2097.46%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-138.17%20MB%20(beats%2091.83%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of positive numbers named `nums` and a number named `limit`. 

You can swap any two numbers in `nums` as long as the absolute difference between their values is less than or equal to `limit`. You can perform as many swaps as you want.

Your goal is to make the array **lexicographically smallest**. An array is lexicographically smaller than another if, at the very first index where they differ, it has a smaller value. In plain terms: you want to get the smallest possible numbers as far to the left of the array as possible.

For example, if `nums = [1, 5, 3, 9, 8]` and `limit = 2`:
* We can swap `5` and `3` (difference is 2, which is <= 2). The array becomes `[1, 3, 5, 9, 8]`.
* We can swap `9` and `8` (difference is 1, which is <= 2). The array becomes `[1, 3, 5, 8, 9]`.
* Result: `[1, 3, 5, 8, 9]`.

## Intuition

The key insight is **transitivity**: if element A can swap with element B, and element B can swap with element C, then element A can move to element C's position by using B as a stepping stone.

This means elements form **connected groups**. Any number inside a group can swap around to occupy any index position owned by that group. Different groups cannot interact with each other.

To make the final array as small as possible:
1. Identify which numbers belong to the same group.
2. Collect all the original positions (indices) occupied by that group.
3. Sort the numbers in the group from smallest to largest.
4. Put the smallest numbers into the leftmost available index positions of that group.

How do we easily find these groups? Sort all `(value, index)` pairs by value! In a sorted list of values, any two adjacent values with a difference greater than `limit` mark the end of one group and the start of a new one.

## Approach

Here is the step-by-step breakdown of how the code implements this logic:

* `vector<pair<int, int>> a;`  
  Creates a list of pairs to store each value alongside its original index in `nums`.

* `for (int i = 0; i < n; i++) a.push_back(make_pair(nums[i], i));`  
  Fills `a` with `(nums[i], i)` pairs so we remember where each value originally lived.

* `sort(a.begin(), a.end());`  
  Sorts all pairs in ascending order based on their values.

* `vector<int> tempVal, tempInd;`  
  Creates temporary vectors to store the values (`tempVal`) and original index positions (`tempInd`) for the group currently being gathered.

* `tempVal.push_back(a[0].first); tempInd.push_back(a[0].second);`  
  Starts the first group using the smallest value and its original index from the sorted list.

* `for (int i = 1; i <= n; i++)`  
  Loops through the sorted pairs from index 1 up to `n` (using `i = n` as a signal to process the final group).

* `if (i == n || a[i].first - a[i - 1].first > limit)`  
  Checks if we reached the end of the list or if the gap between the current value and previous value exceeds `limit`. If either is true, the current group is complete and ready to be placed.

* `sort(tempInd.begin(), tempInd.end());`  
  Sorts the collected index positions in ascending order so we can assign values from left to right.

* `for (int j = 0; j < m; j++) nums[tempInd[j]] = tempVal[j];`  
  Overwrites `nums` by placing the smallest values of the group into the leftmost available index positions of that group.

* `tempVal.clear(); tempInd.clear();`  
  Empties the temporary vectors to get ready for building the next group.

* `if (i < n) { tempVal.push_back(a[i].first); tempInd.push_back(a[i].second); }`  
  If we haven't reached the end of the array, adds the current pair to start the new group.

* `return nums;`  
  Returns the updated array, which is now in its lexicographically smallest form.

## Dry Run

### Case 1: Standard case with multiple multi-element groups
`nums = [1, 5, 3, 9, 8]`, `limit = 2`

After sorting `a`: `[(1,0), (3,2), (5,1), (8,4), (9,3)]`

| `i` | `a[i]` | Action | `tempVal` | `tempInd` | State of `nums` |
| --- | --- | --- | --- | --- | --- |
| Initial | - | Initialize with `a[0]` | `[1]` | `[0]` | `[1, 5, 3, 9, 8]` |
| 1 | `(3,2)` | Difference `3 - 1 = 2 <= 2`. Add to group. | `[1, 3]` | `[0, 2]` | `[1, 5, 3, 9, 8]` |
| 2 | `(5,1)` | Difference `5 - 3 = 2 <= 2`. Add to group. | `[1, 3, 5]` | `[0, 2, 1]` | `[1, 5, 3, 9, 8]` |
| 3 | `(8,4)` | Difference `8 - 5 = 3 > 2`. Flush Group 1. | `[8]` (after clear) | `[3]` (after clear) | `[1, 3, 5, 9, 8]` |
| 4 | `(9,3)` | Difference `9 - 8 = 1 <= 2`. Add to group. | `[8, 9]` | `[4, 3]` | `[1, 3, 5, 9, 8]` |
| 5 | `n/a` | `i == n`. Flush Group 2. | `[]` | `[]` | `[1, 3, 5, 8, 9]` |

Final Output: `[1, 3, 5, 8, 9]`

---

### Case 2: Array where limit breaks elements into singletons and small groups
`nums = [1, 7, 28, 19, 10]`, `limit = 3`

After sorting `a`: `[(1,0), (7,1), (10,4), (19,3), (28,2)]`

| `i` | `a[i]` | Action | `tempVal` | `tempInd` | State of `nums` |
| --- | --- | --- | --- | --- | --- |
| Initial | - | Initialize with `a[0]` | `[1]` | `[0]` | `[1, 7, 28, 19, 10]` |
| 1 | `(7,1)` | Difference `7 - 1 = 6 > 3`. Flush Group 1 (`[1]`). | `[7]` | `[1]` | `[1, 7, 28, 19, 10]` |
| 2 | `(10,4)` | Difference `10 - 7 = 3 <= 3`. Add to group. | `[7, 10]` | `[1, 4]` | `[1, 7, 28, 19, 10]` |
| 3 | `(19,3)` | Difference `19 - 10 = 9 > 3`. Flush Group 2 (`[7, 10]`). | `[19]` | `[3]` | `[1, 7, 28, 19, 10]` |
| 4 | `(28,2)` | Difference `28 - 19 = 9 > 3`. Flush Group 3 (`[19]`). | `[28]` | `[2]` | `[1, 7, 28, 19, 10]` |
| 5 | `n/a` | `i == n`. Flush Group 4 (`[28]`). | `[]` | `[]` | `[1, 7, 28, 19, 10]` |

Final Output: `[1, 7, 28, 19, 10]`

## Time & Space Complexity

* **Time Complexity:** **O(n log n)**
  * Sorting the vector `a` of size `n` takes `O(n log n)` time.
  * Inside the loop, sorting `tempInd` for each group takes `O(k log k)` time, where `k` is the size of that group. Summing `O(k log k)` across all groups takes at most `O(n log n)` total time.
  * Placing values back into `nums` takes `O(n)` total time.
  * Overall time complexity is dominated by sorting: **O(n log n)**.

* **Space Complexity:** **O(n)**
  * The pair vector `a` takes `O(n)` auxiliary space.
  * The temporary vectors `tempVal` and `tempInd` take at most `O(n)` space combined across execution.
  * Overall space complexity is **O(n)**.

### Is this solution optimal?

**Yes, this complexity is already optimal.**

This problem requires sorting elements within their connected components. In the extreme case where `limit` is very large (e.g., infinity), all elements belong to a single group, and the problem reduces directly to standard array sorting. Since comparison-based sorting has a proven theoretical lower bound of `O(n log n)` time, any algorithm solving this problem must take at least `O(n log n)` time in the worst case. 

No further optimization can reduce the Big-O time or space complexity bounds.

## Edge Cases Handled

* **Single-element array (`nums.length == 1`):** The main loop runs for `i = 1` immediately triggering `i == n`, placing the single element into `nums[0]` and returning correctly without out-of-bounds access.
* **No valid swaps possible (`limit` very small):** Every element forms its own group of size 1. The code processes each element individually and returns the array unchanged.
* **All elements in one group (`limit` very large):** The entire array forms a single group. All indices are collected, sorted, and overwritten with the sorted values, resulting in a fully sorted array.
* **Duplicate values in `nums`:** `std::sort` handles identical values smoothly. Their absolute difference is `0 <= limit`, so identical values naturally get grouped together into the same component.
* **Large value ranges (values up to 10^9):** Differences fit safely within standard 32-bit signed integers without integer overflow issues because all input values are positive.
