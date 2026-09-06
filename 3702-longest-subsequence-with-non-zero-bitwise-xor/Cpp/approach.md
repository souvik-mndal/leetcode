![Runtime](https://img.shields.io/badge/Runtime-12%20ms%20(beats%205.29%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-179.3%20MB%20(beats%2021.86%25)-orange?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the longest **subsequence** of an integer array `nums` whose total bitwise **XOR** is not zero.

* A **subsequence** is created by deleting zero or more elements from the array without changing the order of the remaining elements.
* A **bitwise XOR** compares numbers bit by bit. It gives `1` if the bits are different, and `0` if they are the same.
* If every possible subsequence has a total XOR of `0`, we return `0`.

### Example
Suppose `nums = [1, 2, 3]`.
* If we take all elements `[1, 2, 3]`, the XOR sum is `1 XOR 2 XOR 3 = 0`.
* If we drop `1` and keep `[2, 3]`, the XOR sum is `2 XOR 3 = 1` (non-zero).
* The length of `[2, 3]` is `2`, which is the longest possible non-zero XOR subsequence.

---

## Intuition

The key to solving this problem lies in two properties of bitwise XOR:
1. `a XOR a = 0` (XORing a number with itself cancels it out).
2. `a XOR 0 = a` (XORing a number with zero leaves it unchanged).

Because of these properties, we only have three possible cases for any array of size `n`:

1. **All elements are 0:** Every subsequence will XOR to `0`. No non-zero subsequence can be formed, so the answer is `0`.
2. **Total XOR of all `n` elements is non-zero:** We can simply pick all `n` elements. The answer is `n`.
3. **Total XOR of all `n` elements is 0, but there is at least one non-zero element:** If the total XOR of all elements is `0`, removing any non-zero element `y` leaves behind `n - 1` elements. The XOR of those `n - 1` elements will be `0 XOR y = y`. Since `y` is not zero, this remaining group of `n - 1` elements has a non-zero XOR! So the answer is `n - 1`.

Thus, the answer is always `0`, `n`, or `n - 1`.

---

## Approach

Here is what the code does step-by-step:

* `int n = nums.size();` — Stores the length of the input array `nums` in variable `n`.
* `vector<int> zeros(n, 0);` — Creates a helper array called `zeros` filled with `n` zeros.
* `if (nums == zeros) { return 0; }` — Checks if `nums` contains only zeros. If so, no non-zero XOR is possible, so it returns `0`.
* `int x = 0;` — Initializes variable `x` to `0`, which will store the running XOR total of all elements in `nums`.
* `for (int num : nums) { x ^= num; }` — Loops through every element `num` in `nums` and updates `x` using the bitwise XOR operator (`^=`).
* `return x ? n : n - 1;` — Checks if the final total XOR `x` is non-zero. If `x` is non-zero, it returns `n`. Otherwise, it returns `n - 1`.

---

## Dry Run

### Case 1: Total XOR is 0 with non-zero elements (`nums = [1, 2, 3]`)

Initial setup: `n = 3`, `zeros = [0, 0, 0]`. Check `nums == zeros` evaluates to `false`. `x = 0`.

| Step | `num` | `x` | Action |
| --- | --- | --- | --- |
| Start | - | 0 | `nums` is not all zeros. Begin loop. |
| Loop 1 | 1 | 1 | Calculate `0 XOR 1 = 1`. |
| Loop 2 | 2 | 3 | Calculate `1 XOR 2 = 3`. |
| Loop 3 | 3 | 0 | Calculate `3 XOR 3 = 0`. |
| Return | - | 0 | `x` is `0`. Return `n - 1`, which is `2`. |

### Case 2: Total XOR is non-zero (`nums = [2, 3, 4]`)

Initial setup: `n = 3`, `zeros = [0, 0, 0]`. Check `nums == zeros` evaluates to `false`. `x = 0`.

| Step | `num` | `x` | Action |
| --- | --- | --- | --- |
| Start | - | 0 | `nums` is not all zeros. Begin loop. |
| Loop 1 | 2 | 2 | Calculate `0 XOR 2 = 2`. |
| Loop 2 | 3 | 1 | Calculate `2 XOR 3 = 1`. |
| Loop 3 | 4 | 5 | Calculate `1 XOR 4 = 5`. |
| Return | - | 5 | `x` is `5` (non-zero). Return `n`, which is `3`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — Comparing `nums == zeros` checks all `n` elements. Then, the loop visits all `n` elements once. Total operations scale linearly with array size `n`.
* **Space Complexity:** **O(n)** — Creating the `zeros` vector allocates space for `n` integers.

### Can this be improved?

**Yes!** The space complexity can be improved from **O(n)** to **O(1)**.

#### Why the improvement works
Instead of allocating a full `zeros` vector to test if all elements are zero, we can track whether we have seen any non-zero element using a boolean variable inside the main loop. This eliminates the extra array creation entirely and reduces work to a single pass over `nums`.

#### Optimized Code Snippet
```cpp
int x = 0;
bool has_nonzero = false;

for (int num : nums) {
    x ^= num;
    if (num != 0) {
        has_nonzero = true;
    }
}

if (!has_nonzero) return 0;
return x ? n : n - 1;
```

* `has_nonzero` tracks if any non-zero element exists while computing `x`.
* If `has_nonzero` remains `false` after the loop, every element is `0`, so we return `0`.

#### Resulting Complexity
* **Improved Time:** **O(n)** — Single pass through `nums`.
* **Improved Space:** **O(1)** — Uses only a few integer/boolean variables.

#### Theoretical Best
**O(n) time** and **O(1) space** is the absolute theoretical limit. We must inspect every element at least once to calculate the XOR sum and check for non-zero values, requiring O(n) time. O(1) space is optimal since no extra memory structures are required. The improved version reaches this theoretical best.

---

## Edge Cases Handled

* **All Zeros Array (`nums = [0, 0, 0]`):** Handled correctly. Returns `0` because no non-zero XOR subsequence exists.
* **Single Element Array (`nums = [5]` or `nums = [0]`):** Works for both. `[5]` returns `1`, and `[0]` returns `0`.
* **Total XOR equals 0 (`nums = [1, 2, 3]`):** Handled by returning `n - 1` (length `2`).
* **Large Numbers (`nums[i] <= 10^9`):** Bitwise XOR operations work natively on 32-bit standard integers without overflow risk.
