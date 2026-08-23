![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-95.03%20MB%20(beats%2080.00%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given two sorted lists of numbers, `nums1` and `nums2`. Both lists are already sorted in ascending order (smallest to largest).

Your goal is to find the **median** of all numbers combined into a single sorted list.

*   The **median** is the exact middle value of a sorted list.
*   If the total count of numbers is **odd**, the median is the single element right in the middle.
*   If the total count of numbers is **even**, the median is the average of the two middle elements.

**Example:**
*   `nums1 = [1, 3]`
*   `nums2 = [2]`
*   Combined sorted array: `[1, 2, 3]`
*   Total size is 3 (odd). The middle number is `2`. The median is `2.0`.

---

## Intuition

The straightforward way to find the median is to combine both arrays into one large sorted array, then pick the middle item. However, making a new array takes extra memory.

Instead, we can simulate merging the two arrays without creating a new one. We keep two pointers (index counters) starting at the beginning of each array. At each step, we pick the smaller element, advance its pointer, and increment a total counter `cnt`.

We calculate in advance which index positions will hold the median value(s). Once our counter `cnt` reaches those target indices, we save the numbers in `val1` and `val2`. When done, we compute the final median.

---

## Approach

Here is how the code executes step-by-step:

*   `int i=0 , j=0;`: Initializes pointer `i` for `nums1` and pointer `j` for `nums2` at index 0.
*   `int n=nums1.size(); int m=nums2.size();`: Stores the total element count of `nums1` in `n` and `nums2` in `m`.
*   `int t=n+m;`: Calculates `t`, the total combined size of both arrays.
*   `if( t%2 == 1 ){ find2 = t/2; find1=-1; }`: Checks if total length `t` is odd. If odd, sets `find2` to the exact middle index `t / 2` and sets `find1` to `-1` (meaning we only need one value).
*   `else{ find2 = t/2; find1 = t/2-1; }`: If total length `t` is even, sets `find1` and `find2` to the two middle indices `(t / 2) - 1` and `t / 2`.
*   `while( i<n && j<m )`: Loops as long as both arrays still have elements left to inspect.
*   `if( nums1[i]<=nums2[j] )`: Checks if the current element in `nums1` is smaller than or equal to the current element in `nums2`.
*   `if( cnt == find1 ){ val1 = nums1[i]; }`: Saves the element to `val1` if current index `cnt` matches target position `find1`.
*   `if( cnt == find2 ){ val2 = nums1[i]; }`: Saves the element to `val2` if current index `cnt` matches target position `find2`.
*   `cnt++; i++;`: Increments total step count `cnt` and advances pointer `i` in `nums1`.
*   `else { ... cnt++; j++; }`: If `nums2[j]` is smaller, checks target indices for `nums2[j]`, saves matching values, increments `cnt`, and advances pointer `j` in `nums2`.
*   `while( i<n )`: Processes any leftover elements in `nums1` if `nums2` ran out of elements early, checking for target indices `find1` and `find2`.
*   `while( j<m )`: Processes any leftover elements in `nums2` if `nums1` ran out of elements early, checking for target indices `find1` and `find2`.
*   `if( find1 == -1 ){ return (float)val2; }`: If total length was odd, returns `val2` cast to float as the single median.
*   `else{ return (val1+val2)/2.0; }`: If total length was even, returns the average of `val1` and `val2`.

---

## Dry Run

### Case 1: Odd total size (`nums1 = [1,3]`, `nums2 = [2]`)

*   `n = 2`, `m = 1`, `t = 3` (odd total length).
*   `find1 = -1`, `find2 = 1`.

| Step | `i` | `j` | `cnt` | `val1` | `val2` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 0 | 0 | 0 | 0 | 0 | Compare `nums1[0]` (1) and `nums2[0]` (2). 1 is smaller. `cnt` (0) != `find2` (1). Move `i` to 1, `cnt` to 1. |
| 1 | 1 | 0 | 1 | 0 | 2 | Compare `nums1[1]` (3) and `nums2[0]` (2). 2 is smaller. `cnt` (1) == `find2` (1), so `val2` = 2. Move `j` to 1, `cnt` to 2. |
| End | 1 | 1 | 2 | 0 | 2 | Main `while` loop ends (`j < m` fails). Remaining loops do nothing. `find1 == -1`, return `val2` = 2.0. |

---

### Case 2: Even total size (`nums1 = [1,2]`, `nums2 = [3,4]`)

*   `n = 2`, `m = 2`, `t = 4` (even total length).
*   `find1 = 1`, `find2 = 2`.

| Step | `i` | `j` | `cnt` | `val1` | `val2` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 0 | 0 | 0 | 0 | 0 | Compare `nums1[0]` (1) and `nums2[0]` (3). 1 is smaller. Move `i` to 1, `cnt` to 1. |
| 1 | 1 | 0 | 1 | 2 | 0 | Compare `nums1[1]` (2) and `nums2[0]` (3). 2 is smaller. `cnt` (1) == `find1` (1), so `val1` = 2. Move `i` to 2, `cnt` to 2. |
| 2 | 2 | 0 | 2 | 2 | 0 | Main `while` ends (`i < n` fails). Enter `while(j < m)`. `cnt` (2) == `find2` (2), so `val2` = 3. Move `j` to 1, `cnt` to 3. |
| End | 2 | 1 | 3 | 2 | 3 | Return `(val1 + val2) / 2.0` = `(2 + 3) / 2.0` = 2.5. |

---

## Time & Space Complexity

*   **Current Time Complexity:** **O(m + n)** — The code steps through elements one by one until it reaches the middle index. In the worst case, it inspects about half of all total elements `(m + n) / 2`.
*   **Current Space Complexity:** **O(1)** — Only a few primitive integer variables (`i`, `j`, `cnt`, `val1`, `val2`, etc.) are used. Memory consumption remains constant regardless of input size.

### Can this be improved?

**Yes.** The problem prompt specifically requests an **O(log (m+n))** runtime.

To achieve logarithmic time, we must stop walking through elements one by one. Instead, we use **Binary Search** on the smaller array.

#### Why Binary Search Works for This Problem:
If we divide both arrays into a Left partition and a Right partition, the left combined half must contain exactly half of all elements.

We pick a partition point in `nums1` using binary search. The partition point in `nums2` is then automatically determined so that the total left elements equal the total right elements.

A partition is correct when:
1. Max element on left of `nums1` <= Min element on right of `nums2`
2. Max element on left of `nums2` <= Min element on right of `nums1`

If these conditions hold, we have split the combined array correctly in half. We can calculate the median immediately using the boundaries of the split. If not, we adjust our binary search range left or right.

#### Improved Code Snippet (Binary Search Approach):

```cpp
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) 
        return findMedianSortedArrays(nums2, nums1); // Ensure nums1 is smaller

    int n = nums1.size();
    int m = nums2.size();
    int low = 0, high = n;

    while (low <= high) {
        int cut1 = (low + high) / 2;
        int cut2 = (n + m + 1) / 2 - cut1;

        int left1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int left2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];

        int right1 = (cut1 == n) ? INT_MAX : nums1[cut1];
        int right2 = (cut2 == m) ? INT_MAX : nums2[cut2];

        if (left1 <= right2 && left2 <= right1) {
            if ((n + m) % 2 == 0)
                return (max(left1, left2) + min(right1, right2)) / 2.0;
            else
                return max(left1, left2);
        } else if (left1 > right2) {
            high = cut1 - 1; // Move search space left
        } else {
            low = cut1 + 1; // Move search space right
        }
    }
    return 0.0;
}
```

*   **Improved Time Complexity:** **O(log(min(m, n)))** — Binary search runs on the smaller array, cutting the search space in half each step.
*   **Theoretical Best Complexity:** **O(log(min(m, n)))**. The improved version reaches this theoretical best bound.

---

## Edge Cases Handled

*   **One Array is Empty (`m = 0` or `n = 0`):** The first `while` loop skips completely because `i < n && j < m` evaluates to false. The remaining while loop runs for the non-empty array and correctly picks the middle values.
*   **Odd vs Even Total Elements:** Handled cleanly by setting `find1 = -1` for odd lengths and computing appropriate target positions for even lengths.
*   **Duplicate Numbers Across Arrays:** Handled using `<=` in `nums1[i] <= nums2[j]`, preserving standard ordering.
*   **Negative Numbers:** Supported naturally since integer comparison operates correctly across negative and positive bounds.
