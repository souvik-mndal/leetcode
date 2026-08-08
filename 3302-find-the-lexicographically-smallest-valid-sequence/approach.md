![Runtime](https://img.shields.io/badge/Runtime-47%20ms%20(beats%2051.06%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-115.54%20MB%20(beats%2024.47%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given two strings, `s` (called `word1` in the problem description) and `t` (called `word2`).

You need to pick `m` indices from `s` in strictly increasing order, where `m` is the length of `t`. When you take the characters at these indices from `s`, they form a sub-sequence string. This sub-sequence string must be **almost equal** to `t`. 

**Almost equal** means the formed string can differ from `t` by **at most one character** (0 or 1 character can be changed).

If there are multiple valid sequences of indices, you must return the **lexicographically smallest** index list. This means you want the first index to be as small as possible; if there is a tie, the second index as small as possible, and so on.

If no valid sequence exists, return an empty array `[]`.

**Example:**
- `s = "vbcca"`, `t = "abc"`
- We pick indices `[0, 1, 2]`. 
- Characters in `s` at `[0, 1, 2]` are `'v'`, `'b'`, `'c'`.
- Change `'v'` to `'a'`. The string becomes `"abc"`, which matches `t`.
- Indices `[0, 1, 2]` are valid and smallest. Output: `[0, 1, 2]`.

---

## Intuition

To get the lexicographically smallest list of indices, we should be **greedy**. We want to process `s` from left to right (from index `0` to `n-1`) and pick the earliest possible index for each character in `t`.

When we are at index `i` in `s` trying to match character `t[j]`:
1. If `s[i] == t[j]`, we should take index `i` immediately.
2. If `s[i] != t[j]`, we could choose to use our single allowed mismatch here (change `s[i]` to `t[j]`). 

**The key question:** How do we know if using our only mismatch right now is safe?

It is only safe if the remaining suffix of `s` (from index `i + 1` to `n - 1`) can match the rest of `t` (which has `m - j - 1` characters left) **with zero mismatches**.

To answer this question instantly during our forward pass, we can precompute a `dp` array backwards from right to left. `dp[i]` tells us how many characters from the suffix of `t` can be matched as a subsequence inside `s[i ... n-1]`.

If `dp[i + 1]` is greater than or equal to the number of remaining characters needed, we know we can safely use our mismatch at index `i`.

---

## Approach

Here is how the code works step-by-step:

- `int n = s.length(); int m = t.length();`: Store the lengths of `s` and `t`.
- `int i = n - 1, j = m - 1;`: Initialize pointers to start matching from the end of both strings.
- `vector<int> dp(n + 1, 0);`: Create a DP array of size `n + 1` filled with zeros. `dp[i]` stores the max length of `t`'s suffix that matches a subsequence in `s[i ... n-1]`.
- `for(; i >= 0; i--) { if(j >= 0 && s[i] == t[j]) { dp[i] = dp[i + 1] + 1; j--; } else dp[i] = dp[i + 1]; }`: Walk backwards through `s`. If `s[i]` matches `t[j]`, increment the match count and move `j` left. Otherwise, carry over `dp[i + 1]`.
- `vector<int> ans;`: Create an empty vector to store our resulting indices.
- `j = 0;`: Reset pointer `j` to 0 to match `t` from left to right.
- `bool flag = true;`: Tracks whether we still have our 1 mismatch available (`true` means available).
- `for(int i = 0; i < n && j < m; i++)`: Iterate forward through `s` index by index.
- `int rem = m - j - 1;`: Calculate how many characters in `t` must still be matched after current index `i`.
- `if(s[i] == t[j]) { ans.push_back(i); j++; }`: If characters match naturally, greedily take index `i` and advance `j`.
- `else if(flag && dp[i + 1] >= rem) { ans.push_back(i); j++; flag = false; }`: If they do not match, but we still have our wildcard (`flag` is true) and the remaining suffix can complete `t` (`dp[i + 1] >= rem`), we use our wildcard on index `i`, advance `j`, and set `flag = false`.
- `if (j == m) return ans; return {};`: If all `m` characters of `t` were matched, return `ans`. Otherwise, return `{}`.

---

## Dry Run

### Case 1: `s = "vbcca"`, `t = "abc"`

`n = 5`, `m = 3`.

**Backward Pass (Filling `dp`):**

| `i` | `s[i]` | `j` | `t[j]` | Match? | Action | `dp[i]` |
|---|---|---|---|---|---|---|
| 4 | `'a'` | 2 | `'c'` | No | `dp[4] = dp[5] = 0` | 0 |
| 3 | `'c'` | 2 | `'c'` | Yes | `dp[3] = dp[4] + 1 = 1`, `j` becomes 1 | 1 |
| 2 | `'c'` | 1 | `'b'` | No | `dp[2] = dp[3] = 1` | 1 |
| 1 | `'b'` | 1 | `'b'` | Yes | `dp[1] = dp[2] + 1 = 2`, `j` becomes 0 | 2 |
| 0 | `'v'` | 0 | `'a'` | No | `dp[0] = dp[1] = 2` | 2 |

**Forward Pass (Building `ans`):**
Initial state: `j = 0`, `flag = true`, `ans = []`.

| `i` | `s[i]` | `j` | `t[j]` | `rem` | Condition Met | Action | `ans` | `flag` |
|---|---|---|---|---|---|---|---|---|
| 0 | `'v'` | 0 | `'a'` | 2 | `flag && dp[1] (2) >= rem (2)` | Use wildcard at `i=0`, `j++` | `[0]` | `false` |
| 1 | `'b'` | 1 | `'b'` | 1 | Exact match `s[1] == t[1]` | Pick `i=1`, `j++` | `[0, 1]` | `false` |
| 2 | `'c'` | 2 | `'c'` | 0 | Exact match `s[2] == t[2]` | Pick `i=2`, `j++` | `[0, 1, 2]` | `false` |

`j` reached `3` (`m`). Return `[0, 1, 2]`.

---

### Case 2: `s = "bacdc"`, `t = "abc"`

`n = 5`, `m = 3`.

**Backward Pass (Filling `dp`):**

| `i` | `s[i]` | `j` | `t[j]` | Match? | Action | `dp[i]` |
|---|---|---|---|---|---|---|
| 4 | `'c'` | 2 | `'c'` | Yes | `dp[4] = dp[5] + 1 = 1`, `j` becomes 1 | 1 |
| 3 | `'d'` | 1 | `'b'` | No | `dp[3] = dp[4] = 1` | 1 |
| 2 | `'c'` | 1 | `'b'` | No | `dp[2] = dp[3] = 1` | 1 |
| 1 | `'a'` | 1 | `'b'` | No | `dp[1] = dp[2] = 1` | 1 |
| 0 | `'b'` | 1 | `'b'` | Yes | `dp[0] = dp[1] + 1 = 2`, `j` becomes 0 | 2 |

**Forward Pass (Building `ans`):**
Initial state: `j = 0`, `flag = true`, `ans = []`.

| `i` | `s[i]` | `j` | `t[j]` | `rem` | Condition Met | Action | `ans` | `flag` |
|---|---|---|---|---|---|---|---|---|
| 0 | `'b'` | 0 | `'a'` | 2 | `dp[1] (1) < rem (2)` | Mismatch unsafe, skip | `[]` | `true` |
| 1 | `'a'` | 0 | `'a'` | 2 | Exact match `s[1] == t[0]` | Pick `i=1`, `j++` | `[1]` | `true` |
| 2 | `'c'` | 1 | `'b'` | 1 | `flag && dp[3] (1) >= rem (1)` | Use wildcard at `i=2`, `j++` | `[1, 2]` | `false` |
| 3 | `'d'` | 2 | `'c'` | 0 | `s[3] != t[2]`, `flag` false | Skip | `[1, 2]` | `false` |
| 4 | `'c'` | 2 | `'c'` | 0 | Exact match `s[4] == t[2]` | Pick `i=4`, `j++` | `[1, 2, 4]` | `false` |

`j` reached `3` (`m`). Return `[1, 2, 4]`.

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — where `n` is the length of string `s`. We perform one pass backward through `s` to compute `dp`, and one pass forward through `s` to construct `ans`.
- **Space Complexity:** **O(n)** — We allocate a single DP vector of size `n + 1` to store suffix match counts.

### Is this optimal?

Yes, this is already the **most optimal possible complexity** for this problem.

- **Time:** We must examine the characters in `s` to find matches, so reading the input takes at least O(n) time.
- **Space:** During the forward pass, we need instant O(1) lookups for suffix match lengths at arbitrary positions (`dp[i + 1]`). Thus, storing the precomputed suffix information takes O(n) auxiliary space.

---

## Edge Cases Handled

- **No valid sequence exists:** If `s` cannot form `t` even with 1 mismatch (e.g., `s = "aaaaaa"`, `t = "aaabc"`), `j` will not reach `m`, and the code safely returns `{}`.
- **0 mismatches needed:** If `s` contains `t` directly as a sub-sequence, `flag` remains `true` throughout, and the code picks exact matches greedily.
- **Multiple possible smallest index sequences:** Because the loop runs forward from `i = 0` to `n - 1`, it guarantees picking the smallest available index at every step.
- **Mismatch used at index `0`:** Handled cleanly because `dp[1]` is checked to ensure remaining characters can be matched.
- **Large input constraints:** Constraints go up to `3 * 10^5`. Linear O(n) time avoids Time Limit Exceeded (TLE) errors.
