![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.8%20MB%20(beats%2044.14%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a positive integer `n`. Your task is to pick two digits from `n` and multiply them together to get the largest possible result. 

If a digit appears more than once inside `n`, you are allowed to use that digit twice. For example, if `n = 124`, the digits are 1, 2, and 4. The pair that gives the biggest result is 2 and 4, giving 2 * 4 = 8. If `n = 22`, both digits are 2, so the product is 2 * 2 = 4.

## Intuition

The core idea is to extract all the digits from the number `n` and place them into a list. Once all digits are in the list, compare every possible pair of digits, multiply them, and track the largest product found. Because numbers given in this problem are small (at most 10 digits long), comparing every pair is quick and guarantees we find the maximum product.

## Approach

* `vector<int>store;` — Creates an empty list called `store` to hold each digit of `n`.
* `while ( n!= 0 ){` — Starts a loop that runs until all digits have been removed from `n`.
* `store.push_back( n%10);` — Takes the last digit of `n` using the remainder operator (`n % 10`) and adds it to the `store` list.
* `n/=10;` — Removes the last digit from `n` by performing integer division by 10.
* `int maxi = 0;` — Initializes `maxi` to 0, which will store the highest multiplication product found.
* `for( int i=0 ; i<store.size() ; i++ )` — Starts an outer loop to pick the first digit of a pair from `store`.
* `for( int j=i+1 ; j<store.size() ; j++ )` — Starts an inner loop to pick a second digit located after the first digit, ensuring we do not compare a position with itself.
* `maxi = max(maxi,store[i]*store[j]);` — Multiplies the two chosen digits (`store[i] * store[j]`), compares the product with `maxi`, and updates `maxi` if the new product is larger.
* `return maxi;` — Returns the maximum product after testing all possible digit pairs.

## Dry Run

### Case 1: Typical case (`n = 124`)

| Step | `n` | `store` | `i` | `j` | `store[i]` | `store[j]` | `maxi` | Action |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | 124 | `[4]` | - | - | - | - | 0 | Extract `124 % 10 = 4`. `n` becomes 12. |
| 2 | 12 | `[4, 2]` | - | - | - | - | 0 | Extract `12 % 10 = 2`. `n` becomes 1. |
| 3 | 1 | `[4, 2, 1]` | - | - | - | - | 0 | Extract `1 % 10 = 1`. `n` becomes 0. Loop ends. |
| 4 | 0 | `[4, 2, 1]` | 0 | 1 | 4 | 2 | 8 | Check `4 * 2 = 8`. `maxi` updates to 8. |
| 5 | 0 | `[4, 2, 1]` | 0 | 2 | 4 | 1 | 8 | Check `4 * 1 = 4`. `maxi` stays 8. |
| 6 | 0 | `[4, 2, 1]` | 1 | 2 | 2 | 1 | 8 | Check `2 * 1 = 2`. `maxi` stays 8. Loop ends. |

### Case 2: Edge case with duplicate values (`n = 22`)

| Step | `n` | `store` | `i` | `j` | `store[i]` | `store[j]` | `maxi` | Action |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | 22 | `[2]` | - | - | - | - | 0 | Extract `22 % 10 = 2`. `n` becomes 2. |
| 2 | 2 | `[2, 2]` | - | - | - | - | 0 | Extract `2 % 10 = 2`. `n` becomes 0. Loop ends. |
| 3 | 0 | `[2, 2]` | 0 | 1 | 2 | 2 | 4 | Check `2 * 2 = 4`. `maxi` updates to 4. Loop ends. |

## Time & Space Complexity

* **Time Complexity:** **O(d^2)** where **d** is the number of digits in `n`. Extracting the digits takes **d** steps. Comparing every pair of digits takes **d * (d - 1) / 2** operations, which simplifies to **O(d^2)**. Since `n` is at most 10^9, **d** is at most 10, meaning the nested loop runs at most 45 times.
* **Space Complexity:** **O(d)** — storing the digits in the `store` vector uses memory proportional to the number of digits.

### Can this be improved?

Yes! The largest product of two positive single-digit numbers will always come from multiplying the **two largest digits** in `n`. We do not need to check all pairs or store every digit in a vector. 

Instead, we can traverse the digits of `n` once while maintaining two variables: `max1` for the largest digit seen so far, and `max2` for the second-largest digit.

```cpp
int max1 = 0, max2 = 0;
while (n != 0) {
    int digit = n % 10;
    if (digit > max1) {
        max2 = max1;
        max1 = digit;
    } else if (digit > max2) {
        max2 = digit;
    }
    n /= 10;
}
return max1 * max2;
```

* `digit = n % 10;` gets the current rightmost digit.
* `if (digit > max1)` shifts the old largest digit down to `max2` and stores the new highest digit in `max1`.
* `else if (digit > max2)` updates `max2` if the current digit is not strictly greater than `max1`, but is greater than `max2`.

### Improved Complexity

* **Time Complexity:** **O(d)** — we pass through the digits of `n` exactly once.
* **Space Complexity:** **O(1)** — we only use two integer variables (`max1` and `max2`) instead of a vector.

This **O(d)** time and **O(1)** space solution is theoretically optimal because we must look at every digit at least once to ensure we don't miss a larger digit.

## Edge Cases Handled

* **Duplicate Digits:** Inputs like `n = 22` or `n = 991` work correctly. Every digit position is processed individually, allowing duplicate values to be multiplied together.
* **Numbers with Zeros:** Inputs like `n = 10` work fine because `0` is pushed into the list and evaluated alongside `1`, yielding `1 * 0 = 0`.
* **Smallest Allowed Input:** The constraints guarantee `n >= 10`, meaning `n` will always have at least 2 digits, so `store` will always contain at least two elements for pair comparisons.
