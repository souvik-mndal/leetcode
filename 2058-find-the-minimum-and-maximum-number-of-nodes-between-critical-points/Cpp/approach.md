![Runtime](https://img.shields.io/badge/Runtime-11%20ms%20(beats%2012.44%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-137.88%20MB%20(beats%205.02%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a linked list of numbers. A node in this list is called a **critical point** if it is either a **local maxima** or a **local minima**:

*   **Local Maxima:** The node's value is strictly greater than the value of the node before it AND strictly greater than the value of the node after it.
*   **Local Minima:** The node's value is strictly smaller than the value of the node before it AND strictly smaller than the value of the node after it.

Because a critical point needs both a previous neighbor and a next neighbor, the **first node** and the **last node** of a linked list can **never** be critical points.

Your job is to find two distances:
1.  **minDistance:** The smallest distance (index difference) between any two distinct critical points.
2.  **maxDistance:** The largest distance (index difference) between any two distinct critical points.

If the linked list contains fewer than two critical points, you cannot measure a distance between two points, so you must return `[-1, -1]`.

### Example
Take the list: `[5, 3, 1, 2, 5, 1, 2]` (indices `0` through `6`).
*   Node at index 2 (value `1`) is smaller than `3` and `2` (Local Minima).
*   Node at index 4 (value `5`) is greater than `2` and `1` (Local Maxima).
*   Node at index 5 (value `1`) is smaller than `5` and `2` (Local Minima).

Critical point indices are `[2, 4, 5]`.
*   Smallest gap is between index 4 and index 5: `5 - 4 = 1`.
*   Largest gap is between index 2 and index 5: `5 - 2 = 3`.
*   Result: `[1, 3]`.

---

## Intuition

To figure out if a node is a peak (maxima) or a valley (minima), you only need to look at three adjacent elements at a time: previous, current, and next.

The written solution simplifies looking at neighbors by first copying all linked list values into a plain array (`nums`). Once values are in an array, checking neighbors at index `i - 1` and `i + 1` is very easy.

Whenever a critical point is found, its position (index) is saved into a second array called `criticalPoints`. 

Because we collect critical point indices in order from left to right:
*   The **maximum distance** is always the gap between the very first critical point found and the very last critical point found (`last_index - first_index`).
*   The **minimum distance** must be the gap between two *neighboring* critical points in our list of critical points (because any non-adjacent pair will be farther apart).

---

## Approach

Here is how the code executes step-by-step:

*   `vector<int> nums;` — Declares a dynamic array named `nums` to store all node values from the linked list.
*   `while(head)` — Starts a loop that runs until `head` becomes `nullptr` (reaches the end of the list).
*   `nums.push_back(head->val); head = head->next;` — Copies the current node's value into `nums` and advances `head` to the next node.
*   `vector<int> criticalPoints;` — Declares an array named `criticalPoints` to store the index positions where peaks and valleys occur.
*   `int n = nums.size();` — Stores the total count of numbers collected in `n`.
*   `for(int i = 1; i < n - 1; i++)` — Loops through every index from `1` up to `n - 2`. Indices `0` and `n - 1` are intentionally skipped because boundary nodes cannot be critical points.
*   `if(nums[i] > nums[i - 1] && nums[i] > nums[i + 1])` — Checks if the current number at index `i` is strictly larger than both its left neighbor (`i - 1`) and right neighbor (`i + 1`). If true, adds `i` to `criticalPoints`.
*   `else if(nums[i] < nums[i - 1] && nums[i] < nums[i + 1])` — Checks if the current number at index `i` is strictly smaller than both its left and right neighbors. If true, adds `i` to `criticalPoints`.
*   `int m = criticalPoints.size();` — Stores the total number of critical points found in `m`.
*   `if(m < 2) return {-1, -1};` — Checks if we found fewer than 2 critical points. If so, we cannot measure distances, so we return `{-1, -1}` immediately.
*   `int minDist = INT_MAX;` — Sets `minDist` to the maximum possible integer value so any calculated distance will safely update it.
*   `int maxDist = criticalPoints[m - 1] - criticalPoints[0];` — Computes the maximum distance directly by subtracting the index of the first critical point from the index of the last critical point.
*   `for(int i = 1; i < m; i++)` — Loops through all stored critical point indices starting from the second one (index `1`).
*   `minDist = min(minDist, criticalPoints[i] - criticalPoints[i - 1]);` — Calculates the gap between the current critical point and the previous critical point, keeping the smallest gap found so far.
*   `return {minDist, maxDist};` — Returns a pair containing the minimum distance and maximum distance.

---

## Dry Run

### Case 1: Standard input with multiple critical points
Input: `head = [5, 3, 1, 2, 5, 1, 2]`
After list extraction: `nums = [5, 3, 1, 2, 5, 1, 2]`, `n = 7`

| Step / `i` | `nums[i]` | Left (`nums[i-1]`) | Right (`nums[i+1]`) | Critical Point? | `criticalPoints` state | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `i = 1` | 3 | 5 | 1 | No | `[]` | Skip (3 is not strictly greater/smaller than both) |
| `i = 2` | 1 | 3 | 2 | Yes (Minima) | `[2]` | Add index 2 to `criticalPoints` |
| `i = 3` | 2 | 1 | 5 | No | `[2]` | Skip |
| `i = 4` | 5 | 2 | 1 | Yes (Maxima) | `[2, 4]` | Add index 4 to `criticalPoints` |
| `i = 5` | 1 | 5 | 2 | Yes (Minima) | `[2, 4, 5]` | Add index 5 to `criticalPoints` |

After loop: `m = 3` critical points.
*   `maxDist = criticalPoints[2] - criticalPoints[0] = 5 - 2 = 3`
*   Distance loop:
    *   `i = 1`: `criticalPoints[1] - criticalPoints[0] = 4 - 2 = 2` -> `minDist = 2`
    *   `i = 2`: `criticalPoints[2] - criticalPoints[1] = 5 - 4 = 1` -> `minDist = min(2, 1) = 1`
*   Final Output: `[1, 3]`

---

### Case 2: Input with duplicate adjacent values (plateau)
Input: `head = [1, 3, 2, 2, 3, 2, 2, 2, 7]`
After list extraction: `nums = [1, 3, 2, 2, 3, 2, 2, 2, 7]`, `n = 9`

| Step / `i` | `nums[i]` | Left (`nums[i-1]`) | Right (`nums[i+1]`) | Critical Point? | `criticalPoints` state | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `i = 1` | 3 | 1 | 2 | Yes (Maxima) | `[1]` | Add index 1 |
| `i = 2` | 2 | 3 | 2 | No | `[1]` | `2 < 3` but `2 == 2` (not strictly smaller) |
| `i = 3` | 2 | 2 | 3 | No | `[1]` | `2 == 2` (plateau, ignored) |
| `i = 4` | 3 | 2 | 2 | Yes (Maxima) | `[1, 4]` | Add index 4 |
| `i = 5` | 2 | 3 | 2 | No | `[1, 4]` | Not strict inequality |
| `i = 6` | 2 | 2 | 2 | No | `[1, 4]` | Flat region, ignored |
| `i = 7` | 2 | 2 | 7 | No | `[1, 4]` | Not strict inequality |

After loop: `m = 2` critical points.
*   `maxDist = criticalPoints[1] - criticalPoints[0] = 4 - 1 = 3`
*   Distance loop:
    *   `i = 1`: `criticalPoints[1] - criticalPoints[0] = 4 - 1 = 3` -> `minDist = 3`
*   Final Output: `[3, 3]`

---

## Time & Space Complexity

### Current Solution Analysis
*   **Time Complexity:** **O(N)** — Traversing the linked list takes O(N) time. Iterating through `nums` takes O(N) time. Iterating through `criticalPoints` takes O(M) time where M <= N. Overall time is linear, O(N).
*   **Space Complexity:** **O(N)** — We store all node values in vector `nums` (up to 10^5 elements) and store indices in vector `criticalPoints` (up to 10^5 elements).

---

### Optimization Potential

**Can this be improved?**
**Yes.** While the time complexity is already O(N), the space complexity can be reduced from **O(N) to O(1)** (constant extra memory). This explains why the provided solution beats only ~12% on runtime and ~5% on memory: allocating dynamic vectors of size 10^5 incurs high memory overhead and cache misses.

#### How the Optimization Works
Instead of storing all node values in an array and then finding critical points, we can find critical points **on the fly in a single pass** directly over the linked list pointers.

We only need to track four small variables during traversal:
1.  `firstIndex`: The index of the *very first* critical point found.
2.  `prevIndex`: The index of the *most recent* critical point found.
3.  `minDist`: The running minimum distance between consecutive critical points.
4.  `currIndex`: The current node position index.

As we traverse with three pointers (`prev`, `curr`, `curr->next`):
*   Check if `curr` is a peak or valley.
*   If it is a critical point:
    *   If `firstIndex` is not set yet, set `firstIndex = currIndex` and `prevIndex = currIndex`.
    *   If `firstIndex` was already set, we update `minDist = min(minDist, currIndex - prevIndex)`, and then update `prevIndex = currIndex`.
*   After traversing the entire list:
    *   If `prevIndex == firstIndex` (fewer than 2 critical points found), return `{-1, -1}`.
    *   Otherwise, `maxDist = prevIndex - firstIndex`. Return `{minDist, maxDist}`.

#### Optimized C++ Code Snippet
```cpp
class Solution {
public:
    vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        // We need at least 3 nodes to have any critical point
        if (!head || !head->next || !head->next->next) return {-1, -1};

        ListNode* prev = head;
        ListNode* curr = head->next;
        int currIndex = 1;

        int firstIndex = -1;
        int prevIndex = -1;
        int minDist = INT_MAX;

        while (curr->next) {
            bool isMaxima = (curr->val > prev->val && curr->val > curr->next->val);
            bool isMinima = (curr->val < prev->val && curr->val < curr->next->val);

            if (isMaxima || isMinima) {
                if (firstIndex == -1) {
                    firstIndex = currIndex; // Save index of first critical point
                } else {
                    minDist = min(minDist, currIndex - prevIndex); // Update min gap
                }
                prevIndex = currIndex; // Keep track of latest critical point
            }
            prev = curr;
            curr = curr->next;
            currIndex++;
        }

        if (firstIndex == -1 || firstIndex == prevIndex) {
            return {-1, -1}; // Fewer than 2 critical points found
        }

        return {minDist, prevIndex - firstIndex};
    }
};
```

#### Final Complexity Comparison
*   **Improved Time Complexity:** **O(N)** — Performs a single pass through the linked list.
*   **Improved Space Complexity:** **O(1)** — Uses only a few primitive variables regardless of list length.
*   **Theoretical Best:** **O(N) Time and O(1) Space**. Every node must be examined at least once to determine if it is a peak/valley, so O(N) time is required. The improved version achieves the theoretical optimal limit.

---

## Edge Cases Handled

1.  **Fewer than 2 critical points (0 or 1 critical point):**
    *   *Input:* `[3, 1]` or `[2, 3, 3, 2]`
    *   *Handling:* Checks `m < 2` (or `firstIndex == prevIndex` in optimized code) and safely returns `[-1, -1]`.

2.  **Flat regions / Equal adjacent values (Plateaus):**
    *   *Input:* `[2, 2, 2]` or `[1, 3, 3, 1]`
    *   *Handling:* Strict inequality conditions (`>` and `<`) ensure adjacent duplicates are ignored. A node equal to its neighbor is never flagged as a peak or valley.

3.  **Short Linked Lists (length 2 or 3):**
    *   *Input:* `[1, 2]`, `[1, 2, 3]`
    *   *Handling:* The loop starts checking at index 1 and stops before index `n - 1`. Short lists naturally fail to find 2 critical points and return `[-1, -1]`.

4.  **Multiple Critical Points with Identical Spacing:**
    *   *Input:* Peaks at indices 2, 5, and 8.
    *   *Handling:* `min()` correctly processes equal distances without breaking, returning equal values for minimum and maximum distances if appropriate.
