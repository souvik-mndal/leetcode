![Runtime](https://img.shields.io/badge/Runtime-1046%20ms%20(beats%205.01%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-521%20MB%20(beats%205.10%25)-red?style=for-the-badge)

---

## Problem Explained

A **palindrome** is a word that reads the same backward as forward (like "racecar" or "noon"). A **substring** is a contiguous block of characters inside a string without skipping any letters.

The problem asks us to find the longest substring inside a given text `s` that is also a palindrome. If there are multiple palindromic substrings with the same maximum length, returning any one of them is acceptable.

For example, given `s = "babad"`, both `"bab"` and `"aba"` are valid palindromes. The longest length is 3, so returning `"bab"` is correct.

## Intuition

Think of a palindrome like a folding mirror. It centers around a middle point and expands outward identically in both directions.

To find the longest one, we can test every possible position in the string as a potential center and expand outward to the left and right as long as the matching characters stay equal.

There is one important detail: palindromes can have two types of centers:
1. **Odd length** (like `"aba"`): centered on a single character (`'b'`).
2. **Even length** (like `"abba"`): centered between two characters (`'b'` and `'b'`).

By trying both odd and even centers at every position in the string, we are guaranteed to find the longest overall palindrome.

## Approach

Here is how the code works line-by-line:

* `string ans = "";` — Initializes an empty string to store the longest palindrome found across the whole run.
* `for( int i=0 ; i<s.size() ; i++ )` — Loops through every character index `i` in string `s`, treating `i` as the middle of a potential palindrome.
* `string one = check(s,i,i);` — Calls helper function `check` to look for an **odd-length** palindrome centered at character index `i`.
* `string two = check(s,i,i+1);` — Calls helper function `check` to look for an **even-length** palindrome centered between index `i` and `i+1`.
* `while( st>=0 && end<s.size() )` — Inside `check`, keeps expanding outward as long as start pointer `st` and end pointer `end` stay within string boundaries.
* `if( s[st] == s[end] )` — Checks if the current left character at `st` matches the right character at `end`.
* `store = s.substr(st,(end-st)+1);` — Extracts and saves the matching substring from `st` to `end` into `store`.
* `else { break; }` — Stops expanding immediately if characters do not match, because a non-matching character breaks the palindrome property.
* `st--; end++;` — Moves left pointer `st` back by 1 and right pointer `end` forward by 1 to expand the search window.
* `return store;` — Returns the longest valid palindrome substring found by `check` for this specific center.
* `if( one.size() > two.size() ) { temp = one; } else { temp = two; }` — Picks the longer palindrome between the odd-length result `one` and even-length result `two`.
* `if( temp.size() > ans.size() ) { ans = temp; }` — Updates overall answer `ans` if `temp` is longer than the previous best.
* `return ans;` — Returns the longest palindromic substring after checking all positions.

## Dry Run

### Case 1: Standard case with odd-length palindrome (`s = "babad"`)

| `i` | `s[i]` | `one` (odd check) | `two` (even check) | `temp` | `ans` | Action |
|---|---|---|---|---|---|---|
| 0 | 'b' | "b" | "" | "b" | "b" | Odd check finds "b". `ans` updated to "b". |
| 1 | 'a' | "bab" | "" | "bab" | "bab" | Odd check expands around 'a' to "bab". `ans` updated to "bab". |
| 2 | 'b' | "aba" | "" | "aba" | "bab" | Odd check expands around 'b' to "aba". Length 3 does not beat length 3, `ans` stays "bab". |
| 3 | 'a' | "a" | "" | "a" | "bab" | Odd check gives "a". `ans` remains "bab". |
| 4 | 'd' | "d" | "" | "d" | "bab" | Odd check gives "d". Loop ends. Return "bab". |

### Case 2: Even-length palindrome (`s = "cbbd"`)

| `i` | `s[i]` | `one` (odd check) | `two` (even check) | `temp` | `ans` | Action |
|---|---|---|---|---|---|---|
| 0 | 'c' | "c" | "" | "c" | "c" | Odd check finds "c". `ans` updated to "c". |
| 1 | 'b' | "b" | "bb" | "bb" | "bb" | Even check centered at index 1 and 2 finds "bb". `ans` updated to "bb". |
| 2 | 'b' | "b" | "" | "b" | "bb" | Odd finds "b", even fails. `ans` stays "bb". |
| 3 | 'd' | "d" | "" | "d" | "bb" | Odd finds "d". Loop ends. Return "bb". |

## Time & Space Complexity

### Current Solution Performance
* **Time Complexity:** O(N^3) — The main loop runs N times. Inside `check`, expanding pointers takes up to N steps. Inside that expansion loop, `s.substr(...)` creates a brand new string copy on every step, which takes up to O(N) time per copy. That gives N * N * N operations overall.
* **Space Complexity:** O(N) — Creating multiple temporary substring copies (`store`, `one`, `two`, `temp`) allocates new memory proportional to string length N.

### Is this solution optimal?
No, this code is very slow (as seen by the 5% runtime submission metric). It can be greatly improved.

### How to improve it
We can fix performance by changing what `check` returns. Instead of creating and copying a `string` inside the loop repeatedly, `check` should simply count and return the **length** of the expanded palindrome. We can track the starting position and maximum length, then construct the substring **only once at the very end**.

```cpp
int expand(string &s, int st, int end) {
    while (st >= 0 && end < s.size() && s[st] == s[end]) {
        st--;
        end++;
    }
    // Length of the palindrome found:
    return end - st - 1; 
}
```

* `s[st] == s[end]` checks matching characters without allocating memory.
* `return end - st - 1;` calculates the length using basic subtraction because `st` and `end` move one step too far before the loop ends.

### Resulting Improved Complexity
* **Improved Time Complexity:** O(N^2) — N centers, each expanding up to N steps without redundant string copy operations.
* **Improved Space Complexity:** O(1) — No intermediate string objects are created.

### Theoretical Best Complexity
* **Theoretical Best Time:** O(N)
* **Theoretical Best Space:** O(N)
* The theoretical limit is achieved using **Manacher's Algorithm**, which reuses palindrome symmetry information from previous centers to skip redundant comparisons. However, for interview settings, the O(N^2) time with O(1) space expand-around-center approach is standard and usually expected.

## Edge Cases Handled

* **Single Character (`s = "a"`):** Loop runs once (`i = 0`), `check` returns `"a"`, and the code correctly returns `"a"`.
* **Two Identical Characters (`s = "bb"`):** Handled by the even center check (`i = 0, i + 1 = 1`), returning `"bb"`.
* **No Repeating Patterns (`s = "abcde"`):** Every character acts as its own palindrome of length 1, so the code returns the first character `"a"`.
* **Entire String is Palindrome (`s = "racecar"`):** Expanding from center index 3 (`'e'`) stretches all the way to string bounds, correctly capturing the entire string.
