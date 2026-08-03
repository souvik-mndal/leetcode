![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2076.58%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.19%20MB%20(beats%2039.86%25)-orange?style=for-the-badge)

---

## Problem Explained

The problem asks you to take a string and rearrange its characters in a visual "zigzag" pattern across a fixed number of rows. Once the characters are placed in this pattern, you read them off row by row, from top to bottom, to produce a new string.

For example, take the word `"PAYPALISHIRING"` with `numRows = 3`:

1. You write characters straight down the rows until you hit the bottom row.
2. Then, you write characters diagonally up and to the right until you reach the top row.
3. You repeat this down-and-up process until all characters are used.

Visually, it looks like this:

```text
P   A   H   N
A P L S I I G
Y   I   R
```

Reading this line by line gives:
- Row 0: `"PAHN"`
- Row 1: `"APLSIIG"`
- Row 2: `"YIR"`

Combining these rows gives the output: `"PAHNAPLSIIGYIR"`.

---

## Intuition

You do not need to build a full two-dimensional grid with empty spaces. 

Instead, imagine having a separate box (a string buffer) for each row. You process the original string character by character and bounce between rows:
- Walk down from the top row to the bottom row, placing one character per row.
- Walk up diagonally from near the bottom row back to near the top row.

Once all characters are placed into their row boxes, simply stitch the rows together in order from top to bottom.

---

## Approach

Here is how the code implements this step-by-step:

- **Step 1: Set up row storage.** Create a dynamic array (vector) of strings named `store` with a size of `numRows`. Each index in `store` acts as a text box for that specific row.
- **Step 2: Track current position.** Use an index variable `i` set to `0` to keep track of which character in string `s` is being placed.
- **Step 3: Process in down-and-up cycles.** Run a loop while `i` is less than the total length of `s`:
  - **Move Down:** Loop a counter `j` from row `0` down to `numRows - 1`. Append `s[i]` to `store[j]` and increment `i`. If `i` reaches the end of string `s`, stop early.
  - **Move Up Diagonally:** Loop a counter `k` from row `numRows - 2` down to row `1` (skipping the very top and bottom rows to avoid duplicating ends). Append `s[i]` to `store[k]` and increment `i`. Stop early if `i` reaches the end of `s`.
- **Step 4: Combine the rows.** Create a single string `ans`. Iterate through every row string in `store` and append it to `ans`.
- **Step 5: Return result.** Return `ans` as the final converted string.

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where N is the length of string `s`. Every character in `s` is visited once during placement and once during concatenation.
- **Space Complexity:** **O(N)** — extra space is used to hold the characters inside the `store` vector across all row strings.

### Can Space Complexity Be Improved?

Yes. While **O(N)** time is already optimal (you must read all N characters at least once), extra **space** can be reduced from **O(N)** to **O(1)** (ignoring the output string itself). 

Instead of storing rows in a vector, you can calculate the exact index of each character mathematically. One full down-and-up cycle repeats every `cycleLen = 2 * numRows - 2` characters.

Here is the key snippet for the **O(1)** extra space approach:

```cpp
string convert(string s, int numRows) {
    if (numRows == 1 || numRows >= s.size()) return s;

    string ans = "";
    int cycleLen = 2 * numRows - 2;

    for (int r = 0; r < numRows; r++) {
        for (int j = 0; j + r < s.size(); j += cycleLen) {
            ans += s[j + r]; // Vertically aligned character
            
            // Diagonally aligned character (middle rows only)
            if (r != 0 && r != numRows - 1 && j + cycleLen - r < s.size()) {
                ans += s[j + cycleLen - r];
            }
        }
    }
    return ans;
}
```

- **Improved Complexity:** **Time:** O(N), **Space:** O(1) auxiliary space.
- **Theoretical Best:** **Time:** O(N), **Space:** O(1) auxiliary space. The improved index-math method reaches this theoretical limit.

---

## Edge Cases Handled

- **`numRows = 1`:** The downward loop puts characters into `store[0]`. The diagonal loop condition (`k = numRows - 2 = -1`) evaluates to `k > 0`, which is false, so it never runs. The algorithm safely puts all characters into a single row without out-of-bounds errors.
- **`numRows >= s.length()`:** The downward loop places each character into its own row string up to the end of `s`. The inner guard `i < n` stops the execution safely before any empty rows or diagonal steps crash the program.
- **Single character string (`s = "A"`):** Handled cleanly due to `i < n` safety checks in every loop step.
- **Punctuation and spaces:** Characters are copied directly without filtering, preserving all original characters as required.
