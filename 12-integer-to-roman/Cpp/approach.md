![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%2038.39%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.3%20MB%20(beats%2082.48%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The goal is to convert an integer into its Roman numeral string representation.

Roman numerals use seven basic symbols:
* **I** = 1
* **V** = 5
* **X** = 10
* **L** = 50
* **C** = 100
* **D** = 500
* **M** = 1000

Normally, numbers are written from largest value to smallest value by combining symbols (for example, 15 is 10 + 5, which is **XV**).

However, Roman numerals do not allow writing the same symbol four times in a row. Instead of writing 4 as `IIII`, it is written as 4 = 5 - 1, which is **IV**. This is called a **subtractive form**.

There are six subtractive forms in total:
* **IV** = 4
* **IX** = 9
* **XL** = 40
* **XC** = 90
* **CD** = 400
* **CM** = 900

Given an integer between 1 and 3999, convert it to a Roman numeral string.

For example, if the input is `num = 58`:
* 50 is **L**
* 5 is **V**
* 3 is **III**
* Put them together to get **LVIII**.

---

## Intuition

The core trick is to use a **greedy strategy**. We always want to subtract the largest possible Roman numeral value from our input number `num`.

Normally, handling subtractive forms like 4 (**IV**) or 90 (**XC**) requires extra conditional checks. But if we treat those six subtractive forms as regular standard symbols alongside the basic seven, we end up with 13 distinct values. 

If we order these 13 values from largest (1000) to smallest (1), we can simply iterate through them. As long as our remaining `num` is greater than or equal to the current value, we append the symbol to our answer and subtract the value from `num`.

---

## Approach

* `string roman[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};`  
  This array stores all 13 Roman numeral symbols ordered from largest value to smallest value. It includes both single-letter symbols and two-letter subtractive combinations.

* `int value[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};`  
  This array stores the exact integer value corresponding to each Roman symbol at the same index in the `roman` array.

* `string ans="";`  
  This variable initializes an empty string where we will build and store our final Roman numeral result.

* `for( int i=0 ; i<13 ; i++)`  
  This loop goes through each of the 13 possible Roman numeral values sequentially, starting from the largest (1000) down to the smallest (1).

* `while(num>=value[i])`  
  This inner loop checks if the remaining integer `num` is big enough to fit the current Roman value `value[i]`. It keeps running as long as `value[i]` can be subtracted from `num`.

* `ans+=roman[i];`  
  Inside the inner loop, this appends the current Roman symbol `roman[i]` to our result string `ans`.

* `num-=value[i];`  
  This reduces `num` by `value[i]` to reflect that we have converted that portion of the integer into Roman characters.

* `return ans;`  
  Once the outer loop finishes checking all 13 values, `num` becomes 0, and we return the final Roman numeral string.

---

## Dry Run

### Case 1: Typical case (`num = 1994`)

| Step | `i` | `value[i]` | `roman[i]` | `num` (before) | `num` (after) | `ans` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | - | 1994 | 1994 | `""` | Initialize variables |
| 1 | 0 | 1000 | `"M"` | 1994 | 994 | `"M"` | `1994 >= 1000`: append `"M"`, subtract 1000 |
| 2 | 0 | 1000 | `"M"` | 994 | 994 | `"M"` | `994 < 1000`: move to `i = 1` |
| 3 | 1 | 900 | `"CM"` | 994 | 94 | `"MCM"` | `994 >= 900`: append `"CM"`, subtract 900 |
| 4 | 1 | 900 | `"CM"` | 94 | 94 | `"MCM"` | `94 < 900`: skip `i = 2, 3, 4` |
| 5 | 5 | 90 | `"XC"` | 94 | 4 | `"MCMXC"` | `94 >= 90`: append `"XC"`, subtract 90 |
| 6 | 5 | 90 | `"XC"` | 4 | 4 | `"MCMXC"` | `4 < 90`: skip `i = 6, 7, 8, 9, 10` |
| 7 | 11 | 4 | `"IV"` | 4 | 0 | `"MCMXCIV"` | `4 >= 4`: append `"IV"`, subtract 4 |
| End | 11 | 4 | `"IV"` | 0 | 0 | `"MCMXCIV"` | `num` is 0; return `"MCMXCIV"` |

### Case 2: Smaller case with repeating ones (`num = 58`)

| Step | `i` | `value[i]` | `roman[i]` | `num` (before) | `num` (after) | `ans` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | - | 58 | 58 | `""` | Initialize variables |
| 1 | 6 | 50 | `"L"` | 58 | 8 | `"L"` | `58 >= 50`: append `"L"`, subtract 50 |
| 2 | 10 | 5 | `"V"` | 8 | 3 | `"LV"` | `8 >= 5`: append `"V"`, subtract 5 |
| 3 | 12 | 1 | `"I"` | 3 | 2 | `"LVI"` | `3 >= 1`: append `"I"`, subtract 1 |
| 4 | 12 | 1 | `"I"` | 2 | 1 | `"LVII"` | `2 >= 1`: append `"I"`, subtract 1 |
| 5 | 12 | 1 | `"I"` | 1 | 0 | `"LVIII"` | `1 >= 1`: append `"I"`, subtract 1 |
| End | 12 | 1 | `"I"` | 0 | 0 | `"LVIII"` | `num` is 0; return `"LVIII"` |

---

## Time & Space Complexity

* **Time:** O(1) — The outer loop always runs exactly 13 times. The inner `while` loop runs at most 15 times total across all iterations (since the longest possible Roman numeral up to 3999 is 15 characters long, for 3888: `MMMDCCCLXXXVIII`). Since the work is strictly bounded by constant constraints, time complexity is constant.
* **Space:** O(1) — The memory used by arrays `roman` and `value` is fixed at 13 elements. The output string `ans` takes at most 15 characters.

### Optimization

While the current solution is already O(1), we can speed up runtime performance by eliminating the loops completely using a **direct digit place-value lookup**.

#### Why this works
In our base-10 number system, every number up to 3999 consists of thousands, hundreds, tens, and units places:
* **Thousands:** `num / 1000` (can only be 0, 1, 2, or 3)
* **Hundreds:** `(num % 1000) / 100` (0 through 9)
* **Tens:** `(num % 100) / 10` (0 through 9)
* **Units:** `num % 10` (0 through 9)

Because each digit place operates independently in Roman numerals, we can predefine four lookup arrays for thousands, hundreds, tens, and units. Then we index directly into those arrays using simple math.

```cpp
string M[] = {"", "M", "MM", "MMM"};
string C[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
string X[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
string I[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

return M[num / 1000] + C[(num % 1000) / 100] + X[(num % 100) / 10] + I[num % 10];
```

* `M[num / 1000]` extracts the thousands digit (e.g., `1994 / 1000 = 1`, giving `"M"`).
* `C[(num % 1000) / 100]` extracts the hundreds digit (e.g., `994 / 100 = 9`, giving `"CM"`).
* `X[(num % 100) / 10]` extracts the tens digit (e.g., `94 / 10 = 9`, giving `"XC"`).
* `I[num % 10]` extracts the units digit (e.g., `4 % 10 = 4`, giving `"IV"`).

* **Improved Complexity:** Still **O(1) Time** and **O(1) Space**, but executes much faster with 0 loop iterations and fewer string concatenations.
* **Theoretical Best:** O(1) overall. The direct lookup reaches the absolute minimum work possible.

---

## Edge Cases Handled

* **Subtractive Digits (4 and 9):** Inputs like 4 (`IV`), 9 (`IX`), 40 (`XL`), 90 (`XC`), 400 (`CD`), and 900 (`CM`) are naturally processed because subtractive pairs are included directly as values in the lookup array.
* **Minimum Constraint Value (`num = 1`):** Correctly skips all values until `value[12] = 1`, outputting `"I"`.
* **Maximum Constraint Value (`num = 3999`):** Processes `3000` (`MMM`), `900` (`CM`), `90` (`XC`), and `9` (`IX`), producing `"MMMCMXCIX"`.
* **Numbers with Zeros:** For numbers like `1000` or `204`, zero-valued place digits are naturally skipped because `num` becomes 0 before reaching smaller symbol checks.
