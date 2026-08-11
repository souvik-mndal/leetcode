![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%206.78%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-23.08%20MB%20(beats%2029.32%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given an array of integers named `nums`. 

First, look at the very beginning of the array and find the **sequential prefix**. A prefix is sequential if every number after the first one is exactly **1 greater** than the number right before it. 

For example:
- In `[1, 2, 3, 2, 5]`, the sequence starts at `1`. The next number `2` is `1 + 1`. The next number `3` is `2 + 1`. The next number `2` is **not** `3 + 1`, so the sequential prefix stops at `[1, 2, 3]`.
- In `[3, 4, 5, 1]`, the sequential prefix is `[3, 4, 5]`.

Once you identify this starting sequential sequence, add up all its numbers to get a **sum**. 

Finally, find the smallest integer that is **greater than or equal to this sum** and is **missing** from the original array `nums`.

---

## Intuition

The problem breaks down into two distinct parts:
1. **Find the sequential sum:** Walk from the start of the array to the right as long as each element increases by `1`. Sum these numbers up. As soon as a number breaks the `+1` pattern, stop.
2. **Find the missing number:** Start a candidate variable (`answer`) at the sum you just calculated. Check if `answer` exists anywhere in the array `nums`. If it does, increase `answer` by `1` and check again. Repeat this until you find a value that does not exist in `nums`.

To make checking whether a number exists in `nums` fast, we can put all elements of `nums` into a **hash set** (a data structure called `unordered_set` in C++). A hash set lets us look up whether a number is in our array almost instantly instead of searching through the array repeatedly.

---

## Approach

Here is how the code executes step-by-step:

* ``int sum = nums[0];``: Initialize our running total `sum` with the first element of the array, because a sequential prefix always contains at least `nums[0]`.
* ``for (int i = 1; i < nums.size(); i++)``: Start a loop from the second element (index 1) to inspect the rest of the array.
* ``if (nums[i] == nums[i - 1] + 1)``: Check if the current element is exactly 1 greater than the previous element.
* ``sum += nums[i];``: If it is sequential, add the current element to `sum`.
* ``} else { break; }``: If the sequence breaks, exit the loop immediately because the sequential prefix has ended.
* ``unordered_set<int> seen(nums.begin(), nums.end());``: Create a hash set named `seen` containing all elements from `nums` for fast O(1) average lookup times.
* ``int answer = sum;``: Set our starting candidate `answer` to equal `sum`.
* ``while (seen.count(answer))``: Check if `answer` exists inside the `seen` set.
* ``answer++;``: If `answer` is found in the set, add 1 to it and repeat the check until `seen.count(answer)` returns 0 (meaning the number is missing from the array).
* ``return answer;``: Return `answer`, which is now the smallest missing integer greater than or equal to the sequential prefix sum.

---

## Dry Run

### Case 1: Standard case with prefix break (`nums = [1, 2, 3, 2, 5]`)

| `i` | `nums[i]` | `sum` | `answer` | Action |
| --- | --- | --- | --- | --- |
| - | - | 1 | - | Initialize `sum = nums[0] = 1`. |
| 1 | 2 | 3 | - | `2 == 1 + 1` is true. `sum` becomes `1 + 2 = 3`. |
| 2 | 3 | 6 | - | `3 == 2 + 1` is true. `sum` becomes `3 + 3 = 6`. |
| 3 | 2 | 6 | - | `2 == 3 + 1` is false. Sequence breaks. Loop exits. |
| - | - | 6 | 6 | Populate `seen = {1, 2, 3, 5}`. Set `answer = 6`. |
| - | - | 6 | 6 | Check `seen.count(6)`. Not found (0). `while` loop ends. Return 6. |

---

### Case 2: Overlapping sum values (`nums = [3, 4, 5, 1, 12, 14, 13]`)

| `i` | `nums[i]` | `sum` | `answer` | Action |
| --- | --- | --- | --- | --- |
| - | - | 3 | - | Initialize `sum = nums[0] = 3`. |
| 1 | 4 | 7 | - | `4 == 3 + 1` is true. `sum` becomes `3 + 4 = 7`. |
| 2 | 5 | 12 | - | `5 == 4 + 1` is true. `sum` becomes `7 + 5 = 12`. |
| 3 | 1 | 12 | - | `1 == 5 + 1` is false. Sequence breaks. Loop exits. |
| - | - | 12 | 12 | Populate `seen = {1, 3, 4, 5, 12, 13, 14}`. Set `answer = 12`. |
| - | - | 12 | 13 | `seen.count(12)` is true. Increment `answer` to 13. |
| - | - | 12 | 14 | `seen.count(13)` is true. Increment `answer` to 14. |
| - | - | 12 | 15 | `seen.count(14)` is true. Increment `answer` to 15. |
| - | - | 12 | 15 | `seen.count(15)` is false (0). `while` loop ends. Return 15. |

---

## Time & Space Complexity

- **Time Complexity:** Average **O(N)**, where N is the length of `nums`.
  - Finding the prefix sum takes at most N operations.
  - Inserting N elements into `unordered_set` takes average O(N) time.
  - The `while` loop runs at most N times because there are only N numbers in the array that could block `answer`.
  - *Worst-case note:* `unordered_set` in C++ can degrade to O(N^2) time overall if hash collisions occur.

- **Space Complexity:** **O(N)** to store up to N elements inside the `seen` hash set.

### Can it be improved?

Yes, worst-case performance and memory usage can both be optimized! 

Since the constraints state that `nums.length <= 50` and `nums[i] <= 50`, the maximum possible value for any element is small. The largest possible prefix sum is `50 * 50 = 2500`. 

Instead of an `unordered_set` (which has hashing overhead and potential hash collision delays), we can use a fixed-size **boolean lookup array** (or a `vector<bool>`). Checking a boolean array gives guaranteed **O(1) worst-case time** for lookups and avoids dynamically allocating set nodes.

Here is the key change:

```cpp
// Replace the set with a direct boolean lookup table
vector<bool> seen(2551, false);
for (int num : nums) {
    seen[num] = true;
}

int answer = sum;
while (seen[answer]) {
    answer++;
}
```

- **Explanation of the fix:**
  - `vector<bool> seen(2551, false);`: Creates a fixed boolean array where index `x` is `true` if `x` exists in `nums`. The size 2551 guarantees we never go out of bounds for valid constraints.
  - `seen[num] = true;`: Marks numbers as present in constant time with zero hashing overhead.
  - `while (seen[answer])`: Performs direct array index lookups, guaranteeing true O(1) time per step.

- **Improved Complexity:**
  - **Time:** Strictly **O(N)** in the worst case (no hash collision risk).
  - **Space:** **O(1)** auxiliary space (since 2551 is a constant fixed size).

- **Theoretical Best Complexity:** **O(N) Time and O(1) Space**. The improved version reaches this theoretical optimal bound.

---

## Edge Cases Handled

* **Single-element array (`nums = [7]`):** The loop condition `i < nums.size()` finishes immediately. `sum` stays `7`. If `7` exists in `seen`, it returns `8`.
* **No consecutive numbers at start (`nums = [5, 2, 3]`):** The sequential prefix is just `[5]`. The loop breaks on the first step (`2 != 5 + 1`), leaving `sum = 5`.
* **Entire array is sequential (`nums = [1, 2, 3, 4]`):** The loop runs to the very end of the array without breaking. Sum becomes `10`, and `10` is checked against `seen`.
* **Duplicate values in the array (`nums = [1, 2, 2, 3]`):** The hash set automatically ignores duplicate values during set creation, preventing redundant entries or infinite loops.
* **Sequential sum already exists later in array (`nums = [1, 2, 3, 6, 7]`):** Sum is `1 + 2 + 3 = 6`. Since `6` and `7` are present in `nums`, `answer` correctly skips `6` and `7` to return `8`.
