![Runtime](https://img.shields.io/badge/Runtime-20%20ms%20(beats%2010.35%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-31.5%20MB%20(beats%2078.87%25)-green?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums`. Your goal is to pick three numbers from this list and multiply them together to get the largest possible result. 

For example, if `nums = [1, 2, 3, 4]`, picking `2`, `3`, and `4` gives `2 * 3 * 4 = 24`. That is the highest product you can get from any three numbers in this list.

## Intuition

If all numbers are positive, the choice is simple: multiply the three largest numbers together.

However, negative numbers introduce a twist. Multiplying two negative numbers produces a positive number. If an array has two very large negative numbers (like `-100` and `-100`), multiplying them gives a large positive number (`10000`). If you multiply that result by the largest positive number in the array, you might end up with a product far greater than multiplying the three largest positive numbers.

Because of this, there are only two candidates for the maximum product:
1. The product of the three largest numbers.
2. The product of the two smallest (most negative) numbers and the single largest number.

Sorting the list from smallest to largest brings the most negative numbers to the beginning and the largest positive numbers to the end. That allows us to easily compute both possibilities and pick the bigger one.

## Approach

* `int n = nums.size();`  
  Calculates the total count of numbers in `nums` and stores it in `n` so we can reference positions relative to the end of the array.

* `sort(begin(nums), end(nums));`  
  Sorts all elements in `nums` in ascending order (from smallest to largest).

* `return max(nums[n - 1] * nums[n - 2] * nums[n - 3], nums[0] * nums[1] * nums[n - 1]);`  
  Calculates the two potential maximum products and returns the higher value using `max()`. The first expression (`nums[n - 1] * nums[n - 2] * nums[n - 3]`) multiplies the three largest numbers at the back of the sorted list. The second expression (`nums[0] * nums[1] * nums[n - 1]`) multiplies the two smallest numbers at the front with the largest number at the back.

## Dry Run

### Case 1: All positive numbers (`nums = [1, 2, 3, 4]`)

| Step | `n` | `nums` | `nums[n-1]*nums[n-2]*nums[n-3]` | `nums[0]*nums[1]*nums[n-1]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `4` | `[1, 2, 3, 4]` | — | — | Store size `n = 4`. |
| 2 | `4` | `[1, 2, 3, 4]` | — | — | Sort array (order stays same). |
| 3 | `4` | `[1, 2, 3, 4]` | `4 * 3 * 2 = 24` | `1 * 2 * 4 = 8` | Compute option 1 and option 2. |
| 4 | `4` | `[1, 2, 3, 4]` | `24` | `8` | Return `max(24, 8)`, which is `24`. |

### Case 2: All negative numbers (`nums = [-1, -2, -3]`)

| Step | `n` | `nums` | `nums[n-1]*nums[n-2]*nums[n-3]` | `nums[0]*nums[1]*nums[n-1]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `3` | `[-1, -2, -3]` | — | — | Store size `n = 3`. |
| 2 | `3` | `[-3, -2, -1]` | — | — | Sort array in ascending order. |
| 3 | `3` | `[-3, -2, -1]` | `(-1) * (-2) * (-3) = -6` | `(-3) * (-2) * (-1) = -6` | Compute option 1 and option 2. |
| 4 | `3` | `[-3, -2, -1]` | `-6` | `-6` | Return `max(-6, -6)`, which is `-6`. |

## Time & Space Complexity

- **Time Complexity:** **O(n log n)** — Sorting the vector `nums` dominates the runtime, where `n` is the number of elements.
- **Space Complexity:** **O(log n)** — Modern sorting algorithms like C++ `std::sort` use extra stack space for recursive calls during sorting.

### Can this be improved?

**Yes.** We do not need to sort the entire array. We only care about 5 specific values: the 3 largest numbers and the 2 smallest numbers.

Instead of sorting, we can iterate through `nums` once and keep track of these 5 values using simple variables. This avoids full sorting and reduces the time complexity to linear time.

```cpp
int min1 = INT_MAX, min2 = INT_MAX;
int max1 = INT_MIN, max2 = INT_MIN, max3 = INT_MIN;

for (int num : nums) {
    if (num > max1) {
        max3 = max2; max2 = max1; max1 = num;
    } else if (num > max2) {
        max3 = max2; max2 = num;
    } else if (num > max3) {
        max3 = num;
    }

    if (num < min1) {
        min2 = min1; min1 = num;
    } else if (num < min2) {
        min2 = num;
    }
}

return max(max1 * max2 * max3, min1 * min2 * max1);
```

- `max1`, `max2`, `max3` maintain the top 3 largest values in order. When a new number beats `max1`, the existing values shift down.
- `min1`, `min2` maintain the top 2 smallest values.
- Finally, `max()` compares the product of the 3 largest values against the product of the 2 smallest values and the 1 largest value.

- **Improved Time Complexity:** **O(n)** — We inspect each element in `nums` exactly once.
- **Improved Space Complexity:** **O(1)** — We only store 5 fixed integer variables regardless of array size.
- **Theoretical Best Complexity:** **O(n)** time and **O(1)** space is the theoretical best possible because every number in `nums` must be checked at least once to ensure no larger or smaller value is missed. The improved single-pass approach reaches this theoretical limit.

## Edge Cases Handled

- **Array of minimum length (`nums.size() == 3`)**: Both product options evaluate all three elements in the array, correctly producing their product.
- **All negative numbers**: When every number is negative, picking the 3 largest (least negative) numbers correctly results in the maximum product (closest to zero).
- **Mix of negative and positive numbers**: Automatically checks whether two large negative numbers multiplied by a positive number beat three positive numbers.
- **Presence of zeros**: Zeros are handled naturally without special rules or division-by-zero risks.
