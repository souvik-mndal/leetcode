![Runtime](https://img.shields.io/badge/Runtime-2%20ms%20(beats%2091.01%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.1%20MB%20(beats%2056.92%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a string made up of only three letters: **a**, **b**, and **c**. 

A **substring** is a slice of the string taken from anywhere inside it, without breaking the order of the letters. For example, in the string **"abc"**, the substrings are **"a"**, **"b"**, **"c"**, **"ab"**, **"bc"**, and **"abc"**.

This problem asks you to find the **total number of substrings** that contain **at least one** **a**, **at least one** **b**, and **at least one** **c**. 

If a substring has all three letters, we count it. If a substring appears multiple times in different positions, we count each occurrence separately.

---

## Intuition

The "aha" moment for this problem comes from a realization about **sliding windows**. 

If we find a valid substring that contains an **a**, a **b**, and a **c**, we don't just stop there. Any longer substring that extends *beyond* the end of this valid substring will *also* automatically be valid. 

For example, if the slice from index 0 to 2 is **"abc"** (which is valid), then extending it to index 3 gives **"abca"**, extending it to index 4 gives **"abcab"**, and so on. All of these extended substrings are *also* valid because they still contain the original **a**, **b**, and **c**. 

So, our strategy is:
1. Use two pointers, **st** (start) and **e** (end), to slide across the string and find the *first* window that contains at least one **a**, **b**, and **c**.
2. Once we find it, every possible ending from the current **e** pointer all the way to the end of the string forms a valid substring with our current **st** position. We add all of those to our **count** at once.
3. Then, we advance the **st** pointer forward to see if we can find more valid windows starting from the next character.

---

## Approach

Here is how the code implements this step-by-step:

- `int count = 0;` — Initializes our total tally of valid substrings to zero.
- `int a = 0; int b = 0; int c = 0;` — Creates three counters to track how many **a**s, **b**s, and **c**s are currently inside our active window.
- `int st = 0 , e = 0;` — Sets up our two sliding window pointers. **st** marks the start of the window, and **e** marks the end.
- `while( st < s.size()-2 )` — Runs a loop as long as the start pointer has enough room left in the string to possibly form a 3-character window.
- `if( a >= 1 && b >=1 && c >= 1 )` — Checks if our current window contains at least one **a**, one **b**, and one **c**.
- `count++;` — Counts the current valid window itself.
- `count += (s.size() - e);` — Adds all the extra valid substrings that can be formed by extending the window all the way to the end of the string.
- `if( s[st] == 'a' ) a--; else if( s[st] == 'b' ) b--; else c--;` — Removes the character at the **st** pointer from our window counts because we are about to shrink the window from the left.
- `st++;` — Moves the start pointer forward to test a new starting position.
- `else if( e >= s.size() ) break;` — Stops the loop if the end pointer has run past the end of the string.
- `else if( s[e] == 'a' ) { a++; e++; }` — Expands the window to the right by capturing an **a** at the **e** pointer and advancing **e**.
- `else if( s[e] == 'b' ) { b++; e++; }` — Expands the window to the right by capturing a **b** at the **e** pointer and advancing **e**.
- `else if( s[e] == 'c' ) { c++; e++; }` — Expands the window to the right by capturing a **c** at the **e** pointer and advancing **e**.
- `return count;` — Returns the final accumulated total of valid substrings.

---

## Dry Run

### Case 1: Typical case ("abc")

| `st` | `e` | `a` | `b` | `c` | `count` | Action |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | Initial state. Window is empty. |
| 0 | 1 | 1 | 0 | 0 | 0 | Sees 'a' at `e=0`, increments `a` and moves `e` to 1. |
| 0 | 2 | 1 | 1 | 0 | 0 | Sees 'b' at `e=1`, increments `b` and moves `e` to 2. |
| 0 | 3 | 1 | 1 | 1 | 0 | Sees 'c' at `e=2`, increments `c` and moves `e` to 3. |
| 0 | 3 | 1 | 1 | 1 | 1 | All three found! Adds 1 (for "abc") + (3 - 3) = 1. Shrinks window by moving `st` to 1 and decrementing `a`. |

*(The loop continues briefly to check remaining bounds, yielding a final count of 1).*

### Case 2: Edge case with repeated characters ("aaacb")

| `st` | `e` | `a` | `b` | `c` | `count` | Action |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | Initial state. |
| 0 | 3 | 3 | 0 | 0 | 0 | Expands `e` through indices 0, 1, 2, accumulating three 'a's (`a=3`). |
| 0 | 4 | 3 | 1 | 0 | 0 | Sees 'c' at `e=3`, increments `c` and moves `e` to 4. |
| 0 | 5 | 3 | 1 | 1 | 0 | Sees 'b' at `e=4`, increments `b` and moves `e` to 5. |
| 0 | 5 | 3 | 1 | 1 | 1 | All three found! Adds 1 + (5 - 5) = 1 (substring "aaacb"). Shrinks window by moving `st` to 1, decrementing `a` (`a=2`). |
| 1 | 5 | 2 | 1 | 1 | 2 | Valid again! Adds 1 + (5 - 5) = 1 (substring "aacb"). Shrinks window by moving `st` to 2, decrementing `a` (`a=1`). |
| 2 | 5 | 1 | 1 | 1 | 3 | Valid again! Adds 1 + (5 - 5) = 1 (substring "acb"). Shrinks window by moving `st` to 3, decrementing `a` (`a=0`). |

*(After this, `a` drops to 0, so the loop stops trying to shrink and finishes. Final count is 3).*

---

## Time & Space Complexity

- **Time:** O(N) — where N is the length of the string `s`. Both the `st` pointer and the `e` pointer move forward across the string at most once, meaning each character is visited a constant number of times.
- **Space:** O(1) — because we only use a few integer variables (`count`, `a`, `b`, `c`, `st`, `e`), which take up a fixed amount of memory regardless of how long the string is.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is **already optimal** in both time and space complexity. 

- **Time-wise:** To solve this problem, you fundamentally have to look at the characters in the string, which takes at least O(N) time. You cannot go faster than linear time because you must read the input.
- **Space-wise:** We only use a handful of scalar variables, achieving O(1) auxiliary space, which is the absolute minimum possible. 

No further improvement is possible.

---

## Edge Cases Handled

- **Minimum length strings:** The problem states the string length is at least 3 (`3 <= s.length`). The code handles this smoothly because the loop condition `st < s.size()-2` ensures we don't run out of bounds when checking windows of size 3.
- **Strings with heavy clustering of one character:** As seen in the "aaacb" test case, multiple identical characters in a row (like three **a**s) are safely counted without breaking the pointer logic.
- **Pointers reaching the end:** The check `else if( e >= s.size() ) break;` safely stops the loop if the right pointer hits the end of the string before the start pointer finishes its job.
