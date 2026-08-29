![Runtime](https://img.shields.io/badge/Runtime-82%20ms%20(beats%2098.31%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-138.06%20MB%20(beats%2091.83%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums` and a positive number called `limit`. 

You can swap any two numbers in the array, but only if the absolute difference between their values is less than or equal to `limit`. You can do this swap operation as many times as you want.

Your goal is to reorder the array so that it becomes **lexicographically smallest**. An array is lexicographically smaller if, at the very first position where two arrays differ, your array has a smaller number. In plain terms: you want the smallest possible numbers to appear as early as possible in the array.

### Example
Suppose `nums = [1, 5, 3, 9, 8]` and `limit = 2`.
* You can swap `5` and `3` because `5 - 3 = 2` (which is `<= 2`). The array becomes `[1, 3, 5, 9, 8]`.
* Next, you can swap `9` and `8` because `9 - 8 = 1` (which is `<= 2`). The array becomes `[1, 3, 5, 8, 9]`.
* Output: `[1, 3, 5, 8, 9]`.

---

## Intuition

The key insight is **transitivity**. 

If number A can swap with number B, and number B can swap with number C, then A, B, and C all belong to the same connected **group**. Once numbers belong to the same group, you can rearrange them into **any order you want** across their original positions.

So how do we find these groups?
1. If we sort all the numbers in ascending order, any two adjacent numbers whose difference is `<= limit` belong to the same group.
2. If two adjacent sorted numbers have a difference `> limit`, the chain breaks and a new group begins.

Once a group is identified:
* We collect all the **original index positions** where this group's numbers were located in the original array.
* We sort those index positions from smallest to largest.
* We place the smallest value into the smallest index, the second-smallest value into the second-smallest index, and so on.

---

## Approach

Here is how the solution works step-by-step:

* `int[] sortedIndices = new int[n];`  
  Creates an array of index positions from `0` to `n - 1`.

* `Array.Sort(sortedIndices, (a, b) => nums[a].CompareTo(nums[b]));`  
  Sorts the indices based on the values stored in `nums`. This lets us inspect the elements in ascending order without losing track of where they originally lived.

* `while (start < n)`  
  Iterates through the sorted indices to identify each connected group of elements.

* `while (end + 1 < n && nums[sortedIndices[end + 1]] - nums[sortedIndices[end]] <= limit)`  
  Expands the current group as long as the difference between adjacent sorted values is within `limit`.

* `int[] groupIndices = new int[groupSize];`  
  Extracts all original index positions belonging to the current connected group.

* `Array.Sort(groupIndices);`  
  Sorts these original index positions from smallest to largest so we can fill the final output array from left to right.

* `result[groupIndices[i]] = nums[sortedIndices[start + i]];`  
  Assigns the smallest available value in the group to the smallest available index position in the group.

---

## Dry Run

### Case 1: Typical case with multiple groups
* **Input:** `nums = [1, 5, 3, 9, 8]`, `limit = 2`
* **Sorted order by value:** `1` (idx 0), `3` (idx 2), `5` (idx 1), `8` (idx 4), `9` (idx 3)
* **`sortedIndices` array:** `[0, 2, 1, 4, 3]`

| Step | `start` | `end` | Group Values | `groupIndices` (sorted) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | 2 | `[1, 3, 5]` | `[0, 1, 2]` | Chain breaks after 5 because `8 - 5 = 3 > 2`. Fill `result[0]=1`, `result[1]=3`, `result[2]=5`. |
| 2 | 3 | 4 | `[8, 9]` | `[3, 4]` | End of array reached. Fill `result[3]=8`, `result[4]=9`. |

* **Final Result:** `[1, 3, 5, 8, 9]`

---

### Case 2: Duplicate values and separate chains
* **Input:** `nums = [1, 7, 6, 18, 2, 1]`, `limit = 3`
* **Sorted order by value:** `1` (idx 0), `1` (idx 5), `2` (idx 4), `6` (idx 2), `7` (idx 1), `18` (idx 3)
* **`sortedIndices` array:** `[0, 5, 4, 2, 1, 3]`

| Step | `start` | `end` | Group Values | `groupIndices` (sorted) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | 2 | `[1, 1, 2]` | `[0, 4, 5]` | Chain breaks after 2 because `6 - 2 = 4 > 3`. Fill `result[0]=1`, `result[4]=1`, `result[5]=2`. |
| 2 | 3 | 4 | `[6, 7]` | `[1, 2]` | Chain breaks after 7 because `18 - 7 = 11 > 3`. Fill `result[1]=6`, `result[2]=7`. |
| 3 | 5 | 5 | `[18]` | `[3]` | Single element group. Fill `result[3]=18`. |

* **Final Result:** `[1, 6, 7, 18, 1, 2]`

---

## Time & Space Complexity

### Current Complexity
* **Time Complexity:** **O(N log N)**  
  Sorting the index array of size N takes `O(N log N)` time. Splitting into groups and sorting each group's index set takes `O(N log N)` total time across all groups combined.
* **Space Complexity:** **O(N)**  
  We store the `sortedIndices` array, the `result` array, and temporary `groupIndices` arrays, which require `O(N)` extra memory in total.

### Is this optimal?
**Yes, this is already optimal.** 

To group elements by value difference on arbitrary numbers up to `10^9`, sorting the array values is necessary. Comparison-based sorting has a proven lower bound of `O(N log N)` time. Thus, no algorithm can guarantee a better time complexity for general inputs.

---

## Edge Cases Handled

* **Duplicate Numbers in Array:** Handled automatically because the difference between identical numbers is `0`, which is always `<= limit`. They stay in the same group.
* **`limit` is very small (e.g., 0 or 1):** Elements that cannot pair with anything form isolated groups of size 1 and remain in their original positions.
* **`limit` is very large:** All elements join a single group, resulting in a fully sorted array.
* **Large Integer Values (up to 10^9):** Difference calculations do not overflow standard integer limits because `nums` contains positive integers and elements are subtracted in non-decreasing order (`nums[sortedIndices[end + 1]] - nums[sortedIndices[end]] >= 0`).
