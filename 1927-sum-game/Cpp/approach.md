![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2047.55%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.09%20MB%20(beats%2033.57%25)-orange?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a turn-based game on a string of numbers of even length. The string is split into two equal halves: a left half and a right half. 

Some characters in the string are digits (`0` through `9`), while others are question marks (`?`). 

- Alice goes first, and players take turns.
- On each turn, a player replaces one `?` with any digit from `0` to `9`.
- The game ends when all `?` characters are replaced.

The goal depends on the player:
- **Bob wins** if the sum of digits in the left half equals the sum of digits in the right half.
- **Alice wins** if the two sums are not equal.

Your job is to determine if Alice can force a win assuming both players play with the best possible strategy. Return `true` if Alice wins, and `false` if Bob wins.

---

## Intuition

At first, this looks like a complex game theory problem where we need to simulate every possible turn. However, Bob has a very clean defense strategy.

Whenever Alice places a digit `d` in a question mark, Bob can counter her move. 
- If Bob plays in a question mark on the **same side**, he can pick `9 - d`. That way, every pair of question marks played on the same side by the two players always adds up to `9`.
- This means each question mark on that side contributes an average of `4.5` (which is `9 / 2`) to the final sum!

If there is an odd number of question marks overall, Alice gets the very last move. She can easily pick a digit that breaks any balance Bob tried to build, guaranteeing an Alice win.

If there is an even number of total question marks, Bob can always pair up every move. As long as the starting values balance out when treating each `?` as `4.5`, Bob can guarantee a tie in sums and win. If they do not balance out to zero under this `4.5` value rule, Alice can force a win.

So the entire problem reduces to a quick math calculation: replace every `?` with `4.5`, sum up the left half, subtract the right half, and check if the result is `0.0`.

---

## Approach

Here is how the code works step-by-step:

- `const int n = num.length();`: Measures the total length of the string `num`.
- `double ans = 0.0;`: Creates a floating-point tracker variable `ans` to hold the net difference between the left half and right half.
- `for (int i = 0; i < n / 2; ++i)`: Loops through the left half of the string.
- `ans += getExpectation(num[i]);`: Adds the value of each character in the left half to `ans`. If it is a digit, it adds its numeric value. If it is `?`, it adds `4.5`.
- `for (int i = n / 2; i < n; ++i)`: Loops through the right half of the string.
- `ans -= getExpectation(num[i]);`: Subtracts the value of each character in the right half from `ans`.
- `return ans != 0.0;`: Returns `true` if the final net value `ans` is not equal to `0.0` (meaning Alice wins). Returns `false` if `ans` is `0.0` (meaning Bob wins).
- `double getExpectation(char c)`: A private helper function that returns `4.5` if `c` is `'?'`, or converts `c` to its integer value `c - '0'` if it is a digit.

---

## Dry Run

### Case 1: `num = "25??"`

Left half: `"25"`  
Right half: `"??"`

| Loop Index `i` | Character `num[i]` | `getExpectation(num[i])` | Running `ans` | Action |
| :--- | :--- | :--- | :--- | :--- |
| Start | - | - | `0.0` | Initialize `ans = 0.0` |
| `0` | `'2'` | `2.0` | `2.0` | Left side: add `2.0` |
| `1` | `'5'` | `5.0` | `7.0` | Left side: add `5.0` |
| `2` | `'?'` | `4.5` | `2.5` | Right side: subtract `4.5` |
| `3` | `'?'` | `4.5` | `-2.0` | Right side: subtract `4.5` |

Final check: `ans` is `-2.0`, which is not `0.0`.  
Result: Returns `true` (Alice wins).

---

### Case 2: `num = "?3295???"`

Left half: `"?329"`  
Right half: `"5???"`

| Loop Index `i` | Character `num[i]` | `getExpectation(num[i])` | Running `ans` | Action |
| :--- | :--- | :--- | :--- | :--- |
| Start | - | - | `0.0` | Initialize `ans = 0.0` |
| `0` | `'?'` | `4.5` | `4.5` | Left side: add `4.5` |
| `1` | `'3'` | `3.0` | `7.5` | Left side: add `3.0` |
| `2` | `'2'` | `2.0` | `9.5` | Left side: add `2.0` |
| `3` | `'9'` | `9.0` | `18.5` | Left side: add `9.0` |
| `4` | `'5'` | `5.0` | `13.5` | Right side: subtract `5.0` |
| `5` | `'?'` | `4.5` | `9.0` | Right side: subtract `4.5` |
| `6` | `'?'` | `4.5` | `4.5` | Right side: subtract `4.5` |
| `7` | `'?'` | `4.5` | `0.0` | Right side: subtract `4.5` |

Final check: `ans` is `0.0`, which equals `0.0`.  
Result: Returns `false` (Bob wins).

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — The algorithm loops through the string `num` of length `n` exactly once in two sequential loops.
- **Space Complexity:** **O(1)** — It uses a constant amount of memory for variables (`n`, `ans`, and `i`).

### Is this optimal?

**Yes, the time and space complexity are optimal.** We must inspect every character in the string at least once to know its digits and question marks, which takes O(n) time. Storing only a variable requires O(1) space.

While floating-point math can sometimes suffer from precision issues, using `4.5` is completely exact in binary floating-point representation, so it works reliably.

If you want to avoid floating-point math entirely and write cleaner integer-based C++, you can multiply everything by `2` (making digits worth `2 * digit` and `'?'` worth `9`).

```cpp
// Integer alternative logic (same overall complexity)
int sum_diff = 0;
int q_diff = 0;

for (int i = 0; i < n / 2; ++i) {
    if (num[i] == '?') q_diff++;
    else sum_diff += num[i] - '0';
}
for (int i = n / 2; i < n; ++i) {
    if (num[i] == '?') q_diff--;
    else sum_diff -= num[i] - '0';
}

// Bob wins if the question mark difference can balance out the sum difference
return sum_diff * 2 + q_diff * 9 != 0;
```

Both versions are **O(n) Time** and **O(1) Space**, which is the theoretical best possible performance.

---

## Edge Cases Handled

- **No Question Marks (e.g., `"5023"`)**: The function directly sums the left digits and right digits, returning `false` if they are equal and `true` if they are not.
- **All Question Marks (e.g., `"????"`)**: Left gets `4.5 + 4.5 = 9.0` and Right gets `4.5 + 4.5 = 9.0`. `ans` becomes `0.0`, correctly returning `false` (Bob can mirror every move).
- **Unequal Number of Question Marks**: Handled naturally by the math equation.
- **Maximum Input Length (10^5)**: Performs standard O(n) iteration without recursion or dynamic allocation, avoiding stack overflow or memory limit issues.
