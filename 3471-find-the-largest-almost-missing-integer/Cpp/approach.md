![Runtime](https://img.shields.io/badge/Runtime-62%20ms%20(beats%205.01%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-63.9%20MB%20(beats%208.67%25)-red?style=for-the-badge)

---

## Problem Explained

We are given an array of numbers called **`nums`** and a window size **`k`**. 

A **subarray** is a contiguous slice of the array. We look at all possible subarrays of length **`k`** inside **`nums`**.

A number is called **almost missing** if it appears in **exactly one** of these size-`k` subarrays. Our goal is to find the **largest** almost missing integer. If no such number exists, we return `-1`.

### Example
Suppose `nums = [3, 9, 2, 1, 7]` and `k = 3`.

The subarrays of size 3 are:
* `[3, 9, 2]`
* `[9, 2, 1]`
* `[2, 1, 7]`

Now let's count how many subarrays contain each number:
* `3` appears in **1** subarray (`[3, 9, 2]`)
* `9` appears in **2** subarrays (`[3, 9, 2]` and `[9, 2, 1]`)
* `2` appears in **3** subarrays
* `1` appears in **2** subarrays
* `7` appears in **1** subarray (`[2, 1, 7]`)

The numbers that appear in exactly 1 subarray are `3` and `7`. The largest among them is `7`, so the answer is `7`.

---

## Intuition

To solve this, we can count how many size-`k` subarrays contain each element of `nums`. 

For every number `nums[i]`, we slide a window of size `k` from left to right across the array. Every time that window contains `nums[i]`, we add 1 to its count. 

After counting for all elements, we filter out any element whose count is not 1. Among the remaining valid elements, we pick the maximum value.

---

## Approach

Here is how the code works step-by-step:

* `vector<int>store(nums.size());`  
  Creates an array named `store` with the same length as `nums` to track how many size-`k` subarrays contain each element `nums[i]`.

* `for( int i=0 ; i<nums.size() ; i++ )`  
  Starts a loop to evaluate every element in `nums` one by one using index `i`.

* `int st=0 , end=k;`  
  Sets the start pointer `st` to `0` and end pointer `end` to `k` to define the first size-`k` window (`[0]` to `[k-1]`).

* `int total = counting(nums , st , end-1 , nums[i]);`  
  Calls the helper function `counting` to check if `nums[i]` is present in the first window. Adds `1` to `total` if found, or `0` if not.

* `st++;`  
  Increments `st` by `1` to move the left boundary forward for the next window.

* `while(end<nums.size() )`  
  Runs a loop to check all remaining size-`k` windows in `nums` until `end` goes past the end of the array.

* `total+=counting(nums , st , end , nums[i]);`  
  Checks if `nums[i]` is inside the current window boundaries from `st` to `end` and adds `1` to `total` if present.

* `end++; st++;`  
  Shifts both window boundaries right by `1` position to prepare for the next window.

* `store[i]=total;`  
  Stores the final total count of size-`k` subarrays containing `nums[i]` into `store[i]`.

* `int ans = -1;`  
  Initializes `ans` to `-1` as the default answer if no number meets the criteria.

* `for( int i=0 ; i<store.size() ; i++ )`  
  Loops through all recorded subarray counts stored in `store`.

* `if( store[i] == 1 && ans < nums[i])`  
  Checks if `nums[i]` appeared in exactly 1 subarray and is larger than the current best answer `ans`.

* `ans = nums[i];`  
  Updates `ans` to `nums[i]` whenever a larger valid number is found.

* `return ans;`  
  Returns the largest almost missing integer, or `-1` if none exist.

---

## Dry Run

### Case 1: Typical Case (`nums = [3, 9, 2, 1, 7]`, `k = 3`)

Windows of size 3 are:
* Window 0: `[3, 9, 2]`
* Window 1: `[9, 2, 1]`
* Window 2: `[2, 1, 7]`

| `i` | `nums[i]` | `total` | `store[i]` | `ans` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 3 | 1 | 1 | -1 | Found only in Window 0 |
| 1 | 9 | 2 | 2 | -1 | Found in Window 0 and Window 1 |
| 2 | 2 | 3 | 3 | -1 | Found in Window 0, Window 1, and Window 2 |
| 3 | 1 | 2 | 2 | -1 | Found in Window 1 and Window 2 |
| 4 | 7 | 1 | 1 | -1 | Found only in Window 2 |
| 0 | 3 | - | 1 | 3 | `store[0] == 1`, update `ans = 3` |
| 4 | 7 | - | 1 | 7 | `store[4] == 1` and `7 > 3`, update `ans = 7` |

Final Return Value: **7**

---

### Case 2: Edge Case with Duplicate Values (`nums = [0, 0]`, `k = 1`)

Windows of size 1 are:
* Window 0: `[0]` (at index 0)
* Window 1: `[0]` (at index 1)

| `i` | `nums[i]` | `total` | `store[i]` | `ans` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 0 | 2 | 2 | -1 | Appears in both size-1 windows |
| 1 | 0 | 2 | 2 | -1 | Appears in both size-1 windows |
| - | - | - | - | -1 | Loop finishes; no `store[i] == 1` exists |

Final Return Value: **-1**

---

## Time & Space Complexity

* **Time Complexity:** **O(N^3)**  
  Let `N` be `nums.size()`. The outer loop runs `N` times. The inner `while` loop runs `N - k + 1` times. Inside the loop, `counting(vector<int> nums, ...)` receives `nums` **by value**, which creates a complete copy of the array of size `N` on every single call (taking `O(N)` time). Multiplying these gives `N * (N - k) * N`, leading to an **O(N^3)** worst-case time complexity.

* **Space Complexity:** **O(N)**  
  The `store` vector uses `O(N)` memory. Additionally, passing `nums` by value in `counting` allocates `O(N)` temporary memory on the call stack.

### Can this be improved?

**Yes, significantly.** We can reduce the time complexity from **O(N^3)** down to **O(N * k)** or even **O(N)**.

#### How the Optimization Works
Instead of checking every element against every window repeatedly:
1. We slide a window of size `k` across `nums`.
2. For each window, we use a hash set to extract unique numbers in that window.
3. We update a **hash map** that tracks how many total windows contained each number.
4. Finally, we iterate through the hash map to find the largest number with a count of `1`.

#### Improved Code Snippet
```cpp
unordered_map<int, int> window_counts;

for (int i = 0; i <= (int)nums.size() - k; i++) {
    unordered_set<int> unique_in_window;
    for (int j = i; j < i + k; j++) {
        unique_in_window.insert(nums[j]);
    }
    for (int num : unique_in_window) {
        window_counts[num]++;
    }
}

int ans = -1;
for (auto& [num, count] : window_counts) {
    if (count == 1 && num > ans) {
        ans = num;
    }
}
return ans;
```

#### Explanation of Optimized Lines
* `unordered_set<int> unique_in_window`: Prevents duplicate entries of the same number within a single size-`k` window from inflating its count.
* `window_counts[num]++`: Counts how many distinct size-`k` subarrays contain `num`.
* `if (count == 1 && num > ans)`: Selects numbers appearing in exactly one window and tracks the maximum value.

#### Resulting Improved Complexity
* **Time Complexity:** **O(N * k)** — We process `N - k + 1` windows, spending `O(k)` time per window. For `N <= 50`, this executes in fewer than 2500 operations.
* **Space Complexity:** **O(N)** — Hash map and set store at most `N` elements.

This improved version easily reaches the optimal performance tier for this problem.

---

## Edge Cases Handled

* **Window size `k = 1`:** Every individual element is its own subarray. Duplicates show up in multiple subarrays, ensuring only truly unique numbers have a count of `1`.
* **Window size `k = nums.size()`:** There is only 1 window containing all elements. All unique elements in `nums` have a count of `1`, so the maximum value in `nums` is correctly returned.
* **Array with repeating values:** Repeated numbers appear in multiple windows, so their counts exceed `1`. The code correctly ignores them.
* **No almost missing integer present:** Returns `-1` whenever every element appears in 0 or multiple size-`k` subarrays.
* **Small arrays (`nums.size() <= 50`):** Handles small boundary sizes without out-of-bounds indexing errors.
