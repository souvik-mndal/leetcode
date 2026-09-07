![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.91%20MB%20(beats%2062.00%25)-green?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the total number of **distinct non-empty subsequences** of a given string `s`. 

A **subsequence** is a sequence that can be derived from the string by deleting zero or more characters without changing the order of the remaining characters. For example, from the string `"abc"`, `"ac"` is a subsequence because you can delete `"b"`, but `"ca"` is not because the characters are out of order.

When a string contains repeated letters, deleting different positions can produce the exact same text. For example, in `"aba"`, deleting the first `'a'` leaves `"ba"`, and deleting the last `'a'` leaves `"ab"`. But picking just one `'a'` could come from either the first or the last position. We must count each unique string outcome only once.

Because the final count can be huge, we return the answer modulo `10^9 + 7` (1,000,000,007).

---

## Intuition

Imagine building subsequences one letter at a time from left to right.

If we include the empty string `""` as a starting point:
1. Before seeing a new letter `c`, let `dp` be the number of distinct subsequences we have built so far.
2. When letter `c` arrives, we can either keep all existing subsequences as they are, or create new ones by appending `c` to the end of every existing subsequence.
3. If `c` is completely new, appending `c` doubles our count, giving `dp * 2`.

**The Aha Moment:**
What if we have already seen letter `c` earlier in the string? 

Any subsequence created by appending `c` this time that was **already** created during `c`'s previous appearance will be a duplicate.

To avoid double-counting, we subtract the number of subsequences that existed right before `c` appeared the last time. We record that snapshot in a hash map called `last`.

The formula for each letter becomes:
`new_dp = (dp * 2) - previous_count_for_c`

After processing all characters, we subtract 1 from `dp` to remove the empty string `""` that we used as our base.

---

## Approach

Here is the code for the solution:

```python
dp = 1
last = {}
for c in s:
    dp, last[c] = (dp * 2 - last.get(c, 0)) % (10**9 + 7), dp
return (dp - 1) % (10**9 + 7)
```

Here is how the logic works step-by-step:

* `dp = 1`: Initialize `dp` to 1. This represents the single empty string `""` as our base starting count.
* `last = {}`: Create a hash map named `last` to track the snapshot of `dp` the last time each character was processed.
* `for c in s:`: Iterate through each character `c` in the input string `s` from left to right.
* `dp, last[c] = (dp * 2 - last.get(c, 0)) % (10**9 + 7), dp`: Double `dp` to account for appending `c` to all existing subsequences. Subtract `last.get(c, 0)` to remove duplicate subsequences formed by `c`'s previous appearance. Apply the modulo `10^9 + 7`, and save the old `dp` value into `last[c]` for future duplicate checks.
* `return (dp - 1) % (10**9 + 7)`: Subtract 1 to exclude the empty string, apply modulo to keep the answer non-negative, and return the result.

---

## Dry Run

Let's trace the execution step-by-step on two example inputs using the code's real variables: `c`, `dp`, and `last`.

### Case 1: Typical case with repeated characters (`s = "aba"`)

Initial state: `dp = 1`, `last = {}`

| Step | `c` | `last.get(c, 0)` | `dp` calculation | New `dp` | `last[c]` updated to | Action / Plain English |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `'a'` | 0 | `1 * 2 - 0 = 2` | 2 | 1 | Appended `'a'`. Subsequences: `""`, `"a"`. |
| 2 | `'b'` | 0 | `2 * 2 - 0 = 4` | 4 | 2 | Appended `'b'`. Subsequences: `""`, `"a"`, `"b"`, `"ab"`. |
| 3 | `'a'` | 1 | `4 * 2 - 1 = 7` | 7 | 4 | Appended `'a'`. Subtracted duplicate `"a"` (snapshot was 1). Subsequences: `""`, `"a"`, `"b"`, `"ab"`, `"aa"`, `"ba"`, `"aba"`. |

Final step: `(dp - 1) % (10^9 + 7)` = `(7 - 1) % 1000000007` = **6**.

---

### Case 2: Edge case with all identical characters (`s = "aaa"`)

Initial state: `dp = 1`, `last = {}`

| Step | `c` | `last.get(c, 0)` | `dp` calculation | New `dp` | `last[c]` updated to | Action / Plain English |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `'a'` | 0 | `1 * 2 - 0 = 2` | 2 | 1 | First `'a'`. Subsequences: `""`, `"a"`. |
| 2 | `'a'` | 1 | `2 * 2 - 1 = 3` | 3 | 2 | Second `'a'`. Subtracted 1 duplicate (`""` + `'a'`). Subsequences: `""`, `"a"`, `"aa"`. |
| 3 | `'a'` | 2 | `3 * 2 - 2 = 4` | 4 | 3 | Third `'a'`. Subtracted 2 duplicates. Subsequences: `""`, `"a"`, `"aa"`, `"aaa"`. |

Final step: `(dp - 1) % (10^9 + 7)` = `(4 - 1) % 1000000007` = **3**.

---

## Time & Space Complexity

* **Time Complexity:** **O(N)** — where N is the length of string `s`. We loop through the string once. Each dictionary lookup and basic arithmetic update takes constant time O(1).
* **Space Complexity:** **O(1)** — the hash map `last` stores at most 26 key-value pairs (one for each lowercase English letter). Since the alphabet size is fixed, space usage does not grow with the input length.

**Is this optimal?**
Yes, this solution is already optimal. We must inspect every character in `s` at least once to determine the distinct subsequences, which requires at least O(N) time. The O(1) space complexity uses a fixed set of 26 keys. Neither time nor space complexity can be improved further.

---

## Edge Cases Handled

* **All identical characters (e.g., `s = "aaa"`):** The algorithm correctly subtracts the growing overlap at each step, yielding 3 unique subsequences (`"a"`, `"aa"`, `"aaa"`).
* **All unique characters (e.g., `s = "abc"`):** Since `last.get(c, 0)` always returns 0 for new characters, `dp` doubles at every step, yielding `2^N - 1` unique subsequences.
* **Single character string (e.g., `s = "a"`):** The loop runs once, `dp` becomes 2, and returning `dp - 1` correctly gives `1`.
* **Large string lengths (up to 2000 characters):** The modulo operations inside the loop prevent intermediate integer overflow and keep calculations fast.
