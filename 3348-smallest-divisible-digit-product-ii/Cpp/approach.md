![Runtime](https://img.shields.io/badge/Runtime-121%20ms%20(beats%2026.03%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-34.1%20MB%20(beats%2089.43%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `num` representing a large positive integer, and an integer `t`.

Your task is to find the **smallest positive integer** that satisfies three conditions:
1. It is **greater than or equal to** `num`.
2. It is **zero-free** (it does not contain the digit `'0'` anywhere).
3. The product of all its digits is **divisible by `t`**.

If no such number can ever be formed, return `"-1"`.

**Example:** 
If `num = "1234"` and `t = 256`:
- `1234` is zero-free, but its digit product is `1 * 2 * 3 * 4 = 24`, which is not divisible by `256`.
- The smallest zero-free number greater than `1234` whose digit product is divisible by `256` is `1488` (product = `1 * 4 * 8 * 8 = 256`).

---

## Intuition

Since we form the digit product using single digits from `'1'` to `'9'`, the only prime factors those digits can ever contribute are **2, 3, 5, and 7**.

- If `t` has any prime factor greater than 7 (like 11 or 13), no combination of single digits can ever produce a product divisible by `t`. We can immediately return `"-1"`.
- Digit `'5'` is the **only** digit that provides prime factor 5.
- Digit `'7'` is the **only** digit that provides prime factor 7.
- Digits 2, 3, 4, 6, 8, and 9 provide combinations of prime factors 2 and 3:
  - `'2'` gives one 2
  - `'3'` gives one 3
  - `'4'` gives two 2s
  - `'6'` gives one 2 and one 3
  - `'8'` gives three 2s
  - `'9'` gives two 3s

Because factors 5 and 7 are non-flexible (each 5 requires a digit `'5'`, each 7 requires a digit `'7'`), the core challenge is finding the fewest digits needed to gather enough factors of 2 and 3. We use **Dynamic Programming (DP)** to precalculate `dp[i][j]`: the minimum number of digits needed to supply at least `i` factors of 2 and `j` factors of 3.

To find the smallest valid number `>= num`:
1. Check if `num` itself is zero-free and already valid.
2. If not, try to keep a prefix of `num` unchanged, increase the digit at index `i` to a larger value, and fill the remaining positions greedily with the smallest digits that satisfy the factor counts. We scan `i` from right to left, stopping if we encounter a `'0'` (since any digit after a `'0'` cannot be kept unchanged).
3. If no prefix of length `n` works, we must increase the number of digits. We set the new length to `M = max(n + 1, min_len_needed)` and fill all `M` positions greedily.

---

## Approach

Here is how the code works step-by-step:

- `while (temp % 2 == 0) { temp /= 2; req2++; } ...`  
  Counts how many factors of 2, 3, 5, and 7 are required by `t`, storing them in `req2`, `req3`, `req5`, and `req7`.

- `if (temp > 1) return "-1";`  
  If `t` has any prime factor other than 2, 3, 5, or 7, it is impossible to form the required product using single digits. Returns `"-1"`.

- `dp[0][0] = 0;` and transition loop  
  Precomputes the minimum number of digits needed to supply `i` twos and `j` threes using available digits `{2, 3, 4, 6, 8, 9}` represented by state transitions in `trans`.

- Suffix minimum filling `dp[i][j] = min(dp[i][j], dp[i + 1][j]);`  
  Ensures that `dp[i][j]` represents the minimum digits needed for **at least** `i` twos and **at least** `j` threes.

- Factor lookup arrays `F2`, `F3`, `F5`, `F7`  
  Store how many factors of 2, 3, 5, and 7 each single digit `0` through `9` provides.

- `if (!has_zero) { ... return num; }`  
  Checks if `num` is zero-free and already has a digit product divisible by `t`. If so, returns `num` immediately.

- `int limit = min(n - 1, first_zero);`  
  Restricts prefix matching up to the first `'0'` in `num`. We cannot keep digits after a `'0'` unchanged because the `'0'` itself must be replaced with a non-zero digit.

- `for (int i = limit; i >= 0; --i)`  
  Iterates backward from `limit`. We try to keep the prefix `num[0...i-1]`, increase `num[i]` to a larger digit `d`, and check if the remaining `L = n - 1 - i` positions can supply the remaining needed factors.

- `if (n7 + n5 + dp[n2][n3] <= L)`  
  Checks if `L` remaining positions are enough to fit all required 7s (`n7`), all required 5s (`n5`), and the minimum digits needed for remaining 2s and 3s (`dp[n2][n3]`). If yes, fills the remaining `L` positions greedily starting with the smallest digit `'1'` to `'9'` at each step and returns the result.

- `int M = max(n + 1, min_len_needed);`  
  If no same-length answer exists, determines the minimum length `M` for a strictly longer number. Then constructs the lexicographically smallest zero-free number of length `M` digit by digit.

---

## Dry Run

### Case 1: Typical case with replacement

**Input:** `num = "1234"`, `t = 256`

- Factorization of `t = 256`: `req2 = 8, req3 = 0, req5 = 0, req7 = 0`.
- `has_zero = false`. Product of `"1234"` is 24 (not divisible by 256).
- `limit = 3`, `p2 = 0` initially. Prefix sums calculated up to `limit - 1 = 2`:
  - `num[0]='1'` (0 twos), `num[1]='2'` (1 two), `num[2]='3'` (0 twos). Total `p2 = 1`.

| Loop `i` | Current Digit `num[i]` | Try `d` | Remaining Length `L` | Needed Twos `n2` | `dp[n2][0] <= L` | Action |
|---|---|---|---|---|---|---|
| 3 | `'4'` | 5..9 | 0 | 7 - F2[d] | False | No valid digit for `i = 3`. Decrement `p2` by `F2['3'] = 0`. |
| 2 | `'3'` | 4 | 1 | max(0, 8 - 1 - 2) = 5 | `dp[5][0] = 2 <= 1` (False) | 5 twos need 2 digits, but `L = 1`. |
| 2 | `'3'` | 8 | 1 | max(0, 8 - 1 - 3) = 4 | `dp[4][0] = 2 <= 1` (False) | 4 twos need 2 digits, but `L = 1`. No digit works for `i = 2`. Decrement `p2` by `F2['2'] = 1` (now `p2 = 0`). |
| 1 | `'2'` | 3 | 2 | max(0, 8 - 0 - 0) = 8 | `dp[8][0] = 3 <= 2` (False) | 8 twos need 3 digits. |
| 1 | `'2'` | 4 | 2 | max(0, 8 - 0 - 2) = 6 | `dp[6][0] = 2 <= 2` (True!) | Match found! Fixed prefix `"14"`, `L = 2`, `rem2 = 6`. |

Greedy fill for remaining `L = 2` positions:
- Position 0: Try `x = 1..7` (fails), try `x = 8` (`rem2` becomes `max(0, 6 - 3) = 3`, `dp[3][0] = 1 <= 1` True). Pick `'8'`.
- Position 1: Try `x = 1..7` (fails), try `x = 8` (`rem2` becomes `max(0, 3 - 3) = 0`, `dp[0][0] = 0 <= 0` True). Pick `'8'`.
- **Output:** `"1488"`

---

### Case 2: Impossible factorization

**Input:** `num = "11111"`, `t = 26`

| Step | `temp` value | Operation | Action |
|---|---|---|---|
| Initial | 26 | Factorize 2 | `temp` becomes 13, `req2 = 1` |
| Factorize 3 | 13 | Try divide by 3 | `13 % 3 != 0`, unchanged |
| Factorize 5 | 13 | Try divide by 5 | `13 % 5 != 0`, unchanged |
| Factorize 7 | 13 | Try divide by 7 | `13 % 7 != 0`, unchanged |
| Check `temp > 1` | 13 | `13 > 1` holds | Returns `"-1"` immediately |

- **Output:** `"-1"`

---

## Time & Space Complexity

- **Time Complexity:** **O(n + log t)**
  - Factorizing `t` takes **O(log t)** steps.
  - Building the DP table takes **O(60 * 40 * 6)** operations, which is **O(1)** constant time.
  - The outer loop runs at most `n` times. Inside the loop, checking digit choices takes **O(1)** time.
  - Greedily constructing the final answer string takes **O(n)** time.
  - Overall time is linear with respect to the length of `num`.

- **Space Complexity:** **O(n)**
  - The DP table uses a fixed `60 x 40` space, which is **O(1)**.
  - The output string `ans` requires **O(n)** memory space.

### Is this optimal?
**Yes, this is already optimal.** Any valid solution must inspect `num` and output a result string of length `n` (or `n + 1`), which inherently requires at least **O(n)** time and **O(n)** space.

---

## Edge Cases Handled

- **Prime factors greater than 7:** Inputs like `t = 26` (contains prime factor 13) or `t = 33` (contains 11) immediately return `"-1"` because single digits cannot supply prime factors above 7.
- **Numbers containing zero:** Inputs like `num = "1023"` have zeros. The code identifies the `first_zero` index and restricts prefix matching to indices before the zero, ensuring no `'0'` remains in the output.
- **`num` is already zero-free and valid:** Inputs like `num = "12355", t = 50` return `num` directly without modifying any digits.
- **Answer requires a larger length:** Inputs where no modification of same length `n` works cause the code to cleanly fallback to constructing the smallest valid number of length `M = max(n + 1, min_len_needed)`.
- **Target `t = 1`:** Digits don't need to meet any factor requirements, so any zero-free number works. The algorithm handles `req2 = req3 = req5 = req7 = 0` correctly.
