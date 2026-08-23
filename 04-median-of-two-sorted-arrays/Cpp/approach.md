![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.24%20MB%20(beats%2042.54%25)-yellow?style=for-the-badge)

---

## Problem Explained

We are given two sorted lists of numbers, called `nums1` and `nums2`. Our goal is to find the **median** of all these numbers combined into one sorted sequence.

The median is the exact middle number in a sorted list. 
- If the total number of elements is **odd**, the median is the single element in the center.
- If the total number of elements is **even**, the median is the average of the two central numbers.

### Examples
- **Example 1:** `nums1 = [1, 3]`, `nums2 = [2]`
  - If we combine and sort them, we get `[1, 2, 3]`.
  - The total count is 3 (odd). The middle element is `2`.
  - **Output:** `2.00000`

- **Example 2:** `nums1 = [1, 2]`, `nums2 = [3, 4]`
  - If we combine and sort them, we get `[1, 2, 3, 4]`.
  - The total count is 4 (even). The middle two elements are `2` and `3`.
  - The average is `(2 + 3) / 2 = 2.5`.
  - **Output:** `2.50000`

---

## Intuition

To find the middle element, we do not need to merge both arrays into a brand-new array. Merging takes extra space. 

Instead, we can simulate merging by using two pointers (one for each array) to step through elements from smallest to largest. 

Since we know the lengths of both arrays, we can calculate the exact index (or indices) where the median will sit before we start moving pointers. As we step through the numbers in sorted order, we count how many numbers we have seen so far. When our count hits the target index, we save that number. Once we hit our target, we have everything we need to compute the median.

---

## Approach

Here is step-by-step how the code executes this process:

* `int i=0 , j=0;`: Starts index pointer `i` at the beginning of `nums1` and pointer `j` at the beginning of `nums2`.
* `int n=nums1.size();` and `int m=nums2.size();`: Stores the total size of `nums1` in `n` and `nums2` in `m`.
* `int cnt  = 0;`: Tracks how many combined elements we have checked so far.
* `int val1=0,val2=0;`: Holds the values of the target middle elements once we reach them.
* `int t=n+m;`: Calculates the total length of both arrays combined.
* `if( t%2 == 1 ) { find2 = t/2; find1=-1; }`: If total length `t` is odd, sets `find2` to the middle index `t / 2`. Sets `find1` to `-1` as a marker that we only need one middle value.
* `else { find2 = t/2; find1 = t/2-1; }`: If total length `t` is even, sets `find1` and `find2` to the two center indices needed for the average.
* `while( i<n && j<m )`: Loops through both arrays as long as both have unchecked elements.
* `if( nums1[i]<=nums2[j] )`: Checks if the current element in `nums1` is smaller or equal. If it matches `find1` or `find2`, it saves `nums1[i]` into `val1` or `val2`, then increments `cnt` and `i`.
* `else`: Runs if the current element in `nums2` is smaller. If it matches `find1` or `find2`, it saves `nums2[j]` into `val1` or `val2`, then increments `cnt` and `j`.
* `while( i<n )`: Processes any leftover elements in `nums1` if `nums2` runs out first, saving target values if `cnt` matches `find1` or `find2`.
* `while( j<m )`: Processes any leftover elements in `nums2` if `nums1` runs out first, saving target values if `cnt` matches `find1` or `find2`.
* `if( find1 == -1 ) return (float)val2;`: If the total count was odd, returns `val2` directly as a decimal value.
* `else return (val1+val2)/2.00;`: If even, returns the floating-point average of `val1` and `val2`.

---

## Dry Run

### Case 1: Odd Total Length (`nums1 = [1, 3]`, `nums2 = [2]`)
Variables: `n = 2`, `m = 1`, `t = 3`. `find1 = -1`, `find2 = 1`.

| Step | `i` | `j` | `cnt` | Action / Decision | `val1` | `val2` |
|---|---|---|---|---|---|---|
| Start | 0 | 0 | 0 | Compare `nums1[0]` (1) <= `nums2[0]` (2). `cnt` (0) isn't target. Advance `i`. | 0 | 0 |
| 1 | 1 | 0 | 1 | Compare `nums1[1]` (3) > `nums2[0]` (2). `cnt` (1) matches `find2` (1). Set `val2 = 2`. Advance `j`. | 0 | 2 |
| End | 1 | 1 | 2 | Loop terminates (`j == m`). `find1 == -1`, return `val2` = **2.0**. | 0 | 2 |

---

### Case 2: Even Total Length (`nums1 = [1, 2]`, `nums2 = [3, 4]`)
Variables: `n = 2`, `m = 2`, `t = 4`. `find1 = 1`, `find2 = 2`.

| Step | `i` | `j` | `cnt` | Action / Decision | `val1` | `val2` |
|---|---|---|---|---|---|---|
| Start | 0 | 0 | 0 | Compare `nums1[0]` (1) <= `nums2[0]` (3). `cnt` (0) isn't target. Advance `i`. | 0 | 0 |
| 1 | 1 | 0 | 1 | Compare `nums1[1]` (2) <= `nums2[0]` (3). `cnt` (1) matches `find1` (1). Set `val1 = 2`. Advance `i`. | 2 | 0 |
| 2 | 2 | 0 | 2 | Main loop ends (`i == n`). Enter `while(j < m)`. `cnt` (2) matches `find2` (2). Set `val2 = nums2[0]` (3). Advance `j`. | 2 | 3 |
| End | 2 | 1 | 3 | Traversal done. Return `(val1 + val2) / 2.0` = `(2 + 3) / 2.0` = **2.5**. | 2 | 3 |

---

## Time & Space Complexity

- **Time:** **O(m + n)** — In the worst case, the loop steps through half of all total elements across both arrays to reach the median position.
- **Space:** **O(1)** — We only store integer variables (`i`, `j`, `cnt`, `val1`, `val2`, `find1`, `find2`). No new array is created.

### Can this be improved?

**Yes.** The problem statement explicitly requires **O(log (m + n))** runtime complexity. The current code runs in linear **O(m + n)** time.

To reach logarithmic time, we must use **Binary Search** instead of scanning elements one by one.

#### How Binary Search solves this:
Instead of counting from start to middle, we can divide both arrays into two halves (Left Partition and Right Partition) such that:
1. The combined left partition contains half of the total elements.
2. Every number in the left partition is smaller than or equal to every number in the right partition.

Since both arrays are already sorted, we do binary search on the **smaller array** to find the correct partition split point (`partitionX`). The corresponding split point in the second array (`partitionY`) is then calculated automatically.

#### Optimized Code Snippet:

```cpp
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    // Ensure nums1 is smaller array to minimize binary search range
    if (nums1.size() > nums2.size()) 
        return findMedianSortedArrays(nums2, nums1);

    int x = nums1.size();
    int y = nums2.size();
    int low = 0, high = x;

    while (low <= high) {
        int partitionX = (low + high) / 2;
        int partitionY = (x + y + 1) / 2 - partitionX;

        int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
        int minRightX = (partitionX == x) ? INT_MAX : nums1[partitionX];

        int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
        int minRightY = (partitionY == y) ? INT_MAX : nums2[partitionY];

        if (maxLeftX <= minRightY && maxLeftY <= minRightX) {
            if ((x + y) % 2 == 0) {
                return (max(maxLeftX, maxLeftY) + min(minRightX, minRightY)) / 2.0;
            } else {
                return max(maxLeftX, maxLeftY);
            }
        } else if (maxLeftX > minRightY) {
            high = partitionX - 1; // Move left in nums1
        } else {
            low = partitionX + 1;  // Move right in nums1
        }
    }
    return 0.0;
}
```

#### Why these non-obvious lines work:
- `if (nums1.size() > nums2.size())`: Ensures we binary-search on the shorter array, keeping search steps as small as possible.
- `partitionY = (x + y + 1) / 2 - partitionX`: Automatically balances the total count of numbers on the left side vs the right side.
- `INT_MIN` / `INT_MAX`: Prevents out-of-bounds errors when a partition takes zero elements from one array.

#### Resulting Complexities:
- **Improved Complexity:** **Time:** **O(log(min(m, n)))**, **Space:** **O(1)**.
- **Theoretical Best:** **O(log(min(m, n)))** is the absolute theoretical best because binary search halves the search space of the smaller array at each step. The improved version reaches this optimal limit.

---

## Edge Cases Handled

- **One array is empty (`m = 0` or `n = 0`):** The main combined loop skips, and the remaining `while` loop scans through the non-empty array directly to pick up the median values.
- **Single element total (`m + n = 1`):** `find2` becomes `0` and `find1` becomes `-1`. The logic picks up the single element and returns it as a floating-point number.
- **Duplicate elements across arrays:** Uses `<=` in comparisons, ensuring elements with identical values are counted properly without skipped iterations.
- **Negative numbers:** Handles values down to `-10^6` cleanly because comparisons and standard average division maintain standard integer and double signs.
