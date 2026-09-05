![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2025.82%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-92.12%20MB%20(beats%2024.22%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of distinct numbers. You need to remove two specific targets from this row:
1. The **smallest number** (minimum).
2. The **largest number** (maximum).

You are only allowed to remove numbers from the **far left** (front) or the **far right** (back) of the row. Each number you take off counts as 1 deletion. 

Your goal is to find the **smallest total number of deletions** needed to get rid of both the minimum and maximum values.

For example, given the array `[2, 10, 7, 5, 4, 1, 8, 6]`:
- Smallest value is `1` (at index 5).
- Largest value is `10` (at index 1).
- We can remove `10` by popping 2 elements from the front.
- We can remove `1` by popping 3 elements from the back.
- Total deletions = 2 + 3 = 5.

---

## Intuition

First, scan the array once to find where the minimum and maximum numbers are located (their indices).

Once you know their positions, say `first` (the one closer to the left) and `second` (the one closer to the right), you only ever have **3 strategies** to clear both numbers:

1. **Delete both from the left:** Keep removing elements from the left side until you reach the target that is further to the right.
2. **Delete both from the right:** Keep removing elements from the right side until you reach the target that is further to the left.
3. **Delete from both sides:** Remove one target from the left side and the other target from the right side.

The minimum cost among these 3 choices is guaranteed to be the optimal answer.

---

## Approach

Here is how the provided code implements this logic step-by-step:

* `int miniInd = 0 , maxInd = 0; int eleMin = nums[0] , eleMax = nums[0];`  
  Initializes tracking variables. We assume index 0 holds both the smallest (`eleMin`) and largest (`eleMax`) values to start.

* `for( int i=1 ; i<nums.size() ; i++ )`  
  Loops through the rest of the array to find the true minimum and maximum values and their respective index positions (`miniInd` and `maxInd`).

* `Lmin = miniInd+1; Rmin = nums.size() - miniInd;`  
  Calculates how many deletions it takes to reach the minimum element from the left (`Lmin`) versus from the right (`Rmin`).

* `Lmax = maxInd+1; Rmax = nums.size() - maxInd;`  
  Calculates how many deletions it takes to reach the maximum element from the left (`Lmax`) versus from the right (`Rmax`).

* `if( min(Lmin,Rmin) < min(Lmax,Rmax) )`  
  Compares which target (min or max) is closer to an outer boundary. It decides to delete the closer target first.

* `count = count + min(Lmin,Rmin);`  
  Adds the deletion count needed to remove the closer target.

* `if( Lmin<Rmin ) { Lmax -= Lmin; } else { Rmax -= Rmin; }`  
  If we deleted from the left, we reduce the left distance needed for the remaining target (`Lmax -= Lmin`). If we deleted from the right, we reduce its right distance (`Rmax -= Rmin`).

* `count = count + min(Rmax,Lmax);`  
  Adds the remaining deletions needed to clear the second target and returns `count`.

---

## Dry Run

### Case 1: `nums = [2, 10, 7, 5, 4, 1, 8, 6]` (Array size = 8)

* Minimum value: `1` at `miniInd = 5`
* Maximum value: `10` at `maxInd = 1`

Calculated distances:
- `Lmin = 5 + 1 = 6`, `Rmin = 8 - 5 = 3`
- `Lmax = 1 + 1 = 2`, `Rmax = 8 - 1 = 7`

| Step | Variables & Values | Decisions / Actions |
| :--- | :--- | :--- |
| 1 | `Lmin=6, Rmin=3, Lmax=2, Rmax=7` | Compare `min(6,3) = 3` vs `min(2,7) = 2`. Since `3 < 2` is false, jump to `else`. |
| 2 | `count = 0 + min(2,7) = 2` | Clear maximum target (`10`) from the left using 2 deletions. |
| 3 | `Lmax < Rmax` (2 < 7 is true) | Subtract used left deletions from minimum's left distance: `Lmin = 6 - 2 = 4`. |
| 4 | `count = 2 + min(3, 4) = 5` | Clear remaining target (`1`) from the right using 3 deletions. Total = 5. |

---

### Case 2: `nums = [0, -4, 19, 1, 8, -2, -3, 5]` (Array size = 8)

* Minimum value: `-4` at `miniInd = 1`
* Maximum value: `19` at `maxInd = 2`

Calculated distances:
- `Lmin = 1 + 1 = 2`, `Rmin = 8 - 1 = 7`
- `Lmax = 2 + 1 = 3`, `Rmax = 8 - 2 = 6`

| Step | Variables & Values | Decisions / Actions |
| :--- | :--- | :--- |
| 1 | `Lmin=2, Rmin=7, Lmax=3, Rmax=6` | Compare `min(2,7) = 2` vs `min(3,6) = 3`. Since `2 < 3` is true, take `if` branch. |
| 2 | `count = 0 + min(2,7) = 2` | Clear minimum target (`-4`) from the left using 2 deletions. |
| 3 | `Lmin < Rmin` (2 < 7 is true) | Subtract used left deletions from maximum's left distance: `Lmax = 3 - 2 = 1`. |
| 4 | `count = 2 + min(6, 1) = 3` | Clear remaining target (`19`) from the left using 1 more deletion. Total = 3. |

---

## Time & Space Complexity

* **Time Complexity:** **O(N)** — We loop through the array of size N exactly once to find `miniInd` and `maxInd`. All distance calculations and comparisons take O(1) constant time.
* **Space Complexity:** **O(1)** — We only store a few integer variables (`miniInd`, `maxInd`, `Lmin`, `Rmin`, etc.), requiring no extra memory relative to input size.

### Can it be improved?

The **O(N)** time and **O(1)** space are already theoretically optimal because you must inspect every element at least once to identify the minimum and maximum values.

However, the deletion logic inside the `if-else` branches can be greatly simplified. Instead of mutating variables with subtraction (`Lmax -= Lmin`), we can directly evaluate the 3 possible choices using standard library functions:

1. **Remove both from left:** `max(left_index_1, left_index_2)`
2. **Remove both from right:** `max(right_index_1, right_index_2)`
3. **Remove from both sides:** `left_index_1 + right_index_2`

```cpp
// Cleaner and faster direct comparison snippet:
int n = nums.size();
int i = min(miniInd, maxInd);
int j = max(miniInd, maxInd);

// Option 1: Both from left -> j + 1
// Option 2: Both from right -> n - i
// Option 3: One from left, one from right -> (i + 1) + (n - j)
return min({j + 1, n - i, (i + 1) + (n - j)});
```

- **Improved Complexity:** Same O(N) time and O(1) space, but with cleaner code, fewer branches, and better execution performance.
- **Theoretical Limit:** Reached.

---

## Edge Cases Handled

* **Single Element Array (`nums.length == 1`):** The single element is both the minimum and maximum. The code sets `miniInd = 0` and `maxInd = 0`, computing `Lmin = 1, Rmin = 1, Lmax = 1, Rmax = 1`. The calculated deletions equal `1`, which is correct.
* **Negative Numbers:** The original comparisons (`eleMin > nums[i]`) correctly evaluate negative values (e.g., `-4 < 0`).
* **Adjacent Min and Max:** When the minimum and maximum are right next to each other, the distance adjustments properly compute that removing one makes the next one immediately reachable.
* **Min and Max at Extremes:** If min is at index `0` and max is at index `n - 1`, the code correctly compares removing both from opposite ends versus sweeping from one side.
