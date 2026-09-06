![Runtime](https://img.shields.io/badge/Runtime-47%20ms%20(beats%2047.64%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-115.5%20MB%20(beats%2028.45%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given two strings, `s` and `t`. You need to pick a list of character positions (indices) from `s` that form a string almost identical to `t`.

"Almost identical" means the string formed by your chosen indices can differ from `t` by **at most one character**.

Your goal is to find the **lexicographically smallest** list of indices. That means when comparing index lists from left to right, we always prefer smaller index numbers as early as possible.

- The chosen indices must strictly increase.
- The length of the index list must match the length of `t`.
- If no valid list of indices exists, return an empty list `{}`.

### Example
For `s = "vbcca"` and `t = "abc"`:
If we pick indices `[0, 1, 2]`, the characters in `s` are `'v'`, `'b'`, and `'c'`.
Comparing `"vbc"` to `"abc"`: only the first letter differs (`'v'` vs `'a'`). This is allowed because at most one character is different.
Since `[0, 1, 2]` uses the smallest indices possible, `[0, 1, 2]` is our answer.

---

## Intuition

To get the lexicographically smallest list of indices, we should process `s` from left to right. We want to pick indices for `ans` as early as possible.

When looking at position `i` in `s` to match position `j` in `t`:
1. If `s[i] == t[j]`, taking `i` right now is always optimal. It gives the smallest index for `ans[j]` and saves our 1 allowed character replacement for later.
2. If `s[i] != t[j]`, we could potentially use our 1 allowed character replacement right here. But can we afford to spend it?

We can only spend our 1 replacement at index `i` if the remainder of `s` (from `i + 1` to the end) can match all remaining characters of `t` **with zero mismatches**.

To know this instantly during a forward pass, we precalculate suffix matches using a backward pass. A table `dp` stores how many characters from the tail end of `t` can be matched exact-for-exact starting from index `i` in `s`.

---

## Approach

- `int n = s.length(); int m = t.length();`: Store the lengths of string `s` and string `t`.
- `vector<int> dp(n + 1, 0);`: Create a lookup array `dp` of size `n + 1` filled with zeros. `dp[i]` tells us how many characters from the end of `t` can be matched perfectly using characters from `s[i]` onwards.
- `for(; i >= 0; i--)`: Loop backward through `s` (from `n - 1` down to `0`) and `t` (from `m - 1` down to `0`).
- `if(j >= 0 && s[i] == t[j]) { dp[i] = dp[i + 1] + 1; j--; } else dp[i] = dp[i + 1];`: If characters match, increment `dp[i]` compared to `dp[i + 1]` and move `j` left. Otherwise, copy `dp[i + 1]` into `dp[i]`.
- `bool flag = true;`: Initialize a boolean flag to `true`. This tracks if our 1 allowed character mismatch is still available.
- `for(int i = 0; i < n && j < m; i++)`: Loop forward through `s` to greedily pick the smallest valid indices for each position `j` of `t`.
- `int rem = m - j - 1;`: Calculate how many characters in `t` still need to be matched *after* position `j`.
- `if( s[i] == t[j]) { ans.push_back(i); j++; }`: If characters match exactly, append index `i` to `ans` and advance `j`.
- `else if(flag && dp[i + 1] >= rem) { ans.push_back(i); j++; flag = false; }`: If characters do not match, check if we still have our wildcard (`flag`) and if `dp[i + 1] >= rem`. If both are true, spend our wildcard here, add `i` to `ans`, advance `j`, and set `flag = false`.
- `if (j == m) return ans; return {};`: If all `m` characters of `t` were successfully matched, return `ans`. Otherwise, return an empty array `{}`.

---

## Dry Run

### Case 1: Standard match with 1 mismatch used
`s = "vbcca"`, `t = "abc"` (`n = 5`, `m = 3`)

**Backward Pass (`dp` construction):**
- Starts with `j = 2` (matching `'c'`).
- `i = 4 ('a')`: mismatch with `'c'` -> `dp[4] = 0`
- `i = 3 ('c')`: matches `t[2] ('c')` -> `dp[3] = 1`, move `j` to `1`
- `i = 2 ('c')`: mismatch with `t[1] ('b')` -> `dp[2] = 1`
- `i = 1 ('b')`: matches `t[1] ('b')` -> `dp[1] = 2`, move `j` to `0`
- `i = 0 ('v')`: mismatch with `t[0] ('a')` -> `dp[0] = 2`

Resulting `dp` array: `[2, 2, 1, 1, 0, 0]`

**Forward Pass:**

| `i` | `s[i]` | `j` | `t[j]` | `flag` | `rem` | `dp[i + 1]` | Action | `ans` |
|---|---|---|---|---|---|---|---|---|
| 0 | 'v' | 0 | 'a' | true | 2 | `dp[1] = 2` | Mismatch allowed (`2 >= 2`). Take `i=0`, set `flag=false`. | `[0]` |
| 1 | 'b' | 1 | 'b' | false | 1 | - | Exact match. Take `i=1`. | `[0, 1]` |
| 2 | 'c' | 2 | 'c' | false | 0 | - | Exact match. Take `i=2`. | `[0, 1, 2]` |

`j == 3 == m`. Output: `[0, 1, 2]`.

---

### Case 2: Impossible match
`s = "aaaaaa"`, `t = "aaabc"` (`n = 6`, `m = 5`)

**Backward Pass (`dp` construction):**
- `s` only contains `'a'`, but `t` ends with `'b'` and `'c'`.
- No suffix matches are possible, so `dp` array is `[0, 0, 0, 0, 0, 0, 0]`.

**Forward Pass:**

| `i` | `s[i]` | `j` | `t[j]` | `flag` | `rem` | `dp[i + 1]` | Action | `ans` |
|---|---|---|---|---|---|---|---|---|
| 0 | 'a' | 0 | 'a' | true | 3 | - | Exact match. Take `i=0`. | `[0]` |
| 1 | 'a' | 1 | 'a' | true | 2 | - | Exact match. Take `i=1`. | `[0, 1]` |
| 2 | 'a' | 2 | 'a' | true | 1 | - | Exact match. Take `i=2`. | `[0, 1, 2]` |
| 3 | 'a' | 3 | 'b' | true | 1 | `dp[4] = 0` | Mismatch fails (`0 < 1`). Skip `i=3`. | `[0, 1, 2]` |
| 4 | 'a' | 3 | 'b' | true | 1 | `dp[5] = 0` | Mismatch fails (`0 < 1`). Skip `i=4`. | `[0, 1, 2]` |
| 5 | 'a' | 3 | 'b' | true | 1 | `dp[6] = 0` | Mismatch fails (`0 < 1`). Skip `i=5`. | `[0, 1, 2]` |

`j = 3 != 5`. Output: `{}`.

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — We make one backward pass over string `s` of length `n`, and one forward pass over string `s` of length `n`. Every character is processed in constant time O(1).
- **Space Complexity:** **O(n)** — We store an integer array `dp` of size `n + 1`.

### Is this optimal?
**Yes, this is already optimal.** 

To answer the problem, we must inspect the input strings, which takes at least O(n) time. Because our forward greedy search needs to check suffix matching capabilities computed from right to left, storing these suffix states in an O(n) array is necessary. Neither time nor space complexity can be reduced further in terms of Big-O bounds.

---

## Edge Cases Handled

- **No valid sequence possible:** If `s` cannot match `t` even with one character changed, `j` will not reach `m` by the end of the loop, and the code safely returns `{}`.
- **Exact match with no changes needed:** If `s` contains `t` as an exact subsequence, `flag` remains `true` throughout, and exact matches are picked greedily.
- **Mismatch used at index 0:** If the very first character of `s` must be changed, `dp[1] >= m - 1` checks if the rest of `s` can match the remainder of `t`. If true, index 0 is selected.
- **Mismatch used at the very last character of `t`:** When `j = m - 1`, `rem = 0`. Since `dp[i + 1] >= 0` is always true, the wildcard can safely be used on the final character if needed.
- **Multiple valid options for mismatch:** The forward pass iterates left-to-right and takes the first index where `dp[i + 1] >= rem` holds. This guarantees the index chosen for the mismatch is as small as possible.
