![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-32.3%20MB%20(beats%2059.56%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a list of unique numbers called `nums`. 

Imagine a full sequence of consecutive numbers starting at the smallest value in `nums` and ending at the largest value. Some numbers in this complete sequence might be missing from `nums`. 

Your job is to find all those missing numbers and return them in a sorted list from smallest to largest. If no numbers are missing, return an empty list.

For example, if `nums = [1, 4, 2, 5]`:
* The smallest number is `1` and the largest is `5`.
* The full range should be `[1, 2, 3, 4, 5]`.
* Comparing this to `nums`, the only missing number is `3`.
* Output: `[3]`

## Intuition

If we sort `nums` from smallest to largest, the numbers should ideally count up by 1 at every step (like `1, 2, 3, 4...`).

We can use a target variable, `toFind`, to keep track of the exact number we expect to see next. We compare `toFind` against our current element in the sorted array.

* If the current element matches `toFind`, everything is on track! We move to the next element in `nums` and increment `toFind`.
* If it does not match, `toFind` must be missing from `nums`. We record `toFind` in our result list and increment `toFind` by 1. We stay on the same element in `nums` so we can compare it against the *new* `toFind` on the next turn.

## Approach

* `vector<int>store;`: Creates an empty list named `store` to collect all missing numbers as we find them.
* `sort( nums.begin(),nums.end());`: Sorts `nums` in ascending order so we can process the numbers sequentially from smallest to largest.
* `int toFind = nums[0];`: Sets `toFind` to the smallest value in the sorted array, which serves as our starting point.
* `int counter = 0;`: Sets `counter` to index 0 to keep track of our position inside `nums`.
* `while( counter < nums.size()){`: Starts a loop that runs until we have processed every number in `nums`.
* `if( nums[counter] == toFind ){`: Checks if the value at the current index of `nums` matches the expected number `toFind`.
* `toFind++; counter++;`: Advances `toFind` to the next expected number and increments `counter` to check the next element in `nums`.
* `else{ store.push_back(toFind); toFind++; }`: Executes when `toFind` is missing from `nums`. Saves `toFind` into `store` and increments `toFind` by 1 without advancing `counter`.
* `return store;`: Returns the final list of missing numbers.

## Dry Run

### Case 1: Single missing number (`nums = [1, 4, 2, 5]`)

After sorting, `nums` becomes `[1, 2, 4, 5]`. 
Initial state: `toFind = 1`, `counter = 0`, `store = []`.

| counter | nums[counter] | toFind | store | Action |
|---|---|---|---|---|
| 0 | 1 | 1 | [] | Match! `toFind` becomes 2, `counter` becomes 1. |
| 1 | 2 | 2 | [] | Match! `toFind` becomes 3, `counter` becomes 2. |
| 2 | 4 | 3 | [] | Mismatch! Add 3 to `store`. `toFind` becomes 4. |
| 2 | 4 | 4 | [3] | Match! `toFind` becomes 5, `counter` becomes 3. |
| 3 | 5 | 5 | [3] | Match! `toFind` becomes 6, `counter` becomes 4. Loop ends. |

### Case 2: Multiple missing numbers (`nums = [5, 1]`)

After sorting, `nums` becomes `[1, 5]`. 
Initial state: `toFind = 1`, `counter = 0`, `store = []`.

| counter | nums[counter] | toFind | store | Action |
|---|---|---|---|---|
| 0 | 1 | 1 | [] | Match! `toFind` becomes 2, `counter` becomes 1. |
| 1 | 5 | 2 | [] | Mismatch! Add 2 to `store`. `toFind` becomes 3. |
| 1 | 5 | 3 | [2] | Mismatch! Add 3 to `store`. `toFind` becomes 4. |
| 1 | 5 | 4 | [2, 3] | Mismatch! Add 4 to `store`. `toFind` becomes 5. |
| 1 | 5 | 5 | [2, 3, 4] | Match! `toFind` becomes 6, `counter` becomes 2. Loop ends. |

## Time & Space Complexity

* **Time Complexity:** O(N * log N), where N is the length of `nums`. Sorting the array dominates the running time. The subsequent `while` loop runs in linear time proportional to the range between the minimum and maximum values.
* **Space Complexity:** O(1) auxiliary space (excluding the output array `store`), because we only use a few simple integer variables (`toFind` and `counter`).

### Can this be improved?

**Yes, we can improve the time complexity to O(N).**

Instead of sorting the array, we can insert all elements of `nums` into a Hash Table (a set). Then, we find the minimum and maximum values in `nums`. Finally, we loop from the minimum value to the maximum value and check if each number exists in our hash set. Looking up a number in a hash set takes average O(1) time.

Here is how the key part of the improved logic looks:

```cpp
int minVal = *min_element(nums.begin(), nums.end());
int maxVal = *max_element(nums.begin(), nums.end());
unordered_set<int> present(nums.begin(), nums.end());

for (int num = minVal; num <= maxVal; num++) {
    if (present.find(num) == present.end()) {
        store.push_back(num);
    }
}
```

* `minVal` and `maxVal` find the start and end of the range in O(N) time.
* `unordered_set<int> present(...)` stores all original numbers in O(N) time.
* `present.find(num) == present.end()` checks in O(1) time if `num` is missing.

* **Improved Time Complexity:** O(N + Range) which simplifies to **O(N)** for fixed range bounds.
* **Improved Space Complexity:** **O(N)** to store elements in the hash set.
* **Theoretical Best Complexity:** **O(N)** time, because any algorithm must look at every element in `nums` at least once to determine the minimum, maximum, and present values. The hash table approach reaches this theoretical limit.

## Edge Cases Handled

* **No missing elements:** If all numbers in the range are present (e.g., `nums = [7, 8, 6, 9]`), the code matches every element without triggering the `else` block, returning an empty `store` vector.
* **Large gaps / Consecutive missing numbers:** If multiple numbers in a row are missing (e.g., `nums = [5, 1]`), the code repeatedly hits the `else` branch, adding each missing value while keeping `counter` stationary until the gap is filled.
* **Unsorted input:** The initial `sort` call ensures that inputs provided in random order (e.g., `nums = [4, 1, 5, 2]`) are correctly ordered before processing.
* **Minimum constraint size:** Handles small arrays down to `nums.length = 2` without array index out-of-bounds issues.
