![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.8%20MB%20(beats%2099.33%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an integer called `n`. Your goal is to create a brand new number by taking all the digits of `n`, throwing away any zeros, and gluing the remaining digits together in their original order. Let us call this new number `x`. 

Next, you need to add up all the individual digits of that new number `x` to get a total called `sum`. Finally, you multiply `x` by `sum` and return the result. 

If `n` has no non-zero digits at all (like if `n` is zero), then `x` becomes `0`.

For example, if `n` is `10203004`:
- You filter out the zeros, leaving the digits `1`, `2`, `3`, and `4`.
- Gluing them together gives `x = 1234`.
- The sum of those digits is `1 + 2 + 3 + 4 = 10`, so `sum = 10`.
- You multiply them together to get `1234 * 10 = 12340`.

---

## Intuition

When processing the digits of a number, we naturally extract them from right to left using the modulo operator (getting the remainder when divided by `10`) and division (cutting off the last digit). 

The clever trick here is building the new number `x` and calculating its digit sum **at the exact same time**, during a single loop over the digits of `n`. As we pull off each digit from `n`, if it is not zero, we prepend it to `x` (placing it to the left of what we have already built using a multiplier called `len`), and we add it directly to our running `sum`. Because we inspect the digits from right to left, prepending them in this manner naturally restores their original left-to-right order in `x`.

---

## Approach

Here is how the code works step-by-step:

- `long long x = 0, sum = 0, len = 1;`: Initialize `x` to store our filtered number, `sum` to track the sum of the digits of `x`, and `len` as a positional multiplier starting at `1` to help place non-zero digits into their correct decimal places as we build `x` from right to left.
- `while(n != 0) {`: Start a loop that continues as long as there are still digits left to process in `n`.
- `x = (n % 10) * len + x;`: Take the last digit of `n` (using `n % 10`), multiply it by the current place value `len`, and add it to `x`. This shifts our previously built digits to the left and inserts the new digit at the correct position.
- `if(n % 10 != 0) len = len * 10;`: Check if the extracted digit is not zero. If it is a valid non-zero digit, multiply `len` by `10` so the next non-zero digit we find will be placed one position further to the left.
- `sum += n % 10;`: Add the current digit of `n` to our running `sum`. *Note: Even though the problem says sum is the sum of digits in x, notice that the non-zero digits of n are the exact same digits that end up in x, and zero digits contribute nothing to a sum, so adding them here gives the correct final sum.*
- `n = n / 10;`: Chop off the last digit of `n` by dividing it by `10`, moving us on to the next digit for the next loop iteration.
- `return sum * x;`: Once all digits of `n` are processed, multiply the final `sum` by the final number `x` and return the result.

---

## Dry Run

### Case 1: Typical case (`n = 10203004`)

| `n` before loop | `n % 10` | `x` before update | `len` before update | `sum` before update | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `10203004` | `4` | `0` | `1` | `0` | `4` is non-zero. `x` becomes `4`, `len` becomes `10`, `sum` becomes `4`, `n` becomes `1020303`. |
| `1020303` | `0` | `4` | `10` | `4` | `0` is zero. `x` stays `4`, `len` stays `10`, `sum` adds `0`, `n` becomes `102030`. |
| `102030` | `0` | `4` | `10` | `4` | `0` is zero. `x` stays `4`, `len` stays `10`, `sum` adds `0`, `n` becomes `10203`. |
| `10203` | `3` | `4` | `10` | `4` | `3` is non-zero. `x` becomes `34`, `len` becomes `100`, `sum` becomes `7`, `n` becomes `1020`. |
| `1020` | `0` | `34` | `100` | `7` | `0` is zero. `x` stays `34`, `len` stays `100`, `sum` adds `0`, `n` becomes `102`. |
| `102` | `2` | `34` | `100` | `7` | `2` is non-zero. `x` becomes `234`, `len` becomes `1000`, `sum` becomes `9`, `n` becomes `10`. |
| `10` | `0` | `234` | `1000` | `9` | `0` is zero. `x` stays `234`, `len` stays `1000`, `sum` adds `0`, `n` becomes `1`. |
| `1` | `1` | `234` | `1000` | `9` | `1` is non-zero. `x` becomes `1234`, `len` becomes `10000`, `sum` becomes `10`, `n` becomes `0`. |

Loop ends because `n` is `0`. Returns `sum * x`, which is `10 * 1234 = 12340`.

---

### Case 2: Edge case with all zeros except leading one (`n = 1000`)

| `n` before loop | `n % 10` | `x` before update | `len` before update | `sum` before update | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `1000` | `0` | `0` | `1` | `0` | `0` is zero. `x` stays `0`, `len` stays `1`, `sum` adds `0`, `n` becomes `100`. |
| `100` | `0` | `0` | `1` | `0` | `0` is zero. `x` stays `0`, `len` stays `1`, `sum` adds `0`, `n` becomes `10`. |
| `10` | `0` | `0` | `1` | `0` | `0` is zero. `x` stays `0`, `len` stays `1`, `sum` adds `0`, `n` becomes `1`. |
| `1` | `1` | `0` | `1` | `0` | `1` is non-zero. `x` becomes `1`, `len` becomes `10`, `sum` becomes `1`, `n` becomes `0`. |

Loop ends because `n` is `0`. Returns `sum * x`, which is `1 * 1 = 1`.

---

## Time & Space Complexity

- **Time:** O(log10(n)) — The loop divides `n` by `10` on every single step. This means the number of loop iterations is equal to the number of digits in `n`, which grows logarithmically with the value of `n`.
- **Space:** O(1) — We only use a few variables (`x`, `sum`, `len`) to keep track of our data, requiring a constant amount of extra memory regardless of how large `n` is.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, the code is already at the optimal possible time complexity of O(log10(n)) and space complexity of O(1). You cannot solve this problem without visiting each digit of the input number at least once, meaning you must touch every digit, which takes time proportional to the number of digits. No further improvement is possible.

---

## Edge Cases Handled

- **Numbers with all zeros after the first digit (like `1000`):** Handled correctly because the trailing zeros add nothing to `sum` and do not increase `len`, leaving `x` as just `1` and `sum` as `1`.
- **Zero input (`n = 0`):** The loop condition `n != 0` immediately fails, skipping the loop entirely. It returns `sum * x` which is `0 * 0 = 0`, matching the problem statement requirement that `x = 0` if there are no non-zero digits.
- **Large numbers up to constraint limits (`10^9`):** Handled safely because `x`, `sum`, and `len` are declared as `long long`, preventing integer overflow when `len` multiplies up to `10^9` or when the final multiplication happens.
