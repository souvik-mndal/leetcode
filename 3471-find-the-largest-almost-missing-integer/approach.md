![Runtime](https://img.shields.io/badge/Runtime-62%20ms%20(beats%205.09%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-63.91%20MB%20(beats%208.36%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums` and a window size called `k`. 

A **subarray** is a continuous chunk of numbers from the array. In this problem, we look at all possible continuous chunks that have a length of exactly `k`.

A number is called **almost missing** if it shows up in **exactly one** of these `k`-length chunks. If a number shows up in zero chunks, or in two or more chunks, it does not qualify.

Your goal is to find the **largest** number in the array that is almost missing. If no number appears in exactly one `k`-length chunk, return `-1`.

### Example
Suppose `nums = [3, 9, 2, 1, 7]` and `k = 3`.

The chunks of length 3 are:
1. `[3, 9, 2]`
2. `[9, 2, 1]`
3. `[2, 1, 7]`

Now let's count how many chunks each number appears in:
* `3` appears in 1 chunk: `[3, 9, 2]`.
* `9` appears in 2 chunks: `[3, 9, 2]` and `[9, 2, 1]`.
* `2` appears in 3 chunks: `[3, 9, 2]`, `[9, 2, 1]`, and `[2, 1, 7]`.
* `1` appears in 2 chunks: `[9, 2, 1]` and `[2, 1, 7]`.
* `7` appears in 1 chunk: `[2, 1, 7]`.

The numbers that appear in exactly 1 chunk are `3` and `7`. The largest of these is `7`, so the answer is `7`.

---

## Intuition

The standard way to solve this is to follow the problem description step by step:

1. Pick a number from `nums`.
2. Slide a window of size `k` across `nums` from left to right.
3. Count how many of these windows contain that number.
4. Remember this count for each number.
5. Look at all numbers that had a count of exactly 1, and pick the biggest one.

This brute-force strategy directly tests every number against every window.

---

## Approach

Here is how the provided C++ code carries out this idea:

* `bool counting( vector<int>nums , int st , int end , int target )`: This helper function scans `nums` from index `st` to index `end`. It returns `1` if `target` is found inside that range, and `0` if it is not.
* `vector<int>store(nums.size());`: Creates a tracking array `store` of the same size as `nums`. `store[i]` will hold the total number of `k`-sized windows that contain `nums[i]`.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts an outer loop to evaluate each number `nums[i]` one by one.
* `int st=0 , end=k;`: Sets up the start index `st` and end index `end` for the very first window of length `k`.
* `int total = counting(nums , st , end-1 , nums[i]);`: Checks if `nums[i]` is inside the first window (from index `0` to `k-1`) and sets `total` to `1` if present, or `0` if absent.
* `while(end<nums.size() )`: Starts a inner loop that shifts the window rightward by one step at a time until reaching the end of `nums`.
* `total+=counting(nums , st , end , nums[i]);`: Checks if `nums[i]` is in the newly shifted window and adds the result (`1` or `0`) to `total`.
* `store[i]=total;`: Saves the total number of windows containing `nums[i]` into `store[i]`.
* `int ans = -1;`: Initializes `ans` to `-1`, which serves as the default answer if no almost missing number is found.
* `if( store[i] == 1 && ans < nums[i])`: Loops through `store`. If `nums[i]` appeared in exactly `1` window and is larger than the current `ans`, `ans` is updated to `nums[i]`.

---

## Dry Run

### Case 1: Standard case (`nums = [3, 9, 2, 1, 7]`, `k = 3`)

Windows of size 3 are: `[3, 9, 2]` (indices 0..2), `[9, 2, 1]` (indices 1..3), `[2, 1, 7]` (indices 2..4).

| `i` | `nums[i]` | Calculated `total` | `store[i]` | `ans` | Action |
|---|---|---|---|---|---|
| 0 | 3 | 1 | 1 | -1 | 3 is only in window 1. `store[0] = 1`. |
| 1 | 9 | 2 | 2 | -1 | 9 is in windows 1 and 2. `store[1] = 2`. |
| 2 | 2 | 3 | 3 | -1 | 2 is in all 3 windows. `store[2] = 3`. |
| 3 | 1 | 2 | 2 | -1 | 1 is in windows 2 and 3. `store[3] = 2`. |
| 4 | 7 | 1 | 1 | -1 | 7 is only in window 3. `store[4] = 1`. |
| End | - | - | - | 7 | Final pass: `store[0] == 1` sets `ans = 3`. `store[4] == 1` sets `ans = 7`. Returns 7. |

### Case 2: Duplicate numbers with no valid result (`nums = [0, 0]`, `k = 1`)

Windows of size 1 are: `[0]` (index 0) and `[0]` (index 1).

| `i` | `nums[i]` | Calculated `total` | `store[i]` | `ans` | Action |
|---|---|---|---|---|---|
| 0 | 0 | 2 | 2 | -1 | 0 appears in both length-1 windows. `store[0] = 2`. |
| 1 | 0 | 2 | 2 | -1 | 0 appears in both length-1 windows. `store[1] = 2`. |
| End | - | - | - | -1 | Final pass finds no `store[i] == 1`. Returns default `ans = -1`. |

---

## Time & Space Complexity

* **Current Time Complexity:** `O(N * (N - k + 1) * k)`
  * **Why:** The outer loop runs `N` times. The inner `while` loop runs `N - k + 1` times (once per window). Inside that loop, `counting` scans `k` elements. When `k` is roughly half of `N`, this simplifies to around `O(N^3)` operations.
* **Current Space Complexity:** `O(N)`
  * **Why:** The code allocates a vector called `store` of size `N` to hold the subarray counts.

### Can this be improved?

**Yes, drastically!** We can improve this from `O(N^3)` down to **`O(N)` time** and **`O(1)` extra space** by using logical observations:

1. **If `k == 1`**: Every window is just a single element. A number appears in exactly 1 window if and only if it appears exactly once in the entire array. We can just count the total frequency of each number and pick the largest one with frequency 1.
2. **If `k == N`**: There is only 1 window (the entire array). Every number present in the array appears in this 1 window. So the answer is simply the maximum number in `nums`.
3. **If `1 < k < N`**: Any number in the middle of the array (not at the very ends) is guaranteed to overlap with at least 2 adjacent windows of size `k`. Therefore, middle numbers can **never** appear in exactly 1 window. Only boundary numbers—`nums[0]` (the first element) and `nums[N - 1]` (the last element)—can potentially appear in only 1 window. They will, provided they do not repeat elsewhere in `nums`.

### Optimized Code Snippet

```cpp
int largestInteger(vector<int>& nums, int k) {
    int n = nums.size();
    // Count overall frequencies of numbers in nums
    int freq[51] = {0};
    for (int x : nums) freq[x]++;

    // Case 1: k == 1
    if (k == 1) {
        int ans = -1;
        for (int i = 0; i <= 50; i++) {
            if (freq[i] == 1) ans = max(ans, i);
        }
        return ans;
    }

    // Case 2: k == n
    if (k == n) {
        int ans = -1;
        for (int x : nums) ans = max(ans, x);
        return ans;
    }

    // Case 3: 1 < k < n (only boundary elements nums[0] and nums[n-1] can qualify)
    int ans = -1;
    if (freq[nums[0]] == 1) ans = max(ans, nums[0]);
    if (freq[nums[n - 1]] == 1) ans = max(ans, nums[n - 1]);

    return ans;
}
```

* `freq[51] = {0}`: Tracks total occurrences of each number using a fixed-size array (since `nums[i] <= 50`).
* `if (k == 1)`: Returns the highest value that appears exactly once in `nums`.
* `if (k == n)`: Returns the largest overall number in `nums`.
* `freq[nums[0]] == 1` & `freq[nums[n - 1]] == 1`: Checks if either boundary element is unique overall, as middle elements are mathematically guaranteed to sit inside 2 or more windows.

* **Improved Time Complexity:** `O(N)` — requires only a single pass to count frequencies.
* **Improved Space Complexity:** `O(1)` — uses a fixed array of size 51.
* **Theoretical Best Complexity:** `O(N)` time and `O(1)` space. The improved solution reaches this theoretical limit.

---

## Edge Cases Handled

* **`k = 1`**: Correctly handles individual element windows without out-of-bounds errors.
* **`k = nums.size()`**: Correctly treats the entire array as a single window.
* **Duplicate Values**: Correctly ignores duplicate values when their frequency causes them to appear in multiple windows (returning `-1` if no number qualifies).
* **No Valid Answer**: Correctly defaults to `-1` when every number appears in multiple windows or zero numbers qualify.
* **Small Inputs**: Handles the smallest allowed array size (`nums.length = 1`).
