![Runtime](https://img.shields.io/badge/Runtime-12%20ms%20(beats%208.39%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-179.28%20MB%20(beats%202.01%25)-red?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the length of the longest **subsequence** in an array of numbers (`nums`) whose total bitwise XOR is not zero. A subsequence is formed by deleting zero or more elements from the array without changing the order of the remaining elements.

The bitwise XOR operation compares two numbers bit by bit:
- 0 XOR 0 = 0
- 1 XOR 1 = 0
- 1 XOR 0 = 1

For example, if `nums = [1, 2, 3]`:
- The full array XOR is `1 XOR 2 XOR 3 = 0`.
- If we remove `1`, the remaining subsequence `[2, 3]` has XOR `2 XOR 3 = 1`, which is non-zero.
- The length of this subsequence is 2, which is the longest possible non-zero result.

If every element in the array is zero, no matter what subsequence you pick, the XOR sum will always be zero. In that case, the answer is `0`.

---

## Intuition

The key to solving this problem lies in two fundamental properties of the bitwise XOR operation:
1. Any number XORed with itself equals 0 (`A XOR A = 0`).
2. Any number XORed with 0 stays the same (`A XOR 0 = A`).

Now, consider the total XOR sum of all elements in the entire array:

- **Case 1: The total XOR of all elements is non-zero.**
  If the XOR sum of the whole array is already non-zero, we do not need to drop any elements. The longest valid subsequence is the entire array itself, so the answer is `n` (the total length of the array).

- **Case 2: The total XOR of all elements is zero.**
  If the total XOR is 0, we have two possibilities:
  - **All elements are 0:** Every number in `nums` is 0. Any subsequence will only contain zeros, so it is impossible to get a non-zero XOR. The answer is `0`.
  - **At least one element is non-zero:** Suppose the element `val` is non-zero. If we calculate the total XOR of all elements *except* `val`, what do we get?
    
    ```
    (XOR of all other elements) XOR val = Total XOR = 0
    ```
    
    If we XOR both sides of this equation by `val`, we get:
    
    ```
    XOR of all other elements = val
    ```
    
    Since `val` is non-zero, the XOR of the remaining `n - 1` elements is guaranteed to equal `val`, which is non-zero! Therefore, by removing just one non-zero element, we get a valid subsequence of length `n - 1`.

In short:
- If all elements are 0 -> return `0`.
- Else if full array XOR is non-zero -> return `n`.
- Else (full array XOR is 0 but non-zeros exist) -> return `n - 1`.

---

## Approach

Here is how the provided C++ code executes this logic step-by-step:

- `int n = nums.size();`  
  Calculates and stores the total number of elements in `nums` inside `n`.

- `vector<int> zeros(n, 0);`  
  Creates a helper vector named `zeros` of size `n`, filled entirely with zeros.

- `if (nums == zeros)`  
  Compares the entire input array `nums` against the zero-filled vector. If every element is 0, it returns `0` immediately because no non-zero XOR subsequence can be made.

- `int x = 0;`  
  Initializes a running XOR accumulator `x` to 0.

- `for (int num : nums)`  
  Loops through each number `num` in the `nums` array one by one.

- `x ^= num;`  
  Updates `x` by taking its bitwise XOR with the current element `num`.

- `return x ? n : n - 1;`  
  Checks if `x` is non-zero. If `x` is non-zero, it returns `n` (the entire array). If `x` is zero, it returns `n - 1` (removing one non-zero element gives a non-zero XOR).

---

## Dry Run

### Case 1: `nums = [1, 2, 3]`

Initialization: `n = 3`. `zeros = [0, 0, 0]`. `nums == zeros` evaluates to `false`. `x = 0`.

| Step | `num` | `x` (Before) | `x` (After `x ^= num`) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `1` | `0` | `1` (`0 XOR 1`) | Process first element |
| 2 | `2` | `1` | `3` (`1 XOR 2`) | Process second element |
| 3 | `3` | `3` | `0` (`3 XOR 3`) | Process third element |

After loop: `x = 0`.  
Final step: `x ? n : n - 1` evaluates to `3 - 1 = 2`.  
**Output:** `2`

---

### Case 2: `nums = [2, 3, 4]`

Initialization: `n = 3`. `zeros = [0, 0, 0]`. `nums == zeros` evaluates to `false`. `x = 0`.

| Step | `num` | `x` (Before) | `x` (After `x ^= num`) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `2` | `0` | `2` (`0 XOR 2`) | Process first element |
| 2 | `3` | `2` | `1` (`2 XOR 3`) | Process second element |
| 3 | `4` | `1` | `5` (`1 XOR 4`) | Process third element |

After loop: `x = 5`.  
Final step: `x ? n : n - 1` evaluates to `x != 0`, returning `n = 3`.  
**Output:** `3`

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — The code makes one comparison pass creating and comparing a vector of size `n`, followed by a single loop over `n` elements.
- **Space Complexity:** **O(n)** — The line `vector<int> zeros(n, 0)` allocates a new vector of size `n` in memory, which uses extra memory proportional to the input size.

### Can this be improved?

**Yes.** We can optimize the extra space from **O(n)** down to **O(1)** (constant extra space).

Instead of allocating a new vector of zeros and comparing `nums == zeros` upfront, we can track whether we have seen any non-zero element directly inside our single loop while calculating the total XOR sum. 

If we keep a boolean flag `has_non_zero`, we can set it to `true` whenever `num != 0`. At the end of the loop:
1. If `has_non_zero` is `false`, all numbers were zero, so we return `0`.
2. Otherwise, if total XOR `x` is non-zero, we return `n`.
3. If total XOR `x` is zero, we return `n - 1`.

Here is the optimized C++ code snippet:

```cpp
int x = 0;
bool has_non_zero = false;

for (int num : nums) {
    x ^= num;
    if (num != 0) {
        has_non_zero = true;
    }
}

if (!has_non_zero) return 0;
return x ? n : n - 1;
```

- **Improved Time Complexity:** **O(n)** — Still a single pass over the array, but without the overhead of vector allocation and array comparison.
- **Improved Space Complexity:** **O(1)** — Uses only two simple variables (`x` and `has_non_zero`).
- **Theoretical Best Complexity:** **O(n)** time and **O(1)** auxiliary space. This optimized code reaches the theoretical best possible performance because we must examine each number at least once to compute the XOR sum.

---

## Edge Cases Handled

- **All zeros (`nums = [0, 0, 0]`):**  
  Correctly identifies that no non-zero XOR can be formed and returns `0`.

- **Single element array (`nums = [0]` vs `nums = [5]`):**  
  - For `[0]`, it returns `0`.
  - For `[5]`, the total XOR is `5` (non-zero), returning `n = 1`.

- **Total XOR is zero with mixed elements (`nums = [1, 2, 3]`):**  
  Correctly recognizes that dropping one element leaves `n - 1` elements with a non-zero XOR, returning `2`.

- **Large inputs (`nums.size()` up to 10^5):**  
  The linear time complexity ensures the solution completes well within time limits without running out of memory.
