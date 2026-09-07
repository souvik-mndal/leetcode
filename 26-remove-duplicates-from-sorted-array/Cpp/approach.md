![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-22.5%20MB%20(beats%2080.57%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The goal is to take a list of numbers (`nums`) that is already sorted from smallest to largest and remove all repeated numbers in-place. "In-place" means you must modify the given list directly without creating a second list.

After removing duplicates:
1. Every unique number should appear only once at the beginning of the array.
2. The numbers must stay in their original sorted order.
3. You must return $k$, the total count of unique numbers.

Anything left in the array after index $k - 1$ does not matter and can be ignored.

**Example:**
If `nums = [1, 1, 2]`, the unique numbers are `1` and `2`. Your function should rewrite `nums` so its first two elements are `1` and `2`, and return `k = 2`.

---

## Intuition

Since the array is already sorted, identical numbers always sit right next to each other. This is the key insight.

We can use two pointers (index markers):
- A **slow pointer** (`l`) keeps track of where the last unique number was saved.
- A **fast pointer** (`r`) scans forward through the array to discover new numbers.

When the fast pointer finds a number that is different from the one at the slow pointer, we know we found a new unique value. We write this new value right next to our slow pointer and advance the slow pointer.

---

## Approach

* `` `int l = 0 , r = 1;` ``: Initialize `l` at index 0 to mark the position of the first unique element. Initialize `r` at index 1 to scan through the rest of the array.
* `` `while( r < nums.size() )` ``: Start a loop that runs until `r` reaches the end of the array.
* `` `if( nums[l] != nums[r] )` ``: Compare the element at `r` with the last unique element found at `l`. If they are different, we have found a new unique number.
* `` `nums[l+1] = nums[r];` ``: Place this newly discovered unique number into the position right after `l`.
* `` `l++;` ``: Move `l` forward by one position so it now points to this newly updated unique number.
* `` `r++;` ``: Move `r` forward to check the next element in the array regardless of whether a match was found.
* `` `return l+1;` ``: Once `r` reaches the end, return `l + 1`. Because indices start at 0, `l + 1` is the total count of unique numbers found.

---

## Dry Run

### Case 1: Simple array with duplicates (`nums = [1, 1, 2]`)

| Step | `r` | `l` | `nums[l]` | `nums[r]` | Action |
| --- | --- | --- | --- | --- | --- |
| Start | 1 | 0 | 1 | 1 | `nums[0] == nums[1]` (1 == 1). Duplicate found. Move `r` forward. |
| 1 | 2 | 0 | 1 | 2 | `nums[0] != nums[2]` (1 != 2). Copy `nums[2]` to `nums[1]`. Increment `l` to 1. Move `r`. |
| End | 3 | 1 | 2 | - | Loop ends because `r == nums.size()`. Return `l + 1 = 2`. |

**Final state of `nums`:** `[1, 2, 2]` (First `k = 2` elements are `[1, 2]`).

---

### Case 2: Array with multiple repeating groups (`nums = [0, 0, 1, 1, 1, 2, 2, 3, 3, 4]`)

| Step | `r` | `l` | `nums[l]` | `nums[r]` | Action |
| --- | --- | --- | --- | --- | --- |
| Start | 1 | 0 | 0 | 0 | `nums[0] == nums[1]`. Move `r`. |
| 1 | 2 | 0 | 0 | 1 | `nums[0] != nums[2]`. Copy `1` to `nums[1]`. `l` becomes 1. Move `r`. |
| 2 | 3 | 1 | 1 | 1 | `nums[1] == nums[3]`. Move `r`. |
| 3 | 4 | 1 | 1 | 1 | `nums[1] == nums[4]`. Move `r`. |
| 4 | 5 | 1 | 1 | 2 | `nums[1] != nums[5]`. Copy `2` to `nums[2]`. `l` becomes 2. Move `r`. |
| 5 | 6 | 2 | 2 | 2 | `nums[2] == nums[6]`. Move `r`. |
| 6 | 7 | 2 | 2 | 3 | `nums[2] != nums[7]`. Copy `3` to `nums[3]`. `l` becomes 3. Move `r`. |
| 7 | 8 | 3 | 3 | 3 | `nums[3] == nums[8]`. Move `r`. |
| 8 | 9 | 3 | 3 | 4 | `nums[3] != nums[9]`. Copy `4` to `nums[4]`. `l` becomes 4. Move `r`. |
| End | 10 | 4 | 4 | - | Loop ends because `r == nums.size()`. Return `l + 1 = 5`. |

**Final state of `nums`:** `[0, 1, 2, 3, 4, ...]` (First `k = 5` elements are `[0, 1, 2, 3, 4]`).

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where N is the total number of elements in `nums`. The right pointer `r` moves through the array from start to end in a single pass.
- **Space Complexity:** **O(1)** — Memory usage is constant. We only use two integer variables (`l` and `r`) regardless of how large the array is, modifying `nums` directly in-place.

**Is this optimal?**
Yes, this solution is already fully optimal. To check every element for duplicates, we must look at each element at least once, which takes O(N) time. The problem requires modifying the array in-place, which means O(1) extra space is the strict requirement. No algorithm can perform better than O(N) time and O(1) space for this problem.

---

## Edge Cases Handled

- **Array with no duplicates (e.g., `[1, 2, 3]`):** The condition `nums[l] != nums[r]` triggers on every step. Elements overwrite themselves in place, `l` advances to the end, and the full size is returned correctly.
- **All elements identical (e.g., `[1, 1, 1, 1]`):** The condition `nums[l] != nums[r]` is never true. `l` stays at 0, and the function returns `1`.
- **Single-element array (e.g., `[7]`):** The loop condition `r < nums.size()` (1 < 1) evaluates to false immediately. The function returns `l + 1 = 1` without errors.
- **Negative numbers (e.g., `[-3, -3, -1, 0, 2]`):** The comparison `nums[l] != nums[r]` works seamlessly with negative integer values.
- **Maximum constraint limits (up to 30,000 elements):** O(N) single-pass runtime handles the maximum input size well within competitive programming execution time limits.
