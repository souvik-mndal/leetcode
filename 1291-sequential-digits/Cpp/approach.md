![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.2%20MB%20(beats%2095.76%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

We need to find all integers within a given range, from **low** to **high** inclusive, that have **sequential digits**. 

A number has sequential digits if each digit is exactly one greater than the digit that came right before it. For example, the number 123 is sequential because 2 is 1 plus 1, and 3 is 2 plus 1. On the other hand, 134 is not sequential because the jump from 1 to 3 skips 2. 

The problem asks us to look at all possible numbers meeting this rule that fall between **low** and **high**, and return them sorted in a list from smallest to largest.

---

## Intuition

Instead of checking every single number in the range from **low** to **high** (which could be up to 10^9 and take too long), we can build the sequential numbers directly. 

How many sequential numbers can possibly exist? Very few! A sequence of digits can start with any digit from 1 to 9, and can only grow as long as the next digit is 9 or less. For example, if we start with 1, we can append 2, then 3, all the way up to 9 to form 123456789. 

Because there are only a small handful of sequential numbers in total (fewer than 50), the "aha" moment is to generate every single valid sequential number mathematically, check if it falls between **low** and **high**, and collect the matches. 

---

## Approach

Here is how the code builds and filters the sequential numbers step by step:

* `vector<int> res;`: Initializes an empty list named **res** to store all the valid sequential numbers we find that fall within our target range.
* `for (int i = 1; i <= 9; ++i)`: Starts an outer loop where **i** acts as the starting digit for our sequential numbers, running from 1 up to 9.
* `int num = i;`: Sets **num** to our starting digit **i** to begin building a new sequential number.
* `for (int j = i + 1; j <= 9; ++j)`: Starts an inner loop where **j** represents the next consecutive digits that can follow after **i**, running from **i + 1** up to 9.
* `num = num * 10 + j;`: Multiplies the current **num** by 10 and adds the next digit **j** to it, effectively shifting digits left and appending the next sequential number onto the end.
* `if (num >= low && num <= high)`: Checks if our newly built sequential number **num** is greater than or equal to **low** and less than or equal to **high**.
* `res.push_back(num);`: Adds **num** to our results list **res** if it successfully fits inside the requested range.
* `sort(res.begin(), res.end());`: Sorts all the collected numbers in **res** from smallest to largest. (Though they naturally form mostly in order, sorting ensures they are strictly correct).
* `return res;`: Returns the final sorted list of sequential numbers.

---

## Dry Run

### Case 1: low = 100, high = 300

| i | j | num | Action |
|---|---|---|---|
| 1 | 2 | 12 | Built 12; too small (less than 100), not added. |
| 1 | 3 | 123 | Built 123; falls in range [100, 300], added to **res**. |
| 1 | 4 | 1234 | Built 1234; too large (greater than 300), inner loop stops for i = 1. |
| 2 | 3 | 23 | Built 23; too small, not added. |
| 2 | 4 | 234 | Built 234; falls in range [100, 300], added to **res**. |
| 2 | 5 | 2345 | Built 2345; too large, inner loop stops for i = 2. |
| ... | ... | ... | Remaining outer loop iterations produce numbers either too small or too large. |

*Final Output:* `[123, 234]`

### Case 2: low = 1000, high = 13000

| i | j | num | Action |
|---|---|---|---|
| 1 | 2, 3 | 12, 123 | Too small, not added. |
| 1 | 4 | 1234 | Falls in range [1000, 13000], added to **res**. |
| 1 | 5 | 12345 | Falls in range [1000, 13000], added to **res**. |
| 1 | 6 | 123456 | Too large (greater than 13000), inner loop stops for i = 1. |
| 2 | 3, 4 | 23, 234 | Too small, not added. |
| 2 | 5 | 2345 | Falls in range [1000, 13000], added to **res**. |
| ... | ... | ... | Continues testing all starting digits up to 9. |

*Final Output:* `[1234, 2345, 3456, 4567, 5678, 6789, 12345]`

---

## Time & Space Complexity

- **Time:** O(1) — There is a fixed, maximum number of sequential digits that can ever be generated (around 36 total combinations). The loops always run a constant number of times regardless of how large **low** and **high** are.
- **Space:** O(1) — The result list stores a tiny, constant number of integers (at most 36), which takes a fixed amount of memory.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already at the absolute theoretical limit of optimality. Since the total number of sequential digits across all possible lengths is a tiny constant, running in constant time O(1) and constant space O(1) cannot be beaten. No further improvements are possible.

---

## Edge Cases Handled

* **Small ranges:** Correctly handles tight ranges where zero sequential numbers exist (returning an empty list).
* **Single-digit numbers excluded by constraints:** The problem states **low** is at least 10, so single-digit numbers like 1, 2, or 3 are correctly ignored even if they are technically sequential.
* **Maximum boundary limits:** Handles large numbers up to 10^9 safely without integer overflow, stopping generation when numbers exceed 9 digits.
