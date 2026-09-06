![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.2%20MB%20(beats%2054.03%25)-yellow?style=for-the-badge)

---

## Problem Explained

The problem asks us to take a string of text and turn it into a standard 32-bit signed integer (a whole number that can be positive or negative). 

Real-world text inputs are often messy. A string might start with random spaces, a plus or minus sign, numbers, and then suddenly end with letters or other symbols. We have to follow a strict set of rules to parse this text correctly:
1. Ignore any leading spaces at the very beginning of the string.
2. Check if the next character is a minus or a plus sign to decide if the final number is negative or positive. If neither is there, we assume it is positive.
3. Read characters one by one as long as they are digits (0 through 9). Stop reading as soon as we hit a non-digit character or reach the end of the string.
4. If no valid numbers were found at all, return 0.
5. If the number grows too large or too small to fit inside a standard 32-bit signed integer (meaning it goes above INT_MAX or below INT_MIN), we must clamp it to the maximum or minimum allowed limit instead of letting it break or wrap around.

For example, given the input string " -042", we skip the spaces, see the minus sign, read the digits 0, 4, and 2, and return the integer -42.

## Intuition

The core idea is to process the string from left to right in a single pass, doing a job similar to a state machine without needing a heavy framework. We handle the input in three sequential phases: stripping whitespace, capturing the sign, and accumulating digits. 

The biggest trap in this problem is **integer overflow**. If a string has many digits (like "999999999999"), multiplying our running total by 10 and adding the next digit will instantly crash or overflow standard integer limits. The trick is to check *before* we multiply and add. We look ahead to see if our current total is already larger than INT_MAX / 10, or if it equals INT_MAX / 10 and the next digit is greater than 7 (since INT_MAX ends in 7). If it is, we know for sure that adding the next digit will breach the limits, so we stop right there and return the boundary limit immediately.

## Approach

Here is how the code implements this logic, step by step:

- `while( s[i] == ' ' && i<n )` : This loop skips over any blank space characters at the beginning of the string by moving our index pointer `i` forward, as long as `i` stays within the string size `n`.
- `if( (s[i] == '-' || s[i] == '+') && i<n )` : Once spaces are cleared, this checks if the current character is a minus or plus sign. 
- `if( s[i] == '-' ) { sign = true; }` : If the character is a minus sign, we flip our `sign` boolean variable to `true` so we remember to make the final result negative later. Then we increment `i` to move past the sign.
- `while( i<n && isdigit(s[i]) )` : This loop processes all consecutive digit characters. It keeps running as long as we have not reached the end of the string and the current character is a valid number digit.
- `if( ans > INT_MAX/10 || (ans == INT_MAX/10 && s[i]>'7') )` : Inside the digit loop, this guard clause checks if multiplying `ans` by 10 and adding the next digit will overflow the 32-bit integer limit. If it will overflow, it immediately returns `INT_MIN` if `sign` is true, or `INT_MAX` if `sign` is false.
- `ans = ans * 10 + ( s[i] - '0' );` : If safe, this shifts our current accumulated answer one decimal place to the left (by multiplying by 10) and adds the numeric value of the current character (found by subtracting the character code for zero). We then increment `i`.
- `if( sign ) { return -ans; }` : After the digit loop finishes, if our `sign` flag is true, we negate our accumulated answer and return it. Otherwise, we return `ans` as a positive number.

## Dry Run

### Case 1: Typical case with spaces and a negative sign (s = " -042")

| i | s[i] | sign | ans | Action |
|---|---|---|---|---|
| 0 | ' ' | false | 0 | First space skipped, i increments to 1 |
| 1 | '-' | true | 0 | Minus sign found, sign becomes true, i increments to 2 |
| 2 | '0' | true | 0 | Digit '0' processed, ans becomes 0, i increments to 3 |
| 3 | '4' | true | 4 | Digit '4' processed, ans becomes 4, i increments to 4 |
| 4 | '2' | true | 42 | Digit '2' processed, ans becomes 42, i increments to 5 (loop ends) |
| - | - | true | -42 | Sign is true, returns -ans (-42) |

### Case 2: Edge case with trailing letters (s = "1337c0d3")

| i | s[i] | sign | ans | Action |
|---|---|---|---|---|
| 0 | '1' | false | 1 | Digit '1' processed, ans becomes 1, i increments to 1 |
| 1 | '3' | false | 13 | Digit '3' processed, ans becomes 13, i increments to 2 |
| 2 | '3' | false | 133 | Digit '3' processed, ans becomes 133, i increments to 3 |
| 3 | '7' | false | 1337 | Digit '7' processed, ans becomes 1337, i increments to 4 |
| 4 | 'c' | false | 1337 | Character 'c' is not a digit, loop terminates |
| - | - | false | 1337 | Sign is false, returns ans (1337) |

## Time & Space Complexity

- **Time:** O(n) — where n is the length of the string. In the worst-case scenario, we visit each character in the string at most once during our loops.
- **Space:** O(1) — we only use a few tracking variables (`i`, `ans`, `sign`, `n`), meaning our memory usage stays completely constant no matter how long the input string is.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal possible complexity. 

- **Time complexity optimality:** We must look at the characters in the string to convert them, and in the worst case (where the entire string is valid digits), we have to check every single character. You cannot process a sequence faster than visiting its elements, making O(n) the absolute best theoretical time limit.
- **Space complexity optimality:** We do not create any data structures that grow with the input size (like arrays, maps, or new strings). Keeping extra data strictly limited to a handful of scalar variables means we use O(1) extra space, which is the absolute minimum possible.

Because it hits both O(n) time and O(1) space, no further improvement is possible.

## Edge Cases Handled

- **Leading whitespace:** Correctly skipped by the initial while loop.
- **Missing sign:** Handled gracefully by defaulting the `sign` boolean to `false` and assuming a positive number.
- **Integer overflow and underflow:** Caught safely ahead of time by checking against `INT_MAX / 10` before multiplying and adding, returning `INT_MAX` or `INT_MIN` accordingly.
- **Non-digit characters mixed in:** The digit accumulation loop naturally stops the exact moment it hits a letter, symbol, or space (e.g., "1337c0d3" or "words and 987").
- **Empty strings or strings with only spaces:** If the string is empty or contains only spaces, the index `i` reaches `n` immediately, loops are skipped, and the code returns `0` safely.
