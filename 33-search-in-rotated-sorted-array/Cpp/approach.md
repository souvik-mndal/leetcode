![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.1%20MB%20(beats%2096.77%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers sorted in increasing order, like `[0, 1, 2, 4, 5, 6, 7]`. 

Now imagine someone cuts this list at some point and moves the left portion to the right end. For instance, shifting it at index 3 turns `[0, 1, 2, 4, 5, 6, 7]` into `[4, 5, 6, 7, 0, 1, 2]`. This is called a **rotated sorted array**.

You are given this rotated array and a target number. Your task is to find the index (0-based position) where the target number is located. If the target is not in the array, return `-1`.

You must solve this in **O(log n)** time, which means you cannot scan through every element one by one. You need a method that repeatedly cuts the search space in half.

---

## Intuition

In a standard sorted array, **binary search** works by looking at the middle element and immediately discarding whichever half cannot contain our target.

In a rotated sorted array, the array is broken into two separate sorted sections:
1. The left portion (larger numbers, e.g., `[4, 5, 6, 7]`).
2. The right portion (smaller numbers, e.g., `[0, 1, 2]`).

The key "aha" moment is finding the **pivot index**—the position of the largest element right before the numbers drop down to the smallest element. 

Once you find the pivot:
- You split the array into two plain, fully sorted arrays.
- You check if the target fits inside the value range of the left section.
- If it does, run standard binary search on the left section. Otherwise, run standard binary search on the right section.

---

## Approach

Here is how the code executes step-by-step:

- `int pivotIndex = findPivotIndex(nums);` — Calls the helper function to locate the index of the highest value in `nums`.
- `if(s == e) { return s; }` — Inside `findPivotIndex`, if the search range narrows down to a single element, that single element must be the pivot index.
- `if(mid+1 < n && arr[mid] > arr[mid+1]) return mid;` — Checks if the element at `mid` is greater than the element immediately after it. If it is, the drop occurs right after `mid`, making `mid` the pivot.
- `else if(mid-1 >=0 && arr[mid] < arr[mid-1]) return mid-1;` — Checks if the element at `mid` is smaller than the element right before it. If so, the drop happened right before `mid`, making `mid - 1` the pivot.
- `else if(arr[s] > arr[mid] ) e = mid - 1;` — If the start element `arr[s]` is larger than `arr[mid]`, the pivot/drop must be on the left side of `mid`, so shift the right boundary `e` to `mid - 1`.
- `else s = mid + 1;` — Otherwise, the left segment is properly sorted, so the pivot/drop must lie on the right side of `mid`, so shift the left boundary `s` to `mid + 1`.
- `if(target >= nums[0] && target <= nums[pivotIndex])` — Back in `search`, checks if `target` falls within the range of values in the left sorted section (from index `0` up to `pivotIndex`).
- `ans = binarySearch(nums, 0, pivotIndex, target);` — Performs standard binary search on the left section if the target lies within its value range.
- `ans = binarySearch(nums, pivotIndex+1, n-1, target);` — Performs standard binary search on the right section if the target falls outside the left section's range.
- `return ans;` — Returns the index of the target if found, or `-1` if it is not present.

---

## Dry Run

### Case 1: Target present in array (`nums = [4, 5, 6, 7, 0, 1, 2]`, `target = 0`)

#### Phase 1: `findPivotIndex`

| `s` | `e` | `mid` | `arr[mid]` | Action |
|---|---|---|---|---|
| 0 | 6 | 3 | 7 | `arr[3] > arr[4]` (`7 > 0`) is true. Return pivot index `3`. |

#### Phase 2: Decision in `search`
- `nums[0]` is `4`, `nums[pivotIndex]` is `7`.
- `target` (`0`) is not between `4` and `7`. Search right section: `binarySearch(nums, 4, 6, 0)`.

#### Phase 3: `binarySearch` on right section

| `s` | `e` | `mid` | `arr[mid]` | Action |
|---|---|---|---|---|
| 4 | 6 | 5 | 1 | `target` (`0`) < `arr[mid]` (`1`). Set `e = mid - 1 = 4`. |
| 4 | 4 | 4 | 0 | `arr[mid]` (`0`) == `target` (`0`). Return index `4`. |

**Output:** `4`

---

### Case 2: Target not in array (`nums = [4, 5, 6, 7, 0, 1, 2]`, `target = 3`)

#### Phase 1: `findPivotIndex`

| `s` | `e` | `mid` | `arr[mid]` | Action |
|---|---|---|---|---|
| 0 | 6 | 3 | 7 | `arr[3] > arr[4]` (`7 > 0`) is true. Return pivot index `3`. |

#### Phase 2: Decision in `search`
- `target` (`3`) is not between `nums[0]` (`4`) and `nums[pivotIndex]` (`7`). Search right section: `binarySearch(nums, 4, 6, 3)`.

#### Phase 3: `binarySearch` on right section

| `s` | `e` | `mid` | `arr[mid]` | Action |
|---|---|---|---|---|
| 4 | 6 | 5 | 1 | `target` (`3`) > `arr[mid]` (`1`). Set `s = mid + 1 = 6`. |
| 6 | 6 | 6 | 2 | `target` (`3`) > `arr[mid]` (`2`). Set `s = mid + 1 = 7`. |
| 7 | 6 | - | - | Loop ends because `s > e` (`7 > 6`). Return `-1`. |

**Output:** `-1`

---

## Time & Space Complexity

- **Time Complexity:** **O(log n)** — Finding the pivot takes O(log n) time because the search space is halved every step. The subsequent binary search on one half also takes O(log n) time. O(log n) + O(log n) simplifies to **O(log n)**.
- **Space Complexity:** **O(1)** — Only a few integer variables (`s`, `e`, `mid`, `pivotIndex`, `ans`) are used. Memory usage stays constant regardless of array size.

### Is this optimal?

**Yes, O(log n) time and O(1) space is the theoretical best possible complexity for searching in a rotated sorted array.** You must inspect at least logarithmic elements to pinpoint an item without checking every element.

However, we can simplify the implementation by doing the search in **a single pass** instead of two separate steps.

Instead of finding the pivot first, you can perform binary search directly. At any `mid` index, **at least one half of the array (left or right) is guaranteed to be normally sorted**. You check which half is sorted, see if the `target` falls inside that sorted half's boundaries, and adjust `s` or `e` accordingly.

Here is the single-pass version:

```cpp
int search(vector<int>& nums, int target) {
    int s = 0, e = nums.size() - 1;
    while (s <= e) {
        int mid = s + (e - s) / 2;
        if (nums[mid] == target) return mid;

        // Check if the left half is sorted
        if (nums[s] <= nums[mid]) {
            if (target >= nums[s] && target < nums[mid]) e = mid - 1; // target in left half
            else s = mid + 1; // target in right half
        } 
        // Otherwise, the right half must be sorted
        else {
            if (target > nums[mid] && target <= nums[e]) s = mid + 1; // target in right half
            else e = mid - 1; // target in left half
        }
    }
    return -1;
}
```

- `if (nums[s] <= nums[mid])` checks if the segment from `s` to `mid` has no rotation break, meaning it is strictly sorted.
- `if (target >= nums[s] && target < nums[mid])` checks if `target` lies inside this clean range. If yes, search left; if no, search right.
- This single-pass approach retains **O(log n) time** and **O(1) space**, but cuts out helper functions and extra passes.

---

## Edge Cases Handled

- **Single-element array (`nums = [1]`)**: The check `if(s == e) return s;` inside `findPivotIndex` handles arrays of size 1 gracefully without going out of bounds.
- **Unrotated array (`nums = [1, 2, 3, 4]`)**: If the array is rotated by 0 positions, `findPivotIndex` returns the last element index (`n-1`), and the code cleanly falls back to standard binary search on the whole array.
- **Target at boundaries**: Targets located at index `0` or index `n-1` are correctly matched due to inclusive boundary checks (`<=` and `>=`).
- **Target out of range**: Targets smaller than the overall minimum or larger than the overall maximum return `-1` correctly.
