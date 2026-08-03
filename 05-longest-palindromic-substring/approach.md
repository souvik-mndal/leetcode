![Runtime](https://img.shields.io/badge/Runtime-1036%20ms%20(beats%205.00%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-520.97%20MB%20(beats%205.27%25)-red?style=for-the-badge)

---

## Problem Explained

A **palindrome** is a word or phrase that reads the exact same forward and backward. For example, "racecar" and "noon" are palindromes, but "apple" is not.

A **substring** is a continuous block of characters inside a string. For example, "aba" is a substring of "babad", but "bbd" is not.

This problem asks you to take a string `s` and find the longest substring inside it that is also a palindrome. If there are multiple palindromes of the same longest length, returning any one of them is acceptable.

**Example:**
* Input: `s = "babad"`
* Output: `"bab"` (or `"aba"`, since both have a length of 3)

---

## Intuition

Instead of checking every possible start and end position in the string, think about how palindromes are formed. 

Every palindrome expands outward from its center:
* **Odd-length palindromes** (like "aba") have a single character at the center ('b').
* **Even-length palindromes** (like "abba") have a center between two identical characters ('b' and 'b').

**The core idea:** 
Treat every character (and every gap between characters) as a potential center. Expand outward to the left and right one step at a time. Keep expanding as long as the left character matches the right character. Stop as soon as they do not match or you hit the string boundary.

---

## Approach

Here is how the current C++ code works step-by-step:

* **Define a helper function (`check`):**
  * Takes the string `s` and two starting indices, `st` (left) and `end` (right).
  * While `st` is within the left bound (`st >= 0`) and `end` is within the right bound (`end < s.size()`):
    * If `s[st]` equals `s[end]`, extract the current substring using `s.substr()` and save it.
    * Move `st` one step left (`st--`) and `end` one step right (`end++`).
    * If characters do not match, break the loop immediately.
  * Return the last matching substring stored.

* **Main loop (`longestPalindrome`):**
  * Create a variable `ans` initialized to an empty string.
  * Loop through each index `i` from `0` to `s.size() - 1`.
  * For each index `i`, check two center types:
    * **Odd-length palindrome center:** Call `check(s, i, i)`.
    * **Even-length palindrome center:** Call `check(s, i, i + 1)`.
  * Compare the two returned strings and keep the longer one in a temporary variable `temp`.
  * If `temp` is longer than `ans`, update `ans` with `temp`.
  * After checking all indices, return `ans`.

---

## Time & Space Complexity

* **Time Complexity:** **O(N^3)** in the worst case. 
  * The outer loop runs `N` times. 
  * Expanding outward takes up to `N` steps. 
  * Inside the expansion loop, `s.substr()` creates a new string object and copies up to `N` characters each time. Copying strings inside a nested loop multiplies the time complexity: `N * N * N = N^3`.

* **Space Complexity:** **O(N^2)** extra space.
  * Creating multiple dynamic substring copies inside the expansion loop constantly allocates memory on the heap.

### Can this be improved?

**Yes, significantly.** 

The current code beats only ~5% of submissions because it calls `s.substr()` inside the expansion loop. Creating strings repeatedly is slow and wastes memory.

**The Fix:** Expand using integer coordinates (`left` and `right`) to track only the length and starting index. Extract the substring **only once** at the very end.

```cpp
// Optimized expansion helper returning palindrome length
int expand(const string& s, int left, int right) {
    while (left >= 0 && right < s.size() && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1; // Length of palindrome
}

// In main function, track start position and max length:
int start = 0, maxLen = 0;
for (int i = 0; i < s.size(); i++) {
    int len1 = expand(s, i, i);     // Odd length
    int len2 = expand(s, i, i + 1); // Even length
    int len = max(len1, len2);
    if (len > maxLen) {
        maxLen = len;
        start = i - (len - 1) / 2;
    }
}
return s.substr(start, maxLen); // Single substr call at the end
```

### Resulting Improved Complexity:
* **Time Complexity:** **O(N^2)** — Expanding around `N` centers takes at most `N` comparison steps without any string copying inside the loop.
* **Space Complexity:** **O(1)** — Uses only a few integer variables.

### Theoretical Best Complexity:
* **Theoretical Best:** **O(N)** time and **O(N)** space using **Manacher's Algorithm**. 
* The **O(N^2)** time / **O(1)** space center-expansion solution is the standard expected answer in most technical interviews, as Manacher's algorithm is rarely required.

---

## Edge Cases Handled

* **Single character string (e.g., `s = "a"`):** The loop runs once, checks `check(s, 0, 0)`, and correctly returns `"a"`.
* **Two identical characters (e.g., `s = "bb"`):** The even center expansion `check(s, 0, 1)` catches the pair and correctly returns `"bb"`.
* **String with all unique characters (e.g., `s = "abc"`):** Every single character is a palindrome of length 1. The code returns the first character `"a"`.
* **Boundary conditions:** Boundary checks (`st >= 0 && end < s.size()`) prevent out-of-bounds errors when expanding past the edges of the string.
