![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2026.70%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.61%20MB%20(beats%2082.87%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a binary string `s` (a string containing only `'0'`s and `'1'`s) and a target integer `k`. 

A contiguous part of this string (a substring) is called **beautiful** if it contains exactly `k` ones. 

Your goal is to find the **shortest** beautiful substring. If there are multiple beautiful substrings that share this same shortest length, pick the one that is **lexicographically smallest** (comes first in alphabetical order). If no substring has `k` ones, return an empty string `""`.

For example, if `s = "100011001"` and `k = 3`:
- Beautiful substrings with 3 ones exist with lengths ranging from 5 to 9.
- The shortest length with 3 ones is 5.
- The beautiful substrings of length 5 are `"10001"` and `"11001"`.
- Alphabetically, `"11001"` is smaller than `"10001"` because `'1'` comes after `'0'`, wait — actually `"10001"` vs `"11001"`: at index 1, `'0'` is smaller than `'1'`. But wait, in the given example, `"11001"` has length 5 and starts at index 4 (`s[4..8] = "11001"`). `"10001"` is at `s[0..4]`, which also has length 5. Between `"10001"` and `"11001"`, `"10001"` starts with `"10"`, while `"11001"` starts with `"11"`. The problem output for this example is `"11001"` because `"10001"` has 2 ones (`s[0..4]` is `"10001"`, which has only 2 ones!). The beautiful ones of length 5 are `"10001"`? No, `s[0..4]` is `"10001"` which has 2 ones. `s[4..8]` is `"11001"` which has 3 ones. So `"11001"` is the only beautiful substring of length 5!

---

## Intuition

The core trick here is using a **128-bit integer** (`__uint128_t`) to represent the sliding window instead of using standard string manipulation.

Binary strings with a leading `'1'` have two special math properties:
1. **Length comparison:** A shorter binary number is always numerically smaller than a longer binary number. For instance, `"1111"` (15 in decimal) is smaller than `"10000"` (16 in decimal).
2. **Alphabetical comparison:** Between two binary strings of the *same* length, alphabetical order is identical to numeric order. For instance, `"1001"` (9 in decimal) is smaller than `"1100"` (12 in decimal).

Because any minimal beautiful substring must start with `'1'` (a leading `'0'` could be removed to make it shorter), every active window starts with a `'1'`. 

Therefore, comparing the numeric integer values of two valid windows (`curr < best`) automatically handles both rules at once: it favors shorter substrings first, and breaks ties alphabetically!

---

## Approach

Here is how the algorithm processes the string character by character:

- `int n = s.size(), start = n;`: Store the length of the string `n`. Set `start` to `n` as a default (if no solution is found, `s.substr(n)` returns an empty string).
- `__uint128_t curr = 0, best = -1;`: Initialize `curr` to 0 to store the binary mask of our current window. Set `best` to the maximum possible 128-bit integer (`-1` in unsigned representation).
- `for (int i = 0; i < n; i++)`: Iterate through each character of the string `s`.
- `curr = (curr << 1) | (s[i] & 1);`: Shift `curr` left by 1 bit and append the current bit (`0` or `1`).
- `k -= s[i] & 1;`: Subtract 1 from `k` whenever we see a `'1'`.
- `if (k < 0)`: If `k` drops below 0, our window contains `k + 1` ones. We must trim the leftmost `'1'`.
- `k = 0, curr &= ((__uint128_t)1 << (bit_width(curr) - 1)) - 1;`: Reset `k` to 0 and remove the most significant bit (the leading `'1'`) from `curr` using a bitmask.
- `if (k == 0 && curr < best)`: When the window contains exactly `k` ones, check if `curr` is numerically smaller than `best`.
- `start = i - bit_width(curr) + 1, best = curr;`: If `curr` is smaller, update `best` to `curr` and record the starting index of this window.
- `return move(s).substr(start, bit_width(best));`: Extract and return the substring corresponding to `best` starting at `start`.

---

## Dry Run

### Case 1: Typical case with multiple candidates
**Input:** `s = "1011"`, `k = 2`

| `i` | `s[i]` | `curr` (binary) | `k` | `best` (binary) | `start` | Action |
|---|---|---|---|---|---|---|
| Initial | - | `0` | `2` | `-1` | `4` | Variables initialized. |
| `0` | `'1'` | `1` | `1` | `-1` | `4` | Append `'1'`. `k` becomes 1. |
| `1` | `'0'` | `10` | `1` | `-1` | `4` | Append `'0'`. `k` is still 1. |
| `2` | `'1'` | `101` | `0` | `101` | `0` | `k` reaches 0. `curr` (5) < `best`. Set `best = 101`, `start = 0`. |
| `3` | `'1'` | `1011` -> `11` | `0` | `11` | `2` | `k` drops to -1. Trim leading bit to get `11`. `curr` (3) < `best` (5). Update `best = 11`, `start = 2`. |

**Output:** `s.substr(2, 2)` = `"11"`.

---

### Case 2: String with no valid answer
**Input:** `s = "000"`, `k = 1`

| `i` | `s[i]` | `curr` (binary) | `k` | `best` | `start` | Action |
|---|---|---|---|---|---|---|
| Initial | - | `0` | `1` | `-1` | `3` | Variables initialized. |
| `0` | `'0'` | `0` | `1` | `-1` | `3` | Append `'0'`. `k` is 1. |
| `1` | `'0'` | `0` | `1` | `-1` | `3` | Append `'0'`. `k` is 1. |
| `2` | `'0'` | `0` | `1` | `-1` | `3` | Append `'0'`. `k` is 1. |

**Output:** `s.substr(3, 128)` = `""` (empty string, as `start` remained `3`).

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — We iterate through the string of length `n` once. Inside the loop, bitwise shifts, bit masking, and bit-width checks run in O(1) constant time.
- **Space Complexity:** **O(1)** extra space — The algorithm uses a few 128-bit primitive variables (`curr`, `best`) and integer counters. No dynamic data structures are allocated.

### Is this solution optimal?

**Yes**, O(n) time and O(1) space is the theoretical optimal bound for this problem because we must inspect every character at least once.

However, using `__uint128_t` restricts this implementation to strings of length `n <= 128` (the problem constraint states `n <= 100`, so it passes). 

If string length `n` were up to `100,000`, a bit integer would overflow. In that scenario, a standard **two-pointer sliding window** using string references would be used:

1. Maintain two pointers `left` and `right` to track the window boundaries.
2. Maintain `ones_count` to count `'1'`s.
3. Whenever `ones_count == k`, shrink from the `left` until the leading bit is `'1'`.
4. Compare window length against `min_len`. If shorter, update the result. If equal length, compare string content using standard string comparison.

Here is how the standard two-pointer sliding window logic looks:

```cpp
string shortestBeautifulSubstring(string s, int k) {
    int n = s.size();
    int left = 0, ones = 0;
    string best = "";

    for (int right = 0; right < n; right++) {
        if (s[right] == '1') ones++;

        while (ones == k) {
            // Trim leading zeros to make window minimal
            while (s[left] == '0') left++;
            
            string candidate = s.substr(left, right - left + 1);
            if (best.empty() || candidate.length() < best.length() || 
               (candidate.length() == best.length() && candidate < best)) {
                best = candidate;
            }
            
            // Move left past the first '1' to search for next window
            ones--;
            left++;
        }
    }
    return best;
}
```

- **Two-pointer approach complexity:** **O(n^2)** worst-case time due to string slicing/comparisons, or **O(n)** if using string views. Space is **O(n)** to store the result string.

---

## Edge Cases Handled

- **`k` larger than total `'1'`s in `s`:** `k` never reaches 0. `best` remains `-1`, and `start` remains `n`, safely returning `""`.
- **String with only `'0'`s:** `k` is never decremented, returning `""`.
- **`k = 1`:** Correctly identifies single `'1'` characters and picks the first `'1'` present (since all single `'1'`s have equal length 1 and equal value).
- **Multiple equal-length answers:** Automatically selects the lexicographically smaller string because smaller numeric binary value corresponds to alphabetically smaller binary text.
