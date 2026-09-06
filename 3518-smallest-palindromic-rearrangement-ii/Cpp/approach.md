![Runtime](https://img.shields.io/badge/Runtime-31%20ms%20(beats%2027.37%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-17.4%20MB%20(beats%2028.89%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you are given a string `s` that is already guaranteed to be a palindrome (it reads the same forwards and backwards, like "abba" or "racecar"). 

Because you are allowed to rearrange the letters however you want, you can make many different palindromic strings out of those exact same letters. The problem asks you to find the **k-th lexicographically smallest** palindromic permutation of `s`. 

"Lexicographically smallest" just means dictionary order—alphabetical order, like words in a dictionary where "a" comes before "b". Also, note a special rule: if two different rearangements result in the exact same palindrome string, they count as just **one** distinct palindrome. If there are fewer than `k` unique palindromes possible, you return an empty string.

## Intuition

A palindrome is completely symmetrical. If you know the first half of a palindrome, the second half is automatically fixed because it is just the first half flipped around backwards. If the original string has an odd length, there is also a single leftover character that must go right in the middle.

Because of this symmetry, we don't need to build the full string letter by letter from scratch. We only need to build the **first half** of the string. 

To find the `k`-th smallest palindrome, we can build the first half character by character, starting from the letter "a" and moving up to "z". At each position, we ask a powerful math question: *"If I place this specific letter here, how many total valid palindromes can I still form with the remaining letters?"* 

If that number of remaining ways is greater than or equal to `k`, we know our answer must start with this letter! We lock it in, and move to the next position. If the number of ways is less than `k`, it means our target `k`-th palindrome must start with a *larger* letter. We subtract that subset of ways from `k`, and try the next letter in the alphabet. 

## Approach

* `vector<int> freq(26, 0);` -> Counts how many times each letter from 'a' to 'z' appears in the original string `s`.
* `vector<int> half(26, 0);` -> Stores half of each letter's total frequency, because a palindrome's first half only uses half of its characters.
* `string mid = "";` -> Holds the single odd-count middle character if the string length is odd, or stays empty if it is even.
* `int len = 0;` -> Tracks the total length of the first half string we need to build.
* `if (ways(half) < k) return "";` -> Calculates the total number of unique palindromes possible using all letters in `half`. If this total is less than `k`, it is impossible to reach the `k`-th palindrome, so it immediately returns an empty string.
* `for (int pos = 0; pos < len; pos++) { ... }` -> Loops through each position in the first half from left to right to build the string character by character.
* `for (int c = 0; c < 26; c++) { ... }` -> Tries letters alphabetically from 'c' = 0 ('a') up to 25 ('z').
* `if (half[c] == 0) continue;` -> Skips any letter that we have run out of in our `half` counts.
* `half[c]--;` -> Temporarily places the current letter `c` at the current position by decrementing its available count.
* `ll cnt = ways(half);` -> Calculates how many valid palindromic ways remain with this letter choice.
* `if (cnt >= k) { first += char('a' + c); break; }` -> If the remaining choices cover or exceed `k`, this letter is correct! We append it to `first` and stop searching for this position.
* `k -= cnt; half[c]++;` -> If `cnt` is less than `k`, our target is further down the alphabetical line. We subtract `cnt` from `k` and put the letter back (`half[c]++`) to try the next alphabetical letter.
* `string second = first; reverse(second.begin(), second.end());` -> Creates the second half by taking `first` and reversing it.
* `return first + mid + second;` -> Combines the first half, the middle character, and the second half to form the final complete palindrome and returns it.

## Dry Run

### Case 1: Typical case (`s = "abba"`, `k = 2`)

| `pos` | `c` (Letter) | `half` state | `cnt` (ways) | Action |
| :--- | :--- | :--- | :--- | :--- |
| 0 | 0 ('a') | `[0, 1]` | 1 | `cnt` (1) < `k` (2). Subtract `cnt` from `k` (`k = 1`), restore `half`. |
| 0 | 1 ('b') | `[1, 0]` | 1 | `cnt` (1) >= `k` (1). Append 'b' to `first`, break. |
| 1 | 0 ('a') | `[0, 0]` | 1 | `cnt` (1) >= `k` (1). Append 'a' to `first`, break. |

*Final result construction:* `first` = "ba", `mid` = "", `second` = "ab". Returns **"baab"**.

### Case 2: Edge case with duplicate values (`s = "aa"`, `k = 2`)

| `pos` | `c` (Letter) | `half` state | `cnt` (ways) | Action |
| :--- | :--- | :--- | :--- | :--- |
| N/A | N/A | `[1]` | 1 | `ways(half)` returns 1. Since 1 < `k` (2), it immediately returns `""`. |

*Final result construction:* Returns **""**.

## Time & Space Complexity

**Time:** O(N * 26^2) — The outer loop runs for the length of the first half (up to N / 2 times). Inside, we loop through the alphabet (26 times), and inside *that*, the `ways` function loops through the alphabet again (26 times) to compute combinations using multinomial coefficients. This results in roughly N * 26 * 26 operations, which easily runs well within time limits for N = 10^4.

**Space:** O(N) — We store frequency arrays of fixed size 26, but the recursion-free string builders (`first`, `second`, `mid`) and the original string storage scale with the length of `s`, taking linear space.

**Is this already the most optimal possible complexity, or can it be improved?**

The code is already very efficient and runs well within acceptable time limits. However, the time complexity can be optimized from O(N * 26^2) down to O(N * 26) by avoiding the full `ways()` recalculation from scratch every single time we test a letter. 

### How to optimize:
Instead of calling `ways(half)` completely fresh inside the letter-testing loop, we can notice that the number of ways to arrange a multiset of characters is a multinomial coefficient formula: 
`total_factorial / (factorial(count_1) * factorial(count_2) * ...)`

When we temporarily decrement `half[c]`, we are just changing the counts slightly. We can maintain or incrementally update the combinations, or optimize the inner loop by pre-calculating factorials and doing O(1) math divisions instead of re-looping through all 26 alphabet buckets inside `ways`. 

However, since 26 is a very small constant, an O(N * 26^2) approach already executes in just a few milliseconds. 

**Resulting improved time complexity:** O(N * 26) using precomputed factorials.
**Theoretical best possible complexity:** O(N) is the theoretical best because you must at least read the input string of length N to count the character frequencies.

## Edge Cases Handled

* **Fewer than `k` palindromes:** Handled by checking `if (ways(half) < k) return "";` right at the start.
* **Odd length palindromes:** Handled cleanly by capturing the single odd-frequency character into `mid` and placing it right between `first` and `second`.
* **Single character / Minimum size strings (N = 1):** Handled naturally as `len` becomes 1 or 0, loops execute correctly, and the single character returns properly.
* **All identical characters (e.g., "aaaa"):** Handled correctly because `ways` will calculate only 1 distinct permutation, safely returning it for `k = 1` or returning `""` if `k > 1`.
