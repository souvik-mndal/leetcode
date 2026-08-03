![Runtime](https://img.shields.io/badge/Runtime-30%20ms%20(beats%2028.93%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-17.33%20MB%20(beats%2029.01%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given a string `s` that is already a palindrome (a word that reads the same backward as forward, like "abba" or "bacab"). You are also given an integer `k`.

Your task is to reorder the letters of `s` to form all possible unique palindromes, sort those palindromes in alphabetical order, and return the `k`-th palindrome from that sorted list.

If `k` is larger than the total number of unique palindromes you can form, return an empty string `""`.

### Example
If `s = "bacab"` and `k = 1`:
* The unique palindromes made from these letters are `"abcba"` and `"bacab"`.
* Alphabetically sorted: 1st = `"abcba"`, 2nd = `"bacab"`.
* For `k = 1`, the answer is `"abcba"`.

---

## Intuition

A palindrome is symmetrical. The second half is just a mirrored copy of the first half. If the string has an odd length, there is one fixed character in the exact middle.

Because of this symmetry, **we only need to build the first half of the string**. Once the first half is built, the middle character (if any) and the reversed first half are automatically decided.

To build the $k$-th smallest first half:
1. We construct it letter by letter, from left to right.
2. For each position, we try candidate letters in alphabetical order ('a', then 'b', 'c', and so on).
3. For each letter, we calculate: *"If we put this letter here, how many valid combinations can be made with the remaining letters?"*
4. If that count is at least `k`, our desired string **must** start with this letter. We lock it in and move to the next character position.
5. If that count is smaller than `k`, it means our answer lies further down the list. We subtract that count from `k` and try the next candidate letter.

Since `k` is at most $10^6$, any combination count larger than $10^6$ can be capped at $10^6 + 1$. This prevents huge numbers from overflowing integer limits.

---

## Approach

Here is how the code works, step-by-step:

* **Count Frequencies:** Count how many times each character appears in `s`.
* **Split into First Half and Middle:**
  * Divide each letter's count by 2 to get the pool of characters available for the first half (`half`).
  * If a letter has an odd total count, save one copy of it in `mid` (the center character).
* **Check Feasibility:** Compute the total number of unique arrangements possible using `half`. If total arrangements < `k`, return `""`.
* **Build First Half (Position by Position):**
  * Loop through each position of the first half from left to right.
  * Try characters `c` from 'a' to 'z':
    * If `half[c] == 0`, skip it (no characters of this type left).
    * Temporarily place character `c` by subtracting 1 from `half[c]`.
    * Calculate `cnt`, the number of ways to arrange all remaining unused characters in `half`.
    * If `cnt >= k`: character `c` is the correct choice for this position. Keep `half[c]` decremented, append `c` to the result, and break to move to the next position.
    * If `cnt < k`: character `c` is not large enough to reach `k`. Subtract `cnt` from `k` (`k -= cnt`), restore `half[c]++`, and try the next letter 'c + 1'.
* **Construct Final String:** Assemble the full result: `first_half + mid + reverse(first_half)` and return it.

---

## Time & Space Complexity

### Current Complexity
* **Time Complexity:** **O(N^2)** where N is the length of string `s`.
  * *Why:* The first half has length `N / 2`. For each position, we try up to 26 characters. For each character, `ways(half)` calculates combinations by looping over 26 characters and calling `C(n, r)`, which takes up to `O(N)` steps. Total time is roughly `(N / 2) * 26 * 26 * N`, which simplifies to `O(N^2)`.
* **Space Complexity:** **O(N)**
  * *Why:* Frequency arrays take fixed space `O(26)`, but storing strings `first`, `second`, and `mid` takes space proportional to string length `N`.

### Can it be improved?
**Yes.** Instead of recalculating combinations from scratch using `ways(half)` at every step, we can compute the number of ways in **O(1)** time using simple arithmetic.

When placing character `c` at a position with `rem_len` remaining slots, the number of valid arrangements starting with `c` is:

```
cnt = old_total_ways * half[c] / rem_len
```

Using this formula reduces the per-position cost from `O(N)` down to `O(1)`.

```cpp
// Optimized inner check in O(1) arithmetic instead of full ways() recalculation:
ll cnt = current_total_ways * half[c] / remaining_len;
if (cnt >= k) {
    first += char('a' + c);
    half[c]--;
    current_total_ways = cnt; // Update remaining ways directly
    break;
}
k -= cnt;
```

* **Improved Time Complexity:** **O(N)** — We do constant work `O(26)` for each of the `N / 2` positions.
* **Theoretical Best Complexity:** **O(N)** — We must construct a string of length `N`, so `O(N)` is optimal. The improved version reaches this theoretical limit.

---

## Edge Cases Handled

* **`k` is out of bounds:** If `k` is larger than total possible palindromes, initial check `ways(half) < k` returns `""` immediately.
* **Odd-length strings:** Correctly isolates the single middle character using `freq[i] & 1` and places it in `mid`.
* **Even-length strings:** `mid` remains an empty string, building a standard mirrored palindrome.
* **Duplicate letters:** Managed naturally by using multiset permutation counts (combinations formula `N! / (c1! * c2! ...)`), avoiding overcounting duplicate arrangements.
* **Integer Overflow:** Combinatorial counts grow extremely fast. Capping all calculations at `LIM + 1` (`10_000_001`) prevents `long long` integer overflow while keeping exact numbers for `k <= 10^6`.
