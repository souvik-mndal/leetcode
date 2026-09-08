![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.50%20MB%20(beats%2084.49%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

When we write big numbers, we put commas to make them easier to read. A comma is placed after every three digits starting from the right side of the number. 

* Numbers from 1 to 999 have no commas (for example: "123" or "999").
* Numbers from 1,000 to 99,999 have 1 comma (for example: "1,000" or "45,678").
* The number 100,000 has 1 comma ("100,000").

The problem asks: if you write down all whole numbers starting from 1 up to a given number `n`, how many total commas will you write?

**Example:** If `n = 1002`, you write the numbers 1 through 1002. Only 1000, 1001, and 1002 have commas ("1,000", "1,001", "1,002"). Each has 1 comma, so the answer is 3.

---

## Intuition

Look at the constraints given in the problem: `n` is at most 100,000 (`10^5`). 

Because `n` never exceeds 100,000, we notice a helpful pattern across all numbers in this range:
* Any number below 1000 has **0 commas**.
* Any number from 1000 to 100,000 has **exactly 1 comma**.

Since every single number from 1000 up to `n` gives us exactly 1 comma, we do not need to check each number one by one. We just need to count how many numbers exist in the range from 1000 to `n`.

The count of numbers from 1000 to `n` is calculated as `n - 1000 + 1`, which simplifies directly to `n - 999`.

---

## Approach

* `if( n < 1000 ) return 0;`: Checks if `n` is less than 1000. If so, no numbers in the range `[1, n]` contain any commas, so we return 0 right away.
* `return (n-999);`: If `n` is 1000 or larger, calculates the total count of numbers from 1000 up to `n`. Since each of these numbers contributes exactly 1 comma, returning `n - 999` gives the total comma count.

---

## Dry Run

### Case 1: Typical case with `n = 1002`

| Step | Code Expression | Value of `n` | Decision / Result | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `if( n < 1000 )` | 1002 | `1002 < 1000` is **False** | Skip `return 0` and move to next line |
| 2 | `return (n - 999);` | 1002 | `1002 - 999 = 3` | Return `3` |

### Case 2: Small input with `n = 998`

| Step | Code Expression | Value of `n` | Decision / Result | Action |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `if( n < 1000 )` | 998 | `998 < 1000` is **True** | Return `0` immediately |

---

## Time & Space Complexity

**Time Complexity:** O(1) — Constant time. The code only performs one comparison and one basic subtraction, regardless of how large `n` is.
**Space Complexity:** O(1) — Constant space. No extra memory or variables are allocated.

### Is this optimal?
Yes, this solution is optimal. O(1) time and O(1) space is the absolute best possible complexity, as it solves the problem instantly in constant mathematical steps without looping or allocating memory.

---

## Edge Cases Handled

* **`n` less than 1000 (e.g., `n = 1` or `n = 998`):** Correctly caught by `n < 1000` and returns 0.
* **Exact threshold boundary (`n = 1000`):** `1000 - 999` gives 1, which correctly accounts for the single comma in "1,000".
* **Maximum constraint boundary (`n = 100,000`):** `100000 - 999` gives 99,001, which correctly counts 1 comma for every number from 1,000 to 100,000.
