![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.6%20MB%20(beats%2074.43%25)-green?style=for-the-badge)

---

## Problem Explained

You are given two numbers: `n` and `t`. 

Your goal is to find the smallest integer that is greater than or equal to `n` whose **digit product** (the result of multiplying all its individual digits together) can be divided by `t` with no remainder.

For example, if `n = 15` and `t = 3`:
- First, try `15`: digits are 1 and 5. Product = 1 * 5 = 5. Is 5 divisible by 3? No.
- Next, try `16`: digits are 1 and 6. Product = 1 * 6 = 6. Is 6 divisible by 3? Yes (6 / 3 = 2).
- Return `16`.

## Intuition

Since the constraints on `n` are very small (`n` is at most 100), we can simply test numbers one by one starting from `n` (`n`, `n + 1`, `n + 2`, and so on).

Here is the key insight: every block of 10 consecutive numbers contains at least one number that ends in 0 (like 10, 20, 30). Any number with a digit of 0 has a digit product equal to 0. Since 0 is divisible by any integer `t`, we will **never** need to check more than 10 consecutive numbers before finding a valid answer!

This guarantees that a simple step-by-step search will finish almost instantly.

## Approach

Here is how the solution works step-by-step:

### Helper Function: `product(int n)`
- `int total = 1;`: Start with a running product of 1.
- `while( n!= 0 )`: Loop until we have extracted every digit from `n`.
- `total = total * (n%10 );`: Extract the last digit of `n` using the modulo operator (`n % 10`) and multiply `total` by it.
- `n/=10;`: Remove the last digit from `n` using integer division.
- `return total;`: Return the final product of all digits.

### Main Function: `smallestNumber(int n, int t)`
- `while( true )`: Start an infinite loop to test candidates starting at `n`.
- `int prod = product( n );`: Calculate the digit product of the current number `n`.
- `if( prod % t == 0 )`: Check if `prod` is evenly divisible by `t`.
- `return n;`: If it is divisible, return `n` immediately (since we start at `n` and count up, the first match is guaranteed to be the smallest).
- `else{ n++; }`: If not divisible, increment `n` by 1 and try again.

## Dry Run

### Case 1: Starting number is already valid (`n = 10`, `t = 2`)

| Step | Candidate `n` | `product(n)` output (`prod`) | Check (`prod % t == 0`) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | 10 | 1 * 0 = 0 | 0 % 2 == 0 (True) | Return 10 immediately |

### Case 2: Iterating to find valid product (`n = 15`, `t = 3`)

| Step | Candidate `n` | `product(n)` output (`prod`) | Check (`prod % t == 0`) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | 15 | 1 * 5 = 5 | 5 % 3 == 2 (False) | Increment `n` to 16 |
| 2 | 16 | 1 * 6 = 6 | 6 % 3 == 0 (True) | Return 16 |

## Time & Space Complexity

- **Time Complexity:** O(1) constant time. In the worst-case scenario, the loop runs at most 10 times. For each candidate number, calculating the digit product takes operations proportional to its number of digits (at most 3 digits for `n <= 100`). Thus, the total execution steps are strictly bounded by a small constant.
- **Space Complexity:** O(1) constant space. The code only allocates a few integer variables (`total`, `prod`, `n`, `t`). Memory usage stays constant regardless of the input values.

### Is this optimal?
Yes. The solution already runs in O(1) time and O(1) space. No algorithm can execute faster than constant time, so no further asymptotic optimizations are possible or required.

## Edge Cases Handled

- **Input `n` is already valid:** The loop checks `n` first before incrementing, so if `n` already satisfies the condition (like `n = 10`, `t = 2`), it returns `n` on step 1.
- **`t = 1`:** Every digit product is divisible by 1. The code returns `n` right away.
- **Single-digit inputs (`n < 10`):** The helper function correctly handles single-digit numbers because `n % 10` equals `n`, and `n / 10` immediately becomes 0, terminating the helper loop correctly.
- **Numbers containing zero:** The digit product becomes 0, and `0 % t` is always 0 for any valid `t` (where `1 <= t <= 10`).
