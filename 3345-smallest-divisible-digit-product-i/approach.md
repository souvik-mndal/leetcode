![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%202.61%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.56%20MB%20(beats%2073.40%25)-green?style=for-the-badge)

---

## Problem Explained

You are given two numbers: a starting number `n` and a target divisor `t`. 

Your goal is to find the smallest number that is greater than or equal to `n` whose **digit product** (the result of multiplying all its individual digits together) can be divided by `t` with no remainder.

For example, if `n = 15` and `t = 3`:
- Check 15: Digits are 1 and 5. Product is 1 * 5 = 5. Is 5 divisible by 3? No.
- Check 16: Digits are 1 and 6. Product is 1 * 6 = 6. Is 6 divisible by 3? Yes (6 / 3 = 2).
- Result: 16.

## Intuition

The core idea is **linear search** (checking numbers one by one starting from `n`).

Why does this work so quickly without running forever?
Every group of 10 consecutive numbers contains at least one number that ends in 0 (like 10, 20, 30, etc.). Any number with a digit of 0 has a digit product of 0. Since 0 is divisible by every number `t`, you will never need to check more than 10 numbers before finding a valid answer.

Because `n` is very small (at most 100), testing numbers one by one is simple, fast, and guaranteed to stop almost instantly.

## Approach

Here is how the solution works step-by-step:

- `int total = 1;`: Inside the helper function `product`, initialize a accumulator variable `total` to 1 to hold the running product of the digits.
- `while( n!= 0 )`: Set up a loop that runs until all digits of `n` have been processed.
- `total = total * (n%10 );`: Extract the last digit of `n` using modulo 10 (`n % 10`), and multiply `total` by this digit.
- `n/=10;`: Remove the last digit from `n` using integer division by 10 so the loop can move to the next digit.
- `while( true )`: In `smallestNumber`, start an infinite loop that keeps checking numbers starting from `n` and counting upwards.
- `int prod = product( n );`: Calculate the digit product of the current number `n` using the `product` helper function.
- `if( prod % t == 0 )`: Check if `prod` is evenly divisible by `t` (remainder is 0). If it is, return `n` immediately.
- `else{ n++; }`: If `prod` is not divisible by `t`, add 1 to `n` and test the next number in the next loop iteration.

## Dry Run

### Case 1: Immediate Match (`n = 10`, `t = 2`)

| `n` | `prod` (Digit Product) | `prod % t == 0` | Action |
|---|---|---|---|
| 10 | 1 * 0 = 0 | 0 % 2 == 0 (True) | Return 10 immediately |

---

### Case 2: Number Increment Needed (`n = 15`, `t = 3`)

| `n` | `prod` (Digit Product) | `prod % t == 0` | Action |
|---|---|---|---|
| 15 | 1 * 5 = 5 | 5 % 3 == 2 (False) | Increment `n` to 16 |
| 16 | 1 * 6 = 6 | 6 % 3 == 0 (True) | Return 16 |

## Time & Space Complexity

- **Time Complexity:** **O(1)** — constant time. The code checks at most 10 numbers because every sequence of 10 consecutive numbers includes a multiple of 10 (which has a digit product of 0). For each number, calculating the digit product takes at most 3 operations because `n` is at most 100.
- **Space Complexity:** **O(1)** — constant space. The code only uses a few integer variables (`total`, `prod`, `n`, `t`) and does not allocate any extra memory structures.

### Is this already optimal?
**Yes, this is already optimal.** 

Both time and space complexity are **O(1)**. The low runtime percentile on online judges (like 2.61%) is just natural variation when measuring tiny execution times (fractions of a millisecond). No algorithmic changes can improve upon O(1) time and O(1) space.

## Edge Cases Handled

- **Single-digit numbers (`n` < 10):** The digit product of a single digit is the digit itself (e.g., for `n = 5`, digit product is 5).
- **Starting number is already valid:** If `n` already satisfies the condition (like `n = 10, t = 2`), the loop terminates on the very first try without extra iterations.
- **Numbers ending in 0:** Numbers like 10, 20, 30 have a digit product of 0, which is divisible by any integer `t` from 1 to 10.
- **Maximum constraint (`n = 100`):** The digit product of 100 is 1 * 0 * 0 = 0, which is divisible by all valid values of `t`.
