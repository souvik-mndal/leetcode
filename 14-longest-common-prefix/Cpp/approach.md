![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.8%20MB%20(beats%2059.00%25)-yellow?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the longest sequence of characters that appears at the very beginning (the prefix) of **every** string in a given list.

If all strings share a starting part, we return that shared starting string. If they do not start with the exact same character, or if there is no shared sequence at all, we return an empty string `""`.

For example:
- Given `["flower", "flow", "flight"]`: All three strings start with `"f"`, and all three start with `"fl"`. But the third word has `'i'` next while the first two have `'o'`. So the longest common prefix is `"fl"`.
- Given `["dog", "racecar", "car"]`: The first word starts with `'d'`, the second with `'r'`. They share no common starting letter, so the answer is `""`.

---

## Intuition

Think of listing the words line-by-line, matching up their characters in vertical columns:

```text
Column: 0 1 2 3 4 5
Word 0: f l o w e r
Word 1: f l o w
Word 2: f l i g h t
        | | |
        v v v
Match:  Y Y N  --> Prefix stops at "fl"
```

We look at column `0` across all words. If every word has `'f'`, we keep `'f'` and move to column `1`. Then we check column `1` across all words. If every word has `'l'`, we keep `'l'` and move to column `2`. 

At column `2`, word 0 has `'o'`, word 1 has `'o'`, but word 2 has `'i'`. Because there is a mismatch, we immediately stop and return everything matched so far (`"fl"`).

---

## Approach

Here is how the code executes the vertical column check step-by-step:

- `string ans = "";` — Creates an empty string `ans` to build up our answer character-by-character.
- `int index = 0;` — Creates a column counter `index` starting at `0`, representing the character position we are currently checking across all strings.
- `if( strs.size() == 1 ){ return strs[0]; }` — Checks if the input array contains only one string. If so, that string is its own prefix, so we return `strs[0]` immediately.
- `while( true )` — Starts an endless loop that keeps inspecting columns until a mismatch occurs or a string ends.
- `bool check = true;` — Sets a flag `check` to `true` at the beginning of each column check. This flag tracks whether all strings match `strs[0]` at the current `index`.
- `for( int i=1 ; i<strs.size() ; i++ )` — Loops through all other strings in the array, starting from the second string (`i = 1`) up to the last string.
- `if( index < strs[i].size() && strs[0][index] == strs[i][index] )` — Checks if string `strs[i]` is long enough to have a character at `index`, and if that character equals `strs[0][index]`. If both are true, `check` stays `1` (true).
- `else{ check = false; break; }` — Runs if `strs[i]` is too short or if its character does not match. Sets `check` to `false` and breaks out of the inner loop immediately.
- `if( check ){ ans.push_back( strs[0][index++] ); }` — If every string matched at the current `index`, appends character `strs[0][index]` to `ans` and increases `index` by 1.
- `else{ break; }` — If `check` is false, breaks out of the outer `while` loop because the prefix cannot extend any further.
- `return ans;` — Returns the final collected prefix string `ans`.

---

## Dry Run

### Case 1: Standard case with a common prefix
`strs = ["flower", "flow", "flight"]`

| `index` | `i` | `check` | `ans` | Action |
|---|---|---|---|---|
| 0 | - | true | `""` | `strs.size()` is 3. Start `while` loop at `index = 0`. |
| 0 | 1 | true | `""` | `0 < 4` and `'f' == 'f'` in `"flow"`. Match. |
| 0 | 2 | true | `""` | `0 < 6` and `'f' == 'f'` in `"flight"`. Match. |
| 0 | - | true | `"f"` | Inner loop ends. `check` is true. Append `'f'`, `index` becomes 1. |
| 1 | 1 | true | `"f"` | `1 < 4` and `'l' == 'l'` in `"flow"`. Match. |
| 1 | 2 | true | `"f"` | `1 < 6` and `'l' == 'l'` in `"flight"`. Match. |
| 1 | - | true | `"fl"` | Inner loop ends. `check` is true. Append `'l'`, `index` becomes 2. |
| 2 | 1 | true | `"fl"` | `2 < 4` and `'o' == 'o'` in `"flow"`. Match. |
| 2 | 2 | false | `"fl"` | `2 < 6` but `'o' != 'i'` in `"flight"`. `check = false`, break inner loop. |
| 2 | - | false | `"fl"` | `check` is false. Break `while` loop. Return `"fl"`. |

### Case 2: No common prefix
`strs = ["dog", "racecar", "car"]`

| `index` | `i` | `check` | `ans` | Action |
|---|---|---|---|---|
| 0 | - | true | `""` | `strs.size()` is 3. Start `while` loop at `index = 0`. |
| 0 | 1 | false | `""` | Mismatch: `strs[0][0]` (`'d'`) != `strs[1][0]` (`'r'`). `check = false`, break inner loop. |
| 0 | - | false | `""` | `check` is false. Break `while` loop. Return `""`. |

---

## Time & Space Complexity

- **Time Complexity:** **O(N * M)** where **N** is the number of strings and **M** is the length of the shortest string (or length of the common prefix). In the worst case (e.g., all strings are identical), the code compares characters across all **N** strings up to length **M**.
- **Space Complexity:** **O(1)** extra auxiliary space, ignoring the space needed to return the output string `ans` (which takes up to **O(M)** space).

### Can it be improved?

In terms of Big-O complexity, **O(N * M)** time is already optimal because we must inspect characters until we find a mismatch. 

However, the code logic can be simplified to run faster in practice. Instead of manually maintaining a `check` flag and calling `ans.push_back(...)` inside a while loop, we can directly return a slice (substring) of the first string as soon as a mismatch occurs.

```cpp
string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";
    
    // Loop through each character position of the first string
    for (int i = 0; i < strs[0].size(); i++) {
        char c = strs[0][i];
        
        // Compare this character against all other strings
        for (int j = 1; j < strs.size(); j++) {
            // If out of bounds or character mismatch, return substring immediately
            if (i == strs[j].size() || strs[j][i] != c) {
                return strs[0].substr(0, i);
            }
        }
    }
    return strs[0]; // All characters of strs[0] matched everywhere
}
```

- `i == strs[j].size() || strs[j][i] != c` — Immediately catches when string `j` ends or has a different character at column `i`.
- `return strs[0].substr(0, i);` — Returns the prefix matching from index `0` up to `i` without needing boolean flags or extra string pushes.

**Resulting Complexity:**
- **Improved Time:** **O(N * M)** (same theoretical class, but executes fewer instructions per step).
- **Improved Space:** **O(1)** extra space (returns substring directly).
- **Theoretical Best:** **O(S)** where **S** is the total number of processed characters across all strings. This vertical approach reaches that theoretical best.

---

## Edge Cases Handled

- **Single string in input (`strs.size() == 1`):** Early check returns `strs[0]` immediately without running loops.
- **Strings of unequal length:** The condition `index < strs[i].size()` stops the check safely if a string ends earlier than `strs[0]`.
- **No shared starting character:** The code sets `check = false` on the very first character check (`index = 0`) and returns `""`.
- **Identical strings:** The code processes character by character until `index` reaches the end of the strings, returning the entire full string.
