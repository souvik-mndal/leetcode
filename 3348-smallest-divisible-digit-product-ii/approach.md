![Runtime](https://img.shields.io/badge/Runtime-121%20ms%20(beats%2032.76%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-34.09%20MB%20(beats%2055.17%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a string `num` representing a large positive integer and a target integer `t`. 

You need to find the smallest number (as a string) that satisfies three conditions:
1. It is greater than or equal to `num` (`ans >= num`).
2. It contains **no zero digits** anywhere (it is "zero-free", so digits 1 through 9 only).
3. The product of all its digits is divisible by `t`.

If no such number can ever exist, return `"-1"`.

**Example:**
If `num = "1234"` and `t = 256`, the answer is `"1488"`.
- `"1488"` is greater than `"1234"`.
- It contains no zeros.
- The product of its digits is `1 * 4 * 8 * 8 = 256`, which is divisible by `256`.

---

## Intuition

The key observation comes from single digits (1 through 9) and their prime factors:
- Digits 1 to 9 can only contribute prime factors **2, 3, 5, and 7**.
- If `t` has any prime factor greater than 7 (like 11, 13, 17, etc.), it is **impossible** to form a divisible digit product. In that case, we immediately return `"-1"`.

Next, let's break down how digits contribute to these prime factors:
- Factors of **5** can only come from the digit `5` (1 factor per digit).
- Factors of **7** can only come from the digit `7` (1 factor per digit).
- Factors of **2** and **3** can be shared across digits in flexible ways:
  - Digit `2` gives one 2.
  - Digit `3` gives one 3.
  - Digit `4` gives two 2s.
  - Digit `6` gives one 2 and one 3.
  - Digit `8` gives three 2s.
  - Digit `9` gives two 3s.

Because 2 and 3 can combine into different single digits (like `6` or `8`), we can use **Dynamic Programming (DP)** to precompute `dp[i][j]`: *the minimum number of digits needed to supply at least `i` factors of 2 and `j` factors of 3*.

To find the smallest valid number:
1. **Same Length Search:** Try to keep as much of `num`'s prefix as possible. From right to left, try changing a digit at position `i` to a larger digit `d > num[i]`. Check if the remaining length `L` to the right is long enough to hold all needed prime factors. If yes, greedily pick the smallest valid digits for the rest of the string.
2. **Greater Length Search:** If no same-length solution exists, construct a slightly longer string (e.g. length `num.length() + 1`) greedily from left to right using digits 1 through 9.

---

## Approach

Here is the step-by-step breakdown of how the code executes:

- `while (temp % 2 == 0) ...`: Factorize `t` into prime factor counts: `req2`, `req3`, `req5`, and `req7`.
- `if (temp > 1) return "-1";`: If `t` still has a remainder > 1 after removing all 2s, 3s, 5s, and 7s, return `"-1"` because digits 1-9 cannot supply any other prime factors.
- `int dp[60][40]; ...`: Initialize a 2D DP table with large values (`1e9`). Set `dp[0][0] = 0`.
- `for (auto& tr : trans)`: Populate `dp[i][j]` using basic digits (`2`, `3`, `4`, `6`, `8`, `9`) to find the shortest digit length needed to get `i` twos and `j` threes.
- `for (int i = 59; i >= 0; --i) ...`: Run a reverse pass over the DP table so that `dp[i][j]` represents the minimum digits needed for *at least* `i` twos and *at least* `j` threes.
- `int F2[] = ...`: Set up static lookup tables `F2`, `F3`, `F5`, `F7` that map each digit `0..9` to its count of prime factors 2, 3, 5, and 7.
- `if (!has_zero) { ... }`: If `num` contains no zeros, test if `num` itself already has a digit product divisible by `t`. If so, return `num` immediately.
- `int limit = min(n - 1, first_zero);`: Calculate the max prefix boundary. We cannot reuse any prefix that includes or goes past a `'0'`.
- `for (int i = limit; i >= 0; --i)`: Iterate backwards through the prefix positions to find the rightmost position `i` where we can increment `num[i]` to a higher digit `d`.
- `if (n7 + n5 + dp[n2][n3] <= L)`: Check if the remaining length `L` can fit all needed factors of 7, 5, 2, and 3.
- `for (int pos = 0; pos < L; ++pos)`: If valid, fix the prefix up to `i` with digit `d`, and greedily pick the smallest digits (`1..9`) for each remaining position.
- `int M = max(n + 1, min_len_needed);`: If no same-length string works, calculate the minimum target length `M` for a longer number, and build it greedily from left to right.

---

## Dry Run

### Case 1: Standard Prefix Modification (`num = "1234"`, `t = 256`)

- Factorize `t = 256`: `req2 = 8`, `req3 = 0`, `req5 = 0`, `req7 = 0`.
- `num` has no zeros. Check `num = "1234"`: factors provided are `1` (from digit 2) + `2` (from digit 4) = 3 twos. Needs 8 twos. Not divisible.
- `n = 4`, `limit = 3`.

| Step | Index `i` | Digit `d` tried | `L` (Rem. Length) | `n2` needed | Minimum Digits Needed (`dp[n2][0]`) | Valid? | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `i = 3` (`'4'`) | `5` | 0 | 5 | `dp[5][0] = 2` | 2 <= 0 (False) | Try next `d` |
| 2 | `i = 3` (`'4'`) | `8` | 0 | 2 | `dp[2][0] = 1` | 1 <= 0 (False) | Loop `d` ends |
| 3 | `i = 2` (`'3'`) | `4` | 1 | 5 | `dp[5][0] = 2` | 2 <= 1 (False) | Try next `d` |
| 4 | `i = 1` (`'2'`) | `3` | 2 | 8 | `dp[8][0] = 3` | 3 <= 2 (False) | Try next `d` |
| 5 | `i = 1` (`'2'`) | `4` | 2 | 6 | `dp[6][0] = 2` | 2 <= 2 (True) | **Match found!** Prefix becomes `"14"`. |

Now fill remaining `L = 2` positions greedily:
- Position 0 (`pos = 0`): Try digits `1..9`. Digit `x = 8` gives `nn2 = 6 - 3 = 3`. `dp[3][0] = 1 <= 1` (True). Pick `'8'`.
- Position 1 (`pos = 1`): Try digits `1..9`. Digit `x = 8` gives `nn2 = 3 - 3 = 0`. `dp[0][0] = 0 <= 0` (True). Pick `'8'`.
- Final Output: `"1488"`.

---

### Case 2: Impossible Target (`num = "11111"`, `t = 26`)

- Factorize `t = 26`:
  - `26 / 2 = 13` -> `req2 = 1`.
  - `temp = 13` after checking factors 2, 3, 5, and 7.

| Step | Variable | Value | Action |
| :--- | :--- | :--- | :--- |
| 1 | `temp` | 13 | Factorization finishes with `temp > 1`. |
| 2 | `temp > 1` Check | True | `13` is a prime factor greater than 7. Return `"-1"`. |

- Final Output: `"-1"`.

---

## Time & Space Complexity

- **Time Complexity:** **O(n + log t)** where `n` is `num.length()` and `t` is the target value.
  - Factorizing `t` takes `O(log t)` time.
  - Building the DP table takes `O(60 * 40) = O(1)` constant time.
  - Finding the valid prefix takes at most `n` steps, with up to 9 digit iterations per step, doing `O(1)` DP lookups.
  - Constructing the final suffix takes `O(n)` steps.
  - Total time simplifies to **O(n)**.

- **Space Complexity:** **O(n)**
  - The DP table `dp[60][40]` uses fixed `O(1)` space.
  - The output string `ans` uses **O(n)** space to store the result.

### Is this optimal?
**Yes, this is optimal.** Any algorithm must at least inspect `num` (length `n`) and output a string of length `O(n)`, requiring at least `O(n)` time and space.

---

## Edge Cases Handled

1. **Target `t` contains prime factors > 7:** Prime factors like 11, 13, 17 cannot be formed by multiplying digits 1 through 9. Handled by returning `"-1"` when `temp > 1`.
2. **`num` contains zeros:** Zero digits cannot exist in the answer string. The variable `first_zero` limits the prefix search so no prefix containing or extending past a `'0'` is considered.
3. **`num` is already valid:** If `num` has no zeros and its digit product is already divisible by `t`, the code directly returns `num`.
4. **Answer must be longer than `num`:** If no prefix modification works within length `n`, the code seamlessly falls back to building a new string of length `M = max(n + 1, min_len_needed)`.
