![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2078.83%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-30.99%20MB%20(beats%2051.58%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given an array of single digits called `digits`. Your job is to find out how many unique 3-digit even numbers you can build using these digits.

A valid number must follow these rules:
1. It must be a 3-digit number (from 100 to 999).
2. It cannot start with `0` (no leading zero).
3. It must be even (the last digit must be 0, 2, 4, 6, or 8).
4. Each element in the array can only be used once per number (you pick digits from three distinct positions in the array).
5. Repeated final numbers must only be counted once.

For example, if `digits = [1, 2, 3, 4]`, you can form numbers like 124, 132, and 432. All of these are 3 digits, even, and have no leading zero. The total count of such unique numbers is 12.

## Intuition

To form a 3-digit number, we need to pick three distinct positions from our array: one for the hundreds place, one for the tens place, and one for the ones (units) place.

Instead of doing complex math upfront, we can simply try every possible combination of 3 different positions from the array using three loops:
* The first loop picks a index for the **hundreds** place. If that digit is `0`, we skip it right away because numbers cannot start with zero.
* The second loop picks a index for the **tens** place. It must not use the same array position as the hundreds place.
* The third loop picks a index for the **ones** place. It must be a different position from both hundreds and tens, and the digit itself must be **even**.

Whenever we find three valid positions, we put the three digits together into a single number. We store every formed number in a set. The set automatically ignores duplicates, leaving us with only the distinct valid numbers at the end.

## Approach

* `unordered_set<int> seen;` — Creates a set to store the formed numbers. A set automatically drops repeated numbers so we only count unique results.
* `int n = digits.size();` — Gets the total number of digits in the array.
* `for(int h = 0; h < n; h++)` — Starts a loop to pick an index `h` for the hundreds digit.
* `if (digits[h] == 0) continue;` — Skips index `h` if its value is `0`, preventing numbers with leading zeros.
* `for(int t = 0; t < n; t++)` — Starts a loop to pick an index `t` for the tens digit.
* `if (t == h) continue;` — Skips index `t` if it uses the same array element as the hundreds place.
* `for(int u = 0; u < n; u++)` — Starts a loop to pick an index `u` for the units (ones) digit.
* `if(u == h || u == t) continue;` — Skips index `u` if it reuses the array element picked for the hundreds or tens place.
* `if(digits[u] % 2 != 0) continue;` — Skips index `u` if the digit is odd, keeping only even numbers.
* `int num = digits[h]*100+digits[t]*10+digits[u];` — Combines the three chosen digits into a complete 3-digit number.
* `seen.insert(num);` — Adds the newly formed number to our set.
* `return seen.size();` — Returns the total count of unique numbers stored in the set.

## Dry Run

### Case 1: Edge case with duplicate values (`digits = [0, 2, 2]`)

Here `n = 3`. The code checks positions `h`, `t`, and `u`:

| h | t | u | digits[h], digits[t], digits[u] | Action | `seen` Set |
|---|---|---|---|---|---|
| 0 | - | - | 0, -, - | Skip: `digits[h]` is 0 (leading zero) | `{}` |
| 1 | 0 | 2 | 2, 0, 2 | Valid even number! `num = 202` | `{202}` |
| 1 | 2 | 0 | 2, 2, 0 | Valid even number! `num = 220` | `{202, 220}` |
| 2 | 0 | 1 | 2, 0, 2 | Duplicate number `202`, set ignores it | `{202, 220}` |
| 2 | 1 | 0 | 2, 2, 0 | Duplicate number `220`, set ignores it | `{202, 220}` |

**Final Result:** `seen.size()` is 2.

---

### Case 2: No valid even numbers (`digits = [1, 3, 5]`)

Here `n = 3`. All digits are odd.

| h | t | u | digits[h], digits[t], digits[u] | Action | `seen` Set |
|---|---|---|---|---|---|
| 0 | 1 | 2 | 1, 3, 5 | Skip: `digits[u]` (5) is odd | `{}` |
| 0 | 2 | 1 | 1, 5, 3 | Skip: `digits[u]` (3) is odd | `{}` |
| 1 | 0 | 2 | 3, 1, 5 | Skip: `digits[u]` (5) is odd | `{}` |
| 1 | 2 | 0 | 3, 5, 1 | Skip: `digits[u]` (1) is odd | `{}` |
| 2 | 0 | 1 | 5, 1, 3 | Skip: `digits[u]` (3) is odd | `{}` |
| 2 | 1 | 0 | 5, 3, 1 | Skip: `digits[u]` (1) is odd | `{}` |

**Final Result:** `seen.size()` is 0.

## Time & Space Complexity

* **Time Complexity:** O(N^3) — The three nested loops iterate through all index combinations. Since the constraints state `N <= 10`, `N^3` is at most 1000 operations, which runs almost instantly.
* **Space Complexity:** O(1) bound — The set stores unique 3-digit even numbers. There are at most 450 possible 3-digit even numbers (from 100 to 998), so space usage never grows beyond a small constant limit.

### Can we optimize it?

Yes, we can improve the time complexity to **O(N)**!

Instead of generating numbers from the array indices (which takes O(N^3) time), we can invert the thinking:
1. Count the frequency of each digit (0 through 9) in `digits` in a single pass of O(N) time.
2. Loop through all possible 3-digit even numbers from 100 to 998 (stepping by 2). There are only 450 such numbers.
3. For each candidate number, count how many of each digit it needs. If our frequency array has enough copies of those digits, we increment our total count.

Since checking 450 numbers takes constant time, the overall time drops to O(N).

Here are the key lines for the optimized approach:

```cpp
vector<int> freq(10, 0);
for (int d : digits) freq[d]++;

int count = 0;
// Test every valid 3-digit even number
for (int num = 100; num < 1000; num += 2) {
    int h = num / 100;
    int t = (num / 10) % 10;
    int u = num % 10;
    
    vector<int> req(10, 0);
    req[h]++; req[t]++; req[u]++;
    
    // Check if we have enough of each digit available
    if (req[h] <= freq[h] && req[t] <= freq[t] && req[u] <= freq[u]) {
        count++;
    }
}
return count;
```

* **Improved Time Complexity:** O(N) — One pass to count input frequencies, followed by a fixed 450 iterations.
* **Improved Space Complexity:** O(1) — Uses fixed array sizes of 10 for counting digits.
* **Theoretical Best Complexity:** O(N) time and O(1) space. We must read the input array at least once to know what digits we have, so O(N) is optimal, and the improved version reaches it.

## Edge Cases Handled

* **Leading Zeros:** Digits equal to `0` are rejected for the hundreds place (`digits[h] == 0`), ensuring numbers like `022` are never created.
* **Duplicate Digits in Input:** Multiple copies of a digit in the array (e.g., `[0, 2, 2]`) allow numbers that use that digit multiple times (like `202`), while single copies restrict reuse.
* **Duplicate Formed Numbers:** Using a set ensures that identical numbers constructed from different index combinations are counted only once.
* **No Even Digits Present:** If all input digits are odd (e.g., `[1, 3, 5]`), the condition `digits[u] % 2 != 0` skips all paths and correctly returns `0`.
* **Minimum Input Size:** Handles the smallest constraint size (`digits.length = 3`) correctly without out-of-bounds errors.
