![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.4%20MB%20(beats%2083.67%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a single positive whole number called **n**. 

The problem asks you to calculate two separate totals based on **n**:
1. **sumOdd**: The sum of the first **n** positive odd numbers. (For example, if **n** is 3, the first three odd numbers are 1, 3, and 5. Their sum is 1 + 3 + 5 = 9).
2. **sumEven**: The sum of the first **n** positive even numbers. (For example, if **n** is 3, the first three even numbers are 2, 4, and 6. Their sum is 2 + 4 + 6 = 12).

Once you have both sums, you need to find the **GCD** (greatest common divisor, which is the largest positive integer that divides evenly into both numbers without leaving a remainder) of **sumOdd** and **sumEven**. Finally, you return that GCD value.

---

## Intuition

If you start writing loops to generate all the odd and even numbers, add them up, and then use a loop or Euclidean algorithm to find their GCD, it feels like a lot of work. 

However, there is a neat mathematical pattern hiding here:
- The sum of the first **n** odd numbers is always equal to **n * n** (or **n** squared).
- The sum of the first **n** even numbers is always equal to **n * (n + 1)**.

Let us test this with an example where **n = 4**:
- **sumOdd** = 1 + 3 + 5 + 7 = 16, which is 4 * 4.
- **sumEven** = 2 + 4 + 6 + 8 = 20, which is 4 * (4 + 1) = 4 * 5 = 20.

Now, we need the GCD of 16 and 20. Both numbers can be written using **n**:
- 16 = 4 * 4
- 20 = 4 * 5

What is the greatest common divisor of (4 * 4) and (4 * 5)? It is simply **4**. 

If you try this for any number **n**, **sumOdd** will always be **n * n** and **sumEven** will always be **n * (n + 1)**. Both of these expressions share **n** as a common factor. In fact, **n** and **n + 1** share no other common factors. Therefore, their greatest common divisor is always just **n** itself. 

The "aha" moment is realizing you do not need to calculate any sums or run any math algorithms at all. The answer is always just **n**.

---

## Approach

Here is how the C++ code implements this solution step by step:

- `return n;`: The function directly returns the input parameter **n** as the final result, skipping all calculations because the math proves the GCD of the odd sum and even sum for a given **n** will always equal **n**.

---

## Dry Run

### Case 1: Typical case (n = 4)

| n | Action | Return Value |
|---|---|---|
| 4 | The function receives **n = 4** and immediately returns it. | 4 |

### Case 2: Edge case with minimum constraint (n = 1)

| n | Action | Return Value |
|---|---|---|
| 1 | The function receives the smallest possible valid input **n = 1** and immediately returns it. (For n = 1, sumOdd = 1, sumEven = 2, and GCD(1, 2) = 1). | 1 |

---

## Time & Space Complexity

- **Time:** O(1) — constant time. The code only looks at the input **n** and returns it immediately without any loops or repeating steps.
- **Space:** O(1) — constant space. No extra memory or data structures are created; it only uses the single input variable.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this code is already fully optimal. It runs in O(1) time and O(1) space, which is the absolute theoretical best possible complexity for any algorithm solving this problem, because you must at least read the input integer **n** to return an answer. No further improvements are possible.

---

## Edge Cases Handled

- **Minimum constraint value (n = 1):** Handled correctly because the math holds true for 1 (sumOdd = 1, sumEven = 2, GCD = 1).
- **Larger values up to the constraint limit (n = 1000):** Handled instantly because the solution uses a direct O(1) formula instead of looping up to 1000, preventing any performance slowdown.
