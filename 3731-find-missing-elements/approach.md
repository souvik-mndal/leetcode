![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-32.32%20MB%20(beats%2054.68%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given an array of unique integers called `nums`. Originally, `nums` was a continuous range of numbers starting from a smallest number up to a largest number, with no numbers skipped.

However, some numbers inside that range might have been removed. The smallest and largest numbers from the original range are guaranteed to still be present in `nums`.

Your task is to find all the missing numbers from that original range and return them in a sorted list. If no numbers are missing, return an empty list.

**Example:**
If `nums = [1, 4, 2, 5]`:
- The smallest number is `1`.
- The largest number is `5`.
- The full range should be `[1, 2, 3, 4, 5]`.
- Looking at `nums`, the number `3` is missing.
- You return `[3]`.

---

## Intuition

If the array is sorted, the numbers should count up by 1 at each step (e.g., `1, 2, 3, 4`).

When we sort `nums`, the smallest number sits at index `0`. We can keep track of an expected number (`toFind`), starting from `nums[0]`. 

We then compare `toFind` with the actual number in `nums`:
- If they match, the number is present! We increment `toFind` to look for the next expected number and move to the next item in `nums`.
- If they do not match, `toFind` is missing! We save `toFind` to our answer list, and increment `toFind` to check if the next expected number is present, while staying at the same spot in `nums`.

---

## Approach

Here is how the code works step-by-step:

- `vector<int>store;`: Creates an empty list named `store` to collect all missing numbers.
- `sort( nums.begin(),nums.end());`: Sorts `nums` in ascending order. This places the smallest number at the beginning and arranges existing numbers in sequence.
- `int toFind = nums[0];`: Sets `toFind` to the smallest element in `nums`. This variable represents the number we expect to see next in the full range.
- `int counter = 0;`: Creates an index pointer `counter` set to `0` to track our current position in `nums`.
- `while( counter < nums.size())`: Starts a loop that continues until we have processed every number present in `nums`.
- `if( nums[counter] == toFind )`: Checks if the actual number at `nums[counter]` matches our expected number `toFind`.
- `toFind++; counter++;`: If there is a match, we advance `toFind` to expect the next consecutive integer and increment `counter` to move to the next position in `nums`.
- `else`: Runs when `nums[counter]` is greater than `toFind`, meaning `toFind` was skipped and is missing.
- `store.push_back(toFind);`: Saves the missing number `toFind` into the `store` list.
- `toFind++;`: Increments `toFind` to check the next integer in the sequence, while keeping `counter` fixed at the same spot in `nums`.
- `return store;`: Returns the final list `store` containing all the missing integers in sorted order.

---

## Dry Run

### Case 1: Multiple missing numbers (`nums = [5, 1]`)

After sorting: `nums = [1, 5]`
Initial state: `toFind = 1`, `counter = 0`, `store = []`

| Step | `counter` | `nums[counter]` | `toFind` | Action | `store` |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | 1 | 1 | Match! `counter` becomes 1, `toFind` becomes 2. | `[]` |
| 2 | 1 | 5 | 2 | No match! Add 2 to `store`. `toFind` becomes 3. | `[2]` |
| 3 | 1 | 5 | 3 | No match! Add 3 to `store`. `toFind` becomes 4. | `[2, 3]` |
| 4 | 1 | 5 | 4 | No match! Add 4 to `store`. `toFind` becomes 5. | `[2, 3, 4]` |
| 5 | 1 | 5 | 5 | Match! `counter` becomes 2, `toFind` becomes 6. | `[2, 3, 4]` |

Loop ends because `counter` reaches `nums.size()` (2). Returns `[2, 3, 4]`.

---

### Case 2: No missing numbers (`nums = [7, 8, 6, 9]`)

After sorting: `nums = [6, 7, 8, 9]`
Initial state: `toFind = 6`, `counter = 0`, `store = []`

| Step | `counter` | `nums[counter]` | `toFind` | Action | `store` |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 0 | 6 | 6 | Match! `counter` becomes 1, `toFind` becomes 7. | `[]` |
| 2 | 1 | 7 | 7 | Match! `counter` becomes 2, `toFind` becomes 8. | `[]` |
| 3 | 2 | 8 | 8 | Match! `counter` becomes 3, `toFind` becomes 9. | `[]` |
| 4 | 3 | 9 | 9 | Match! `counter` becomes 4, `toFind` becomes 10. | `[]` |

Loop ends because `counter` reaches `nums.size()` (4). Returns `[]`.

---

## Time & Space Complexity

- **Time Complexity:** **O(N log N)** — Sorting the array takes O(N log N) time, where N is the length of `nums`. The while loop runs at most O(N + K) times, where K is the number of missing elements (at most 100 based on constraints). Sorting dominates the time spent.
- **Space Complexity:** **O(1)** auxiliary space — The code sorts `nums` in place and uses a few integer variables. (Note: Returning the output vector `store` takes O(K) space for the result).

### Optimization

**Can this be improved?**
Yes. We can improve the time complexity from O(N log N) to **O(N)** by avoiding sorting altogether.

Instead of sorting, we can:
1. Find the minimum and maximum values in `nums` in a single pass (O(N) time).
2. Insert all elements of `nums` into a **Hash Set** (or a boolean frequency array) in O(N) time.
3. Iterate through every integer from the minimum to the maximum value, checking if each number exists in our set in O(1) time. If a number is absent, add it to `store`.

```cpp
unordered_set<int> present(nums.begin(), nums.end());
int minVal = *min_element(nums.begin(), nums.end());
int maxVal = *max_element(nums.begin(), nums.end());

for (int i = minVal; i <= maxVal; ++i) {
    if (present.find(i) == present.end()) {
        store.push_back(i);
    }
}
```

- `unordered_set<int> present(...)`: Builds a hash lookup table from `nums` so checking if a number exists takes O(1) average time.
- `min_element` and `max_element`: Find the range bounds in O(N) time without sorting.
- `for (int i = minVal; i <= maxVal; ++i)`: Iterates sequentially through the full range. Since we loop from smallest to largest, the missing numbers are naturally collected in sorted order.

- **Improved Time Complexity:** **O(N + K)**, where N is `nums.size()` and K is the range size (`maxVal - minVal + 1`).
- **Improved Space Complexity:** **O(N)** to store elements inside the hash set.

**Theoretical Best Complexity:** **O(N)** time is the best possible complexity, because we must read every element in `nums` at least once to know which numbers are present. The hash set approach reaches this theoretical limit.

---

## Edge Cases Handled

- **No missing elements:** Handled correctly (e.g., `[6, 7, 8, 9]`). The `store` array remains empty and returns `[]`.
- **Multiple consecutive missing elements:** Handled correctly (e.g., `[1, 5]`). The `counter` stays stationary while `toFind` increments through `2, 3, 4`, pushing all missing elements into `store`.
- **Unsorted input:** Handled correctly. Sorting `nums` at the beginning ensures numbers are evaluated in correct ascending sequence regardless of the original order.
- **Smallest array size:** Handled correctly. The minimum input length is `2` elements (e.g., `[1, 2]` or `[1, 3]`), which works smoothly without index out-of-bounds errors.
