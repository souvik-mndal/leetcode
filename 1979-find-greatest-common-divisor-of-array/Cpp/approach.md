![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-16.3%20MB%20(beats%2034.73%25)-orange?style=for-the-badge)

---

## Problem Explained

This problem asks you to do three simple tasks with a list of numbers:
1. Find the smallest number in the list.
2. Find the largest number in the list.
3. Calculate the **Greatest Common Divisor (GCD)** of those two numbers.

The **Greatest Common Divisor** is the biggest positive whole number that divides both numbers completely, leaving zero remainder. 

For example, if your list is `[2, 5, 6, 9, 10]`:
* The smallest number is **2**.
* The largest number is **10**.
* The largest number that divides both 2 and 10 without leaving a remainder is **2**. So, the answer is 2.

## Intuition

You do not need to compare every single pair of numbers in the array. You only need to care about two specific values: the minimum and the maximum.

Once you loop through the array once to pick out the smallest (`mini`) and largest (`maxi`) values, the problem reduces to finding the GCD of two integers. 

To find the GCD, this solution uses a math idea called the **Euclidean Algorithm** through repeated subtraction. If you have two numbers and keep subtracting the smaller one from the larger one, they will eventually become equal. That final equal number is their greatest common divisor.

## Approach

Here is how the code works step-by-step:

* `int maxi=-1,mini=INT_MAX;`: We set up two variables to track our bounds. `maxi` starts at `-1` (smaller than any valid input), and `mini` starts at `INT_MAX` (the largest possible integer) so any real number in the array will replace them.
* `for( int i=0 ; i<nums.size() ; i++ )`: We loop through the array `nums` from start to end, checking one element at a time.
* `maxi=max(maxi,nums[i]);`: We update `maxi` if the current element `nums[i]` is larger than our current maximum.
* `mini=min(mini,nums[i]);`: We update `mini` if the current element `nums[i]` is smaller than our current minimum.
* `return gcd(maxi,mini);`: After finding the smallest and largest numbers, we pass them into our custom `gcd` function and return the result.
* `while( a != b )`: Inside the `gcd` function, we run a loop that keeps going until both numbers `a` and `b` become equal.
* `if( a>b ) { a=a-b; }`: If `a` is larger than `b`, we subtract `b` from `a` to make `a` smaller.
* `else { b=b-a; }`: If `b` is larger than or equal to `a`, we subtract `a` from `b` to make `b` smaller.
* `return a;`: When `a` and `b` become equal, the loop ends, and we return `a` as the greatest common divisor.

## Dry Run

### Case 1: Standard case `nums = [7, 5, 6, 8, 3]`

**Step 1: Finding `maxi` and `mini`**

| `i` | `nums[i]` | `maxi` | `mini` | Action |
| :--- | :--- | :--- | :--- | :--- |
| - | - | -1 | 2147483647 | Initial state |
| 0 | 7 | 7 | 7 | Updated `maxi` to 7, `mini` to 7 |
| 1 | 5 | 7 | 5 | Updated `mini` to 5 |
| 2 | 6 | 7 | 5 | No change |
| 3 | 8 | 8 | 5 | Updated `maxi` to 8 |
| 4 | 3 | 8 | 3 | Updated `mini` to 3 |

Result: `maxi = 8`, `mini = 3`.

**Step 2: Computing `gcd(8, 3)`**

| `a` | `b` | Condition (`a != b`) | Action |
| :--- | :--- | :--- | :--- |
| 8 | 3 | True | `a > b`, so `a = 8 - 3 = 5` |
| 5 | 3 | True | `a > b`, so `a = 5 - 3 = 2` |
| 2 | 3 | True | `a < b`, so `b = 3 - 2 = 1` |
| 2 | 1 | True | `a > b`, so `a = 2 - 1 = 1` |
| 1 | 1 | False | Loop ends, return `a = 1` |

Output: **1**

---

### Case 2: Edge case with identical elements `nums = [3, 3]`

**Step 1: Finding `maxi` and `mini`**

| `i` | `nums[i]` | `maxi` | `mini` | Action |
| :--- | :--- | :--- | :--- | :--- |
| - | - | -1 | 2147483647 | Initial state |
| 0 | 3 | 3 | 3 | Updated `maxi` to 3, `mini` to 3 |
| 1 | 3 | 3 | 3 | No change |

Result: `maxi = 3`, `mini = 3`.

**Step 2: Computing `gcd(3, 3)`**

| `a` | `b` | Condition (`a != b`) | Action |
| :--- | :--- | :--- | :--- |
| 3 | 3 | False | Loop never runs, return `a = 3` |

Output: **3**

## Time & Space Complexity

* **Time Complexity:** **O(N + max_val)** where N is the length of the array and `max_val` is the largest element in the array. Finding the min and max takes O(N) time. The custom `gcd` function uses subtraction, which in the worst case (like `gcd(1000, 1)`) subtracts 1 a total of 999 times.
* **Space Complexity:** **O(1)** auxiliary space. The solution only uses a fixed set of simple variables (`maxi`, `mini`, `a`, `b`) and no extra array or dynamic memory.

### Can it be improved?

**Yes, the time complexity can be improved.**

Repeated subtraction is slow when one number is much larger than the other. Instead of subtracting `b` from `a` over and over again until `a` is smaller than `b`, we can jump straight to the final answer using the **remainder (modulo)** operator `%`. This is the standard Euclidean algorithm. Alternatively, C++ has a built-in standard library function `std::gcd` inside the `<numeric>` header.

Here is the key change using standard modern C++:

```cpp
#include <numeric> // gives access to std::gcd

class Solution {
public:
    int findGCD(vector<int>& nums) {
        int maxi = -1, mini = INT_MAX;
        for (int num : nums) {
            maxi = max(maxi, num);
            mini = min(mini, num);
        }
        return std::gcd(maxi, mini); // Uses fast modulo Euclidean algorithm
    }
};
```

* **Improved Time Complexity:** **O(N + log(min_val))**. Finding min and max takes O(N) time. Modern GCD using modulo takes logarithmic time relative to the smaller number. Since the max value in constraints is 1000, `std::gcd` takes at most around 10 operations.
* **Improved Space Complexity:** **O(1)** space.
* **Is this optimal?** Yes. We must inspect every element in `nums` at least once to find the minimum and maximum, so any solution must take at least O(N) time. This improved code reaches that theoretical minimum limit.

## Edge Cases Handled

* **Smallest possible array size (2 elements):** The loop handles arrays with as few as two elements properly without out-of-bounds errors.
* **All elements are identical (e.g., `[3, 3]`):** `maxi` and `mini` both become `3`. The GCD loop immediately sees `a == b` and returns `3`.
* **Numbers with no common prime factors (Coprime numbers, e.g., `[3, 8]`):** The GCD logic reduces down to `1` correctly.
* **Array elements equal to constraints bounds (1 to 1000):** Initial values (`maxi = -1` and `mini = INT_MAX`) ensure proper updates regardless of array contents.
