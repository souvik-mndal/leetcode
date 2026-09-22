![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2016.17%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.59%20MB%20(beats%2091.36%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to calculate a numerical score for a string called the **reverse degree**. 

To find the reverse degree, we look at each character in the string one by one:
1. Note its **1-based position in the string** (1st letter, 2nd letter, 3rd letter, and so on).
2. Find its **reverse alphabet rank**, where 'z' is 1, 'y' is 2, ..., and 'a' is 26.
3. Multiply its string position by its reverse alphabet rank.
4. Add all these values together to get the total reverse degree.

For example, given `s = "abc"`:
* 'a' is at position 1 and has a reverse rank of 26: `1 * 26 = 26`.
* 'b' is at position 2 and has a reverse rank of 25: `2 * 25 = 50`.
* 'c' is at position 3 and has a reverse rank of 24: `3 * 24 = 72`.
* Total reverse degree = `26 + 50 + 72 = 148`.

---

## Intuition

The core idea is to simulate the process step-by-step: iterate through the string, compute each character's reverse score, multiply by its position, and accumulate the result.

To convert a lowercase letter to its standard alphabet position (1 to 26), we subtract 96 from its ASCII character value (since ASCII 'a' is 97, `97 - 96 = 1`). To reverse this rank so 'a' becomes 26 and 'z' becomes 1, we subtract the standard position from 27 (`27 - 1 = 26`).

---

## Approach

* `int ans = 0;`: Initializes a counter `ans` to store the running sum of the reverse degree.
* `int i = 1;`: Sets `i` to 1 to track the 1-based index of each character as we move through the string.
* `for( auto it: s )`: Starts a loop to visit every character in string `s` from left to right.
* `char ch = it;`: Copies the current character from the iterator `it` into variable `ch`.
* `int index = ch - 96;`: Computes the 1-based forward alphabet position of `ch` ('a' becomes 1, 'b' becomes 2, etc.).
* `int rev = 27 - index;`: Flips the alphabet position so 'a' becomes 26 and 'z' becomes 1.
* `ans = ans + ( i++ * rev );`: Multiplies the current position `i` by `rev`, adds the product to `ans`, and then increments `i` by 1 for the next loop pass.
* `return ans;`: Returns the final accumulated score after processing all characters.

---

## Dry Run

### Case 1: Typical case with distinct characters ("abc")

| `i` (before step) | `ch` | `index` (`ch - 96`) | `rev` (`27 - index`) | Added Value (`i * rev`) | `ans` (after step) | Action |
|---|---|---|---|---|---|---|
| 1 | 'a' | 1 | 26 | 1 * 26 = 26 | 26 | Process 'a', set `i` to 2 |
| 2 | 'b' | 2 | 25 | 2 * 25 = 50 | 76 | Process 'b', set `i` to 3 |
| 3 | 'c' | 3 | 24 | 3 * 24 = 72 | 148 | Process 'c', set `i` to 4 |

Final output: `148`

### Case 2: Case with repeating characters ("zaza")

| `i` (before step) | `ch` | `index` (`ch - 96`) | `rev` (`27 - index`) | Added Value (`i * rev`) | `ans` (after step) | Action |
|---|---|---|---|---|---|---|
| 1 | 'z' | 26 | 1 | 1 * 1 = 1 | 1 | Process 'z', set `i` to 2 |
| 2 | 'a' | 1 | 26 | 2 * 26 = 52 | 53 | Process 'a', set `i` to 3 |
| 3 | 'z' | 26 | 1 | 3 * 1 = 3 | 56 | Process 'z', set `i` to 4 |
| 4 | 'a' | 1 | 26 | 4 * 26 = 104 | 160 | Process 'a', set `i` to 5 |

Final output: `160`

---

## Time & Space Complexity

* **Time:** **O(N)** — where N is the length of the string `s`. The algorithm loops through the string exactly once and performs basic arithmetic operations at each character.
* **Space:** **O(1)** — only a constant number of scalar integer variables (`ans`, `i`, `index`, `rev`) are used, consuming no additional memory relative to the input size.

### Is this optimal?

**Yes, the algorithmic complexity is already optimal.** Any correct algorithm must read every character in the string at least once, requiring at least O(N) time. Storing no extra memory requires O(1) space.

However, we can simplify the code readability by directly writing `'z' - ch + 1` instead of hardcoding ASCII magic numbers like `96` and `27`:

```cpp
// Direct calculation without magic numbers:
int rev = 'z' - ch + 1;
```

This change does not change the O(N) time or O(1) space complexity, but it makes the code clearer and less prone to offset bugs.

---

## Edge Cases Handled

* **Single Character Strings (e.g., `s = "a"` or `s = "z"`):** Works correctly on the first iteration where `i = 1`.
* **Alphabet Boundary Characters ('a' and 'z'):** Correctly maps 'a' to 26 and 'z' to 1 without off-by-one errors.
* **Repeated Characters (e.g., `s = "zaza"`):** Accurately accounts for position multiplier increments even when character values repeat.
* **Maximum Input Length (N = 1000):** The maximum possible sum for a 1000-character string is around `1000 * 26 * 1000 / 2 = 13,000,000`, which comfortably fits inside a standard 32-bit signed integer (`int`) without overflowing.
