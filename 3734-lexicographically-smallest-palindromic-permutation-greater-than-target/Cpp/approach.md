![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2076.92%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.85%20MB%20(beats%2080.00%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to find a specific string made by reordering the letters of a given string `s`. This new string must meet two conditions:
1. It must be a **palindrome** (it reads the exact same forward and backward, like "racecar" or "baab").
2. It must be **lexicographically strictly greater** than a second string called `target` (meaning it comes later in dictionary alphabetical order).

If there are multiple palindromes larger than `target`, we want the smallest one among them (the one that comes earliest in the dictionary). If it is impossible to form any valid palindrome that is greater than `target`, we return an empty string `""`.

### Example
Suppose `s = "baba"` and `target = "abba"`.
- Letters in `s`: two 'a's and two 'b's.
- Palindromes made from these letters (in alphabetical order): `"abba"` and `"baab"`.
- Both are valid palindromes, but only `"baab"` is strictly greater than `target` (`"abba"`).
- Output: `"baab"`.

---

## Intuition

A palindrome is symmetrical. Because the second half is always a mirror image of the first half, the first half completely controls how the palindrome compares alphabetically to other strings.

For an even-length string, we only need to construct the first half (length `n / 2`). For an odd-length string, we also place the single odd-frequency character in the middle.

The core idea is:
1. Try to construct the smallest possible first half (`half`) that is greater than or equal to the first half of `target` (`targetHalf`).
2. Mirror `half` around the middle character to form a full candidate palindrome.
3. If this candidate is strictly greater than `target`, we are done!
4. If it is equal to or smaller than `target` (which can happen when `half == targetHalf` but the mirrored tail isn't large enough), we generate the next immediately larger permutation of `half` using standard next-permutation logic and mirror it again.

This avoids generating all possible palindromes, taking us directly to the smallest valid answer.

---

## Approach

Here is the step-by-step logic matching the code:

* `vector<int> frequency(26, 0);`: We count the frequency of each lowercase character in `s`.
* `if (oddCount > 1) return "";`: A palindrome can have at most one character with an odd count (placed right in the middle). If more than one character has an odd count, no palindrome can be formed, so we return `""`.
* `halfCount[c] = frequency[c] / 2;`: We calculate how many copies of each character belong in the first half of the palindrome by dividing each frequency by 2.
* `string targetHalf = target.substr(0, k);`: We extract the first `k` characters of `target` (where `k = n / 2`) to use as our target prefix.
* `string half = smallestGreaterOrEqual(halfCount, targetHalf);`: We call a helper function to build the lexicographically smallest half-string using our available characters in `halfCount` that is greater than or equal to `targetHalf`.
* Inside `smallestGreaterOrEqual`:
  * `while (matched < k && count[targetHalf[matched] - 'a'] > 0)`: Matches as many characters as possible with `targetHalf` from left to right.
  * `for (int pos = matched; pos >= 0; --pos)`: If we couldn't match the full prefix or need to make the string larger, we backtrack from `matched` down to index 0.
  * `for (int c = targetHalf[pos] - 'a' + 1; c < 26; ++c)`: At position `pos`, we try to place a character `c` that is strictly larger than `targetHalf[pos]`.
  * `result.append(count[ch], char('a' + ch));`: Once a larger character is placed at `pos`, we fill all remaining positions with the smallest remaining characters in sorted order to keep the string as small as possible.
* `string candidate = buildPalindrome(half, middle);`: We reconstruct the full palindrome candidate by sticking `half`, the `middle` character (if `n` is odd), and the reversed `half` together.
* `if (candidate > target) return candidate;`: If this candidate string is strictly greater than `target`, it is our answer.
* `if (!nextPermutation(half)) return "";`: If `candidate` was not strictly greater than `target`, we advance `half` to its next alphabetical permutation. If no higher permutation exists, we return `""`.
* `return buildPalindrome(half, middle);`: We build and return the palindrome made from this updated `half`.

---

## Dry Run

### Case 1: Standard Case (`s = "baba"`, `target = "abba"`)

| Step | Variables & State | Action |
| :--- | :--- | :--- |
| 1 | `frequency` = {a:2, b:2}, `oddCount` = 0 | Count character frequencies. `oddCount` <= 1, valid palindrome possible. |
| 2 | `halfCount` = {a:1, b:1}, `k` = 2, `targetHalf` = "ab" | Half length `k` is 2. `halfCount` has 1 'a' and 1 'b'. |
| 3 | `smallestGreaterOrEqual` matches 'a', then 'b' | Matches `targetHalf` ("ab") exactly. Returns `half` = "ab". |
| 4 | `candidate` = `buildPalindrome("ab", 0)` -> "abba" | Reconstructs full palindrome candidate "abba". |
| 5 | Check `candidate > target` ("abba" > "abba") | Comparison is `false`. Must find next permutation. |
| 6 | `nextPermutation(half)` changes `half` from "ab" to "ba" | Returns `true`. Updated `half` = "ba". |
| 7 | `buildPalindrome("ba", 0)` -> "baab" | Returns "baab" as the final result. |

### Case 2: Odd Length with Middle Character (`s = "aac"`, `target = "abb"`)

| Step | Variables & State | Action |
| :--- | :--- | :--- |
| 1 | `frequency` = {a:2, c:1}, `oddCount` = 1, `middle` = 'c' | One odd count ('c'). Valid middle character set to 'c'. |
| 2 | `halfCount` = {a:1, c:0}, `k` = 1, `targetHalf` = "a" | Half length `k` is 1. `halfCount` has 1 'a'. |
| 3 | `smallestGreaterOrEqual` matches 'a' | Matches `targetHalf` ("a") exactly. Returns `half` = "a". |
| 4 | `candidate` = `buildPalindrome("a", 'c')` -> "aca" | Reconstructs full palindrome "a" + 'c' + "a" = "aca". |
| 5 | Check `candidate > target` ("aca" > "abb") | Comparison is `true`. |
| 6 | Return `candidate` | Returns "aca" as the final result. |

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** where `n` is the length of `s` (and `target`).
  - Counting character frequencies takes O(n) time.
  - `smallestGreaterOrEqual` runs at most `k` steps (where `k = n / 2`), with an inner loop checking 26 alphabet characters. Since 26 is a constant, this takes O(26 * n) = O(n) time.
  - `buildPalindrome` creates a string of length `n` in O(n) time.
  - `nextPermutation` runs in O(n) time.
  - Overall time is linear in relation to string length.

- **Space Complexity:** **O(n)**
  - We store `frequency` and `halfCount` arrays of fixed size 26 (O(1) space).
  - We create helper strings like `targetHalf`, `half`, and `candidate`, each taking up to O(n) space.

### Is this optimal?
**Yes, this is already strictly optimal.** 

Reading the input strings of length `n` takes O(n) time, and generating an output string of length `n` requires O(n) space and time. No algorithm can perform better than O(n) time and O(n) space for this problem.

---

## Edge Cases Handled

- **Impossible Palindromes:** Inputs with more than one odd character count (e.g., `s = "abc"`) are caught immediately by `oddCount > 1` and return `""`.
- **Target Equal to Highest Permutation:** When no palindromic permutation can exceed `target` (e.g., `s = "baba"`, `target = "bbaa"`), `smallestGreaterOrEqual` returns `""` and the main function safely returns `""`.
- **Same First Half, Smaller Mirror Tail:** When `half == targetHalf` but the middle character or reversed tail makes the full palindrome smaller than or equal to `target`, `nextPermutation` cleanly bumps `half` to the next valid combination.
- **Odd vs. Even String Lengths:** Odd length strings properly isolate the middle character while even length strings set `middle = 0`, ensuring exact reconstruction for both types.
