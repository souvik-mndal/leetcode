![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.8%20MB%20(beats%2043.42%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to take a string `s` and create a shorter string (a **subsequence**) that meets three strict rules:

1. **All distinct characters:** It must contain every unique letter present in `s` exactly once.
2. **Preserve relative order:** The letters must appear in the same left-to-right relative order as they originally appeared in `s`.
3. **Lexicographically smallest:** Among all valid strings you could form, the output must come first alphabetically (dictionary order). For example, `"abc"` is smaller than `"bca"`.

### Example
If `s = "bcabc"`:
- The distinct characters in `s` are `'a'`, `'b'`, and `'c'`.
- Valid combinations that keep the relative order and contain all three characters are `"bca"`, `"bac"`, and `"abc"`.
- The alphabetically smallest one is `"abc"`.

---

## Intuition

Think of building the answer letter by letter inside a stack. As we walk through the string `s`, we want to be **greedy** — we always want smaller letters (like `'a'`) to appear as early as possible.

When we consider adding a new letter `c` to our result:
- If `c` is already in our result, we ignore it because we only want one copy of each letter.
- If `c` is smaller than the letter at the end of our current result string, we would love to delete that bigger letter and put `c` there instead.
- **The catch:** We can only delete the bigger letter if it appears again later in `s`. If that bigger letter never shows up again, deleting it would mean losing it forever, which breaks rule 1.

So the "aha!" moment is: keep track of how many times each letter remains in `s`. Whenever we see a smaller letter, pop off larger letters from the end of our result as long as those larger letters are guaranteed to appear again later.

---

## Approach

Here is the exact step-by-step breakdown of how the code implements this logic:

- `unordered_map<char, int> mp;`: Creates a frequency map to count how many times each character appears in `s`.
- `vector<bool> inAns(26, false);`: Creates a boolean array of size 26 (one for each lowercase letter) to track whether a letter is currently placed inside our output string.
- `for (char c : s) { mp[c]++; }`: Loops through `s` once to count the total occurrences of every character.
- `string sol;`: Initializes a string that acts as our stack to build the final result character by character.
- `for (char c : s)`: Starts a second pass through `s`, examining each character one by one.
- `while (!inAns[c - 'a'] && !sol.empty() && sol.back() > c && mp[sol.back()] > 0)`: Checks four conditions before deciding to remove the last character of `sol`:
  1. `!inAns[c - 'a']`: The current character `c` is not already part of our result.
  2. `!sol.empty()`: There is at least one character in our result to compare against.
  3. `sol.back() > c`: The last character in `sol` is alphabetically larger than `c`.
  4. `mp[sol.back()] > 0`: The last character in `sol` appears again later in the string.
- `inAns[sol.back() - 'a'] = false;`: Marks the character being removed as no longer present in `sol`.
- `sol.pop_back();`: Removes the last character from `sol`.
- `mp[c]--;`: Decrements the remaining count of character `c` because we have passed this occurrence.
- `if (!inAns[c - 'a'])`: Checks if character `c` is not yet in `sol`.
- `inAns[c - 'a'] = true;`: Marks character `c` as included in `sol`.
- `sol.push_back(c);`: Appends character `c` to the end of `sol`.
- `return sol;`: Returns the constructed string after processing all characters.

---

## Dry Run

### Case 1: Standard case with repeated characters (`s = "bcabc"`)

Initial counts in `mp`: `b: 2`, `c: 2`, `a: 1`.

| Current `c` | `sol` (Start of Step) | Action Taken | `mp[c]` (After Dec) | `sol` (End of Step) | `inAns` state |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `'b'` | `""` | Push `'b'` | `1` | `"b"` | `b: true` |
| `'c'` | `"b"` | `'b' < 'c'`, push `'c'` | `1` | `"bc"` | `b: true, c: true` |
| `'a'` | `"bc"` | `'c' > 'a'` & `mp['c'] > 0` -> pop `'c'`. `'b' > 'a'` & `mp['b'] > 0` -> pop `'b'`. Push `'a'`. | `0` | `"a"` | `a: true, b: false, c: false` |
| `'b'` | `"a"` | `'a' < 'b'`, push `'b'` | `0` | `"ab"` | `a: true, b: true` |
| `'c'` | `"ab"` | `'b' < 'c'`, push `'c'` | `0` | `"abc"` | `a: true, b: true, c: true` |

**Final Output:** `"abc"`

---

### Case 2: Trapping larger characters that cannot be removed (`s = "cbacdcbc"`)

Initial counts in `mp`: `a: 1`, `b: 2`, `c: 4`, `d: 1`.

| Current `c` | `sol` (Start of Step) | Action Taken | `mp[c]` (After Dec) | `sol` (End of Step) | `inAns` state |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `'c'` | `""` | Push `'c'` | `3` | `"c"` | `c: true` |
| `'b'` | `"c"` | `'c' > 'b'` & `mp['c'] = 3 > 0` -> pop `'c'`. Push `'b'`. | `1` | `"b"` | `b: true, c: false` |
| `'a'` | `"b"` | `'b' > 'a'` & `mp['b'] = 1 > 0` -> pop `'b'`. Push `'a'`. | `0` | `"a"` | `a: true, b: false` |
| `'c'` | `"a"` | Push `'c'` | `2` | `"ac"` | `a: true, c: true` |
| `'d'` | `"ac"` | Push `'d'` | `0` | `"acd"` | `a: true, c: true, d: true` |
| `'c'` | `"acd"` | Already in `sol`, skip insertion | `1` | `"acd"` | `a: true, c: true, d: true` |
| `'b'` | `"acd"` | `'d' > 'b'` BUT `mp['d'] = 0` (stop loop). Push `'b'`. | `0` | `"acdb"` | `a: true, b: true, c: true, d: true` |
| `'c'` | `"acdb"` | Already in `sol`, skip insertion | `0` | `"acdb"` | `a: true, b: true, c: true, d: true` |

**Final Output:** `"acdb"`

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where N is the length of the string `s`.
  - Counting frequency takes O(N) time.
  - The second loop iterates N times. Inside the loop, each character is pushed to `sol` at most once and popped at most once across the entire run. Thus, the inner `while` loop runs at most N total times overall.
- **Space Complexity:** **O(1)** auxiliary space (or O(K) where K is the alphabet size, K = 26).
  - `inAns` uses a fixed size of 26.
  - `sol` holds at most 26 unique characters.
  - `mp` holds at most 26 unique keys.

### Can it be improved?

The time and space complexity are already asymptotically optimal (O(N) time and O(1) space). You must inspect every character at least once, so faster than O(N) is impossible.

However, we can improve **runtime performance** by replacing `unordered_map<char, int>` with a plain fixed-size array `vector<int> count(26, 0)` or `int count[26] = {0}`. 

`unordered_map` incurs memory hashing overhead, dynamic node allocations, and cache misses. A simple array lookup operates directly in fast CPU cache memory with zero hash calculation overhead.

```cpp
// Optimization: Replace unordered_map with a simple array
int mp[26] = {0};

for (char c : s) {
    mp[c - 'a']++;
}

// Inside the loop, replace mp[sol.back()] with mp[sol.back() - 'a']
```

- **Improved Complexity:** Still **O(N)** time and **O(1)** space, but with lower constant factors and faster execution.
- **Theoretical Best:** O(N) time and O(1) space. The optimized version reaches this theoretical limit.

---

## Edge Cases Handled

- **All Identical Characters (`s = "aaaa"`):** The count of `'a'` is 4. The first `'a'` is pushed, and all remaining copies are skipped because `inAns['a']` is true. Returns `"a"`.
- **Already Sorted Input (`s = "abc"`):** Characters are added one by one. The `while` condition `sol.back() > c` never triggers. Returns `"abc"`.
- **Reverse Sorted Input (`s = "cba"`):** As each character is processed, the remaining count of the preceding character hits 0 (`mp[sol.back()] > 0` becomes false). Pop cannot happen because no characters remain later. Returns `"cba"`.
- **Single Character Input (`s = "a"`):** Loops once, pushes `'a'`, and returns `"a"`.
- **Duplicate Letter at the Very End (`s = "bcab"`):** Pops `'b'` and `'c'` when `'a'` is encountered because both `'b'` and `'c'` appear later, yielding `"abc"`.
