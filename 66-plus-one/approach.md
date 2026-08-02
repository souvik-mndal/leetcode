![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.25%20MB%20(beats%2098.09%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of digits representing a large positive whole number. For example, the array `[1, 2, 3]` represents the integer `123`. The digits are listed from left to right, starting with the most significant digit (the highest place value). 

Your task is to add `1` to this number and return the result as a new array of digits. 

Examples:
* `[1, 2, 3]` becomes `[1, 2, 4]` (123 + 1 = 124)
* `[4, 3, 2, 1]` becomes `[4, 3, 2, 2]` (4321 + 1 = 4322)
* `[9]` becomes `[1, 0]` (9 + 1 = 10)

## Intuition

Think about how you add `1` to a number on paper. You always start at the rightmost digit (the ones place).

* **Case 1: The last digit is less than 9.** You simply increment it by 1 (e.g., `3` becomes `4`), and you are done.
* **Case 2: The last digit is 9.** Adding 1 turns it into `10`. You write down `0` and **carry** the `1` over to the left neighbor.
* **Case 3: All digits are 9 (like `999`).** Every `9` turns into `0`, and the carry travels all the way to the front, adding a new leading digit `1` (making `1000`).

The "aha" moment is that as soon as a carry becomes `0`, no more digits change. You can stop processing early and copy the remaining digits as they are.

## Approach

Here is step-by-step how the current code works:

* **Start at the end:** Initialize `size` to point to the last digit in the `digits` array. Create an empty vector `ans_vec` to collect the result digits in reverse order.
* **Loop while moving left:**
  * **Add the increment:** On the first digit (when `carry` is 0), add `1` to `digits[size]`. On later steps, add the existing `carry`.
  * **Store the digit:** Take `sum % 10` (the single digit for this position) and append it to `ans_vec`.
  * **Update the carry:** Set `carry` to `sum / 10` (which is `1` if sum was 10, or `0` otherwise).
  * **Check early exit:** If `carry` is now `0`, stop the loop early because no further leftward digits need modification.
* **Handle extra carry:** If the loop ends and `carry` is still `1` (e.g., for `[9, 9]`), append `1` to `ans_vec`.
* **Copy remaining digits:** If the loop stopped early, copy all remaining untouched digits from the original array into `ans_vec`.
* **Reverse and return:** Reverse `ans_vec` so the digits are back in left-to-right order, then return it.

## Time & Space Complexity

* **Time:** **O(n)** — where $n$ is the number of digits. In the worst case (e.g., `[9, 9, 9]`), the loop runs $n$ times, and reversing the vector takes $O(n)$ time.
* **Space:** **O(n)** — a new vector `ans_vec` is created to store up to $n + 1$ digits.

### Can this be improved?

**Yes, space can be improved.** 

Instead of building a separate `ans_vec` and reversing it, you can modify the original `digits` array in-place. If all digits turn to `0` (e.g., `[9, 9, 9]` becomes `[0, 0, 0]`), you just insert a `1` at the front.

Here is the optimized code:

```cpp
vector<int> plusOne(vector<int>& digits) {
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] < 9) {
            digits[i]++;
            return digits; // Done early!
        }
        digits[i] = 0; // Carry the 1 by turning 9 into 0
    }
    // If all digits were 9, we need a leading 1 (e.g., 999 -> 1000)
    digits.insert(digits.begin(), 1);
    return digits;
}
```

* **Improved Complexity:**
  * **Time:** **O(n)** worst-case, but **O(1)** on average (since most numbers do not end in `9` and return immediately after one step).
  * **Space:** **O(1)** auxiliary space — modifies the array directly without allocating extra memory for a second array.
* **Is this optimal?** Yes. $O(n)$ time is theoretically required because in the worst case (all `9`s), every digit must be updated. $O(1)$ extra space is the absolute best possible memory usage.

## Edge Cases Handled

* **Single-digit numbers (`[0]` through `[8]`):** Increments to the correct digit immediately without loop iterations beyond the first.
* **Single digit 9 (`[9]`):** Turns into `[1, 0]`, adding a new leading digit.
* **No carry propagate (`[1, 2, 3]`):** Changes the last digit to `4` and exits early on the first iteration.
* **Partial carry (`[1, 2, 9]`):** Carries over to turn `9` into `0` and `2` into `3`, then stops before touching `1`, returning `[1, 3, 0]`.
* **All 9s (`[9, 9, 9]`):** Carries through every digit, turning them into `0`s, then correctly appends a leading `1` to return `[1, 0, 0, 0]`.
