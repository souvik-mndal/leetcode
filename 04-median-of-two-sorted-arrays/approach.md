![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.16%20MB%20(beats%2061.53%25)-green?style=for-the-badge)

---

## Problem Explained

You are given two separate lists of integers, `nums1` and `nums2`. Both lists are already sorted from smallest to largest. Your task is to find the **median** of all elements across both lists as if they were merged into a single sorted list.

The **median** is the middle value of a sorted collection:
* If the total number of elements across both lists is **odd**, the median is the single element sitting right in the middle. For example, in `[1, 2, 3]`, the median is `2`.
* If the total number of elements is **even**, there are two middle elements. The median is their average. For example, in `[1, 2, 3, 4]`, the middle elements are `2` and `3`, so the median is `(2 + 3) / 2 = 2.5`.

You must return this median as a floating-point number (a decimal value).

---

## Intuition

If you took both lists and combined them into one big array, finding the median would be easy: you would just look up the middle index. However, building a whole new merged array uses extra memory.

Since both arrays are already sorted, we can imagine stepping through a merged array without actually creating it. We use two pointers—one for each array—and always pick whichever current element is smaller. We count each element as we "virtually" place it into position. Once our counter reaches the middle index (or indices), we grab those values, calculate the median, and we are done!

---

## Approach

Here is how the code works step-by-step:

* `int i=0 , j=0;`: Creates two index pointers, `i` for `nums1` and `j` for `nums2`, both starting at position 0.
* `int n=nums1.size();` and `int m=nums2.size();`: Stores the length of `nums1` in `n` and `nums2` in `m`.
* `int cnt  = 0;`: Keeps track of how many total numbers we have inspected so far.
* `int val1=0,val2=0;`: Holds the target middle values. If the total length is even, we need two values (`val1` and `val2`). If odd, we only need `val2`.
* `int t=n+m;`: Calculates the total combined size of both arrays.
* `if( t%2 == 1 ) { find2 = t/2; find1=-1; }`: Checks if the total count is odd. If so, setting `find2` to `t/2` marks the single middle index, while `find1 = -1` acts as a flag that no second value is needed.
* `else { find2 = t/2; find1 = t/2-1; }`: If total count is even, sets `find1` and `find2` to the two middle indices (`t/2 - 1` and `t/2`).
* `while( i<n && j<m )`: Loops while both arrays still have unvisited elements.
* `if( nums1[i]<=nums2[j] )`: Compares current elements from both arrays. If `nums1[i]` is smaller or equal, we process `nums1[i]`. If `cnt` matches `find1` or `find2`, we store `nums1[i]` into `val1` or `val2`. Then we increment `cnt` and move `i` forward.
* `else`: Executes if `nums2[j]` is smaller. We record `nums2[j]` into `val1` or `val2` if `cnt` matches `find1` or `find2`, then increment `cnt` and move `j` forward.
* `while( i<n )` and `while( j<m )`: Runs after one array is exhausted. Handles any remaining elements in the other array, saving target values into `val1` or `val2` as `cnt` advances.
* `if( find1 == -1 ) { return (float)val2; } else { return (val1+val2)/2.0; }`: Checks our odd/even flag. If total size was odd, returns `val2`. Otherwise, returns the average of `val1` and `val2`.

---

## Dry Run

### Example 1: Odd Total Length
* **Inputs:** `nums1 = [1, 3]`, `nums2 = [2]`
* **Initial setup:** `n = 2`, `m = 1`, `t = 3`. Since `t` is odd, `find1 = -1`, `find2 = 1`.

| Step | `i` | `j` | `cnt` | Action | `val1` | `val2` |
|---|---|---|---|---|---|---|
| Start | 0 | 0 | 0 | Compare `nums1[0]=1` <= `nums2[0]=2`. Pick 1. `cnt` is 0 (no match). Advance `i` and `cnt`. | 0 | 0 |
| 1 | 1 | 0 | 1 | Compare `nums1[1]=3` > `nums2[0]=2`. Pick 2. `cnt == find2` (1 == 1), record `val2 = 2`. Advance `j` and `cnt`. | 0 | 2 |
| End | 1 | 1 | 2 | `j` reaches `m`. Main loop ends. `find1 == -1`, so return `(float)val2 = 2.0`. | 0 | 2 |

---

### Example 2: Even Total Length
* **Inputs:** `nums1 = [1, 2]`, `nums2 = [3, 4]`
* **Initial setup:** `n = 2`, `m = 2`, `t = 4`. Since `t` is even, `find1 = 1`, `find2 = 2`.

| Step | `i` | `j` | `cnt` | Action | `val1` | `val2` |
|---|---|---|---|---|---|---|
| Start | 0 | 0 | 0 | Compare `nums1[0]=1` <= `nums2[0]=3`. Pick 1. `cnt` is 0 (no match). Advance `i` and `cnt`. | 0 | 0 |
| 1 | 1 | 0 | 1 | Compare `nums1[1]=2` <= `nums2[0]=3`. Pick 2. `cnt == find1` (1 == 1), record `val1 = 2`. Advance `i` and `cnt`. | 2 | 0 |
| 2 | 2 | 0 | 2 | `i` reached `n`. Go to `j` loop. Take `nums2[0]=3`. `cnt == find2` (2 == 2), record `val2 = 3`. Advance `j` and `cnt`. | 2 | 3 |
| End | 2 | 1 | 3 | Traversal done. `find1 != -1`, return `(val1 + val2) / 2.0` = `(2 + 3) / 2.0 = 2.5`. | 2 | 3 |

---

## Time & Space Complexity

* **Time Complexity:** **O(m + n)** — The code uses two pointers to walk through both arrays element by element. In the worst case, it traverses all `m + n` elements across both arrays.
* **Space Complexity:** **O(1)** — Memory usage is constant because it only uses a few scalar integer variables (`i`, `j`, `cnt`, `val1`, `val2`, `find1`, `find2`) without allocating any extra array.

### Can this be improved?

**Yes.** The problem requests an overall runtime complexity of **O(log (m+n))**. The current code takes **O(m+n)** time.

To achieve logarithmic time, we must avoid visiting every element. Instead of stepping linearly through elements, we can use **Binary Search** to partition (split) both arrays simultaneously. 

#### How Binary Search Partitioning Works:
1. Divide both arrays into a "Left Part" and a "Right Part" such that the Left Part contains exactly half of all total elements.
2. Because both arrays are individually sorted, we binary search for a split point `i` in `nums1`. The split point `j` in `nums2` is automatically fixed to maintain half the total elements on the left side: `j = (n + m + 1) / 2 - i`.
3. A split is valid when all elements in the Left Part are less than or equal to all elements in the Right Part:
   * `nums1[i - 1] <= nums2[j]`
   * `nums2[j - 1] <= nums1[i]`
4. If `nums1[i - 1] > nums2[j]`, our cut in `nums1` is too far right, so we binary search to the left. Otherwise, we move right.
5. Once valid, the median is calculated directly from the boundary values.

Here is the key logic snippet for the optimized approach:

```cpp
// Optimal Binary Search Approach: O(log(min(m, n)))
if (nums1.size() > nums2.size()) 
    return findMedianSortedArrays(nums2, nums1); // Perform search on smaller array

int n = nums1.size(), m = nums2.size();
int low = 0, high = n;

while (low <= high) {
    int i = (low + high) / 2;
    int j = (n + m + 1) / 2 - i;

    int maxLeft1 = (i == 0) ? INT_MIN : nums1[i - 1];
    int minRight1 = (i == n) ? INT_MAX : nums1[i];
    int maxLeft2 = (j == 0) ? INT_MIN : nums2[j - 1];
    int minRight2 = (j == m) ? INT_MAX : nums2[j];

    if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
        if ((n + m) % 2 == 1) 
            return max(maxLeft1, maxLeft2);
        return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
    } else if (maxLeft1 > minRight2) {
        high = i - 1; // Move left in nums1
    } else {
        low = i + 1;  // Move right in nums1
    }
}
```

* **Line breakdown:** Swapping arrays ensures we perform binary search on the shorter array (`nums1`), minimizing operations. `INT_MIN` and `INT_MAX` cleanly handle cases where a partition takes zero or all elements from one array.

* **Improved Complexity:** **Time:** O(log(min(m, n))), **Space:** O(1).
* **Theoretical Best:** O(log(min(m, n))) is the theoretical optimal limit because binary searching partition points in sorted input requires logarithmic time, reaching the problem's requirement.

---

## Edge Cases Handled

* **One empty array (`m = 0` or `n = 0`):** The code skips the main `while(i<n && j<m)` comparison loop entirely and relies on the standalone `while(i<n)` or `while(j<m)` loop to locate the correct middle element from the non-empty array.
* **Odd vs. Even total size:** Correctly determined beforehand using `t % 2 == 1`. Sets `find1 = -1` for odd lengths (returning `val2`) and appropriate pair values for even lengths (returning average of `val1` and `val2`).
* **Arrays of completely different lengths:** Works regardless of size ratio because pointers move independently.
* **Negative numbers:** Handles negative values smoothly because native integer comparison (`<=`) works correctly for signed numbers.
* **Duplicate values across arrays:** The comparison `nums1[i] <= nums2[j]` handles duplicate values safely without infinite loops or off-by-one errors.
