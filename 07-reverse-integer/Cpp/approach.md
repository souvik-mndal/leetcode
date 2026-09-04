![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%2020.05%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.66%20MB%20(beats%2020.43%25)-orange?style=for-the-badge)

---

## Problem Explained

The goal is to take a 32-bit signed integer `x` and turn its digits backwards. For example, `123` becomes `321`, and `-123` becomes `-321`. Any trailing zeros disappear naturally in the math, so `120` becomes `21`.

There is one important condition: standard 32-bit signed integers can only hold values from `-2^31` (`-2,147,483,648`) up to `2^31 - 1` (`2,147,483,647`). If reversing `x` produces a number that spills outside of this range, the function must return `0`. 

You must solve this without using 64-bit integers (like `long long` in C++). All calculations must stay within 32-bit integer limits.

---

## Intuition

Think of how you would read off digits from the end of a physical number display. 

To pop the last digit off a number, you use remainder arithmetic: `x % 10`. To remove that last digit permanently from `x`, you divide by 10: `x / 10`. 

To push that extracted digit into your reversed result, you multiply your running total by 10 (shifting all existing digits one position to the left) and add the extracted digit: `ans = ans * 10 + digit`.

The tricky part is preventing integer overflow. Multiplying `ans` by 10 can instantly break the 32-bit limit before you even store it. The key trick is to check **before** you multiply: if `ans` is already bigger than `INT_MAX / 10`, multiplying it by 10 will definitely break the limit, so you must abort early and return `0`.

---

## Approach

Here is what the code does step-by-step:

* `bool flag=0;`: Creates a boolean flag set to `0` (false) to remember if the original number `x` was negative.
* `if(x<= INT_MIN ){ return 0; }`: Checks if `x` is equal to or smaller than the lowest possible 32-bit integer (`-2,147,483,648`). Inverting `INT_MIN` directly to positive would exceed `INT_MAX` and cause an immediate overflow error, so we return `0` right away.
* `if( x<0 ){ x*=-1; flag=1; }`: If `x` is negative, converts `x` to a positive number so digit extractions are simple positive numbers, and sets `flag` to `1` (true).
* `int ans=0;`: Initializes `ans` to `0` to hold the running reversed number.
* `while(x!=0){`: Starts a loop that keeps going until all digits of `x` have been processed down to zero.
* `int ld=x%10;`: Extracts the last digit (`ld`) of `x` using the modulo operator `%`.
* `if( ans>(INT_MAX/10)){ return 0; }`: Checks if `ans` is already too large. If `ans` is strictly greater than `INT_MAX / 10`, then `ans * 10` will overflow 32-bit limits. It safely returns `0` before overflow happens.
* `ans=ans*10+ld;`: Shifts existing digits in `ans` to the left by multiplying by 10, then adds the last digit `ld`.
* `x/=10;`: Removes the last digit from `x` using integer division.
* `if( flag ){ return -ans; } else{ return ans; }`: After the loop finishes, checks `flag`. If the original number was negative, returns `-ans`. Otherwise, returns `ans`.

---

## Dry Run

### Case 1: Typical positive integer (`x = 123`)

| Step / Action | `x` | `flag` | `ans` | `ld` | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | 123 | 0 | 0 | - | `x <= INT_MIN` is false. `x < 0` is false. |
| Loop 1: Extract digit | 123 | 0 | 0 | 3 | `ld = 123 % 10 = 3`. `ans > INT_MAX/10` is false. |
| Loop 1: Update total | 12 | 0 | 3 | 3 | `ans = 0 * 10 + 3 = 3`. `x = 123 / 10 = 12`. |
| Loop 2: Extract digit | 12 | 0 | 3 | 2 | `ld = 12 % 10 = 2`. `ans > INT_MAX/10` is false. |
| Loop 2: Update total | 1 | 0 | 32 | 2 | `ans = 3 * 10 + 2 = 32`. `x = 12 / 10 = 1`. |
| Loop 3: Extract digit | 1 | 0 | 32 | 1 | `ld = 1 % 10 = 1`. `ans > INT_MAX/10` is false. |
| Loop 3: Update total | 0 | 0 | 321 | 1 | `ans = 32 * 10 + 1 = 321`. `x = 1 / 10 = 0`. |
| End of loop | 0 | 0 | 321 | 1 | Loop ends (`x == 0`). `flag` is 0, so return `321`. |

### Case 2: Negative integer (`x = -123`)

| Step / Action | `x` | `flag` | `ans` | `ld` | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | -123 | 0 | 0 | - | `x <= INT_MIN` is false. `x < 0` is true. |
| Sign Flip | 123 | 1 | 0 | - | `x` becomes `123`, `flag` set to `1`. |
| Loop 1 | 12 | 1 | 3 | 3 | `ld = 3`. `ans = 0 * 10 + 3 = 3`. `x = 12`. |
| Loop 2 | 1 | 1 | 32 | 2 | `ld = 2`. `ans = 3 * 10 + 2 = 32`. `x = 1`. |
| Loop 3 | 0 | 1 | 321 | 1 | `ld = 1`. `ans = 32 * 10 + 1 = 321`. `x = 0`. |
| End of loop | 0 | 1 | 321 | 1 | Loop ends. `flag` is 1, so return `-ans` = `-321`. |

---

## Time & Space Complexity

* **Time Complexity:** **O(log10(x))** — The number of loop iterations equals the number of digits in `x`. Since a 32-bit integer has at most 10 digits, the loop runs at most 10 times. This is effectively **O(1)** constant time.
* **Space Complexity:** **O(1)** — Only a few primitive variables (`flag`, `ans`, `ld`) are created, using a constant amount of extra memory.

### Can it be improved?

The time and space complexities are already optimal, but the code logic can be made cleaner and more robust. 

In C++, modulo division on negative numbers yields negative remainders (e.g., `-123 % 10 = -3`). We can process negative digits directly without converting `x` to positive, avoiding the `flag` variable and the manual check for `INT_MIN`. Furthermore, exact overflow checks should account for the exact tail digits (`7` for positive overflow, `-8` for negative overflow).

Here is the cleaner, unified logic:

```cpp
int reverse(int x) {
    int ans = 0;
    while (x != 0) {
        int ld = x % 10;
        
        // Check upper overflow bound (2147483647)
        if (ans > INT_MAX / 10 || (ans == INT_MAX / 10 && ld > 7)) return 0;
        // Check lower overflow bound (-2147483648)
        if (ans < INT_MIN / 10 || (ans == INT_MIN / 10 && ld < -8)) return 0;
        
        ans = ans * 10 + ld;
        x /= 10;
    }
    return ans;
}
```

* `if (ans > INT_MAX / 10 || ...)`: Prevents positive overflow before it happens, accounting for both standard growth and boundary exactness.
* `if (ans < INT_MIN / 10 || ...)`: Prevents negative overflow without needing special case flags or absolute conversions.

* **Resulting Complexity:** **O(log10(x))** time, **O(1)** space. This reaches the theoretical minimum complexity required for this problem.

---

## Edge Cases Handled

* **Number equals `INT_MIN` (`-2,147,483,648`):** Converting this directly to positive causes an immediate 32-bit overflow. The initial code handles this with `if(x <= INT_MIN) return 0;`.
* **Overflow during reversal (e.g., `x = 1,534,236,469`):** Reversing this gives `9,646,324,351`, which exceeds `INT_MAX`. Handled by checking `ans > INT_MAX / 10` before multiplying.
* **Numbers with trailing zeros (e.g., `x = 120`):** Reversing `120` gives `021`, which automatically simplifies to `21` through basic integer math (`0 * 10 + 2 = 2`).
* **Single-digit numbers (e.g., `x = 0` or `x = 7`):** The loop runs exactly once and returns the single digit immediately.
