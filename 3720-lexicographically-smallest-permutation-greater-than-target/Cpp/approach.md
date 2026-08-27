![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.36%20MB%20(beats%2040.76%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given two strings, `s` and `target`, both of the exact same length `n`. Both strings contain only lowercase English letters.

Your goal is to reorder the letters of `s` (create a permutation of `s`) to form a new string that is:
1. **Strictly greater** than `target` in alphabetical (dictionary) order.
2. The **smallest possible** string among all permutations that are strictly greater than `target`.

If it is impossible to make any permutation of `s` greater than `target`, you must return an empty string `""`.

### Example
If `s = "abc"` and `target = "bba"`:
* The possible permutations of `s` in alphabetical order are: `"abc"`, `"acb"`, `"bac"`, `"bca"`, `"cab"`, `"cba"`.
* The ones strictly greater than `"bba"` are `"bca"`, `"cab"`, and `"cba"`.
* The smallest one among those is `"bca"`.

---

## Intuition

To make a permutation strictly larger than `target` while keeping it as small as possible, we want our new string to match `target` for as long a prefix as possible from left to right.

1. **Match as long as possible:** First, try to match `target` character by character from left to right using the letters available in `s`.
2. **Find the turning point:** Once we cannot match any further (or after matching the whole string), we backtrack from right to left. At index `i`, we check if we have any unused character from `s` that is strictly greater than `target[i]`.
3. **Pick the smallest larger character:** The moment we find a valid position `i` where we can put a character larger than `target[i]`, we choose the **smallest** available character that is greater than `target[i]`. This guarantees our new string becomes strictly larger than `target` at position `i`.
4. **Minimize the suffix:** To make the entire result as small as possible overall, all remaining characters after index `i` are sorted in ascending order (A to Z).
5. **Backtrack if needed:** If index `i` has no available character larger than `target[i]`, we put back the character used at index `i - 1` and move left to try the previous position.

If we backtrack past index 0 without finding any valid split point, it is impossible to form a larger string, so we return `""`.

---

## Approach

Here is the step-by-step breakdown of how the C++ code implements this idea:

* `vector<int> total(26, 0);` — Creates a count array of size 26 to store the total frequency of each character available in `s`.
* `for (char c : s) { total[c - 'a']++; }` — Loops through `s` and records how many copies of each letter we have.
* `vector<int> used(26, 0);` — Creates a second array to track how many copies of each letter are currently consumed by the matching prefix.
* `while (i < n)` — Traverses `target` from left to right, trying to match as many characters as possible.
* `if (used[c] == total[c]) { break; }` — Stops matching if `target[i]` requires a character `c` that we have no remaining copies of.
* `used[c]++; i++;` — Marks one copy of character `c` as used and advances `i` to the next position.
* `while (i >= 0)` — Backtracks from position `i` down to `0` to locate the rightmost position where we can place a strictly larger character.
* `int current = target[i] - 'a';` — Converts the character `target[i]` into its 0-indexed alphabet position (0 for 'a', 1 for 'b', etc.).
* `for (int next = current + 1; next < 26; next++)` — Loops through all letters strictly greater than `target[i]` in alphabetical order.
* `if (used[next] == total[next]) { continue; }` — Skips letter `next` if we have no unused copies of it left.
* `string answer = target.substr(0, i);` — Starts building the output string using the matched prefix up to index `i - 1`.
* `answer += char('a' + next);` — Appends the smallest available larger character at index `i`.
* `for (int c = 0; c < 26; c++) { ... answer += string(remaining, char('a' + c)); }` — Appends all remaining available letters in alphabetical order to make the rest of the string as small as possible.
* `if (i > 0) { used[target[i - 1] - 'a']--; } i--;` — If no larger character can be placed at index `i`, un-uses the letter at index `i - 1` and steps left to try the previous index.
* `return "";` — Returns an empty string if all indices down to 0 fail to produce a greater permutation.

---

## Dry Run

### Case 1: `s = "abc"`, `target = "bba"`

`total` frequencies: `a: 1, b: 1, c: 1`.

| Step | `i` | `used` counts | Action / Decision |
| :--- | :--- | :--- | :--- |
| Prefix match | 0 | `a:0, b:0, c:0` | `target[0]` is 'b'. `used['b'] < total['b']`, so use 'b'. `used['b']` becomes 1. Increment `i` to 1. |
| Prefix match | 1 | `a:0, b:1, c:0` | `target[1]` is 'b'. `used['b'] == total['b']` (1 == 1). Cannot match! Break prefix loop with `i = 1`. |
| Backtrack | 1 | `a:0, b:1, c:0` | At `i = 1`, `target[1]` is 'b'. Look for `next` > 'b'. Found 'c' (`used['c'] = 0 < 1`). |
| Build Answer | 1 | `a:0, b:1, c:0` | Take `target[0..0]` ("b") + 'c' = `"bc"`. Remaining unused: one 'a'. Final answer: `"bca"`. Return `"bca"`. |

---

### Case 2: `s = "baba"`, `target = "bbaa"`

`total` frequencies: `a: 2, b: 2`.

| Step | `i` | `used` counts | Action / Decision |
| :--- | :--- | :--- | :--- |
| Prefix match | 0 | `a:0, b:0` | `target[0]` = 'b'. Use 'b'. `used['b'] = 1`, `i = 1`. |
| Prefix match | 1 | `a:0, b:1` | `target[1]` = 'b'. Use 'b'. `used['b'] = 2`, `i = 2`. |
| Prefix match | 2 | `a:0, b:2` | `target[2]` = 'a'. Use 'a'. `used['a'] = 1`, `i = 3`. |
| Prefix match | 3 | `a:1, b:2` | `target[3]` = 'a'. Use 'a'. `used['a'] = 2`, `i = 4`. Loop ends (`i = 4 == n`). |
| Backtrack | 4 | `a:2, b:2` | `i = 4` is out of bounds (`i < n` is false). Un-use `target[3]` ('a'). `used['a'] = 1`. Decrement `i` to 3. |
| Backtrack | 3 | `a:1, b:2` | At `i = 3`, `target[3]` = 'a'. Try `next` = 'b'. `used['b'] == total['b']` (2 == 2). No character > 'a' available. Un-use `target[2]` ('a'). `used['a'] = 0`. Decrement `i` to 2. |
| Backtrack | 2 | `a:0, b:2` | At `i = 2`, `target[2]` = 'a'. Try `next` = 'b'. `used['b'] == total['b']` (2 == 2). No character > 'a' available. Un-use `target[1]` ('b'). `used['b'] = 1`. Decrement `i` to 1. |
| Backtrack | 1 | `a:0, b:1` | At `i = 1`, `target[1]` = 'b'. No available letters > 'b'. Un-use `target[0]` ('b'). `used['b'] = 0`. Decrement `i` to 0. |
| Backtrack | 0 | `a:0, b:0` | At `i = 0`, `target[0]` = 'b'. No available letters > 'b'. Decrement `i` to -1. |
| End | -1 | `a:0, b:0` | Loop terminates (`i >= 0` false). Return `""`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where `n` is the length of strings `s` and `target`.
  * Counting initial character frequencies in `s` takes O(n) time.
  * The prefix matching loop runs at most `n` times, taking O(1) time per step.
  * The backtracking loop runs at most `n + 1` times. Inside the loop, checking for an available larger character takes at most 26 steps, which is O(1).
  * Constructing the final string `answer` takes O(n) time, but this construction happens **only once** right before returning.
  * Total time is O(n) + O(n) + O(n) = O(n).

* **Space Complexity:** **O(n)** — memory used for the output string `answer` of length `n`.
  * The `total` and `used` frequency vectors both use fixed size 26 arrays, taking O(1) extra space.
  * Excluding the returned answer string, the auxiliary space complexity is **O(1)**.

### Is this solution optimal?

**Yes, this solution is already strictly optimal.**

To process strings of length `n` and produce an output string of length `n`, any algorithm must inspect the input characters and construct the result, which requires at least O(n) time and O(n) space. No further asymptotic improvement is theoretical or practical.

---

## Edge Cases Handled

* **No valid permutation exists:** Handled cleanly when backtracking exhausts all indices down to `i = -1`, returning `""`.
* **Duplicate letters in input:** Handled correctly using `total` and `used` frequency counts rather than unique character sets.
* **`s` already matches `target` exactly:** The initial loop matches all `n` characters (`i = n`). The backtracking then starts from `i = n`, un-uses characters from right to left, and finds the smallest valid higher permutation.
* **Smallest string size (`n = 1`):** Works correctly without any index out-of-bounds errors.
* **Missing required characters:** If `s` lacks characters needed to match a prefix of `target`, matching breaks early at the first missing character and immediately tries to place a larger available letter at that index.
