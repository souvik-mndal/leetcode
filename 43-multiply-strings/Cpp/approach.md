![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-10.9%20MB%20(beats%2013.09%25)-red?style=for-the-badge)

---

## Problem Explained

The problem asks you to multiply two large non-negative numbers that are given as strings, such as `"123"` and `"456"`. You must return the final product as a string, like `"56088"`.

You cannot simply convert the input strings directly into standard numbers (like using `stoi` in C++), because the inputs can be up to 200 digits long. A 200-digit number is far too large to fit into standard integer types. You also cannot use built-in BigInteger libraries. You must write the logic to handle the multiplication digit by digit.

## Intuition

Think back to how you multiply numbers by hand on paper:
1. You take each digit of the second number from right to left.
2. You multiply it by each digit of the first number from right to left.
3. You write down the result, shifting one place to the left for each new digit row.
4. Finally, you add all the columns together, handling carries along the way.

We can simulate this exact process in code using a single array (a list of numbers) to hold our working digit sums.

Here is the key insight: when you multiply the digit at index `i` of `num1` by the digit at index `j` of `num2`, their product belongs across two specific positions in the final result array: `i + j` (for the tens/carry part) and `i + j + 1` (for the ones part). 

Because the maximum possible length of the product of two numbers with lengths `N` and `M` is `N + M`, an array of size `N + M` is always big enough to hold the answer.

## Approach

Here is how the code works step-by-step:

* `if (num1 == "0" || num2 == "0") return "0";`: Checks if either number is zero right at the start. If so, it returns `"0"` immediately to save work and avoid handling leading zero logic later.
* `vector<int> res(num1.size()+num2.size(), 0);`: Creates an integer vector named `res` of size `num1.size() + num2.size()`, initialized with all zeros. This array will hold the digits of the final product.
* `for (int i = num1.size()-1; i >= 0; i--)`: Starts an outer loop that moves backward through `num1` (from the last digit to the first digit).
* `for (int j = num2.size()-1; j >= 0; j--)`: Starts an inner loop that moves backward through `num2`.
* `res[i + j + 1] += (num1[i]-'0') * (num2[j]-'0');`: Converts the character digits `num1[i]` and `num2[j]` into actual numbers by subtracting the character `'0'`, multiplies them, and adds the result to `res[i + j + 1]`.
* `res[i + j] += res[i + j + 1] / 10;`: Takes the carry (any amount greater than 9) from position `i + j + 1` and adds it to position `i + j`.
* `res[i + j + 1] %= 10;`: Keeps only the single digit (remainder after dividing by 10) at position `i + j + 1`.
* `int i = 0; while (res[i] == 0) i++;`: Reuses `i` as an index pointer to scan from the left of `res` and skip any unused leading zeros.
* `while (i < res.size()) ans += to_string(res[i++]);`: Converts each remaining digit in `res` to a string using `to_string`, appends it to `ans`, and returns `ans`.

## Dry Run

### Case 1: Simple input (`num1 = "2"`, `num2 = "3"`)

* Initial state: `res` has length 1 + 1 = 2, filled with zeros: `[0, 0]`.

| `i` | `j` | `res` state | Action |
| --- | --- | --- | --- |
| 0 | 0 | `[0, 0]` | Multiply `num1[0]` ('2') and `num2[0]` ('3') = 6. Add to `res[1]`. `res[1]` becomes 6. |
| 0 | 0 | `[0, 6]` | Carry `6 / 10 = 0` to `res[0]`. Keep `6 % 10 = 6` at `res[1]`. |
| - | - | `[0, 6]` | Skip leading zero at index 0. |
| - | - | `[0, 6]` | Build string from index 1: `ans` = `"6"`. Return `"6"`. |

### Case 2: Multi-digit input (`num1 = "123"`, `num2 = "456"`)

* Initial state: `res` has length 3 + 3 = 6, filled with zeros: `[0, 0, 0, 0, 0, 0]`.

| `i` | `j` | `res` state | Action |
| --- | --- | --- | --- |
| 2 ('3') | 2 ('6') | `[0, 0, 0, 0, 1, 8]` | 3 * 6 = 18. Add to `res[5]`. Carry 1 to `res[4]`, keep 8 at `res[5]`. |
| 2 ('3') | 1 ('5') | `[0, 0, 0, 1, 6, 8]` | 3 * 5 = 15. Add to `res[4]` (was 1, now 16). Carry 1 to `res[3]`, keep 6 at `res[4]`. |
| 2 ('3') | 0 ('4') | `[0, 0, 1, 3, 6, 8]` | 3 * 4 = 12. Add to `res[3]` (was 1, now 13). Carry 1 to `res[2]`, keep 3 at `res[3]`. |
| 1 ('2') | 2 ('6') | `[0, 0, 1, 4, 8, 8]` | 2 * 6 = 12. Add to `res[4]` (was 6, now 18). Carry 1 to `res[3]` (3+1=4), keep 8 at `res[4]`. |
| 1 ('2') | 1 ('5') | `[0, 0, 2, 4, 8, 8]` | 2 * 5 = 10. Add to `res[3]` (was 4, now 14). Carry 1 to `res[2]` (1+1=2), keep 4 at `res[3]`. |
| 1 ('2') | 0 ('4') | `[0, 1, 0, 4, 8, 8]` | 2 * 4 = 8. Add to `res[2]` (was 2, now 10). Carry 1 to `res[1]`, keep 0 at `res[2]`. |
| 0 ('1') | 2 ('6') | `[0, 1, 1, 0, 8, 8]` | 1 * 6 = 6. Add to `res[3]` (was 4, now 10). Carry 1 to `res[2]` (0+1=1), keep 0 at `res[3]`. |
| 0 ('1') | 1 ('5') | `[0, 1, 6, 0, 8, 8]` | 1 * 5 = 5. Add to `res[2]` (was 1, now 6). Keep 6 at `res[2]`. |
| 0 ('1') | 0 ('4') | `[0, 5, 6, 0, 8, 8]` | 1 * 4 = 4. Add to `res[1]` (was 1, now 5). Keep 5 at `res[1]`. |
| - | - | `[0, 5, 6, 0, 8, 8]` | Skip leading zero at index 0. Build string from index 1 to 5. |
| - | - | `[0, 5, 6, 0, 8, 8]` | Final `ans` = `"56088"`. Return `"56088"`. |

## Time & Space Complexity

* **Time Complexity:** O(N * M) — where N is the length of `num1` and M is the length of `num2`. The nested loops run N * M times. Each loop step does basic arithmetic in constant time. Building the final string takes O(N + M) time.
* **Space Complexity:** O(N + M) — using an integer vector `res` of size N + M and a string `ans` of length up to N + M to store the output.

### Optimization Analysis

The algorithm's theoretical time complexity of O(N * M) is optimal for standard simulation of grade school multiplication.

However, the current C++ code can be optimized for lower memory overhead and faster execution speed. The line `ans += to_string(res[i++])` creates temporary string objects and performs multiple heap allocations. We can optimize this by converting the integer directly to a character using ASCII math (`res[i] + '0'`) and appending it using `push_back`.

Here is the key change:

```cpp
// Replace this line:
// while (i < res.size()) ans += to_string(res[i++]);

// With this optimized line:
while (i < res.size()) ans.push_back(res[i++] + '0');
```

* `res[i++] + '0'`: Converts a single digit integer (e.g., `5`) directly to its character representation (`'5'`) without constructing a string object.
* `ans.push_back(...)`: Appends the character directly to the string, avoiding dynamic string conversions.

* **Optimized Time Complexity:** O(N * M)
* **Optimized Space Complexity:** O(N + M) — but uses significantly less memory overhead by avoiding intermediate allocations.
* **Theoretical Best Possible:** O(N * M) for standard multiplication (or O(N log N) using advanced fast Fourier transforms for extremely long numbers, though FFT is far beyond expected solutions for 200-digit constraint limits). This approach achieves the ideal practical bound.

## Edge Cases Handled

* **Multiplication by zero:** `num1 = "0"` or `num2 = "0"` is caught immediately by `if (num1 == "0" || num2 == "0") return "0";`. This prevents outputting answers like `"000"`.
* **Single-digit numbers:** Cases like `"2"` * `"3"` work seamlessly without array out-of-bound errors because the array size `1 + 1 = 2` fits the result cleanly.
* **No leading zeros in output:** The `while (res[i] == 0) i++;` loop trims unnecessary leading zeros from the answer array before generating the final string.
* **Large numbers (up to 200 digits):** Performs calculations digit by digit in an integer array, completely avoiding C++ standard integer overflow limits.
