![Runtime](https://img.shields.io/badge/Runtime-67%20ms%20(beats%2039.42%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-149.3%20MB%20(beats%2083.42%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of integers called `nums` and a positive integer `k`.

A subarray is a continuous section of the array (elements right next to each other without skipping any). We call a subarray **good** if no number inside it appears more than `k` times. 

Your goal is to find the length of the **longest good subarray** in `nums`.

### Example
If `nums = [1, 2, 3, 1, 2, 3, 1, 2]` and `k = 2`:
- The slice `[1, 2, 3, 1, 2, 3]` contains `1` twice, `2` twice, and `3` twice.
- No number appears more than `k = 2` times.
- Its length is 6, which is the longest possible valid slice. So the answer is 6.

---

## Intuition

Think of this using a flexible window that stretches over the array from left to right.

1. You expand the window to the right, adding numbers one by one.
2. You keep count of how many times each number appears in the current window using a frequency map (a table of counts).
3. As long as every count is `k` or less, your window is valid, and you track its length.
4. The moment a number's count exceeds `k`, the window becomes invalid. You must shrink it from the left (remove elements) until that number's count goes back down to `k`.

This dynamic sliding window ensures you test all potential valid subarrays without re-checking elements from scratch.

---

## Approach

Here is how the logic works step-by-step:

- `unordered_map<int, int> m;`: Creates a hash map named `m` to store the count of each number inside our current sliding window.
- `int i = 0, j = 0, n = nums.size();`: Sets the left window boundary `i` to index 0, the right window boundary `j` to index 0, and gets total array size `n`.
- `int res = 0;`: Sets up `res` to hold the length of the longest valid window found so far.
- `while (j < n)`: Loops through the array by moving the right pointer `j` from index 0 all the way to `n - 1`.
- `m[nums[j]]++;`: Increases the frequency count of the current number `nums[j]` in map `m`.
- `while (m[nums[j]] > k)`: Checks if the count of `nums[j]` exceeds `k`. If it does, enters a loop to shrink the window from the left until `m[nums[j]]` is no longer greater than `k`.
- `m[nums[i]]--;`: Reduces the count of the leftmost element `nums[i]` in map `m` because it is being removed from our window.
- `i++;`: Shifts the left pointer `i` one step to the right to shorten the window.
- `res = max(res, j - i + 1);`: Measures current valid window length (`j - i + 1`) and updates `res` if this window is longer than previous ones.
- `j++;`: Shifts the right pointer `j` one step to the right to look at the next element.
- `return res;`: Returns the length of the longest valid subarray after checking the whole array.

---

## Dry Run

### Case 1: `nums = [1, 2, 1, 2, 1, 2, 1, 2]`, `k = 1`

| `j` | `nums[j]` | `i` | Count of `nums[j]` in `m` | `res` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 1 | 0 | `m[1] = 1` | 1 | `m[1]` is <= 1. Window `[1]`, length 1. |
| 1 | 2 | 0 | `m[2] = 1` | 2 | `m[2]` is <= 1. Window `[1, 2]`, length 2. |
| 2 | 1 | 1 | `m[1] = 2` -> shrinks to 1 | 2 | Count of 1 exceeds 1. Shrink window from left: decrement `m[1]`, set `i = 1`. Valid window `[2, 1]`, length 2. |
| 3 | 2 | 2 | `m[2] = 2` -> shrinks to 1 | 2 | Count of 2 exceeds 1. Shrink window from left: decrement `m[2]`, set `i = 2`. Valid window `[1, 2]`, length 2. |
| 4 | 1 | 3 | `m[1] = 2` -> shrinks to 1 | 2 | Count of 1 exceeds 1. Shrink window from left: decrement `m[1]`, set `i = 3`. Valid window `[2, 1]`, length 2. |
| 5 | 2 | 4 | `m[2] = 2` -> shrinks to 1 | 2 | Count of 2 exceeds 1. Shrink window from left: decrement `m[2]`, set `i = 4`. Valid window `[1, 2]`, length 2. |
| 6 | 1 | 5 | `m[1] = 2` -> shrinks to 1 | 2 | Count of 1 exceeds 1. Shrink window from left: decrement `m[1]`, set `i = 5`. Valid window `[2, 1]`, length 2. |
| 7 | 2 | 6 | `m[2] = 2` -> shrinks to 1 | 2 | Count of 2 exceeds 1. Shrink window from left: decrement `m[2]`, set `i = 6`. Valid window `[1, 2]`, length 2. |

End result: `2`.

---

### Case 2: `nums = [5, 5, 5, 5, 5, 5, 5]`, `k = 4`

| `j` | `nums[j]` | `i` | Count of `nums[j]` in `m` | `res` | Action |
| --- | --- | --- | --- | --- | --- |
| 0 | 5 | 0 | `m[5] = 1` | 1 | `m[5]` <= 4. Window length 1. |
| 1 | 5 | 0 | `m[5] = 2` | 2 | `m[5]` <= 4. Window length 2. |
| 2 | 5 | 0 | `m[5] = 3` | 3 | `m[5]` <= 4. Window length 3. |
| 3 | 5 | 0 | `m[5] = 4` | 4 | `m[5]` <= 4. Window length 4. |
| 4 | 5 | 1 | `m[5] = 5` -> shrinks to 4 | 4 | Count of 5 is 5 (> 4). Decrement `m[5]`, increment `i` to 1. Window length 4. |
| 5 | 5 | 2 | `m[5] = 5` -> shrinks to 4 | 4 | Count of 5 is 5 (> 4). Decrement `m[5]`, increment `i` to 2. Window length 4. |
| 6 | 5 | 3 | `m[5] = 5` -> shrinks to 4 | 4 | Count of 5 is 5 (> 4). Decrement `m[5]`, increment `i` to 3. Window length 4. |

End result: `4`.

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where N is the number of elements in `nums`. Both pointers `i` and `j` start at 0 and move forward up to N steps. Neither pointer ever moves backward. Map operations take O(1) time on average.
- **Space Complexity:** **O(N)** — in the worst case, every element in `nums` is unique, requiring space in map `m` for up to N elements.

### Is this already optimal?

Yes, **O(N) time and O(N) space is theoretically optimal**:
- **Time:** You must look at every element at least once to know if it can form a valid window, so you cannot do better than O(N).
- **Space:** You must store element counts for arbitrary integer values (up to 10^9), requiring O(N) space for hash keys in the worst case.

---

## Edge Cases Handled

- **`k` is large (`k >= nums.size()`):** The count condition `m[nums[j]] > k` is never triggered. The left boundary `i` never moves, and the code safely returns the total array size `n`.
- **`k = 1`:** The sliding window shrinks as soon as any duplicate appears, correctly finding the longest contiguous subarray of unique elements.
- **All elements are the same (e.g., `[5, 5, 5, 5]`):** As soon as the count hits `k + 1`, the left pointer `i` advances on every step, maintaining a maximum window size of `k`.
- **Large element values (`nums[i]` up to 10^9):** Using `unordered_map<int, int>` handles huge number values effortlessly without needing an oversized fixed-size array.
