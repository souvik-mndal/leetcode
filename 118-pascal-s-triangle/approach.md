![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.58%20MB%20(beats%2090.02%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Pascal's Triangle is a triangle of numbers where every row starts and ends with `1`. Every number inside the triangle is made by adding the two numbers directly above it from the row before.

Here is what the first 4 rows look like:
```text
      1
     1 1
    1 2 1
   1 3 3 1
```

The problem asks you to build the first `numRows` of this triangle and return them as a list of rows (a 2D list).

**Example:**
If `numRows = 4`, you return: `[[1], [1, 1], [1, 2, 1], [1, 3, 3, 1]]`.

---

## Intuition

Most people build Pascal's Triangle using **Dynamic Programming** (building new values by adding two numbers from the row above). 

However, this code uses a math shortcut based on **Combinations** (often called "$n$ Choose $k$"):
- Row $i$ contains the values of combinations for that row index.
- Instead of reading from the previous row in memory, you can calculate each number in a row directly from the number right before it.
- The formula to get the next number in a row is: `next_number = current_number * (row - column) / column`.

The **"aha!" moment** is that you can generate each row independently using simple arithmetic without needing to look at previous rows.

---

## Approach

Here is how the code builds the triangle step-by-step:

* **Create the main storage:** Initialize an empty 2D list `ans` to hold all the rows.
* **Loop through each row:** Run a loop with index `i` from `1` up to `numRows`.
* **Initialize the row:** For row `i`, set the starting value `c = 1` and create an empty list `r` for the current row.
* **Build row elements:** Run an inner loop with index `j` from `1` up to `i`:
  * Add the current value `c` to row `r`.
  * Calculate the next value in the row using the math update: `c = c * (i - j) / j`.
* **Save the row:** Append the completed row `r` to `ans`.
* **Return the result:** After all rows are built, return `ans`.

---

## Time & Space Complexity

* **Time Complexity:** **$O(\text{numRows}^2)$** — Row 1 has 1 number, Row 2 has 2 numbers, up to Row $N$ which has $N$ numbers. The total numbers generated is $1 + 2 + \dots + N = \frac{N(N+1)}{2}$. Calculating each number takes constant $O(1)$ time.
* **Space Complexity:** **$O(\text{numRows}^2)$** — Storing all generated numbers in the result list requires space proportional to the total count of numbers, which is $\frac{N(N+1)}{2}$.

### Is this already optimal?

**Yes.** To return the triangle, you must generate and store every single number. Since there are $\frac{N(N+1)}{2}$ total numbers, you cannot do better than $O(N^2)$ time and $O(N^2)$ space.

#### Note on Approach
While the math formula used in this code is optimal in time complexity, standard **Dynamic Programming** (adding adjacent numbers from the previous row) is usually preferred in production code for two reasons:
1. Addition is slightly faster for CPUs than multiplication and division.
2. Math formulas run a risk of integer overflow on larger values (though safe here due to the small constraint `numRows <= 30`).

---

## Edge Cases Handled

* **`numRows = 1` (Minimum input size):** The outer loop runs once, inner loop runs once, returning `[[1]]` correctly.
* **Boundary values (First and last items of every row):** The variable `c` starts at `1` for the first element. The formula naturally sets `c` back to `1` at the end of the row (and `0` after insertion), so every row starts and ends with `1` without needing explicit `if` statements.
* **No Out-of-Bounds risk:** Because each row is calculated mathematically from local variables rather than reading array indices from a previous row, there is zero risk of accessing invalid memory positions.
