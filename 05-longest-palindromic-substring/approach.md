![Runtime](https://img.shields.io/badge/Runtime-1046%20ms%20(beats%205.02%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-521.01%20MB%20(beats%205.14%25)-red?style=for-the-badge)

---

## Problem Explained

A **palindrome** is a word or sequence of characters that reads the exact same forward and backward, like `"racecar"` or `"aba"`. A **substring** is a continuous, unbroken sequence of characters inside a string. For example, `"aba"` is a substring of `"babad"`, but `"bad"` is not because its letters are not right next to each other in the original word.

Given a string `s`, your task is to find the longest contiguous piece of `s` that is a palindrome. If there are multiple palindromic substrings of the same maximum length, returning any one of them is acceptable.

For example:
* Input: `s = "babad"` -> Output: `"bab"` (or `"aba"`, both have length 3).
* Input: `s = "cbbd"` -> Output: `"bb"` (length 2).

---

## Intuition

Think of a palindrome like opening a pair of scissors from a central point. Every palindrome expands outward from its center, matching symmetric characters on the left and right sides.

There are two kinds of centers to consider:
1. **Odd-length centers:** A single character sits in the exact middle, like the letter `'b'` in `"aba"`.
2. **Even-length centers:** The center lies between two identical adjacent characters, like between the two `'b'`s in `"abba"`.

The core trick—called **Expand Around Center**—is to treat every character (and every space between adjacent characters) as a potential middle point. For each center, we push a left pointer backward and a right pointer forward, checking if the characters match. As soon as two characters do not match (or we reach the edge of the string), we stop expanding for that center.

---

## Approach

Here is how the code builds this logic step-by-step:

* `for( int i=0 ; i<s.size() ; i++ )`: Loop through every index `i` of the string `s`, treating character `i` as the middle of a potential palindrome.
* `string one = check(s,i,i);`: Call the helper function `check` with `st = i` and `end = i` to expand around a single character center, finding the longest **odd-length** palindrome at index `i`.
* `string two = check(s,i,i+1);`: Call `check` with `st = i` and `end = i + 1` to expand around the gap between adjacent characters, finding the longest **even-length** palindrome centered between index `i` and `i + 1`.
* `while( st>=0 && end<s.size() )`: Inside `check`, keep expanding as long as the left pointer `st` has not gone past the start of the string and the right pointer `end` has not gone past the end.
* `if( s[st] == s[end] )`: Compare the character at `st` with the character at `end`. If they match, the current range forms a valid palindrome.
* `store = s.substr(st,(end-st)+1);`: Create and store the valid palindrome substring starting at index `st` with length `(end - st) + 1` into the `store` variable.
* `st--; end++;`: Move `st` one step left and `end` one step right to check the next expanded pair of characters.
* `else{ break; }`: Stop expanding immediately if the characters at `st` and `end` do not match.
* `if( one.size() > two.size() )`: Compare the odd-length palindrome `one` and even-length palindrome `two` found at center `i`, assigning the longer one to `temp`.
* `if( temp.size() > ans.size() )`: Compare `temp` with `ans` (the overall longest palindrome found so far across all centers) and update `ans` if `temp` is longer.

---

## Dry Run

### Case 1: `s = "babad"` (Typical case)

| `i` | Center Type | `one` | `two` | `temp` | `ans` | Action |
|---|---|---|---|---|---|---|
| `0` | Center `'b'` | `"b"` | `""` | `"b"` | `"b"` | `ans` initialized to `"b"`. |
| `1` | Center `'a'` | `"aba"` | `""` | `"aba"` | `"aba"` | `one` expands to `"aba"`. `ans` updated to `"aba"`. |
| `2` | Center `'b'` | `"bab"` | `""` | `"bab"` | `"aba"` | `one` expands to `"bab"`. Length 3 is not greater than `ans` length 3, so `ans` remains `"aba"`. |
| `3` | Center `'a'` | `"a"` | `""` | `"a"` | `"aba"` | `temp` length 1 is shorter than `ans`. |
| `4` | Center `'d'` | `"d"` | `""` | `"d"` | `"aba"` | `temp` length 1 is shorter than `ans`. |

---

### Case 2: `s = "cbbd"` (Even-length palindrome case)

| `i` | Center Type | `one` | `two` | `temp` | `ans` | Action |
|---|---|---|---|---|---|---|
| `0` | Center `'c'` | `"c"` | `""` | `"c"` | `"c"` | `ans` set to `"c"`. |
| `1` | Center `'b'` | `"b"` | `"bb"` | `"bb"` | `"bb"` | `two` expands at `st=1, end=2` matching `'b'=='b'`. `ans` updated to `"bb"`. |
| `2` | Center `'b'` | `"b"` | `""` | `"b"` | `"bb"` | No longer palindrome found. |
| `3` | Center `'d'` | `"d"` | `""` | `"d"` | `"bb"` | No longer palindrome found. |

---

## Time & Space Complexity

* **Time Complexity:** **O(N^3)** — The outer loop runs `N` times. Inside, the while loop expands up to `N` times. Crucially, calling `s.substr()` inside the while loop creates a brand-new string copy of length up to `N` on **every single matching step**. Creating string slices takes O(N) time. Repeating an O(N) slice operation inside an O(N) expansion for O(N) centers results in `N * N * N` operations, leading to an **O(N^3)** runtime. This explains the 5% performance score.
* **Space Complexity:** **O(N)** — Creating multiple temporary substrings (`store`, `one`, `two`, `temp`, `ans`) allocates memory proportional to string length `N`.

### Can this be improved?

**Yes, significantly.**

#### Optimization 1: Avoid string allocations during expansion
Instead of building substring objects inside the inner loop, we can return just the **integer length** of the palindrome from the helper function. We track only the `start` index and `maxLen` as integers in the main loop, and perform **a single string cut (`substr`) at the very end** when returning the result.

This removes the extra O(N) factor from inside the while loop:
* **Improved Time:** **O(N^2)**
* **Improved Space:** **O(1)** extra memory (excluding the output string).

```cpp
// Fast helper: returns integer length instead of allocating strings
int expand(const string& s, int st, int end) {
    while (st >= 0 && end < s.size() && s[st] == s[end]) {
        st--;
        end++;
    }
    // st and end overshot by 1 step, so length is (end - 1) - (st + 1) + 1
    return end - st - 1;
}

string longestPalindrome(string s) {
    int start = 0, maxLen = 0;
    for (int i = 0; i < s.size(); i++) {
        int len1 = expand(s, i, i);     // Odd length
        int len2 = expand(s, i, i + 1); // Even length
        int len = max(len1, len2);
        
        if (len > maxLen) {
            maxLen = len;
            // Calculate starting index of the palindrome from center i
            start = i - (len - 1) / 2;
        }
    }
    return s.substr(start, maxLen); // Single slice at the end
}
```

* Line `return end - st - 1;`: Calculates the valid palindrome length after pointers `st` and `end` overshoot by one step.
* Line `start = i - (len - 1) / 2;`: Reconstructs the exact starting index from center `i` using basic arithmetic without needing string objects.

#### Theoretical Best Complexity: Manacher's Algorithm
The absolute optimal time complexity for this problem is **O(N)** using **Manacher's Algorithm**. 

Manacher's algorithm works in **O(N) time** and **O(N) space** by inserting dummy characters (like `#`) between every letter to turn even palindromes into odd ones. It keeps track of previously identified palindromes so it can skip redundant character checks entirely. 

---

## Edge Cases Handled

* **Single-character strings (`s = "a"`):** Loop runs once (`i = 0`). `check(s, 0, 0)` returns `"a"`, which becomes `ans`.
* **Strings with no multi-character palindromes (`s = "abc"`):** Expansions fail immediately. `ans` retains the first single character (`"a"`), which is a valid palindrome of length 1.
* **Even-length palindromes (`s = "abba"`):** The second check call `check(s, i, i + 1)` explicitly checks gaps between characters to detect even-length palindromes.
* **Whole string is a palindrome (`s = "racecar"`):** The `while` condition (`st >= 0 && end < s.size()`) safely stops expansion right at the boundary limits.
