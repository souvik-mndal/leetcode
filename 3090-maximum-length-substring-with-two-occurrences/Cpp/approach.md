![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.9%20MB%20(beats%2094.36%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The goal is to find the length of the longest sub-part (a contiguous substring) inside a given string `s` where no character appears more than 2 times.

For example, if `s = "bcbbbcba"`:
- The sub-part `"bcbb"` is invalid because `'b'` appears 3 times.
- The sub-part `"bcba"` is valid because `'b'` appears 2 times, `'c'` appears 1 time, and `'a'` appears 1 time.
- The longest valid sub-part here is `"bcba"`, which has a length of 4.

## Intuition

Think of this like a sliding frame (a **sliding window**) over the string. 

We expand the frame to the right, one character at a time, and count how many times each character appears inside our frame. If adding a character causes its count to jump to 3, our frame becomes invalid.

To make the frame valid again, we slide its left edge to the right, removing characters from our count until the offending character's count drops back down to 2. At every step where the frame is valid, we check its size and record the maximum length seen so far.

## Approach

Here is step-by-step how the code works:

* `int freq[26]={0};` — Creates a fixed list of 26 counters initialized to zero. Each counter tracks how many times a specific letter ('a' through 'z') appears in the current window.
* `int l=0, n=s.size(), len=0;` — Sets the left pointer `l` to index 0, saves the total length of the string as `n`, and initializes the maximum valid length `len` to 0.
* `for(int r=0; r<n; r++)` — Loops through the string using `r` as the right pointer to expand the window character by character.
* `int x=s[r]-'a';` — Converts the character at position `r` into a number from 0 to 25 so it fits into our `freq` counter list.
* `freq[x]++;` — Increases the frequency count for the current character by 1.
* `while(l<r && freq[x]>2)` — Checks if the current character now appears more than 2 times. If it does, this loop runs to shrink the window from the left until the count drops back to 2.
* `freq[s[l]-'a']--;` — Reduces the count of the character at the left pointer `l` because it is being removed from the window.
* `l++;` — Moves the left pointer one step to the right to shorten the window.
* `len=max(len, r-l+1);` — Calculates the current valid window size (`r - l + 1`) and keeps the larger value between it and the previous maximum length `len`.
* `return len;` — Returns the largest valid length found after checking the entire string.

## Dry Run

### Case 1: Typical case (`s = "bcbbbcba"`)

| `r` | `s[r]` | `l` | `freq[s[r]]` after update | Window `s[l...r]` | Action | `len` |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 'b' | 0 | 1 | "b" | Add 'b', valid | 1 |
| 1 | 'c' | 0 | 1 | "bc" | Add 'c', valid | 2 |
| 2 | 'b' | 0 | 2 | "bcb" | Add 'b', valid | 3 |
| 3 | 'b' | 1 | 2 (was 3) | "cbb" | Count of 'b' hit 3; move `l` right to index 1 | 3 |
| 4 | 'b' | 3 | 2 (was 3) | "bb" | Count of 'b' hit 3; move `l` right past 'c' and 'b' to index 3 | 3 |
| 5 | 'c' | 3 | 1 | "bbc" | Add 'c', valid | 3 |
| 6 | 'b' | 4 | 2 (was 3) | "bcb" | Count of 'b' hit 3; move `l` right to index 4 | 3 |
| 7 | 'a' | 4 | 1 | "bcba" | Add 'a', valid window length is 4 | 4 |

Final Output: `4`

---

### Case 2: Edge case with identical values (`s = "aaaa"`)

| `r` | `s[r]` | `l` | `freq[s[r]]` after update | Window `s[l...r]` | Action | `len` |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 'a' | 0 | 1 | "a" | Add 'a', valid | 1 |
| 1 | 'a' | 0 | 2 | "aa" | Add 'a', valid | 2 |
| 2 | 'a' | 1 | 2 (was 3) | "aa" | Count of 'a' hit 3; shrink left by 1 (`l` becomes 1) | 2 |
| 3 | 'a' | 2 | 2 (was 3) | "aa" | Count of 'a' hit 3; shrink left by 1 (`l` becomes 2) | 2 |

Final Output: `2`

## Time & Space Complexity

- **Time:** **O(n)** — where `n` is the length of the string. The right pointer `r` moves from left to right `n` times. The left pointer `l` also only moves forward and never steps back. Since both pointers travel at most `n` steps, the total operations are at most 2 * n, which simplifies to O(n).
- **Space:** **O(1)** — the memory used by `freq` is a fixed array of size 26 regardless of how long the input string `s` is.

**Is this already optimal?**
Yes, this solution is optimal. We must inspect every character in the string at least once to know its frequency, which requires O(n) time minimum. We are using constant O(1) extra space. No faster or smaller approach exists.

## Edge Cases Handled

* **Shortest possible length (`s.length = 2`):** The code safely returns 2 because any 2-character string can have at most 2 occurrences of any character.
* **All characters are identical (e.g., `"aaaa"`):** The `while` loop triggers as soon as the 3rd identical character is added, maintaining a maximum valid window of size 2.
* **No characters repeat more than twice (e.g., `"abcde"` or `"aabbcc"`):** The `while` loop condition is never met, so `l` stays at 0 and the code correctly returns the full string length `n`.
* **Clusters of duplicates (e.g., `"abbba"`):** The left pointer moves as many times as necessary to clear out extra duplicates and restore a valid window state.
