![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-12.4%20MB%20(beats%2052.24%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have two rows of numbers, and both rows are already sorted from smallest to largest. 

The first row, called **nums1**, has extra empty space at the end. The problem gives you two numbers: **m**, which tells you how many actual items are in the first row, and **n**, which tells you how many items are in the second row, called **nums2**. The total size of **nums1** is always **m + n** so that it has just enough room to hold everything.

Your job is to take all the numbers from **nums2** and put them into **nums1** so that the final combined list is completely sorted from smallest to largest. You have to do this directly inside **nums1** without returning anything.

For example, if **nums1** is `[1, 2, 3, 0, 0, 0]` with **m = 3**, and **nums2** is `[2, 5, 6]` with **n = 3**, you need to rearrange and combine them so **nums1** becomes `[1, 2, 2, 3, 5, 6]`.

## Intuition

The core idea here is to treat the two arrays like a two-way exchange program. 

We start by looking at the very end of the valid numbers in **nums1** (using pointer **l**) and the very beginning of **nums2** (using pointer **r**). If the number at the end of **nums1** is bigger than the number at the beginning of **nums2**, it means they are out of order relative to each other, so we swap them. Then we move **l** to the left and **r** to the right to check the next pair. We keep doing this until the largest numbers from **nums2** are pushed over into **nums1**.

Once that initial sorting handshake is done, we copy any remaining elements from **nums2** into the empty tail slots of **nums1**. Finally, because the swapping process leaves things partially unordered, we call a built-in sorting function to put everything neatly in order.

## Approach

Here is step-by-step how the code works:

- `int l = m - 1;`: Creates a pointer named **l** starting at the last valid element of **nums1** (at index **m - 1**).
- `int r = 0;`: Creates a pointer named **r** starting at the very first element of **nums2** (at index **0**).
- `while (l >= 0 && r < n)`: Starts a loop that runs as long as our **l** pointer stays inside the valid part of **nums1** and our **r** pointer stays inside **nums2**.
- `if (nums1[l] > nums2[r])`: Checks if the number at the end of the active part of **nums1** is greater than the number at the beginning of **nums2**.
- `swap(nums1[l], nums2[r]);`: Swaps the two numbers if **nums1**'s number is larger, ensuring smaller elements migrate to **nums1** and larger ones migrate to **nums2**.
- `l--; r++;`: Moves the **l** pointer one step to the left and the **r** pointer one step to the right after a successful swap.
- `else { break; }`: Stops the comparison loop immediately if the number in **nums1** is smaller than or equal to the number in **nums2**, because the arrays are already sorted enough for this phase.
- `for (int i = m; i < m + n; i++)`: Loops through the remaining empty slots in **nums1** starting from index **m**.
- `nums1[i] = nums2[i - m];`: Copies the leftover elements from **nums2** directly into the tail of **nums1**.
- `sort(nums1.begin(), nums1.end());`: Sorts the entire **nums1** array from start to finish so that everything is in correct non-decreasing order.

## Dry Run

### Case 1: Typical case
**Inputs:** `nums1 = [1, 2, 3, 0, 0, 0]`, `m = 3`, `nums2 = [2, 5, 6]`, `n = 3`

| l | r | nums1 state | nums2 state | Action |
|---|---|---|---|---|
| 2 | 0 | `[1, 2, 3, 0, 0, 0]` | `[2, 5, 6]` | Compare `nums1[2]` (3) > `nums2[0]` (2). Swap them. `l` becomes 1, `r` becomes 1. |
| 1 | 1 | `[1, 2, 2, 0, 0, 0]` | `[3, 5, 6]` | Compare `nums1[1]` (2) > `nums2[1]` (5) is false. Break out of the loop. |
| - | - | `[1, 2, 2, 3, 5, 6]` | `[3, 5, 6]` | Copy remaining elements of `nums2` into `nums1`, then sort the whole array. Final result. |

### Case 2: Empty nums2
**Inputs:** `nums1 = [1]`, `m = 1`, `nums2 = []`, `n = 0`

| l | r | nums1 state | nums2 state | Action |
|---|---|---|---|---|
| 0 | 0 | `[1]` | `[]` | Loop condition `r < n` (0 < 0) is false immediately. Loop does not run. |
| - | - | `[1]` | `[]` | Copy loop `i = 1` to `1` does not run since `m + n = 1`. |
| - | - | `[1]` | `[]` | Sort runs on `[1]`. Final result is `[1]`. |

## Time & Space Complexity

- **Time:** O((m + n) * log(m + n)) — The `sort` function at the end dominates the performance, taking logarithmic time relative to the total number of elements **m + n**.
- **Space:** O(1) — We sort and modify **nums1** entirely in place, using a constant amount of extra memory regardless of the input sizes.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

It can be improved. While the space complexity is already optimal at O(1), the time complexity can be made faster. 

The current solution relies on a final sorting step that takes O((m + n) * log(m + n)) time because it mixes elements and then re-sorts them from scratch. However, since both input arrays are *already* sorted individually, we can merge them in linear time without ever needing a sorting function. 

To achieve this, we can change the approach by placing a pointer at the very end of the combined storage space in **nums1** (at index **m + n - 1**), and two other pointers at the ends of the actual valid data in **nums1** and **nums2** (at **m - 1** and **n - 1** respectively). By comparing the elements from the back and filling **nums1** from right to left, we always put the largest remaining element in its correct final spot immediately. This eliminates the need for swaps and sorting entirely.

Here is what the optimized code snippet looks like:

```cpp
int p1 = m - 1;
int p2 = n - 1;
int p = m + n - 1;

while (p2 >= 0) {
    if (p1 >= 0 && nums1[p1] > nums2[p2]) {
        nums1[p--] = nums1[p1--];
    } else {
        nums1[p--] = nums2[p2--];
    }
}
```

- `int p = m + n - 1;`: Sets our writing pointer to the very last slot of **nums1** where the largest overall number belongs.
- `if (p1 >= 0 && nums1[p1] > nums2[p2])`: Checks if **nums1** still has elements left and if its current back element is larger than the back element of **nums2**.
- `nums1[p--] = nums1[p1--];`: Places the larger element from **nums1** into the write position, then shifts both pointers backward.
- `else { nums1[p--] = nums2[p2--]; }`: Otherwise, takes the element from **nums2** because it is larger (or **nums1** is fully processed), placing it and shifting pointers.

- **Improved Time Complexity:** O(m + n) — Every element from both arrays is visited and placed into its final position in a single pass.
- **Theoretical Best Complexity:** O(m + n) time and O(1) space is the absolute theoretical limit for this problem, because you must look at every element at least once to merge them. The improved version reaches this optimal limit.

## Edge Cases Handled

- **Empty `nums2` (`n = 0`):** The pointer loop safely skips execution, and `nums1` remains untouched and correctly sorted.
- **Empty `nums1` data (`m = 0`):** The pointer **l** starts at `-1`, causing the loop to skip, and all elements from `nums2` are cleanly copied over into `nums1`.
- **Negative numbers:** Because the code relies on standard comparison operators (`>`), negative numbers are ordered correctly from smallest to largest without any special handling.
- **Duplicate values:** Duplicate numbers across the two arrays are handled safely since the comparison uses strict greater-than (`>`), preserving relative order and sorting stability where applicable.
