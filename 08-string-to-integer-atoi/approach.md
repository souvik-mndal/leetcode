![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.19%20MB%20(beats%2055.68%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to convert a text string into a standard 32-bit signed integer (a numbers-only format used in code). This process is often called **atoi** (ASCII to Integer).

You process the string from left to right following these rules:
1. **Ignore leading spaces:** Skip any empty spaces at the very beginning.
2. **Read the sign:** Check if the next character is `'-'` or `'+'`. A `'-'` means the final answer will be negative.
3. **Read digits:** Keep reading characters as long as they are numbers (`'0'` through `'9'`). Stop as soon as you hit a non-digit character or the end of the string.
4. **Handle size limits:** The final number must fit inside a standard 32-bit signed integer range, which is from `-2147483648` (`INT_MIN`) up to `2147483647` (`INT_MAX`). If the number is too big, cap it at `2147483647`. If it is too small, cap it at `-2147483648`.
5. **Return the result:** If no digits were found, return `0`.

**Example:** 
If the input is `" -042abc"`, you skip the spaces, see the `'-'` sign, read `042` as `42`, and stop at `'a'`. The output is `-42`.

---

## Intuition

Think of building a number on a simple calculator. Every time you type a new digit, you push all previous digits one place to the left (by multiplying by 10) and add the new digit. 

For example, to build `42`:
- Start with `0`.
- See `'4'`: `0 * 10 + 4 = 4`.
- See `'2'`: `4 * 10 + 2 = 42`.

The tricky part is preventing integer **overflow** (a situation where a number gets too large for memory to hold). Before you multiply by 10, you must check if doing so would exceed `2147483647` (`INT_MAX`). 

Since `2147483647 / 10 = 214748364`:
- If your current total is strictly greater than `214748364`, multiplying by 10 will definitely overflow.
- If your current total equals `214748364`, you can only add a digit up to `7` (because `2147483647` ends in `7`). Any digit `'8'` or `'9'` will overflow.

---

## Approach

Here is the step-by-step breakdown of how the code works:

* **Skip Spaces:** Use a loop to advance the string index `i` past any leading spaces `' '`.
* **Detect Sign:** Check if the character at `i` is `'-'` or `'+'`. If it is `'-'`, set a flag (`sign = true`) to remember the number is negative. Advance `i` by 1.
* **Convert Digits in a Loop:** Loop through the string as long as `i` is within bounds and the character `s[i]` is a digit:
  * **Check for potential overflow:** Before adding the current digit, check if `ans > INT_MAX / 10`, or if `ans == INT_MAX / 10` and the current digit character is greater than `'7'`.
  * **Handle cap on overflow:** If overflow is detected, immediately return `INT_MIN` (-2147483648) if `sign` is true, or `INT_MAX` (2147483647) if `sign` is false.
  * **Append digit:** Convert character `s[i]` to a number by subtracting the ASCII value of `'0'` (`s[i] - '0'`). Update `ans = ans * 10 + digit`.
  * **Move forward:** Increment `i` to look at the next character.
* **Apply Sign & Return:** Once the loop ends, return `-ans` if `sign` is true, otherwise return `ans`.

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — where `n` is the length of the string `s`. In the worst case, the code scans through the string once from left to right.
- **Space Complexity:** **O(1)** — memory usage remains constant because only a few integer variables and a boolean flag are used.

### Is this optimal?

**Yes, this code is already at the theoretical limit of efficiency.**

- **Time:** To convert a string to an integer, you must look at each relevant character at least once to know its value. Therefore, you cannot do better than **O(n)** time.
- **Space:** You only store a few state variables, which takes minimal extra memory. Therefore, **O(1)** space is the best possible memory usage.

No algorithmic change can improve these complexities.

---

## Edge Cases Handled

* **Leading Whitespace:** Skips all initial `' '` characters (e.g., `"   42"` becomes `42`).
* **Explicit Sign Symbols:** Correctly processes both positive and negative signs (e.g., `"-42"` becomes `-42`, `"+1"` becomes `1`).
* **Non-digit Characters:** Stops reading as soon as an invalid character appears (e.g., `"1337c0d3"` becomes `1337`, `"words and 987"` becomes `0`).
* **Overflow and Underflow:** Numbers exceeding 32-bit limits are correctly capped at `2147483647` (`INT_MAX`) or `-2147483648` (`INT_MIN`).
* **Invalid Sign Sequence:** Inputs like `"0-1"` read `0` first, then stop when hitting `'-'`, returning `0`.
