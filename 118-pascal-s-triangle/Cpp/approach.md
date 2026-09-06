![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.6%20MB%20(beats%2088.33%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Pascal's Triangle is a triangle of numbers where each row builds on the row directly above it. 

Imagine building a pyramid. The top row has a single block with the number 1. Every row below it starts and ends with a 1. For any number inside the triangle, you find its value by adding the two numbers sitting directly above it to its left and right. 

For example, if the top row is `[1]` and the second row is `[1, 1]`, the third row starts with `1`, then adds the two numbers above it (`1 + 1 = 2`), and ends with `1`, giving `[1, 2, 1]`.

The problem asks us to write a function that takes an integer `numRows` and returns the first `numRows` of this triangle as a list of lists of numbers.

## Intuition

The "aha" moment for this solution is realizing you do not actually need to look up values from the previous row to calculate the current row. 

Usually, people think you must save the old row in memory and add pairs of numbers together. But each row in Pascal's triangle is actually just a sequence of binomial coefficients. This means every single number in a row can be calculated directly using a simple math formula based on its position, using only the previous number in the same row. 

Specifically, starting with `1` at the beginning of a row, you can find the next number by taking the current number, multiplying it by `(i - j)`, and dividing it by `j`, where `i` is the row number and `j` is the column position. This lets us build each row completely on the fly using a single integer variable `c` without nested lookups of past rows.

## Approach

* `vector<vector<int>>ans;` creates the main container that will hold all the rows of our triangle, which we will return at the end.
* `for( int i=1;i<=numRows ; i++ )` loops `numRows` times to build the triangle row by row, where `i` represents the current row number starting at 1.
* `int c = 1;vector<int>r;` initializes the first value of every row to `1` (stored in `c`) and creates an empty list `r` to hold the numbers for the current row `i`.
* `for( int j=1;j<=i;j++)` loops through each column position `j` within the current row `i`, running `i` times because row `i` has `i` elements.
* `r.push_back( c );` adds the current calculated value of `c` into our row list `r`.
* `c=c*(i-j)/j;` updates the value of `c` to become the next number in the same row using our math formula, ready for the next iteration of the inner loop.
* `ans.push_back( r );` adds the completed row list `r` into our main triangle container `ans` once the inner loop finishes.
* `return ans;` sends back the fully constructed triangle after all rows have been generated.

## Dry Run

### Case 1: Typical case (numRows = 3)

| i | j | c | r (Row list) | Action |
|---|---|---|---|---|
| 1 | 1 | 1 | [1] | Row 1: push 1, c becomes 1*(1-1)/1 = 0, loop ends. Push [1] to ans. |
| 2 | 1 | 1 | [1] | Row 2, col 1: push 1, c becomes 1*(2-1)/1 = 1 |
| 2 | 2 | 1 | [1, 1] | Row 2, col 2: push 1, c becomes 1*(2-2)/2 = 0, loop ends. Push [1, 1] to ans. |
| 3 | 1 | 1 | [1] | Row 3, col 1: push 1, c becomes 1*(3-1)/1 = 2 |
| 3 | 2 | 2 | [1, 2] | Row 3, col 2: push 2, c becomes 2*(3-2)/2 = 1 |
| 3 | 3 | 1 | [1, 2, 1] | Row 3, col 3: push 1, c becomes 1*(3-3)/3 = 0, loop ends. Push [1, 2, 1] to ans. |

### Case 2: Minimum constraint edge case (numRows = 1)

| i | j | c | r (Row list) | Action |
|---|---|---|---|---|
| 1 | 1 | 1 | [1] | Row 1: push 1, c becomes 0, loop ends. Push [1] to ans. |

## Time & Space Complexity

**Time:** O(numRows^2) — The code uses two nested loops. The outer loop runs `numRows` times, and the inner loop runs `i` times for each row. The total number of steps is roughly `numRows * numRows / 2`, which grows quadratically as the input grows.

**Space:** O(numRows^2) — We store every number of the triangle in a two-dimensional vector (`ans`). The total number of integers stored across all rows is proportional to the square of `numRows`.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is already optimal. 

To return the entire Pascal's triangle, you must output every single number in the triangle. Because there are roughly `numRows^2 / 2` numbers in total, just writing them all down requires visiting and storing that many elements. You cannot solve this problem faster than visiting every required output cell once, meaning both the time complexity of O(numRows^2) and the space complexity of O(numRows^2) have reached the absolute theoretical limit for this problem. No further improvement is possible.

## Edge Cases Handled

* **Minimum input size (`numRows = 1`):** The outer loop runs once, the inner loop runs once, and it correctly returns `[[1]]`.
* **Upper constraint limit (`numRows = 30`):** The nested loops execute safely without running into integer overflow on standard 32-bit integers for `c`, as values up to 30 rows fit safely inside a standard C++ integer.
