![Runtime](https://img.shields.io/badge/Runtime-24%20ms%20(beats%2073.86%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-61%20MB%20(beats%2061.88%25)-green?style=for-the-badge)

---

## Problem Explained

We are given a string `s` that is guaranteed to be a palindrome (a word that reads the same forwards and backwards, like "racecar"). Our job is to rearrange the letters of `s` so that the new string is *also* a palindrome, but comes as early as possible in the dictionary, also known as the **lexicographically smallest** order. 

For example, if we start with the string `"babab"`, the letters can be rearranged to form other palindromes like `"abbba"` or `"baab"`. Out of all possible palindromes we can build from these exact letters, `"abbba"` comes first in alphabetical order. That is our target output.

## Intuition

A palindrome is symmetrical. If we know what characters go on the left half of the string, the right half is just a mirror image. Also, because we want the **lexicographically smallest** palindrome, we should place the smallest available letters (like `'a'`, `'b'`, etc.) as close to the very beginning of the string as possible. 

To achieve this, we can count how many times each letter appears from `'a'` to `'z'`. Since a palindrome pairs up matching characters on both the left and right ends, we can take our counts and greedily place pairs of the smallest available characters at the outside edges (`left` and `right` pointers) moving inward. If a character has an odd count, the leftover single character must go right in the exact middle of the string so the symmetry is preserved. Because we process letters from `'a'` up to `'z'`, we naturally guarantee the smallest characters sit on the outer edges, which makes the whole palindrome as small as possible alphabetically.

## Approach

* `vector<int> freq(26, 0);`: Creates a fixed-size frequency array of 26 zeros to store the count of each English lowercase letter from `'a'` to `'z'`.
* `for (char c : s) freq[c - 'a']++;`: Loops through every character in the input string `s`, subtracting the ASCII value of `'a'` to map `'a'` to index 0, `'b'` to index 1, and so on, incrementing its count.
* `int n = s.size();`: Stores the total length of the string in `n`.
* `string ans(n, ' ');`: Initializes our result string `ans` of length `n` filled with blank spaces.
* `int left = 0, right = n - 1;`: Sets up two pointers: `left` starting at the beginning of the string, and `right` starting at the end of the string, moving inward.
* `for (int i = 0; i < 26; i++) {`: Loops through our alphabet indices from 0 (letter `'a'`) to 25 (letter `'z'`) to build the palindrome in strict alphabetical order.
* `while (freq[i] >= 2) {`: While the current letter has at least 2 remaining copies, we can place a pair of them to keep the palindrome symmetrical.
* `ans[left++] = char('a' + i);`: Places the current letter at the `left` pointer position, then moves the `left` pointer one step to the right.
* `ans[right--] = char('a' + i);`: Places the same letter at the `right` pointer position, then moves the `right` pointer one step to the left.
* `freq[i] -= 2;`: Decrements the frequency count by 2 since we just placed a pair.
* `if (freq[i] == 1) { ans[s.size() / 2] = char('a' + i); }`: If a single leftover character remains after making all possible pairs, places it right in the dead center of the string.
* `return ans;`: Returns the fully built palindromic string.

## Dry Run

### Case 1: Typical case ("babab")

| `i` (Letter) | `freq[i]` before loop | Actions / Updates | `ans` state | `left`, `right` |
| --- | --- | --- | --- | --- |
| 0 (`'a'`) | 2 | Places two `'a'`s at edges, decrements freq to 0 | `"b...b"` | `left = 1`, `right = 3` |
| 1 (`'b'`) | 3 | Places two `'b'`s at edges, leaving 1. Then places middle `'b'` | `"abbba"` | `left = 2`, `right = 2` |
| 2 to 25 | 0 | Loops finish with no changes | `"abbba"` | `left = 2`, `right = 2` |

### Case 2: Edge case with all matching characters ("z")

| `i` (Letter) | `freq[i]` before loop | Actions / Updates | `ans` state | `left`, `right` |
| --- | --- | --- | --- | --- |
| 0 to 24 | 0 | Loops through unused letters with zero counts | `" "` | `left = 0`, `right = 0` |
| 25 (`'z'`) | 1 | `freq[i]` is not >= 2, skips while. Then hits `freq[i] == 1`, places `'z'` in middle | `"z"` | `left = 0`, `right = 0` |

## Time & Space Complexity

* **Time:** O(N) — where N is the length of string `s`. We iterate through `s` once to count the character frequencies (taking N steps), and then we loop through the alphabet 26 times (a constant 26 steps) to place characters into our result string. Thus, the time complexity scales linearly with the length of the string.
* **Space:** O(1) auxiliary space (or O(N) if you count the space required to store the output string `ans`). The frequency vector is fixed at a constant size of 26 integers regardless of how large the input string `s` is.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is already **optimally efficient**. 

- **Time Optimality:** We must look at every character in the input string at least once to count its frequency, making O(N) the absolute lower bound for time complexity. We cannot go faster than O(N).
- **Space Optimality:** Aside from the output string itself, we only use a fixed array of size 26, which uses O(1) extra space. 

No further improvements in time or space complexity are possible.

## Edge Cases Handled

* **Single-character strings:** Handled correctly because the single character skips the pair-building `while` loop, hits the `freq[i] == 1` condition, and gets placed safely in the exact middle index (`0 / 2 = 0`).
* **Strings with all identical characters:** Handled correctly since pairs are steadily consumed from the outside inward, and any leftover odd character lands cleanly in the middle.
* **Large inputs up to constraints (10^5):** Handled efficiently because the linear O(N) pass and fixed 26-step alphabet sweep execute well within standard time limits without running into stack overflows or time-outs.
