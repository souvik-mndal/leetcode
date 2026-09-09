![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.99%20MB%20(beats%2083.63%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

When you write large numbers in standard format, you insert a comma every three digits from the right. 

Here is how many commas different ranges of numbers have:
* Numbers from **1 to 999** have **0** commas (for example: `998`).
* Numbers from **1,000 to 999,999** have **1** comma (for example: `1,002`).
* Numbers from **1,000,000 to 999,999,999** have **2** commas (for example: `1,500,000`).

Given an integer `n`, the problem asks you to calculate the total number of commas used if you wrote down all integers from `1` to `n` (inclusive).

**Example:** If `n = 1002`, the numbers with commas are `1,000`, `1,001`, and `1,002`. Each of these has 1 comma, so the output is `3`.

---

## Intuition

Instead of counting how many commas each individual number has, count layer by layer across all numbers at once.

Think of thresholds like milestones:
1. Every number greater than or equal to `1,000` gets **at least 1 comma**. How many such numbers are there up to `n`? Exactly `n - 1000 + 1`.
2. Every number greater than or equal to `1,000,000` gets a **second comma**. How many such numbers are there up to `n`? Exactly `n - 1000000 + 1`.
3. Every number greater than or equal to `1,000,000,000` gets a **third comma**. How many such numbers are there up to `n`? Exactly `n - 1000000000 + 1`.

By adding up the count of numbers that reach each threshold level, every number automatically gets credited for its 1st, 2nd, 3rd, and subsequent commas.

---

## Approach

Here is how the algorithm works step-by-step:

* `long long ans = 0;`: Initialize `ans` to `0` to accumulate the total count of commas.
* `long long thresh = 1000;`: Set `thresh` to `1000`, which is the smallest number that contains a comma.
* `while( thresh <= n ){`: Start a loop that runs as long as the current threshold is less than or equal to `n`.
* `ans = ans + n - thresh + 1 ;`: Calculate how many numbers from `thresh` up to `n` exist (inclusive). Add this amount to `ans`. This accounts for one extra comma for every number in that range.
* `thresh *= 1000;`: Multiply `thresh` by `1000` to jump to the next threshold (from `1,000` to `1,000,000`, then `1,000,000,000`, etc.).
* `return ans;`: Return `ans`, which now holds the total sum of all commas.

---

## Dry Run

### Case 1: `n = 1002` (Small number with commas)

| Step | `thresh` | `ans` | Action |
| --- | --- | --- | --- |
| Start | 1000 | 0 | Initialize `ans = 0` and `thresh = 1000`. |
| Loop 1 | 1000 | 3 | `1000 <= 1002` is true. `ans += 1002 - 1000 + 1` (adds 3). `thresh` becomes 1,000,000. |
| Loop 2 | 1000000 | 3 | `1000000 <= 1002` is false. Loop terminates. |
| End | 1000000 | 3 | Return `ans = 3`. |

---

### Case 2: `n = 998` (Number smaller than 1000)

| Step | `thresh` | `ans` | Action |
| --- | --- | --- | --- |
| Start | 1000 | 0 | Initialize `ans = 0` and `thresh = 1000`. |
| Check | 1000 | 0 | `1000 <= 998` is false. Loop condition fails immediately. |
| End | 1000 | 0 | Return `ans = 0`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(log n)** base 1000. The loop multiplies `thresh` by 1000 on every iteration. Since `n` is at most 10^15, the `while` loop runs at most 5 times total.
* **Space Complexity:** **O(1)** auxiliary space. Only two variables (`ans` and `thresh`) are allocated in memory.

**Is this optimal?**
Yes, this solution is fully optimal. Running at most 5 loop iterations even for maximum input sizes (10^15) is effectively constant time in practice. No further algorithmic speedups are required or possible.

---

## Edge Cases Handled

* **Numbers under 1000 (`n < 1000`):** The condition `thresh <= n` is false on the first check, so the loop never runs and correctly returns `0`.
* **Exact threshold boundary (`n = 1000`):** `1000 - 1000 + 1 = 1` comma is added properly.
* **Very large inputs (`n` up to 10^15):** Using 64-bit integers (`long long`) prevents integer overflow, which would otherwise happen with standard 32-bit `int` variables.
