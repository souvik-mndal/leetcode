![Runtime](https://img.shields.io/badge/Runtime-59%20ms%20(beats%2029.32%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-172.85%20MB%20(beats%2014.14%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array `nums1` containing distinct positive numbers. Your goal is to create a new array `nums2` of the same length such that **all** elements in `nums2` end up with the same parity — meaning they are either all odd numbers or all even numbers.

For every index `i`, you have to choose one of two options to set `nums2[i]`:
1. Keep the original value: `nums2[i] = nums1[i]`.
2. Subtract a smaller element from `nums1`: `nums2[i] = nums1[i] - nums1[j]`, where `j != i` and `nums1[i] - nums1[j] >= 1`.

Return `true` if you can construct such an array `nums2`, or `false` if it is impossible.

**Example:**
If `nums1 = [1, 4, 7]`:
- Keep `nums2[0] = 1` (odd).
- Change `nums2[1] = 4 - 1 = 3` (odd).
- Keep `nums2[2] = 7` (odd).
- `nums2 = [1, 3, 7]`. All numbers are odd, so return `true`.

---

## Intuition

The key to this problem lies in basic subtraction rules for odd and even numbers:
- `Even - Odd = Odd`
- `Odd - Odd = Even`

If all numbers in `nums1` are already all odd or all even, you don't need to change anything. You just keep `nums2` identical to `nums1`, and the answer is `true`.

If `nums1` contains a mix of odd and even numbers, can we make all elements odd?
To turn an even number into an odd number, you must subtract a smaller odd number from it (`Even - Odd = Odd`). 

If the **smallest overall number** in `nums1` is **odd**, then every even number in `nums1` is strictly larger than this smallest odd number. That means you can subtract this smallest odd number from every even number to make it odd. Since the odd numbers stay odd, all elements become odd! Thus, if the overall minimum element is odd, the answer is always `true`.

What if the smallest overall number is **even**?
- You cannot turn the smallest even number into an odd number because there is no smaller number available to subtract from it.
- You also cannot turn all numbers into even numbers because the smallest odd number in `nums1` has no smaller odd number to subtract from it (subtracting an even number from an odd number keeps it odd).

So, if `nums1` has mixed parities and its overall minimum element is even, it is impossible to make all elements odd or all elements even.

---

## Approach

Here is how the provided C++ solution works step-by-step:

- `sort( nums1.begin() , nums1.end());`: Sorts `nums1` in ascending order so that `nums1[0]` is the overall smallest element.
- `int val = nums1[0]%2;`: Saves the parity (0 for even, 1 for odd) of the smallest element `nums1[0]`.
- `bool allSame = true;`: Sets up a flag to check if all numbers in `nums1` already have the same parity.
- `for( int i=1 ; i<nums1.size() ; i++ )`: Loops through `nums1` starting from the second element.
- `if( val != nums1[i]%2 ){ allSame = false; break; }`: If any element has a different parity than `nums1[0]`, marks `allSame` as `false` and breaks the loop.
- `if( allSame ){ return true; }`: If every number already shares the same parity, returns `true` immediately.
- `int minOdd = 0, minEven = 0;`: Creates variables to store the smallest odd and smallest even numbers in `nums1`.
- `for( int i=0 ; i<nums1.size() ; i++ )`: Scans `nums1` to find the first (smallest) odd value (`minOdd`) and first (smallest) even value (`minEven`).
- `vector<int>arr(nums1.size());`: Allocates a temporary array `arr` (representing `nums2`) initialized with zeros.
- `arr[0]=nums1[0];`: Copies the smallest element to `arr[0]`.
- `for( int i=1 ; i<nums1.size() ; i++ )`: Loops through `nums1` and copies elements that already match the target parity `val` directly into `arr[i]`.
- `for( int i=1 ; i<arr.size() ; i++ )`: Iterates over `arr` to process elements that were left as `0`.
- `if( nums1[i]%2 == 0 && val == 1){ arr[i] = nums1[i]-minOdd; }`: If `nums1[i]` is even and the target parity `val` is odd (1), subtracts `minOdd` from `nums1[i]` to make `arr[i]` odd.
- `if( nums1[i]%2 == 1 && val == 0){ arr[i] = nums1[i]-minEven; }`: Tries to subtract `minEven` if `nums1[i]` is odd and `val` is even (0).
- `for( int i=1 ; i<arr.size() ; i++ )`: Checks if every constructed element in `arr` matches parity `val`.
- `if( allSame ){ return true; }`: Returns `true` if all elements in `arr` match parity `val`; otherwise returns `false`.

---

## Dry Run

### Case 1: Mixed parity with an odd minimum element (`nums1 = [1, 4, 7]`)

After `sort`, `nums1 = [1, 4, 7]`. `val = 1 % 2 = 1` (odd).

| Step / Index `i` | `nums1[i]` | `val` | `minOdd` | `minEven` | `arr[i]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | 1 | 0 | 0 | - | Sort array, check same parity loop finds mixed parities (`allSame = false`). |
| Find Mins | 1, 4, 7 | 1 | 1 | 4 | - | `minOdd` set to 1, `minEven` set to 4. |
| Copy Same | `i = 0` | 1 | 1 | 4 | 1 | Set `arr[0] = nums1[0] = 1`. |
| Copy Same | `i = 1` | 1 | 1 | 4 | 0 | `nums1[1] = 4` (even) doesn't match `val = 1`. Left as 0. |
| Copy Same | `i = 2` | 1 | 1 | 4 | 7 | `nums1[2] = 7` (odd) matches `val = 1`. Set `arr[2] = 7`. |
| Fill Zeros | `i = 1` | 1 | 1 | 4 | 3 | `arr[1] == 0`: `nums1[1]` is 4 (even) and `val == 1`. Set `arr[1] = 4 - 1 = 3`. |
| Check Final | - | 1 | 1 | 4 | - | `arr = [1, 3, 7]`. All elements have parity 1. Return `true`. |

---

### Case 2: Mixed parity with an even minimum element (`nums1 = [2, 3]`)

After `sort`, `nums1 = [2, 3]`. `val = 2 % 2 = 0` (even).

| Step / Index `i` | `nums1[i]` | `val` | `minOdd` | `minEven` | `arr[i]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | 0 | 0 | 0 | - | Sort array, check same parity loop finds mixed parities (`allSame = false`). |
| Find Mins | 2, 3 | 0 | 3 | 2 | - | `minEven` set to 2, `minOdd` set to 3. |
| Copy Same | `i = 0` | 0 | 3 | 2 | 2 | Set `arr[0] = nums1[0] = 2`. |
| Copy Same | `i = 1` | 0 | 3 | 2 | 0 | `nums1[1] = 3` (odd) doesn't match `val = 0`. Left as 0. |
| Fill Zeros | `i = 1` | 0 | 3 | 2 | 1 | `arr[1] == 0`: `nums1[1]` is 3 (odd) and `val == 0`. Set `arr[1] = 3 - 2 = 1`. |
| Check Final | `i = 1` | 0 | 3 | 2 | 1 | `arr = [2, 1]`. `arr[1] % 2 = 1` does not match `val = 0`. Return `false`. |

---

## Time & Space Complexity

- **Time Complexity:** **O(n log n)** — Sorting `nums1` takes `O(n log n)` time, followed by a few linear `O(n)` scans over the array.
- **Space Complexity:** **O(n)** — Creating the `arr` vector requires `O(n)` extra memory.

---

### Can this be improved?

**Yes!** We do not need to sort the array or create a new vector `arr`. 

From our mathematical intuition:
1. If all elements have the same parity, the answer is `true`.
2. Otherwise, if the overall minimum element is **odd**, the answer is `true`.
3. Otherwise, the answer is `false`.

We can check all of this in a **single linear pass** (`O(n)` time) using **O(1)** extra space by tracking the overall minimum element and whether we have seen any odd or even numbers.

```cpp
class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        int minVal = nums1[0];
        bool hasOdd = false;
        bool hasEven = false;

        for (int num : nums1) {
            if (num < minVal) {
                minVal = num;
            }
            if (num % 2 != 0) {
                hasOdd = true;
            } else {
                hasEven = true;
            }
        }

        // If all numbers are odd OR all are even
        if (!hasOdd || !hasEven) {
            return true;
        }

        // If mixed, it's possible if and only if the minimum value is odd
        return (minVal % 2 != 0);
    }
};
```

- `minVal` tracks the overall smallest element without sorting.
- `hasOdd` and `hasEven` track if both parities exist in `nums1`.
- `if (!hasOdd || !hasEven)` checks if all numbers share the same parity.
- `return (minVal % 2 != 0)` checks if the minimum element is odd when parities are mixed.

**Resulting Complexity:**
- **Time Complexity:** **O(n)** — A single loop over the input array of size `n`.
- **Space Complexity:** **O(1)** — Uses only a few integer/boolean variables.

**Theoretical Best Complexity:**
This **O(n)** time and **O(1)** space is the theoretical optimal solution because you must inspect every number in `nums1` at least once to determine its parity and find the minimum value.

---

## Edge Cases Handled

- **All numbers are odd (e.g., `[1, 3, 5]`):** Handled immediately since `allSame` remains `true` (or `!hasEven` in the optimal version).
- **All numbers are even (e.g., `[2, 4, 6]`):** Handled immediately since `allSame` remains `true` (or `!hasOdd` in the optimal version).
- **Smallest element is odd in a mixed array (e.g., `[1, 4, 7]`):** Correctly returns `true` because every even number can subtract `1` to become odd.
- **Smallest element is even in a mixed array (e.g., `[2, 3]`):** Correctly returns `false` because the smallest even element cannot become odd, and the smallest odd element cannot become even.
- **Minimum array size (`n = 1`):** A single element array always has uniform parity, returning `true`.
- **Large integer values (up to `10^9`):** Parity checks (`% 2`) and comparisons work properly without integer overflow.
