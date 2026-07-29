![Runtime](https://img.shields.io/badge/Runtime-23%20ms%20(beats%2065.22%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-17.33%20MB%20(beats%2040.58%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a palindromic string `s` (a string that reads the same backward as forward) and an integer `k`. 

You need to find all unique ways to rearrange `s` such that the new string is still a palindrome. If you sort all these unique palindromes in **alphabetical order** (lexicographical order), your goal is to return the **$k$-th string** in that list. 

If there are fewer than `k` possible unique palindromes, return an empty string `""`.

### Example
For `s = "abba"` and `k = 2`:
* The unique palindromes made from these letters are `"abba"` and `"baab"`.
* Alphabetically sorted: 1st is `"abba"`, 2nd is `"baab"`.
* Since `k = 2`, the answer is `"baab"`.

---

## Intuition

A palindrome is completely determined by its **first half**. Once you know the first half, the second half is just its reversed mirror image. If the original string has an odd length, the middle character is fixed and sits right in the center.

Because of this, we do not need to build the whole string at once. We only need to find the **$k$-th smallest arrangement of the first half**.

To build the first half character by character (from left to right):
1. At each position, we try placing the smallest available letter (e.g., `'a'`, then `'b'`, then `'c'`).
2. If we place a letter, we count **how many valid full arrangements** can be formed using the remaining unused letters.
3. If that count is at least `k`, it means our answer lies within this letter choice! So, we keep this letter and move to the next position.
4. If that count is less than `k`, our answer is beyond this group. We subtract the count from `k` and try the next larger letter.

Since `k` is at most $1,000,000$, any letter combination count larger than $1,000,000$ can be safely **capped at $1,000,001$**. This simple cap prevents large numbers from overflowing standard integer limits!

---

## Approach

1. **Count character frequencies**: Count how many times each letter (`'a'` to `'z'`) appears in `s`.
2. **Split into half and middle**:
   * Divide each letter count by 2 to get the pool of letters available for the first half (`half`).
   * If a letter count is odd, store that letter in `mid` (there will be at most one odd letter).
3. **Check total possibilities**: Calculate the total possible unique arrangements of the first half. If the total is less than `k`, return `""`.
4. **Build the first half (Position by Position)**:
   * Loop through each index of the first half string.
   * Try each letter `c` from `'a'` to `'z'`:
     * If letter `c` is available, temporarily take one away from `half`.
     * Calculate `cnt`, the number of ways to arrange the remaining letters in `half`.
     * **If `cnt >= k`**: This letter `'a' + c` is correct for this position! Append it to `first` and move to the next position.
     * **If `cnt < k`**: Subtract `cnt` from `k`, return the letter back to `half`, and try the next letter choice.
5. **Assemble the final palindrome**: Combine `first + mid + reverse(first)` and return it.

---

## Time & Space Complexity

Let $N$ be the length of string `s`, and $\Sigma = 26$ be the alphabet size.

* **Time Complexity:** $O(N \cdot \Sigma^2)$ — The first half has $N/2$ positions. For each position, we iterate over up to $\Sigma$ candidate letters. For each candidate, we call `ways()`, which does $O(\Sigma)$ operations. Since $\Sigma = 26$ is constant, this runs in $O(N)$ time with a small constant factor ($N/2 \times 26 \times 26 \approx 338 N$ operations).
* **Space Complexity:** $O(N)$ — Memory used to store the output string and frequency arrays of fixed size 26.

### Can it be improved?

**Yes!** Right now, `ways()` recalculates the total permutations from scratch in $O(\Sigma)$ time for every letter test. We can instead maintain the total count dynamically in $O(1)$ time when trying a character, because picking a character `c` changes the available permutations by a simple ratio: 

$$\text{new\_ways} = \frac{\text{old\_ways} \cdot \text{count}(c)}{\text{remaining\_length}}$$

```cpp
// Incremental update idea (concept):
ll candidate_ways = (total_ways * half[c]) / remaining_len;
```

* **Improved Complexity:** **Time:** $O(N \cdot \Sigma)$ — Each position checks 26 characters in $O(1)$ time each. **Space:** $O(N)$.
* **Theoretical Best:** $O(N)$ time and $O(N)$ space (since creating the output string of length $N$ takes at least $O(N)$ work). The improved version reaches this theoretical limit because $\Sigma = 26$ is a constant factor.

---

## Edge Cases Handled

* **`k` is larger than the total possible palindromes:** The code checks `ways(half) < k` at the beginning and returns `""` immediately.
* **Odd-length strings:** The odd character is extracted into `mid` and automatically placed in the middle without messing up the first-half calculation.
* **Repeated characters (e.g., `"aaaa"`):** The combination formula $\binom{n}{r}$ accounts for duplicate characters correctly so arrangements are only counted once.
* **`k = 1`:** The code correctly picks the very first alphabetical arrangement on its first try without subtracting anything from `k`.
* **Combinatorial Overflow:** Counting permutations of long strings can exceed 64-bit integer limits (`long long`). The code caps all combination results at `LIM + 1` (`1,000,001`), keeping values safe while still correctly handling $k \le 10^6$.
