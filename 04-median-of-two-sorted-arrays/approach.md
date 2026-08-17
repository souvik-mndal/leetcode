![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.13%20MB%20(beats%2061.62%25)-green?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the **median** value of two separate sorted integer arrays, `nums1` and `nums2`. 

The median is the middle value of a sorted list. 
* If the total number of elements is odd, the median is the single element right in the middle.
* If the total number of elements is even, the median is the average of the two middle elements.

For example, if `nums1 = [1, 3]` and `nums2 = [2]`:
* If we merge them, we get `[1, 2, 3]`.
* The total length is 3 (odd).
* The middle element is `2`. So the median is `2.0`.

If `nums1 = [1, 2]` and `nums2 = [3, 4]`:
* Merging them gives `[1, 2, 3, 4]`.
* The total length is 4 (even).
* The middle elements are `2` and `3`. 
* Their average is `(2 + 3) / 2 = 2.5`.

Our goal is to find this median without fully creating a new merged array.

---

## Intuition

If we were to create a brand-new array and combine both inputs into it, that would take extra memory. 

Instead, we can simulate the merge process using two pointers (one for each array). We walk through both arrays simultaneously from left to right, always choosing the smaller element next. 

Because we know the total combined length, we know exact index positions where the middle element (or two middle elements) will appear. We just keep counting as we step forward. Once our step count reaches those target middle indices, we record the values. As soon as we have our target values, we return the median!

---

## Approach

Here is how the code works step-by-step:

* `int i=0 , j=0;`: Creates two pointer variables set to index 0. `i` tracks our position in `nums1`, and `j` tracks our position in `nums2`.
* `int n=nums1.size();` and `int m=nums2.size();`: Stores the length of `nums1` in `n` and `nums2` in `m`.
* `int cnt = 0;`: Keeps track of how many elements we have processed so far in sorted order.
* `int val1=0,val2=0;`: Variables to hold the values at our target median position(s).
* `int t=n+m;`: Calculates total combined length of both arrays.
* `if( t%2 == 1 )`: Checks if the combined length is odd or even. If odd, sets target index `find2 = t / 2` and `find1 = -1` (since we only need one middle value). If even, sets `find1 = t / 2 - 1` and `find2 = t / 2` (since we need two middle values).
* `while( i<n && j<m )`: Runs a loop as long as both arrays still have elements to compare.
* `if( nums1[i]<=nums2[j] )`: Compares current elements of both arrays. If `nums1[i]` is smaller or equal, checks if `cnt` equals `find1` or `find2`. If it matches, saves `nums1[i]` into `val1` or `val2`. Then increments `cnt` and moves `i` forward.
* `else`: Executes if `nums2[j]` is smaller. Checks if `cnt` matches `find1` or `find2`, saves `nums2[j]` into `val1` or `val2`, increments `cnt`, and moves `j` forward.
* `while( i<n )`: Processes any remaining elements in `nums1` if `nums2` gets completely traversed first, recording values when `cnt` matches target indices.
* `while( j<m )`: Processes any remaining elements in `nums2` if `nums1` gets completely traversed first, recording values when `cnt` matches target indices.
* `if( find1 == -1 ) return (float)val2; else return (val1+val2)/2.0;`: Returns `val2` directly if overall length is odd. Returns average of `val1` and `val2` as a double if length is even.

---

## Dry Run

### Test Case 1: Odd total length
`nums1 = [1, 3]`, `nums2 = [2]`  
`n = 2`, `m = 1`, `t = 3` (odd). `find1 = -1`, `find2 = 1`.

| Step | Pointer `i` | Pointer `j` | `cnt` | `val1` | `val2` | Action taken |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 0 | 0 | 0 | 0 | 0 | Compare `nums1[0]` (1) and `nums2[0]` (2). 1 <= 2. `cnt` (0) doesn't match `find2` (1). |
| 1 | 1 | 0 | 1 | 0 | 0 | Increment `i` to 1, `cnt` to 1. Compare `nums1[1]` (3) and `nums2[0]` (2). 2 < 3. |
| 2 | 1 | 1 | 2 | 0 | 2 | `cnt` (1) matches `find2` (1). Save `nums2[0]` (2) into `val2`. Increment `j` to 1, `cnt` to 2. |
| End | 1 | 1 | 2 | 0 | 2 | Loop terminates because `j < m` is false. `find1 == -1`, so return `val2` = **2.0**. |

---

### Test Case 2: Even total length
`nums1 = [1, 2]`, `nums2 = [3, 4]`  
`n = 2`, `m = 2`, `t = 4` (even). `find1 = 1`, `find2 = 2`.

| Step | Pointer `i` | Pointer `j` | `cnt` | `val1` | `val2` | Action taken |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 0 | 0 | 0 | 0 | 0 | Compare `nums1[0]` (1) and `nums2[0]` (3). 1 <= 3. `cnt` (0) no match. |
| 1 | 1 | 0 | 1 | 0 | 0 | Increment `i` to 1, `cnt` to 1. Compare `nums1[1]` (2) and `nums2[0]` (3). 2 <= 3. |
| 2 | 2 | 0 | 2 | 2 | 0 | `cnt` (1) matches `find1` (1). Save `nums1[1]` (2) into `val1`. Increment `i` to 2, `cnt` to 2. |
| 3 | 2 | 0 | 2 | 2 | 0 | First `while` loop ends because `i < n` is false (`2 < 2` is false). |
| 4 | 2 | 1 | 3 | 2 | 3 | Enters `while(j < m)`. `cnt` (2) matches `find2` (2). Save `nums2[0]` (3) into `val2`. Increment `j` to 1, `cnt` to 3. |
| End | 2 | 1 | 3 | 2 | 3 | Loop finishes. `find1 != -1`, return `(2 + 3) / 2.0` = **2.5**. |

---

## Time & Space Complexity

* **Current Time Complexity:** **O(m + n)** — The code steps through elements one by one until it reaches the middle index `(m + n) / 2`. In the worst case, it performs steps proportional to the combined size of both arrays.
* **Current Space Complexity:** **O(1)** — Memory usage is constant because it only allocates a few primitive variables (`i`, `j`, `cnt`, `val1`, `val2`, `t`, `find1`, `find2`).

### Can this be improved?

**Yes.** The problem constraints explicitly ask for an **O(log(m + n))** time complexity.

To achieve logarithmic time, we must use **Binary Search** instead of stepping through elements one by one.

#### How Binary Search Works Here:
Instead of counting elements from the start, we place a vertical partition cut inside `nums1` and another inside `nums2`. This divides both arrays into a combined "Left Half" and a combined "Right Half".

We adjust the partition cut in `nums1` using binary search:
1. `partition1` is our split index in `nums1`.
2. `partition2 = (n + m + 1) / 2 - partition1` is the automatically calculated split index in `nums2`.
3. A partition is correct if every element in the combined left side is less than or equal to every element in the combined right side:
   * `maxLeft1 <= minRight2`
   * `maxLeft2 <= minRight1`
4. If `maxLeft1 > minRight2`, our cut in `nums1` is too far right, so we move our binary search boundary left.
5. Otherwise, if `maxLeft2 > minRight1`, our cut in `nums1` is too far left, so we move our binary search boundary right.

By searching only on the smaller array, we quickly find the correct cuts in logarithmic time.

#### Improved Code Snippet:

```cpp
// Ensure binary search is performed on the smaller array for O(log(min(m, n)))
if (nums1.size() > nums2.size()) 
    return findMedianSortedArrays(nums2, nums1);

int n = nums1.size(), m = nums2.size();
int low = 0, high = n;

while (low <= high) {
    int partition1 = low + (high - low) / 2;
    int partition2 = (n + m + 1) / 2 - partition1;

    // Handle edge partitions using boundary values
    int maxLeft1 = (partition1 == 0) ? INT_MIN : nums1[partition1 - 1];
    int minRight1 = (partition1 == n) ? INT_MAX : nums1[partition1];

    int maxLeft2 = (partition2 == 0) ? INT_MAX : nums2[partition2 - 1]; // partition2 == 0 -> INT_MIN
    int minRight2 = (partition2 == m) ? INT_MAX : nums2[partition2];
    
    // Fix typo for maxLeft2 zero case:
    maxLeft2 = (partition2 == 0) ? INT_MIN : nums2[partition2 - 1];

    if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
        if ((n + m) % 2 == 0) {
            return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
        } else {
            return max(maxLeft1, maxLeft2);
        }
    } else if (maxLeft1 > minRight2) {
        high = partition1 - 1; // move cut left
    } else {
        low = partition1 + 1;  // move cut right
    }
}
return 0.0;
```

* **Improved Time Complexity:** **O(log(min(m, n)))** — Binary search eliminates half of the search range of the smaller array at each step.
* **Improved Space Complexity:** **O(1)** — Still uses constant auxiliary memory.
* **Theoretical Best:** **O(log(min(m, n)))** is the optimal theoretical limit for this problem. The improved binary search approach reaches this limit.

---

## Edge Cases Handled

* **One array is empty (`m = 0` or `n = 0`):** The main `while(i < n && j < m)` condition fails immediately. Execution skips directly to whichever single-array `while` loop remains, processing elements without crash or out-of-bounds error.
* **Arrays of unequal sizes:** Handled naturally by tracking indices `i` and `j` independently and bound-checking them separately (`i < n` and `j < m`).
* **Even vs. Odd combined total length:** Handled by `if (t % 2 == 1)`. Odd lengths calculate a single middle index (`find1 = -1`), while even lengths track two adjacent middle indices (`find1` and `find2`).
* **Arrays with duplicate values:** Handled properly because element comparisons use `<=` (`nums1[i] <= nums2[j]`), ensuring elements with identical values preserve natural sequence order without skipping steps.
