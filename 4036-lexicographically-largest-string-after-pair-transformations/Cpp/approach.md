![Runtime](https://img.shields.io/badge/Runtime-103%20ms%20(beats%2072.39%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-209.5%20MB%20(beats%2017.75%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array of positive integers named `nums`. Each integer `x` represents a string that initially consists of `x` copies of the letter `'a'`.

You can repeatedly combine two adjacent identical letters into the next letter in the alphabet:
* Two `'a'`s combine into one `'b'`.
* Two `'b'`s combine into one `'c'`.
* Two `'c'`s combine into one `'d'`, and so on.
* Crucially, two `'z'`s **cannot** be combined into anything.

Your goal is to find the **lexicographically largest** string you can form for each number in `nums`. A string is lexicographically larger if it has an alphabetically later letter at the first character where two strings differ (for example, `"ca"` is larger than `"bba"`, and `"b"` is larger than `"aa"`).

## Intuition

Think of this as counting in binary, where each letter represents a power of 2:
* `'a'` = 1 (which is 2^0)
* `'b'` = 2 (which is 2^1)
* `'c'` = 4 (which is 2^2)
* ...
* `'z'` = 2^25 = 33,554,432 (which is 2^25)

Since `'z'` cannot be combined into anything higher, any block of 2^25 `'a'`s can only max out as a single `'z'`. 

To make the overall string as lexicographically large as possible, we want the biggest letters placed as early in the string as possible:
1. First, make as many `'z'` characters as possible by dividing `x` by 2^25. Put all `'z'`s at the start.
2. Next, look at the remaining remainder (which is strictly less than 2^25). Express this remainder in binary.
3. Every bit that is set to 1 in the binary form corresponds to a specific letter from `'y'` down to `'a'`.
4. Append these letters in **descending order** (from highest letter `'y'` down to `'a'`). Because binary representations are unique and higher bits mean higher letters, putting larger letters first automatically gives the lexicographically largest possible string!

## Approach

Here is how the logic executes step-by-step in the code:

* `vector<int> calveroniq = nums;`: Creates a duplicate copy of the input vector `nums` (this is unused helper memory in the original solution).
* `vector<string> ans;`: Initializes the result vector that will hold the final transformed string for each input number.
* `for (long long x : nums)`: Loops through each number `x` in `nums`, casting it to a 64-bit integer to prevent overflow during bitwise shifts.
* `long long z = x / (1LL << 25);`: Calculates how many complete `'z'` characters can be formed by integer dividing `x` by 2^25 (written as `1LL << 25`).
* `s.append(z, 'z');`: Appends `z` copies of the character `'z'` to the output string `s`.
* `x %= (1LL << 25);`: Reduces `x` to its remainder after taking out all full `z`-blocks.
* `for (int bit = 24; bit >= 0; --bit)`: Iterates through binary bit positions from 24 (which corresponds to letter `'y'`) down to 0 (which corresponds to letter `'a'`).
* `if (x & (1LL << bit))`: Checks if the bit at index `bit` is set to 1 in the number `x`.
* `s += char('a' + bit);`: If the bit is set, appends the corresponding character (`'a' + bit`) to string `s`.
* `ans.push_back(s);`: Adds the constructed string `s` to the `ans` array.

## Dry Run

### Case 1: `nums = [2, 5]` (Small values)

| Loop `x` | Calculated `z` | Remainder `x` | `bit` checked | Bit set? | Current `s` | Action |
|---|---|---|---|---|---|---|
| 2 | 0 | 2 | 1 | Yes (2^1 = 2) | `"b"` | Appends `'a' + 1 = 'b'`. Loop finishes. |
| 5 | 0 | 5 | 2 | Yes (2^2 = 4) | `"c"` | Appends `'a' + 2 = 'c'`. |
| 5 | 0 | 1 | 1 | No | `"c"` | Bit 1 is 0. Skip. |
| 5 | 0 | 1 | 0 | Yes (2^0 = 1) | `"ca"` | Appends `'a' + 0 = 'a'`. Loop finishes. |

### Case 2: `nums = [33554435]` (Value exceeding 2^25)

Note: 33,554,435 = (1 * 2^25) + 3.

| Loop `x` | Calculated `z` | Remainder `x` | `bit` checked | Bit set? | Current `s` | Action |
|---|---|---|---|---|---|---|
| 33554435 | 1 | 3 | - | - | `"z"` | Appends 1 copy of `'z'`. |
| 33554435 | 1 | 3 | 1 | Yes (2^1 = 2) | `"zb"` | Appends `'a' + 1 = 'b'`. |
| 33554435 | 1 | 3 | 0 | Yes (2^0 = 1) | `"zba"` | Appends `'a' + 0 = 'a'`. Loop finishes. |

## Time & Space Complexity

* **Time Complexity:** **O(N)** — where N is the length of `nums`. For each number, calculating `z` takes constant time O(1). The inner bit loop always runs exactly 25 times, which is O(1). Since each number takes O(1) work and string lengths are bounded by at most 28 characters (since max x is 10^8), total runtime scales linearly with N.
* **Space Complexity:** **O(N)** — to store the output array `ans`. The extra memory created by `calveroniq` also takes O(N) memory.

### Optimization Discussion

**Is this already optimal?**

The time complexity O(N) is optimal because we must process every element in `nums` at least once. 

However, the current space usage has an unnecessary allocation:
The line `vector<int> calveroniq = nums;` creates a full copy of the input vector but is never used anywhere in the code. Removing this line frees up O(N) extra temporary memory and speeds up performance.

Here is the cleaned-up key loop:

```cpp
vector<string> largestString(vector<int>& nums) {
    vector<string> ans;
    ans.reserve(nums.size()); // Pre-allocating avoids vector resizing overhead

    for (long long x : nums) {
        string s;
        
        // Calculate count of 'z's
        long long z = x / (1LL << 25);
        s.append(z, 'z');
        x %= (1LL << 25);

        // Convert remainder to descending letters using binary bits
        for (int bit = 24; bit >= 0; --bit) {
            if (x & (1LL << bit)) {
                s += char('a' + bit);
            }
        }

        ans.push_back(s);
    }

    return ans;
}
```

* **Improved Extra Space Complexity:** **O(1)** auxiliary space (excluding the memory required for the output array `ans`).
* **Theoretical Best Complexity:** **O(N)** time and **O(1)** auxiliary space. The optimized code reaches this theoretical limit.

## Edge Cases Handled

* **Smallest constraint input (`x = 1`):** `z = 0`, remainder is 1 (bit 0 is set), correctly outputs `"a"`.
* **Exact powers of 2 (`x = 2, 4, 8...`):** Bitwise check activates for exactly one bit, producing a single character string like `"b"`, `"c"`, or `"d"`.
* **Numbers equal to or larger than 2^25 (`x >= 33,554,432`):** Correctly handles large numbers up to 10^8 by creating one or more `'z'` characters first, then appending the remainder.
* **Numbers with consecutive bits set (`x = 3, 7, 15`):** Properly orders letters from highest to lowest (e.g. `x = 7` produces `"cba"` instead of `"abc"`), satisfying the lexicographical requirement.
