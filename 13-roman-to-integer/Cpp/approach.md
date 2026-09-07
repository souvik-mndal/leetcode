![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.1%20MB%20(beats%2073.18%25)-green?style=for-the-badge)

---

## Problem Explained

Roman numerals express numbers using seven symbols:
* **I** = 1
* **V** = 5
* **X** = 10
* **L** = 50
* **C** = 100
* **D** = 500
* **M** = 1000

Usually, Roman numerals are written from largest value to smallest value from left to right. To find the total value, you simply add all the symbol values together. For example, **VI** is 5 + 1 = 6, and **XII** is 10 + 1 + 1 = 12.

However, when a smaller symbol appears **before** a larger symbol, it means you must subtract the smaller value from the larger one. For example:
* **IV** is 5 - 1 = 4
* **IX** is 10 - 1 = 9
* **XL** is 50 - 10 = 40
* **XC** is 100 - 10 = 90
* **CD** is 500 - 100 = 400
* **CM** is 1000 - 100 = 900

Given a Roman numeral string `s`, convert it into its standard integer value.

---

## Intuition

The key insight is to look at symbols in pairs of two adjacent characters as you move from left to right.

When comparing a symbol with the symbol right after it:
1. **If the current symbol is smaller than the next symbol** (like **I** before **V**), it forms a subtraction rule. You subtract the current symbol's value from the next symbol's value, add that result to your total, and skip both characters.
2. **If the current symbol is larger than the next symbol** (like **V** before **I**), it is a regular addition. You add the current symbol's value to your total and move forward by one character.
3. **If both symbols are equal** (like **I** and **I**), you can add both values to your total at once and skip forward by two characters.

By stepping through the string this way, you naturally handle both regular additions and subtraction pairs.

---

## Approach

Here is how the code implements this logic step-by-step:

* `string roman[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};` — Defines an array listing Roman numeral symbols (including standard symbols and subtraction combos) sorted by value.
* `int value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};` — Defines a parallel array holding the integer value for each entry in `roman`.
* `int i=0, j=1;` — Sets up two pointers: `i` points to the current character, and `j` points to the next character in string `s`.
* `int num=0;` — Initializes a running accumulator variable `num` to store the final integer result.
* `while(i<s.length()){` — Begins a loop that runs until pointer `i` reaches the end of the input string `s`.
* `int value1=0, value2=0;` — Resets local variables `value1` and `value2` to 0 at the start of each iteration to store numerical values of symbols at positions `i` and `j`.
* `string check1=s.substr(i,1);` — Extracts the single-character substring at index `i`.
* `for( int x=0 ; x<13 ; x++){ if(roman[x]==check1){ value1=value[x]; break; } }` — Loops through the `roman` lookup array to find the integer value matching `check1` and stores it in `value1`.
* `if(j<s.length()){ string check=s.substr(j,1); ... }` — Checks if `j` is within bounds. If so, extracts the single character at `j` and finds its integer value from `roman`, storing it in `value2`.
* `if(value1>value2){ num=num+value1; i++; j++; }` — Handles the case where the left symbol is strictly larger than the right symbol. Adds `value1` to `num` and moves both pointers forward by 1 index.
* `if(value2>value1){ num=num+(value2-value1); i+=2; j+=2; }` — Handles a subtraction pair where the right symbol is larger than the left symbol. Adds `value2 - value1` to `num` and advances both pointers forward by 2 indices.
* `if(value1==value2){ num=num+value1+value2; i+=2; j+=2; }` — Handles equal consecutive symbols. Adds both `value1` and `value2` to `num` and advances both pointers forward by 2 indices.
* `return num;` — Returns the total computed integer after processing all characters in the string.

---

## Dry Run

### Case 1: Standard case with subtraction pairs (`s = "MCMXCIV"`)

* String length = 7. Initial values: `i = 0`, `j = 1`, `num = 0`.

| `i` | `j` | `check1` | `check` | `value1` | `value2` | Condition Met | Action | Updated `num` | Next `i`, `j` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 1 | "M" | "C" | 1000 | 100 | `value1 > value2` | Add `value1` (1000) | 1000 | `i=1, j=2` |
| 1 | 2 | "C" | "M" | 100 | 1000 | `value2 > value1` | Add `value2 - value1` (900) | 1900 | `i=3, j=4` |
| 3 | 4 | "X" | "C" | 10 | 100 | `value2 > value1` | Add `value2 - value1` (90) | 1990 | `i=5, j=6` |
| 5 | 6 | "I" | "V" | 1 | 5 | `value2 > value1` | Add `value2 - value1` (4) | 1994 | `i=7, j=8` |

* Loop ends because `i = 7` is not less than `s.length()` (7). Final output: **1994**.

---

### Case 2: Mixed repeating symbols and single remainder (`s = "LVIII"`)

* String length = 5. Initial values: `i = 0`, `j = 1`, `num = 0`.

| `i` | `j` | `check1` | `check` | `value1` | `value2` | Condition Met | Action | Updated `num` | Next `i`, `j` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 1 | "L" | "V" | 50 | 5 | `value1 > value2` | Add `value1` (50) | 50 | `i=1, j=2` |
| 1 | 2 | "V" | "I" | 5 | 1 | `value1 > value2` | Add `value1` (5) | 55 | `i=2, j=3` |
| 2 | 3 | "I" | "I" | 1 | 1 | `value1 == value2` | Add `value1 + value2` (2) | 57 | `i=4, j=5` |
| 4 | 5 | "I" | N/A | 1 | 0 | `value1 > value2` | Add `value1` (1) | 58 | `i=5, j=6` |

* Loop ends because `i = 5` is not less than `s.length()` (5). Final output: **58**.

---

## Time & Space Complexity

### Current Solution Analysis
* **Time Complexity:** **O(N)** where N is the length of string `s`. The outer `while` loop runs at most N times. Searching through the 13-element fixed lookup array and taking length-1 substrings takes constant O(1) time per character. Thus, total time is linear.
* **Space Complexity:** **O(1)** auxiliary space. The arrays `roman` and `value` have fixed sizes of 13 elements, using constant memory regardless of string length.

---

### Can It Be Improved?
The time complexity is already **O(N)** theoretically, which is the optimal lower bound because every character in the string must be read at least once.

However, the current code performs redundant work in practice:
1. It allocates dynamic string memory using `s.substr()` on every iteration.
2. It performs linear searches through a 13-element lookup array up to twice per loop pass.

We can streamline this logic significantly:
Instead of allocating substrings and doing linear array searches, we can write a tiny helper function (or direct `switch` statement) that maps a single `char` to its integer value in O(1) direct operations. Then, we process the string in a single clean pass: if the current character's value is less than the next character's value, subtract it; otherwise, add it.

#### Code Snippet for Cleaner Single-Pass Logic:

```cpp
int getVal(char c) {
    switch(c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

int romanToInt(string s) {
    int num = 0;
    for (int i = 0; i < s.length(); i++) {
        int curr = getVal(s[i]);
        int next = (i + 1 < s.length()) ? getVal(s[i + 1]) : 0;
        
        if (curr < next) {
            num -= curr; // Subtract if smaller value precedes larger value
        } else {
            num += curr; // Otherwise add
        }
    }
    return num;
}
```

* **Resulting Time Complexity:** **O(N)** — Same theoretical time complexity, but much faster execution time because it avoids dynamic memory allocation and array scanning.
* **Resulting Space Complexity:** **O(1)** — Still strictly constant space.
* **Theoretical Best:** **O(N)** time and **O(1)** space. The improved version reaches this theoretical limit.

---

## Edge Cases Handled

* **Single-character strings (e.g., `s = "I"` or `s = "M"`):** Pointer `j` (1) starts beyond the end of the string. `value2` stays 0, so the `value1 > value2` branch executes correctly without reading out of bounds.
* **Subtraction rules (e.g., `IV`, `IX`, `XL`, `XC`, `CD`, `CM`):** Recognized whenever `value2 > value1`. The code correctly computes `value2 - value1` and increments indices by 2 to consume both characters.
* **Repeated identical symbols (e.g., `III`, `XX`):** Handled by the `value1 == value2` branch, adding both values together and moving pointers forward by 2 indices.
* **Trailing single character after double skips (e.g., `LVIII`):** When the last character is left alone because `j` went out of bounds, `value2` defaults to 0, allowing the remaining character to be added via `value1 > value2`.
