![Runtime](https://img.shields.io/badge/Runtime-15%20ms%20(beats%2092.07%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-17.5%20MB%20(beats%2057.64%25)-yellow?style=for-the-badge)

---

## Problem Explained

The problem asks us to find all unique groups of four numbers in an array `nums` that add up to a given number called `target`. Each group of four numbers is called a **quadruplet**.

Here are the key rules:
* You must use four distinct index positions from the array for each quadruplet.
* The sum of the four numbers must equal `target`.
* The final list of quadruplets must not contain any duplicates.
* You can return the quadruplets in any order.

### Example
If `nums = [1, 0, -1, 0, -2, 2]` and `target = 0`:
* Sorting the array gives `[-2, -1, 0, 0, 1, 2]`.
* Valid unique quadruplets that sum to `0` are `[-2, -1, 1, 2]`, `[-2, 0, 0, 2]`, and `[-1, 0, 0, 1]`.

---

## Intuition

If we tried checking every possible combination of four numbers using four nested loops, it would take too long. We can make this much faster by sorting the array first.

When an array is sorted, we can pick the first two numbers using two loops, and then use the **two-pointer** technique to find the remaining two numbers.

1. **Fix the first two numbers:** We use index `i` for the first number and index `j` for the second number.
2. **Use two pointers for the rest:** We place a left pointer `k` right after `j`, and a right pointer `l` at the very end of the array.
3. **Adjust based on the sum:** 
   * If the sum of all four numbers is too small, we move `k` to the right to pick a larger number.
   * If the sum is too large, we move `l` to the left to pick a smaller number.
   * If the sum matches `target`, we record the group and move both pointers inward.

Because the array is sorted, duplicate values sit right next to each other. We can easily skip identical numbers at every step to prevent duplicate quadruplets in our output.

---

## Approach

Here is how the code works step-by-step:

* `sort( nums.begin() , nums.end());`: Sorts the array in non-decreasing order. This allows us to use two pointers and skip duplicate values easily.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts the outermost loop to pick the first element, `nums[i]`.
* `if( i>0 && nums[i]==nums[i-1]){ continue; }`: Checks if `nums[i]` is the same as the previous element. If it is, it skips to the next iteration to prevent duplicate quadruplets.
* `for( int j=i+1 ; j<nums.size() ; j++ )`: Starts the second loop to pick the second element, `nums[j]`.
* `if( j!=i+1 && nums[j]==nums[j-1]){ continue; }`: Skips duplicate values for the second element, making sure we only process each unique value of `nums[j]` once for a given `i`.
* `int k=j+1;`: Places the left pointer `k` immediately after index `j`.
* `int l=nums.size()-1;`: Places the right pointer `l` at the last index of the array.
* `while( k<l )`: Runs a loop to move `k` and `l` toward each other until they meet.
* `long long sum = (long long)nums[i]+nums[j]+nums[k]+nums[l];`: Calculates the sum of the four numbers. It converts the first value to `long long` to prevent integer overflow when adding large numbers.
* `if( sum==target )`: Checks if the current sum matches the target.
* `vector<int>temp = {nums[i],nums[j],nums[k],nums[l]}; ans.push_back( temp );`: Creates a 4-element vector with the matching numbers and adds it to the answer list.
* `k++; l--;`: Moves both pointers inward after finding a valid match.
* `while( k<l && nums[k]==nums[k-1]){ k++; }`: Advances `k` past any duplicate values to avoid repeating the same third number.
* `while( k<l && nums[l]==nums[l+1]){ l--; }`: Moves `l` backward past any duplicate values to avoid repeating the same fourth number.
* `else if( sum<target ){ k++; }`: If the sum is less than `target`, moves the left pointer `k` rightward to get a larger sum.
* `else{ l--; }`: If the sum is greater than `target`, moves the right pointer `l` leftward to get a smaller sum.
* `return ans;`: Returns the list containing all unique quadruplets found.

---

## Dry Run

### Case 1: Typical case with mixed values
* Input: `nums = [1, 0, -1, 0, -2, 2]`, `target = 0`
* Sorted `nums = [-2, -1, 0, 0, 1, 2]` (length = 6)

| `i` | `nums[i]` | `j` | `nums[j]` | `k` | `nums[k]` | `l` | `nums[l]` | `sum` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | -2 | 1 | -1 | 2 | 0 | 5 | 2 | -1 | `sum < target` (-1 < 0), increment `k` to 3 |
| 0 | -2 | 1 | -1 | 3 | 0 | 5 | 2 | -1 | `sum < target` (-1 < 0), increment `k` to 4 |
| 0 | -2 | 1 | -1 | 4 | 1 | 5 | 2 | 0 | `sum == target`, add `[-2, -1, 1, 2]`, `k++` (5), `l--` (4). Loop ends (`k >= l`) |
| 0 | -2 | 2 | 0 | 3 | 0 | 5 | 2 | 0 | `sum == target`, add `[-2, 0, 0, 2]`, `k++` (4), `l--` (4). Loop ends |
| 0 | -2 | 3 | 0 | - | - | - | - | - | Skip duplicate `nums[j] == nums[j-1]` (0 == 0) |
| 0 | -2 | 4 | 1 | 5 | 2 | 5 | 2 | - | Loop ends (`k >= l`) |
| 1 | -1 | 2 | 0 | 3 | 0 | 5 | 2 | 1 | `sum > target` (1 > 0), decrement `l` to 4 |
| 1 | -1 | 2 | 0 | 3 | 0 | 4 | 1 | 0 | `sum == target`, add `[-1, 0, 0, 1]`, `k++` (4), `l--` (3). Loop ends |
| 1 | -1 | 3 | 0 | - | - | - | - | - | Skip duplicate `nums[j] == nums[j-1]` (0 == 0) |
| 2 | 0 | 3 | 0 | 4 | 1 | 5 | 2 | 3 | `sum > target` (3 > 0), decrement `l` to 4. Loop ends |
| 3 | 0 | - | - | - | - | - | - | - | Skip duplicate `nums[i] == nums[i-1]` (0 == 0) |

* Final `ans`: `[[-2, -1, 1, 2], [-2, 0, 0, 2], [-1, 0, 0, 1]]`

---

### Case 2: Edge case with all identical values
* Input: `nums = [2, 2, 2, 2, 2]`, `target = 8`
* Sorted `nums = [2, 2, 2, 2, 2]` (length = 5)

| `i` | `nums[i]` | `j` | `nums[j]` | `k` | `nums[k]` | `l` | `nums[l]` | `sum` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 2 | 1 | 2 | 2 | 2 | 4 | 2 | 8 | `sum == target`, add `[2, 2, 2, 2]`, `k++` (3), `l--` (3). Skip duplicate `k` and `l`. Loop ends |
| 0 | 2 | 2 | 2 | - | - | - | - | - | Skip duplicate `nums[j] == nums[j-1]` |
| 0 | 2 | 3 | 2 | - | - | - | - | - | Skip duplicate `nums[j] == nums[j-1]` |
| 1 | 2 | - | - | - | - | - | - | - | Skip duplicate `nums[i] == nums[i-1]` |
| 2 | 2 | - | - | - | - | - | - | - | Skip duplicate `nums[i] == nums[i-1]` |

* Final `ans`: `[[2, 2, 2, 2]]`

---

## Time & Space Complexity

* **Time Complexity:** **O(N^3)** where N is the length of `nums`.
  * Sorting the array takes **O(N log N)** time.
  * The outer loop `i` runs **N** times.
  * The inner loop `j` runs up to **N** times.
  * The two-pointer scan with `k` and `l` processes the remaining elements in **O(N)** time total per pair of `(i, j)`.
  * Multiplying these gives **N * N * N = O(N^3)** total time.

* **Space Complexity:** **O(1)** auxiliary space (excluding space used by the output list `ans` and C++ internal sort stack which takes **O(log N)** space).

---

### Can We Improve Complexity?

The worst-case time complexity for finding all quadruplets without using extra space structures is theoretically bound by **O(N^3)**. We cannot reduce the asymptotic worst-case big-O complexity further.

However, we can **significantly optimize real-world execution speed** by adding **pruning checks**. 

Before running the inner loops or pointer scans, we can check:
1. **Minimum possible sum:** If the sum of `nums[i]` and the three smallest remaining numbers is greater than `target`, no valid quadruplets exist for this `i` or any larger `i`. We can `break` early.
2. **Maximum possible sum:** If the sum of `nums[i]` and the three largest remaining numbers is less than `target`, this `i` is too small to contribute to any valid quadruplet. We can `continue` to the next `i`.

Here is how to add pruning inside the loops:

```cpp
// Inside loop 'i':
int n = nums.size();
if ((long long)nums[i] + nums[i+1] + nums[i+2] + nums[i+3] > target) break;
if ((long long)nums[i] + nums[n-1] + nums[n-2] + nums[n-3] < target) continue;

// Inside loop 'j':
if ((long long)nums[i] + nums[j] + nums[j+1] + nums[j+2] > target) break;
if ((long long)nums[i] + nums[j] + nums[n-1] + nums[n-2] < target) continue;
```

* **Connecting the code to logic:**
  * `nums[i] + nums[i+1] + nums[i+2] + nums[i+3]` calculates the smallest possible sum starting with `nums[i]`. If even this sum is bigger than `target`, higher numbers will also be too big, so we `break`.
  * `nums[i] + nums[n-1] + nums[n-2] + nums[n-3]` calculates the largest possible sum starting with `nums[i]`. If this sum is smaller than `target`, `nums[i]` is too small, so we `continue` to try a larger `nums[i]`.

* **Resulting Complexity:**
  * **Time Complexity:** Still **O(N^3)** worst-case, but runs much faster in practice on average inputs.
  * **Space Complexity:** **O(1)** auxiliary space.
  * **Theoretical Limit:** **O(N^3)** is the optimal time complexity for 4Sum. The pruned version reaches this optimal limit while maximizing real-world speed.

---

## Edge Cases Handled

* **Duplicate Array Values:** Handled explicitly by the duplicate-skipping `if` conditions and `while` loops (`nums[i] == nums[i-1]`, `nums[j] == nums[j-1]`, `nums[k] == nums[k-1]`, and `nums[l] == nums[l+1]`).
* **Integer Overflow:** Handled by casting calculations to `(long long)` before addition, preventing memory wrapping when numbers near `10^9` or `-10^9` are added.
* **Array Length Less Than 4:** Loop bounds natively prevent execution, safely returning an empty `ans` list without out-of-bounds access.
* **Negative Target and Elements:** The two-pointer logic relies strictly on sorted order, working correctly regardless of whether numbers are positive, negative, or zero.
