![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen)
![Memory](https://img.shields.io/badge/Memory-9.26%20MB%20(beats%2082.66%25)-brightgreen)

---

## Problem in Plain English

The problem asks if two strings, `s` and `t`, have the exact same character pattern. We call two strings **isomorphic** if you can replace the characters in `s` one-by-one to transform it into `t`.

Here are the replacement rules:
1. Every time a specific character appears in `s`, it must always map to the exact same character in `t`.
2. No two different characters from `s` can map to the same character in `t` (the mapping must be one-to-one).
3. A character is allowed to map to itself.
4. Character order must be preserved.

### Examples
* **`s = "egg"`, `t = "add"` $\rightarrow$ `true`**: 'e' turns into 'a', and 'g' turns into 'd'. The pattern matches.
* **`s = "foo"`, `t = "bar"` $\rightarrow$ `false`**: 'o' would need to map to both 'a' and 'r', which breaks the rules.
* **`s = "ab"`, `t = "aa"` $\rightarrow$ `false`**: Both 'a' and 'b' would try to map to 'a', which is not allowed.

---

## Intuition

Think of this as creating a **translation dictionary** between two languages. 

If 'e' translates to 'a', then every 'e' you see later *must* translate to 'a'. Furthermore, no other letter can translate to 'a' because 'a' is already taken. 

To solve this, we need to keep track of two things as we move through the strings:
1. Which character in `t` is assigned to each character in `s`?
2. Has a character in `t` already been claimed by a different character in `s`?

If we try to create a mapping that breaks either rule, the strings are not isomorphic.

---

## Approach

Here is how the provided C++ code executes step-by-step:

* **Setup**: Create two arrays (fixed size of 256 to cover all standard **ASCII** characters):
  * `mp`: Stores the mapped character from `s` to `t`.
  * `chk`: Acts as a checklist to mark which characters in `t` are already taken.
* **First Pass (Building the Map)**: Loop through both strings index by index.
  * Check if the current character `s[i]` has no mapping yet (`mp[s[i]] == 0`) **and** the current character `t[i]` hasn't been claimed yet (`chk[t[i]] == 0`).
  * If both are free, record the mapping by setting `mp[s[i]] = t[i]` and mark `t[i]` as taken by setting `chk[t[i]] = 1`.
* **Second Pass (Verification)**: Loop through the strings a second time.
  * Check if `mp[s[i]]` matches `t[i]` at every index.
  * If any character does not match its expected mapping, return `false` immediately.
* **Return Result**: If both passes complete without any conflicts, return `true`.

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where $n$ is the length of string `s` (or `t`). The code scans the strings twice, doing fast $O(1)$ array lookups for each character.
* **Space Complexity:** **O(1)** — fixed size arrays of size 256 are used. The memory consumed never grows, regardless of how long the input strings are.

### Can it be improved?

**Yes.** While the space complexity is already optimal, the time efficiency can be improved by completing the check in a **single pass** instead of two separate loops. We can also simplify the logic by recording the **last seen position** of characters in both strings.

```cpp
// Optimized single-pass approach:
vector<int> pos_s(256, 0), pos_t(256, 0);

for (int i = 0; i < s.size(); ++i) {
    // If the last seen positions don't match, the pattern is broken
    if (pos_s[s[i]] != pos_t[t[i]]) return false;
    
    // Store current position + 1 (using 1-based indexing so 0 means unseen)
    pos_s[s[i]] = i + 1;
    pos_t[t[i]] = i + 1;
}
return true;
```

* **Improved Complexity:** **Time:** O(n) (single pass), **Space:** O(1).
* **Theoretical Best Complexity:** **Time:** O(n), **Space:** O(1). Every character must be examined at least once, making $O(n)$ time the best possible limit. The single-pass version reaches this theoretical limit.

---

## Edge Cases Handled

* **Many-to-One Conflicts (e.g., `s = "ab"`, `t = "aa"`):** Handled because the `chk` array prevents 'b' from claiming 'a' when 'a' was already assigned to 'a'.
* **One-to-Many Conflicts (e.g., `s = "badc"`, `t = "baba"`):** Handled during validation when a character in `s` maps to a different character than previously recorded.
* **Self-Mapping (e.g., `s = "paper"`, `t = "paper"`):** Characters mapping to themselves (like 'p' to 'p') work smoothly.
* **Special Characters and Symbols:** Uses arrays of size 256, which correctly handles space, numbers, and printable ASCII symbols beyond lowercase letters.
* **Single Character Strings (e.g., `s = "a"`, `t = "b"`):** Loops execute once without out-of-bounds errors and return `true`.
