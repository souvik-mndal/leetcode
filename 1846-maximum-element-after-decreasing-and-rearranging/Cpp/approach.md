![Runtime](https://img.shields.io/badge/Runtime-11%20ms%20(beats%2036.34%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-55.2%20MB%20(beats%2090.08%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of positive numbers called `arr`. You want to make this list as large as possible overall, but you have to follow three strict rules:

1. **The first number must always be 1.** No matter what was there originally, you must force the first element to become 1.
2. **Neighbors can only differ by at most 1.** If you look at any two numbers sitting next to each other, the absolute difference between them cannot be greater than 1. For example, if a number is 5, its neighbor can only be 4, 5, or 6. It cannot jump straight to 7.
3. **You are allowed to change the numbers.** You can rearrange them into any order you like, and you can decrease any number to a smaller positive integer. (Notice you can *only* decrease numbers, never increase them).

Your goal is to follow these rules and then find the **maximum possible value** of the largest element left in the array.

For example, if your input is `[2, 2, 1, 2, 1]`, you can rearrange it and adjust it so it becomes `[1, 2, 2, 2, 1]`. All neighbors stay close, it starts with 1, and the largest element is `2`.

---

## Intuition

To maximize the numbers while keeping neighboring gaps small, we want the values to grow as gradually and smoothly as possible from left to right. 

* **Sorting helps us grow smoothly:** If we sort the array from smallest to largest, we can build our valid sequence step by step from the beginning.
* **Fix the start:** Rule 1 says the first element must be 1. If our sorted array starts with anything larger than 1, we can safely pull it down to 1 without breaking any rules.
* **Keep gaps small:** As we move through the sorted array, if a number is much larger than the one right before it, we don't need it to be that big. We can pull it down so it is at most 1 greater than its predecessor (i.e., `arr[j-1] + 1`). Because we are allowed to decrease numbers and rearrange them, this greedy strategy ensures we push values up as high as legally possible without violating the gap rule.

---

## Approach

* `sort( arr.begin() , arr.end() );`
  Sorts the entire array in ascending order so that the smallest numbers come first, allowing us to build our valid sequence smoothly from left to right.
* `if( arr[0] != 1 ) { arr[0]=1; }`
  Checks if the first element is something other than 1. If it is not 1, forces it to become 1 to satisfy the problem's starting condition.
* `for( int j=1 ; j<arr.size() ; j++ )`
  Loops through the array starting from the second element (index 1) all the way to the end.
* `if( abs(arr[j]-arr[j-1]) > 1 )`
  Checks if the gap between the current element and the previous element is greater than 1, which violates the neighbor rule.
* `arr[j] = arr[j-1]+1;`
  If the gap was too big, lowers the current element so it is exactly 1 greater than the previous element, keeping the sequence tightly packed.
* `return *max_element( arr.begin() , arr.end());`
  Scans the final modified array to find and return the largest element.

---

## Dry Run

### Case 1: Typical case with large gaps (`arr = [100, 1, 1000]`)

| Step | `j` | `arr[j]` (current) | `arr[j-1]` (previous) | Action | Array State |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | - | - | Sort array | `[1, 100, 1000]` |
| Start Fix | 0 | 1 | - | `arr[0]` is already 1, no change | `[1, 100, 1000]` |
| Loop 1 | 1 | 100 | 1 | Gap `100 - 1 = 99` (> 1), so set `arr[1] = 1 + 1` | `[1, 2, 1000]` |
| Loop 2 | 2 | 1000 | 2 | Gap `1000 - 2 = 998` (> 1), so set `arr[2] = 2 + 1` | `[1, 2, 3]` |
| Finish | - | - | - | Return max element (`3`) | `[1, 2, 3]` |

### Case 2: Array already valid (`arr = [1, 2, 3, 4, 5]`)

| Step | `j` | `arr[j]` (current) | `arr[j-1]` (previous) | Action | Array State |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | - | - | Sort array (already sorted) | `[1, 2, 3, 4, 5]` |
| Start Fix | 0 | 1 | - | `arr[0]` is already 1, no change | `[1, 2, 3, 4, 5]` |
| Loop 1 | 1 | 2 | 1 | Gap `2 - 1 = 1` (not > 1), do nothing | `[1, 2, 3, 4, 5]` |
| Loop 2 | 2 | 3 | 2 | Gap `3 - 2 = 1` (not > 1), do nothing | `[1, 2, 3, 4, 5]` |
| Loop 3 | 3 | 4 | 3 | Gap `4 - 3 = 1` (not > 1), do nothing | `[1, 2, 3, 4, 5]` |
| Loop 4 | 4 | 5 | 4 | Gap `5 - 4 = 1` (not > 1), do nothing | `[1, 2, 3, 4, 5]` |
| Finish | - | - | - | Return max element (`5`) | `[1, 2, 3, 4, 5]` |

---

## Time & Space Complexity

* **Time:** `O(N log N)` — We sort the array, which takes `N log N` time (where N is the number of elements in `arr`), followed by a single linear pass through the array (`O(N)`) and a linear search for the maximum element (`O(N)`). The sorting dominates the runtime.
* **Space:** `O(1)` or `O(log N)` auxiliary space — We modify the array in place without using extra data structures. (The sorting function itself uses a small amount of stack space for recursion, typically `O(log N)`).

**Is this already the most optimal possible complexity, or can it be improved?**

The time complexity is already optimal. Because the array elements can start in any random order and must be rearranged and checked relative to one another, we fundamentally need to bring order to the data (sorting) before we can enforce adjacent constraints. Sorting takes `N log N` time, which matches the theoretical lower bound for comparison-based sorting. 

*(Note: If the problem constraints guaranteed numbers fell within a small, fixed range, counting sort could achieve `O(N)` time, but because `arr[i]` can be as large as 10^9, comparison sorting is required).*

---

## Edge Cases Handled

* **First element not 1:** If the smallest number after sorting is greater than 1 (e.g., `[5, 6, 7]`), the code correctly resets `arr[0]` to 1.
* **Massive gaps between numbers:** If numbers jump from 1 to 1,000,000 (e.g., `[1, 1000000]`), the code catches the large gap and safely scales the second number down to 2.
* **Already valid arrays:** If the array is already perfectly sorted and spaced (e.g., `[1, 2, 3]`), the loop skips adjustments and correctly preserves the values.
* **Duplicate values:** If the array contains many identical numbers (e.g., `[2, 2, 2, 2]`), sorting groups them together and gaps remain 0 or 1, which safely avoids unwanted modifications.
