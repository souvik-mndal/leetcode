![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-7.72%20MB%20(beats%2068.55%25)-green?style=for-the-badge)

---

## Problem Explained

The goal is to determine if a positive number `n` is divisible by a special value. This special value is created by adding two numbers together:
1. The sum of all digits in `n`.
2. The product of all digits in `n`.

If `n` divides by this total with no remainder left over, we return `true`. If there is a remainder, we return `false`.

For example, if `n = 99`:
* The digits are 9 and 9.
* Digit sum = 9 + 9 = 18.
* Digit product = 9 * 9 = 81.
* Total combined value = 18 + 81 = 99.
* Since 99 divides by 99 evenly (remainder is 0), the answer is `true`.

---

## Intuition

To solve this, we need to look at each digit of `n` one by one. 

In standard math, taking a number modulo 10 (`m % 10`) gives you its rightmost digit. Dividing a number by 10 (`m / 10`) removes that rightmost digit. 

By repeatedly taking the last digit and then chopping it off in a loop, we can visit every single digit from right to left. While doing this, we keep a running total of the addition (`sum`) and multiplication (`prod`). Once we process all digits, we add `sum` and `prod` together and test if `n` is divisible by that combined total using the remainder operator (`%`).

---

## Approach

Here is step-by-step how the code works:

* `int sum = 0 , prod = 1 , m=n;` — Sets up three tracking variables. `sum` starts at 0 to accumulate addition. `prod` starts at 1 so multiplication works correctly. `m` is a temporary copy of `n` that we will shrink digit by digit without modifying the original `n`.
* `while( m!=0 )` — Keeps looping as long as there are still digits left to extract from `m`.
* `sum += (m%10);` — Takes the last digit of `m` using modulo 10 and adds it to `sum`.
* `prod *= (m%10);` — Takes that same last digit of `m` and multiplies `prod` by it.
* `m/=10;` — Removes the last digit from `m` by dividing by 10 (integer division throws away any decimal remainder).
* `int total = sum+prod;` — Adds the calculated digit sum and digit product together to get the final divisor.
* `if( n%total == 0 )` — Checks if the original number `n` leaves no remainder when divided by `total`.
* `return true;` / `return false;` — Returns `true` if it divides evenly, otherwise returns `false`.

---

## Dry Run

### Case 1: Typical case resulting in true (`n = 99`)

| Step | `m` | `m % 10` (digit) | `sum` | `prod` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 99 | - | 0 | 1 | Initialize variables |
| Step 1 | 99 | 9 | 9 | 9 | Process digit 9, reduce `m` to 9 |
| Step 2 | 9 | 9 | 18 | 81 | Process digit 9, reduce `m` to 0 |
| Final | 0 | - | 18 | 81 | Loop ends. `total` = 18 + 81 = 99. Check `99 % 99 == 0` -> **true** |

### Case 2: Typical case resulting in false (`n = 23`)

| Step | `m` | `m % 10` (digit) | `sum` | `prod` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 23 | - | 0 | 1 | Initialize variables |
| Step 1 | 23 | 3 | 3 | 3 | Process digit 3, reduce `m` to 2 |
| Step 2 | 2 | 2 | 5 | 6 | Process digit 2, reduce `m` to 0 |
| Final | 0 | - | 5 | 6 | Loop ends. `total` = 5 + 6 = 11. Check `23 % 11 == 0` -> **false** |

---

## Time & Space Complexity

* **Time Complexity:** O(log10(n)) — The loop runs once for every digit in `n`. The number of digits in `n` is equal to log10(n) + 1. Since `n` is at most 1,000,000, the loop runs at most 7 times, making execution virtually instant.
* **Space Complexity:** O(1) — We only use a few simple integer variables (`sum`, `prod`, `m`, `total`). No additional arrays or data structures are created.

### Is this optimal?
Yes, this code is already fully optimal. To find the sum and product of digits, every digit must be read at least once. Processing each digit in a single pass uses the minimum possible time and space.

---

## Edge Cases Handled

* **Single-digit numbers (e.g., `n = 5`):** The loop runs once. `sum` becomes 5 and `prod` becomes 5. `total` becomes 10. `5 % 10` equals 5, which correctly returns `false`.
* **Numbers with a zero digit (e.g., `n = 10`):** Multiplying by zero sets `prod` to 0. `sum` becomes 1 + 0 = 1. `total` becomes 1 + 0 = 1. `10 % 1` is 0, returning `true`. Zero digits do not cause division-by-zero because `sum` is always greater than 0 for any positive integer `n`.
* **Upper boundary (`n = 10^6`):** The code safely processes numbers up to 1,000,000 without integer overflow issues since the product of digits for numbers in this range remains small.
