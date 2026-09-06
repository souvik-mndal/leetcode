![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2076.32%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-14.2%20MB%20(beats%2039.69%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a string of text, like `"PAYPALISHIRING"`. The problem asks you to take this text and write it out going down and up diagonally, like a zigzag (or a wave), across a specific number of rows. 

For example, if you choose `numRows = 3`, you write the letters down the first column, then diagonally up and to the right for the next column, and repeat:

```text
P   A   H   N
A P L S I I G
Y   I   R
```

Once the letters are arranged in this zigzag shape, you read them row by row, from left to right. Row 1 gives `"PAHN"`, Row 2 gives `"APLSIIG"`, and Row 3 gives `"YIR"`. When you join them all together, you get the final output: `"PAHNAPLSIIGYIR"`. If `numRows = 1`, the letters just stay in a single straight line, and the string doesn't change at all.

## Intuition

The "aha" moment is realizing that we do not need to build a massive two-dimensional grid or a complex geometric coordinate system to solve this. Instead, we can think of the zigzag pattern as a traffic cop directing letters into separate lanes (rows). 

We create a list of strings called `store`, where each string represents one row. As we walk through the input string character by character, we just need to figure out which row the current character belongs in. The direction of travel alternates:
1. We move **down** the rows from row 0 to `numRows - 1`.
2. Once we hit the bottom row, we immediately turn around and move **up diagonally** back toward row 0.

By simulating this downward and upward bounce using simple loops, we naturally drop each character into its correct row bucket. At the end, we just glue all the rows together.

## Approach

Here is how the code makes this happen, step by step:

- `vector<string>store(numRows);`: This creates a collection of empty strings, with one string for each row requested. This is where we will collect our characters row by row.
- `int i=0,n=s.size();`: We set up an index `i` to track our current position in the input string `s`, and `n` to store the total length of the string so we know when to stop.
- `while( i<n ){`: This outer loop keeps running as long as there are still characters left in the input string to process.
- `for( int j=0 ; j<numRows && i<n ; j++ ){`: This first inner loop moves **down** the rows from top (`j = 0`) to bottom (`j = numRows - 1`). It grabs the next character from `s`, appends it to the correct row string in `store[j]`, and advances our string pointer `i`.
- `for( int k=numRows-2 ; k>0 && i<n ; k-- ){`: This second inner loop handles the **upward diagonal** travel. It starts right above the bottom row (`numRows - 2`) and moves upward toward row 1 (stopping before row 0, because row 0 was already handled at the start of the downward pass). It appends characters to `store[k]` and advances `i`.
- `string ans;`: We create a final blank string that will hold our combined output.
- `for( auto ii:store ){ ans+=ii; }`: We loop through each row string inside `store` from top to bottom and paste them end-to-end into our `ans` string.
- `return ans;`: We return the fully assembled zigzag string.

## Dry Run

Let us trace the code using two examples to see how the variables change.

### Case 1: Typical case with `s = "PAYPALISHIRING"`, `numRows = 3`

| i | j or k | Action | `store` contents |
|---|---|---|---|
| 0 | j = 0 | Append `s[0]` ('P') to row 0 | `["P", "", ""]` |
| 1 | j = 1 | Append `s[1]` ('A') to row 1 | `["P", "A", ""]` |
| 2 | j = 2 | Append `s[2]` ('Y') to row 2 (hits bottom) | `["P", "A", "Y"]` |
| 3 | k = 1 | Append `s[3]` ('P') moving up to row 1 | `["P", "AP", "Y"]` |
| 4 | j = 0 | Down pass starts again, append `s[4]` ('A') to row 0 | `["PA", "AP", "Y"]` |
| ... | ... | Continues bouncing down and up until all 14 characters are placed | `["PAHN", "APLSIIG", "YIR"]` |
| 14 | - | Loop finishes (`i == n`). Rows are joined together. | Returns `"PAHNAPLSIIGYIR"` |

### Case 2: Edge case with single row (`s = "A"`, `numRows = 1`)

| i | j or k | Action | `store` contents |
|---|---|---|---|
| 0 | j = 0 | Append `s[0]` ('A') to row 0. `numRows` loop ends because `j` reaches `numRows` (1). | `["A"]` |
| 1 | - | While loop checks `i < n` (1 < 1 is false), loop terminates. | `["A"]` |
| 1 | - | Join rows together (`ans = "A"`). | Returns `"A"` |

## Time & Space Complexity

- **Time:** O(n) — where `n` is the length of the string `s`. Every character in the string is visited a constant number of times during the down and up loops, and assembling the final string takes linear time relative to the total number of characters.
- **Space:** O(n) — because we store all the characters of the input string across our row vectors and build a final string of length `n`.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this solution is already optimal in terms of time complexity. Because every character in the string must be read and placed into its correct row position in the output, any solution requires at least O(n) time. 

However, we can make a slight optimization to the **space complexity** and minor constant factors by skipping the vector of strings entirely. Instead of collecting characters row-by-row and joining them at the end, you can use math to directly calculate the character indices for each row in the original string, skipping straight through row 0 to `numRows - 1` in a single pass. 

Even without that tweak, the current approach runs in linear time and easily passes all performance benchmarks. No further algorithmic improvement is necessary.

## Edge Cases Handled

- **Single row (`numRows = 1`):** The downward loop runs for `j = 0`, immediately finishes, and the upward loop is skipped because `numRows - 2` is less than 0. The string is returned unchanged, preventing infinite loops or out-of-bounds errors.
- **Rows greater than string length (`numRows >= s.length`):** The inner loops naturally stop because the condition `i < n` halts processing as soon as the string runs out of characters, leaving extra rows empty without crashing.
- **Minimum length string (`s.length = 1`):** Handled cleanly as the loops execute for just a single character and return it immediately.
