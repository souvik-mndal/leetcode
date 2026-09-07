![Runtime](https://img.shields.io/badge/Runtime-6%20ms%20(beats%2011.46%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-12.1%20MB%20(beats%2011.68%25)-red?style=for-the-badge)

---

## Problem Explained

The problem asks us to take any whole number from zero up to about two billion (specifically 2^31 - 1) and turn it into its exact name in English words. 

For example, if you feed the number 123 into the program, it should give you back the string "One Hundred Twenty Three". If you give it 12345, it should return "Twelve Thousand Three Hundred Forty Five". The rules are just like how we speak numbers out loud in real life: grouping them into billions, millions, thousands, and hundreds, followed by the tens and ones digits.

## Intuition

The core idea is **greedy subtraction combined with recursion**. 

Think about how you say a big number like 12345 in your head. You spot the biggest chunk first: "Twelve Thousand". Then you look at what is left over (345), find the next biggest chunk ("Three Hundred"), and repeat until nothing is left. 

We can set up a list of all important English number words sorted from largest to smallest (from "Billion" all the way down to "One"). Then, we loop through that list. As soon as we find a word whose value fits into our number, we use it. 

The clever trick in this specific code is **recursion** (a function calling itself). Whenever our number is 100 or greater, we use recursion to figure out how many hundreds or thousands we have before printing the word itself. For example, when processing 12345, when we hit the "Thousand" rule, we divide by 1000 to get 12, and we run our solver on 12 to get "Twelve" before adding "Thousand".

## Approach

Here is how the code works, step by step:

- `if( n == 0 ) return "Zero";`: Checks if the input number is zero. If it is, it immediately stops and returns the word "Zero" because zero is a special base case.
- `vector<pair<int, string>> v = ...;`: Sets up a lookup list pairing integer values with their English word strings, sorted in strictly descending order from 1,000,000,000 down to 1.
- `string ans = "";`: Initializes an empty string that will collect our final English words sentence as we build it.
- `for( auto &it : v )`: Loops through our list of number-word pairs from largest to smallest to find the biggest chunks that fit into our current number.
- `if( n >= it.first )`: Checks if our remaining number is large enough to contain the current pair's value (stored in `it.first`).
- `int temp = n / it.first;`: Figures out how many times this value fits into our number (for instance, dividing 12345 by 1000 gives 12).
- `if( n>=100) ans += solve(temp);`: If the number is 100 or higher, it recursively calls `solve` on that multiplier (`temp`) to translate it into words first (like turning 12 into "Twelve").
- `ans = ans + it.second + " ";`: Appends the current word (like "Thousand" or "Hundred") and a space to our running answer string.
- `n %= it.first;`: Updates our number `n` to keep only the remainder after taking away the part we just converted, so we can process the rest in the next loop iterations.
- `string ans = solve(num);`: The main function `numberToWords` starts the process by calling our recursive `solve` helper on the full input number.
- `if( ans.back() == ' ') ans.pop_back();`: Removes the trailing extra space left at the very end of our built-up string.
- `return ans;`: Sends the final cleaned-up English words string back to the caller.

## Dry Run

Let's trace two examples to see how the variables change.

### Case 1: Typical case (num = 123)

| Step | Current `n` | Loop Match (`it.first`, `it.second`) | `temp` (`n / it.first`) | `n >= 100` recursive call | `ans` string value | Remainder `n %= it.first` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 123 | 100, "Hundred" | 123 / 100 = 1 | `solve(1)` -> "One" | "One Hundred " | 123 % 100 = 23 |
| 2 | 23 | 20, "Twenty" | 23 / 20 = 1 | Skipped (23 < 100) | "One Hundred Twenty " | 23 % 20 = 3 |
| 3 | 3 | 3, "Three" | 3 / 3 = 1 | Skipped (3 < 100) | "One Hundred Twenty Three " | 3 % 3 = 0 |

*End of loop.* The trailing space is popped off, leaving `"One Hundred Twenty Three"`.

### Case 2: Edge case with larger grouping (num = 12345)

| Step | Current `n` | Loop Match (`it.first`, `it.second`) | `temp` (`n / it.first`) | `n >= 100` recursive call | `ans` string value | Remainder `n %= it.first` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 12345 | 1000, "Thousand" | 12345 / 1000 = 12 | `solve(12)` -> "Twelve" | "Twelve Thousand " | 12345 % 1000 = 345 |
| 2 | 345 | 100, "Hundred" | 345 / 100 = 3 | `solve(3)` -> "Three" | "Twelve Thousand Three Hundred " | 345 % 100 = 45 |
| 3 | 45 | 40, "Forty" | 45 / 40 = 1 | Skipped (45 < 100) | "Twelve Thousand Three Hundred Forty " | 45 % 40 = 5 |
| 4 | 5 | 5, "Five" | 5 / 5 = 1 | Skipped (5 < 100) | "Twelve Thousand Three Hundred Forty Five " | 5 % 5 = 0 |

*End of loop.* The trailing space is popped off, leaving `"Twelve Thousand Three Hundred Forty Five"`.

## Time & Space Complexity

- **Time:** O(1) — Because the input integer has a strict maximum upper limit (2^31 - 1, which is about 2 billion), the number of digits is never greater than 10. The loop always runs a fixed maximum number of times regardless of how large the input grows relative to an infinite scale. Therefore, time complexity is constant time.
- **Space:** O(1) — The vector holding the words is a fixed size (31 items), and recursion depth is very small (at most a few levels deep), meaning memory usage does not scale with a growing input size.

### Is this already optimal?

The code is already running at **O(1) constant time and space**, which is the absolute theoretical best possible complexity for this problem since the input size is bounded by a fixed maximum integer size. No further improvement to time or space complexity is possible. 

However, looking at the performance score (beating only 11% of submissions), the code is slightly slow in practice. This is because it recreates and loops through a large vector of 31 pairs on *every single recursive call*. We could make it run faster in practice by making the word list a `static const` array so it is only built once, but the underlying time complexity class remains O(1).

## Edge Cases Handled

- **Zero (`num = 0`):** Handled immediately by the explicit base case check `if( n == 0 ) return "Zero";`, preventing infinite loops or empty strings.
- **Maximum constraint value (`num = 2^31 - 1`):** Correctly handled because the lookup table includes values up to 1,000,000,000 (Billion), which covers numbers in the two-billion range without overflowing standard integer limits.
- **Exact multiples of thousands/millions:** Numbers like 1000 or 1000000 skip unnecessary trailing words smoothly because the modulo operation leaves a remainder of zero, which simply fails future checks in the loop.
