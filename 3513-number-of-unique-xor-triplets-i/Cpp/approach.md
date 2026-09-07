![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%204.95%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-198.9%20MB%20(beats%2014.57%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array `nums` of length `n`. This array contains a permutation of all numbers from `1` to `n`. That means every whole number from `1` up to `n` appears exactly once.

A **XOR triplet** is formed by choosing three indices `i`, `j`, and `k` from the array, where `i <= j <= k`. You then compute the bitwise XOR of the three values at those indices: `nums[i] XOR nums[j] XOR nums[k]`.

Because `i <= j <= k`, you are allowed to pick the same index more than once (for example, `i = j = k`).

Your task is to find how many **unique** XOR results you can make from all possible choices of three indices.

### Example
If `nums = [1, 2]`:
* `n = 2`, so the array contains numbers `1` and `2`.
* Possible triplets:
  * Pick `(1, 1, 1)` -> `1 XOR 1 XOR 1 = 1`
  * Pick `(1, 1, 2)` -> `1 XOR 1 XOR 2 = 2`
  * Pick `(1, 2, 2)` -> `1 XOR 2 XOR 2 = 1`
  * Pick `(2, 2, 2)` -> `2 XOR 2 XOR 2 = 2`
* The unique values produced are `{1, 2}`.
* Output: `2`.

---

## Intuition

The key insight relies on two simple rules of bitwise XOR:
1. **Index repetition:** Since `i <= j <= k`, picking `i = j = k` gives `nums[i] XOR nums[i] XOR nums[i] = nums[i]`. Picking `i = j` gives `nums[i] XOR nums[i] XOR nums[k] = nums[k]`. This means any individual number in the array is already a reachable XOR output.
2. **Permutation guarantee:** The array contains every integer from `1` to `n`.

For small arrays (`n <= 2`), you can only produce the original elements themselves. So the answer is simply `n`.

For larger arrays (`n >= 3`), having three elements gives enough freedom to combine bit patterns. You can generate **every single integer** starting from `0` up to a maximum bit value.

What is that maximum bit value?
When you take the bitwise **OR** of all numbers from `1` to `n`, you set every bit position used by `n` to `1`. For example, if `n = 3` (binary `11`), the OR sum of `1, 2, 3` is `3` (binary `11`).

Because every number from `0` to `mask` can be formed when `n >= 3`, the total count of unique reachable values is `mask + 1` (the `+ 1` accounts for `0`).

---

## Approach

Here is the step-by-step breakdown of how the provided C++ solution works:

* `int n = nums.size();` — Measures the total number of elements in the `nums` array and stores it in `n`.
* `if (n <= 2) return n;` — Handles the smallest arrays directly. If `n` is `1` or `2`, we can only ever form `n` unique values, so it returns `n` immediately.
* `int mask = 0;` — Creates an integer variable `mask` initialized to `0` to accumulate set bits across all elements.
* `for (int num : nums)` — Loops through each element `num` in the array `nums` one by one.
* `mask |= num;` — Updates `mask` using bitwise OR (`|=`). This flips a bit to `1` in `mask` if that bit is `1` in `num`.
* `return mask + 1;` — Adds `1` to `mask` to count all reachable numbers from `0` up to `mask`, and returns this value as the final answer.

---

## Dry Run

### Case 1: Small array (`n <= 2`)
* Input: `nums = [1, 2]`

| Step | `n` | Action |
| --- | --- | --- |
| 1 | 2 | Calculate size: `n = 2`. |
| 2 | 2 | Check `if (n <= 2)`: Condition is true. Return `2`. |

---

### Case 2: Standard case (`n >= 3`)
* Input: `nums = [3, 1, 2]`

| Step | `n` | `mask` | `num` | Action |
| --- | --- | --- | --- | --- |
| 1 | 3 | 0 | - | `n = 3`. Check `if (n <= 2)`: False. Continue. |
| 2 | 3 | 0 | 3 | First element: `mask |= 3` (`00 OR 11` = `3`). |
| 3 | 3 | 3 | 1 | Second element: `mask |= 1` (`11 OR 01` = `3`). |
| 4 | 3 | 3 | 2 | Third element: `mask |= 2` (`11 OR 10` = `3`). |
| 5 | 3 | 3 | - | Loop finishes. Return `mask + 1` = `3 + 1` = `4`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — The code loops through all `n` elements of `nums` once to compute the bitwise OR mask.
* **Space Complexity:** **O(1)** — Only a few integer variables (`n`, `mask`, `num`) are used. No extra data structures are allocated.

### Can this be improved?

**Yes, we can improve the time complexity from O(n) to O(1).**

Because `nums` is guaranteed to be a permutation of all numbers from `1` to `n`, we do not actually need to iterate through the array to find `mask`.

The bitwise OR of all numbers from `1` to `n` always sets all bit positions up to the most significant bit of `n`.
* For example, if `n = 5` (binary `101`), the highest set bit is at position 3 (value 4).
* The OR sum of `1` through `5` turns all 3 lower bits on, producing `111` in binary, which is `7`.
* `mask + 1` then equals `8`, which is the next power of 2 (`2^3 = 8`).

This means `mask + 1` is always the **smallest power of 2 strictly greater than n**. We can calculate this power of 2 directly from `n` in **O(1)** time using built-in binary instructions, completely ignoring the `nums` array contents!

### Optimized Code Snippet

```cpp
class Solution {
public:
    int uniqueXorTriplets(vector<int>& nums) {
        int n = nums.size();
        if (n <= 2) return n;
        
        // __builtin_clz(n) counts leading zeros in a 32-bit integer.
        // (32 - __builtin_clz(n)) gives the number of bits needed to represent n.
        // 1 << bits gives the smallest power of 2 greater than n.
        return 1 << (32 - __builtin_clz(n));
    }
};
```

* `__builtin_clz(n)` counts leading zero bits in `n`.
* `32 - __builtin_clz(n)` calculates the position of the highest set bit in `n`.
* `1 << (...)` shifts `1` left by that position, producing the next power of 2 instantly.

### Improved Complexity
* **Time Complexity:** **O(1)** — Performs constant-time arithmetic and bit manipulation operations.
* **Space Complexity:** **O(1)** — Uses a fixed amount of extra space.

This O(1) time and O(1) space complexity is the theoretical best possible complexity, as it avoids reading the input array entirely.

---

## Edge Cases Handled

* **Array length of 1 (`n = 1`):** The condition `n <= 2` catches this early and returns `1`. Correct, because only `1 XOR 1 XOR 1 = 1` is possible.
* **Array length of 2 (`n = 2`):** Caught by `n <= 2` and returns `2`. Correct, because only values `1` and `2` can be produced.
* **Powers of 2 (`n = 4`, `n = 8`):** For `n = 4` (binary `100`), the mask becomes `111` in binary (`7`). `mask + 1` correctly yields `8`.
* **Numbers with all bits set (`n = 3`, `n = 7`):** For `n = 3` (binary `11`), the mask stays `3`. `mask + 1` correctly yields `4`.
* **Large inputs (`n = 10^5`):** Fits well within standard 32-bit signed integers without risk of overflow.
