![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-7.7%20MB%20(beats%2065.83%25)-green?style=for-the-badge)

---

## Problem Explained

The task asks us to determine if a given positive integer `n` can be evenly divided by a special number called `total`.

To find this `total`, we look at all the individual digits of `n`:
1. Calculate the **sum** of all digits.
2. Calculate the **product** (multiplication) of all digits.
3. Add the **sum** and the **product** together.

If `n` leaves a remainder of 0 when divided by this `total`, we return `true`. Otherwise, we return `false`.

For example, if `n = 99`:
- Digit sum: 9 + 9 = 18
- Digit product: 9 * 9 = 81
- Combined total: 18 + 81 = 99
- Since 99 divided by 99 equals 1 with remainder 0, the answer is `true`.

---

## Intuition

To solve this, we need to extract each digit of `n` standard base-10 math operations:
- `m % 10` gives us the last (rightmost) digit of `m`.
- `m / 10` drops the last digit from `m`.

By running a loop that repeatedly takes the last digit and then drops it, we can visit every digit from right to left. While doing this, we keep a running sum and a running product. Once all digits are processed, we sum the two results and test if `n` is divisible by the total using the modulo operator (`n % total == 0`).

---

## Approach

* `int sum = 0 , prod = 1 , m=n;`  
  We initialize `sum` to 0 (since adding 0 does not change values) and `prod` to 1 (since multiplying by 1 keeps values intact). We make a copy of `n` called `m` so we can strip away digits without losing the original value of `n`.

* `while( m!=0 )`  
  This loop runs as long as `m` still has digits left to process.

* `sum += (m%10);`  
  `m % 10` grabs the last digit of `m`. We add this digit to our running `sum`.

* `prod *= (m%10);`  
  We multiply our running `prod` by that same last digit.

* `m/=10;`  
  We divide `m` by 10 (using integer division), which removes its last digit so we can move on to the next digit in the next loop turn.

* `int total = sum+prod;`  
  After processing all digits, we add `sum` and `prod` together to get `total`.

* `if( n%total == 0 )`  
  We check if `n` divided by `total` leaves no remainder (`%` is the modulo operator, which gives the remainder of division).

* `return true;` / `return false;`  
  If the remainder is 0, `n` is divisible, so we return `true`. Otherwise, we return `false`.

---

## Dry Run

### Case 1: Divisible number (`n = 99`)

| Step | `n` | `m` | `sum` | `prod` | `total` | Action |
| --- | --- | --- | --- | --- | --- | --- |
| Init | 99 | 99 | 0 | 1 | - | Initialize variables; copy `n` into `m`. |
| Loop 1 | 99 | 9 | 9 | 9 | - | Extract last digit 9. Add to `sum`, multiply into `prod`. Divide `m` by 10. |
| Loop 2 | 99 | 0 | 18 | 81 | - | Extract last digit 9. Add to `sum`, multiply into `prod`. Divide `m` by 10. |
| After Loop | 99 | 0 | 18 | 81 | 99 | Loop ends (`m == 0`). Compute `total = sum + prod` (18 + 81 = 99). |
| Result | 99 | 0 | 18 | 81 | 99 | Check `99 % 99 == 0`. It is true, so return `true`. |

### Case 2: Non-divisible number (`n = 23`)

| Step | `n` | `m` | `sum` | `prod` | `total` | Action |
| --- | --- | --- | --- | --- | --- | --- |
| Init | 23 | 23 | 0 | 1 | - | Initialize variables; copy `n` into `m`. |
| Loop 1 | 23 | 2 | 3 | 3 | - | Extract last digit 3. Add to `sum`, multiply into `prod`. Divide `m` by 10. |
| Loop 2 | 23 | 0 | 5 | 6 | - | Extract last digit 2. Add to `sum`, multiply into `prod`. Divide `m` by 10. |
| After Loop | 23 | 0 | 5 | 6 | 11 | Loop ends (`m == 0`). Compute `total = sum + prod` (5 + 6 = 11). |
| Result | 23 | 0 | 5 | 6 | 11 | Check `23 % 11 == 0`. 23 leaves remainder 1, so return `false`. |

---

## Time & Space Complexity

- **Time Complexity:** **O(log10(n))** — The loop runs once for every digit in `n`. The number of digits in `n` is roughly log10(n). Since the constraints state `n <= 10^6`, `n` has at most 7 digits, so the loop executes at most 7 times. This is effectively **O(1)** in practice.
- **Space Complexity:** **O(1)** — We only store a few integer variables (`sum`, `prod`, `m`, `total`). No additional data structures are used.

### Is this optimal?
**Yes, this solution is already fully optimal.** To calculate the sum and product of digits, every digit must be read at least once. Since inspecting all digits takes time proportional to the number of digits, and space is minimal, no faster or memory-leaner approach exists.

---

## Edge Cases Handled

- **Numbers containing zero (e.g., `n = 10` or `n = 102`):** If any digit is 0, `prod` becomes 0. The code handles this naturally because multiplying by 0 results in 0, leaving `total = sum + 0 = sum`. Division by zero is avoided because positive numbers always have a digit sum greater than 0.
- **Single-digit numbers (e.g., `n = 7`):** For single digits, `sum = 7` and `prod = 7`, so `total = 14`. Since `7 % 14 != 0`, it correctly evaluates to `false`.
- **Upper boundary constraint (`n = 10^6`):** The input `10^6` fits well inside a standard 32-bit signed integer. The code processes digits `1, 0, 0, 0, 0, 0, 0`, giving `sum = 1` and `prod = 0`. `total = 1`, and `10^6 % 1 == 0`, correctly returning `true`.
