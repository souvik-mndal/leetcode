![Runtime](https://img.shields.io/badge/Runtime-6%20ms%20(beats%206.50%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.57%20MB%20(beats%2054.84%25)-yellow?style=for-the-badge)

---

## Problem Explained

The goal is to take a 32-bit signed integer `x` and reverse its digits. 

* If the input is `123`, the output should be `321`.
* If the input is negative, like `-123`, the negative sign stays at the front, giving `-321`.
* Any trailing zeros drop off naturally. For example, `120` becomes `21` because standard numbers do not start with zero.

There is one main catch: 32-bit signed integers can only hold values between `-2^31` (`-2147483648`) and `2^31 - 1` (`2147483647`). If reversing a number causes it to go outside this valid range, you must return `0`. You are also strictly forbidden from using 64-bit integers to hold the reversed value temporarily.

---

## Intuition

To turn a number inside out, you need a way to peel digits off the back one by one and attach them to the front of a new number.

1. **Peeling digits:** Taking a number modulo 10 (`x % 10`) gives you its last digit. Dividing that number by 10 (`x / 10`) removes that last digit.
2. **Building the reversed number:** Every time you get a new digit, you multiply your current answer by 10 (shifting all its existing digits to the left) and add the new digit.

The core challenge is catching **overflow** (going past the maximum allowed limit) before it happens. Once a number overflows a standard integer variable, it wraps around to unpredictable values. To prevent this, you check if your answer is already greater than `INT_MAX / 10` *before* you multiply by 10. If it is, multiplying again will definitely spill over the limit, so you return `0` immediately.

---

## Approach

Here is the step-by-step logic used in the provided C++ code:

* **Step 1: Check for `INT_MIN`.** The lowest possible 32-bit integer is `-2147483648`. Its positive counterpart (`2147483648`) is too large for a 32-bit signed integer. If `x` equals `INT_MIN`, return `0` right away to prevent overflow during sign conversion.
* **Step 2: Remember and remove the sign.** Check if `x` is negative. If it is, set a boolean flag `flag = 1` and make `x` positive by multiplying it by `-1`.
* **Step 3: Loop through digits.** While `x` is greater than `0`:
  * Extract the last digit using `ld = x % 10`.
  * Check for potential overflow: if `ans > INT_MAX / 10`, multiplying by 10 will exceed the maximum allowed limit. Stop and return `0`.
  * Append the digit to your answer: `ans = ans * 10 + ld`.
  * Remove the last digit from `x`: `x = x / 10`.
* **Step 4: Restore the sign.** If `flag` was set to `1`, return `-ans`. Otherwise, return `ans`.

---

## Time & Space Complexity

* **Time Complexity:** **O(1)** (or **O(log10(x))**). A standard 32-bit signed integer has at most 10 digits. The loop will run at most 10 times regardless of the input size. This takes a constant amount of time.
* **Space Complexity:** **O(1)**. The algorithm only creates a few primitive variables (`flag`, `ans`, `ld`) which use a fixed amount of memory.

### Can it be improved?

The time and space complexity are already theoretical bests, but the **runtime performance and code structure can be improved**. 

The current code converts negative numbers to positive numbers. This forces extra checks, a separate boolean flag, and a special fallback for `INT_MIN`. 

In modern C++, both the modulo (`%`) and division (`/`) operators preserve negative signs. For example, `-123 % 10` evaluates to `-3`, and `-123 / 10` evaluates to `-12`. 

By processing negative numbers directly without converting them to positive first, we eliminate branching checks, remove the `flag` variable, and handle positive numbers, negative numbers, and `INT_MIN` cleanly inside a single loop.

```cpp
while (x != 0) {
    int ld = x % 10;
    
    // Check upper and lower 32-bit boundaries before multiplying by 10
    if (ans > INT_MAX / 10 || ans < INT_MIN / 10) {
        return 0;
    }
    
    ans = ans * 10 + ld;
    x /= 10;
}
return ans;
```

* **Why this works:** `ans < INT_MIN / 10` guards against negative overflow in the exact same way `ans > INT_MAX / 10` guards against positive overflow. Because `x` keeps its original sign, `ans * 10 + ld` naturally builds the negative result step-by-step.
* **Resulting Complexity:** Still **O(1) Time** and **O(1) Space**, but runs faster due to fewer operations and fewer CPU branching instructions. This achieves the theoretical best possible performance.

---

## Edge Cases Handled

* **`INT_MIN` (`-2147483648`):** Safely handled (either by the initial guard clause in the original code or naturally by the unified loop in the improved version).
* **Trailing zeros (`120` -> `21`):** Trailing zeros become leading zeros during reversal (`021`), which naturally drop off during arithmetic operations.
* **Single-digit numbers (`0`, `7`, `-5`):** The loop runs once (or zero times for `0`) and correctly returns the number as-is.
* **Reversals that exceed limits (`1534236469`):** Reversing this input yields `9646324351`, which overflows a 32-bit integer. The boundary check `ans > INT_MAX / 10` catches this before the overflow happens and safely returns `0`.
