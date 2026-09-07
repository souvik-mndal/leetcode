![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-13.5%20MB%20(beats%2020.40%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums`. Your goal is to pick two different positions (indices) in the array, subtract 1 from each of the two numbers at those positions, and multiply them together. You want to find the **maximum product** possible.

For example, if `nums = [3, 4, 5, 2]`:
* If you pick 4 and 5, you subtract 1 from each to get 3 and 4.
* Multiply them together: `3 * 4 = 12`. 
* This is the largest result you can get from this array.

All numbers in the array are positive integers (1 or greater), and the array will always have at least 2 numbers.

---

## Intuition

Since all numbers are positive, subtracting 1 from a larger number still leaves a larger number. To get the biggest possible product, you always want to pick the **two largest numbers** in the entire array.

The simplest way to find the two largest numbers is to **sort** the array in ascending order (smallest to largest). Once sorted, the two largest numbers will automatically end up at the very end of the array. You just take those last two numbers, subtract 1 from each, and multiply them.

---

## Approach

Here is how the code works step-by-step:

* `sort( nums.begin() , nums.end());`: This rearranges all elements in `nums` from smallest to largest. The largest number moves to the last index (`nums.size() - 1`), and the second largest number moves to the second-to-last index (`nums.size() - 2`).
* `return (nums[nums.size()-1]-1)*(nums[nums.size()-2]-1);`: This fetches the largest number using `nums[nums.size()-1]` and subtracts 1 from it. It then fetches the second largest number using `nums[nums.size()-2]` and subtracts 1 from it. Finally, it multiplies these two values together and returns the result.

---

## Dry Run

### Case 1: Standard case with unique values (`nums = [3, 4, 5, 2]`)

| Step | `nums` state | Action |
| :--- | :--- | :--- |
| 1. Start | `[3, 4, 5, 2]` | Input array received. Size is 4. |
| 2. Sort | `[2, 3, 4, 5]` | Array sorted in ascending order. |
| 3. Pick elements | `[2, 3, 4, 5]` | Largest is `nums[3] = 5`. Second largest is `nums[2] = 4`. |
| 4. Calculate | `[2, 3, 4, 5]` | Compute `(5 - 1) * (4 - 1) = 4 * 3 = 12`. |
| 5. Return | `[2, 3, 4, 5]` | Return 12. |

### Case 2: Case with duplicate largest values (`nums = [1, 5, 4, 5]`)

| Step | `nums` state | Action |
| :--- | :--- | :--- |
| 1. Start | `[1, 5, 4, 5]` | Input array received. Size is 4. |
| 2. Sort | `[1, 4, 5, 5]` | Array sorted in ascending order. |
| 3. Pick elements | `[1, 4, 5, 5]` | Largest is `nums[3] = 5`. Second largest is `nums[2] = 5`. |
| 4. Calculate | `[1, 4, 5, 5]` | Compute `(5 - 1) * (5 - 1) = 4 * 4 = 16`. |
| 5. Return | `[1, 4, 5, 5]` | Return 16. |

---

## Time & Space Complexity

* **Time Complexity:** **O(N * log N)** — Sorting an array of length N takes O(N * log N) operations.
* **Space Complexity:** **O(log N)** — standard sorting in C++ (`std::sort`) uses a small amount of memory on the call stack for quicksort recursive calls.

### Can this be improved?

**Yes!** We do not need to sort the entire array just to find the top two numbers. We can find the two largest numbers in a single pass (looking at each number once).

Instead of sorting, we can keep two variables, `max1` (largest) and `max2` (second largest). As we walk through the array:
* If the current number is bigger than `max1`, the old `max1` becomes our new `max2`, and the current number becomes our new `max1`.
* Otherwise, if the current number is bigger than `max2`, it becomes our new `max2`.

Here is the key logic to replace the sorting step:

```cpp
int max1 = 0;
int max2 = 0;

for (int x : nums) {
    if (x > max1) {
        max2 = max1; // Old maximum drops to second place
        max1 = x;    // New maximum found
    } else if (x > max2) {
        max2 = x;    // New second-largest found
    }
}

return (max1 - 1) * (max2 - 1);
```

* `max2 = max1;`: Whenever we find a number bigger than `max1`, the previous biggest number doesn't disappear — it is now the second biggest.
* `else if (x > max2)`: If a number isn't bigger than `max1`, it might still be bigger than our current `max2`.

### Improved Complexity:
* **Time Complexity:** **O(N)** — We look at each element in the array exactly once.
* **Space Complexity:** **O(1)** — We only use two extra variables (`max1` and `max2`), requiring constant memory.

### Theoretical Best:
The absolute best possible time complexity is **O(N)** because we must inspect every number at least once to ensure we don't miss a large value. The improved single-pass approach reaches this optimal limit.

---

## Edge Cases Handled

* **Minimum array size (2 elements):** For an input like `[3, 7]`, `nums[nums.size()-1]` is index 1 (7) and `nums[nums.size()-2]` is index 0 (3). The math works seamlessly without index out-of-bounds errors.
* **Duplicate values:** For inputs like `[1, 5, 4, 5]`, sorting groups duplicate values together at the end. Taking the last two indices correctly picks both `5`s.
* **Smallest element values (all 1s):** For an input like `[1, 1]`, the code computes `(1 - 1) * (1 - 1) = 0`. Since all values are 1 or greater, there is no danger of negative numbers or arithmetic underflow.
