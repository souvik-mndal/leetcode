![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.9%20MB%20(beats%2062.55%25)-green?style=for-the-badge)

---

## Problem Explained

The problem asks us to find how many **unique non-empty subsequences** can be formed from a given string `s`. 

A **subsequence** is a sequence created by deleting zero or more characters from `s` without changing the order of the remaining characters. For example, from `"abc"`, you can form `"a"`, `"ac"`, `"bc"`, etc., but not `"ca"` because the character order changed.

Multiple different deletions might produce the exact same sequence of letters. We must count each unique string only once. Because the answer can be very large, we must return the count modulo `10^9 + 7`.

---

## Intuition

Think about building subsequences step-by-step, character by character.

Start with just the empty string `""`. That is `1` subsequence.

If every character in `s` were unique (like `"abc"`):
* At each new character, every existing subsequence can either **not include** the new character or **append** it.
* This means adding a new character doubles the total number of subsequences.
* For `"abc"`:
  * Start: `1` (empty string `""`)
  * Add `'a'`: `1 * 2 = 2` (`""`, `"a"`)
  * Add `'b'`: `2 * 2 = 4` (`""`, `"a"`, `"b"`, `"ab"`)
  * Add `'c'`: `4 * 2 = 8` (`""`, `"a"`, `"b"`, `"ab"`, `"c"`, `"ac"`, `"bc"`, `"abc"`)

What happens when characters repeat (like `"aba"`)?
* When we see the second `'a'`, we double our current count of `4` to get `8`.
* But some newly created strings are duplicates!
* Specifically, any string formed by appending `'a'` to subsequences that already existed **before the first `'a'`** was added will be duplicates.
* Therefore, when appending a repeated character, we subtract the number of subsequences that existed right before its previous appearance.

The core formula at each step is:

```text
new_total = (2 * current_total) - total_before_this_character_last_appeared
```

At the very end, we subtract `1` to remove the empty string `""` because the problem asks only for **non-empty** subsequences.

---

## Approach

Here is how the code works step-by-step:

* `const long long MOD = 1000000007;`: Defines our modulo constant to keep numbers from overflowing standard integer bounds.
* `long long dp = 1;`: Sets `dp` to `1` initially to represent the single empty subsequence `""`.
* `vector<long long> last(26, 0);`: Creates an array of size 26 initialized to 0. It tracks the `dp` value right before each character ('a' through 'z') was last processed.
* `for (char c : s)`: Loops through each character in string `s` from left to right.
* `int index = c - 'a';`: Converts character `c` into an index from 0 to 25.
* `long long oldDp = dp;`: Saves the current value of `dp` before we modify it.
* `dp = (2 * dp - last[index] + MOD) % MOD;`: Doubles `dp` (adding `c` to all existing subsequences) and subtracts `last[index]` (removing duplicate subsequences created by previous occurrences of `c`). Adds `MOD` before taking the remainder to handle negative numbers safely.
* `last[index] = oldDp;`: Updates `last[index]` with `oldDp`, saving the snapshot of total subsequences present just before this instance of `c`.
* `return (dp - 1 + MOD) % MOD;`: Subtracts `1` from `dp` to exclude the empty subsequence `""` and returns the final answer modulo `10^9 + 7`.

---

## Dry Run

### Case 1: `s = "aba"` (Typical case with duplicates)

Initial State: `dp = 1`, `last = [0, 0, ..., 0]`

| Step | Character `c` | `index` | `oldDp` | Formula for `dp` | New `dp` | Updated `last[index]` | Action / Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | - | - | 1 | - | Represents `""` |
| 1 | `'a'` | 0 | 1 | `(2 * 1 - 0 + MOD) % MOD` | 2 | `last[0] = 1` | Subsequences: `""`, `"a"` |
| 2 | `'b'` | 1 | 2 | `(2 * 2 - 0 + MOD) % MOD` | 4 | `last[1] = 2` | Subsequences: `""`, `"a"`, `"b"`, `"ab"` |
| 3 | `'a'` | 0 | 4 | `(2 * 4 - 1 + MOD) % MOD` | 7 | `last[0] = 4` | Removes 1 duplicate (`"a"`). Total = 7 |

Final calculation: `return (7 - 1 + MOD) % MOD` -> **`6`**.

---

### Case 2: `s = "aaa"` (Edge case with all duplicate characters)

Initial State: `dp = 1`, `last = [0, 0, ..., 0]`

| Step | Character `c` | `index` | `oldDp` | Formula for `dp` | New `dp` | Updated `last[index]` | Action / Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | - | - | 1 | - | Represents `""` |
| 1 | `'a'` | 0 | 1 | `(2 * 1 - 0 + MOD) % MOD` | 2 | `last[0] = 1` | Subsequences: `""`, `"a"` |
| 2 | `'a'` | 0 | 2 | `(2 * 2 - 1 + MOD) % MOD` | 3 | `last[0] = 2` | Subsequences: `""`, `"a"`, `"aa"` |
| 3 | `'a'` | 0 | 3 | `(2 * 3 - 2 + MOD) % MOD` | 4 | `last[0] = 3` | Subsequences: `""`, `"a"`, `"aa"`, `"aaa"` |

Final calculation: `return (4 - 1 + MOD) % MOD` -> **`3`**.

---

## Time & Space Complexity

* **Time Complexity:** **O(N)** — where N is the length of string `s`. We loop through the string once. Inside the loop, all arithmetic operations and array lookups take O(1) constant time.
* **Space Complexity:** **O(1)** — the `last` array has a fixed size of 26 (one entry for each lowercase English letter), using a constant amount of memory regardless of string length.

### Is this optimal?

**Yes, this code is already fully optimal.**

* **Time:** We must read all N characters at least once to consider every character in the string, making O(N) time the absolute minimum possible.
* **Space:** We use O(1) extra space because 26 entries is constant. We cannot use less space while maintaining state for lowercase English alphabet characters.

---

## Edge Cases Handled

* **All characters identical (e.g., `"aaa"`):** Handled correctly. Every repetition subtracts the exact count of duplicates, growing the count by 1 at each step instead of doubling.
* **All unique characters (e.g., `"abc"`):** Handled correctly. `last[index]` remains 0 for all characters, resulting in standard doubling (`2^N - 1`).
* **Single character string (e.g., `"a"`):** Handled correctly. `dp` becomes `2`, final result returns `2 - 1 = 1`.
* **Negative modulo operations:** Standard `%` in C++ can yield negative results if the left operand is negative. The code safely adds `MOD` (`2 * dp - last[index] + MOD`) before taking modulo `% MOD`.
* **Large string length (up to 2000):** Avoids integer overflow by using `long long` for intermediate calculations and taking modulo `10^9 + 7` at every step.
