![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.07%20MB%20(beats%2098.82%25)-brightgreen?style=for-the-badge)

---

## Problem in Plain English

The problem asks if two strings, `s` and `t`, have the exact same character pattern. 

You can think of it as swapping characters: can you replace every character in `s` with a new character to turn it into `t`?

There are a few simple rules:
* Every instance of a character must map to the same target character. For example, if 'e' becomes 'a', all 'e's must become 'a's.
* No two different characters can map to the same target character. If 'e' becomes 'a', no other letter is allowed to become 'a'.
* A character can map to itself (e.g., 'a' can map to 'a').
* The order of characters must stay the same.

**Example 1:**  
`s = "paper"`, `t = "title"` $\rightarrow$ **True**  
'p' $\rightarrow$ 't', 'a' $\rightarrow$ 'i', 'e' $\rightarrow$ 'l', 'r' $\rightarrow$ 'e'. The pattern matches perfectly.

**Example 2:**  
`s = "foo"`, `t = "bar"` $\rightarrow$ **False**  
'o' would have to map to both 'a' and 'r', which is not allowed.

---

## Intuition

To solve this, you need a **two-way lock** (a one-to-one relationship). 

Think of it like assigning secret code names:
1. When you see a letter in `s`, you give it a code name from `t`.
2. Once a code name from `t` is taken, no other letter in `s` can claim it.

If you ever see a letter in `s` trying to map to two different target letters, or two letters in `s` trying to share the same code name in `t`, the pattern breaks.

---

## Approach

Here is how the code works step-by-step:

* **Create fixed-size lookup tables:**
  * Use an array `mp` of size 256 (covering all standard character codes) to store which character in `s` maps to which character in `t`.
  * Use a boolean array `chk` of size 256 to mark which target characters in `t` are already claimed.

* **Pass 1: Build the mappings:**
  * Loop through each character position `i` in the strings.
  * Check if `s[i]` does not have a mapping yet (`mp[s[i]] == 0`) AND `t[i]` has not been claimed yet (`chk[t[i]] == 0`).
  * If both conditions are true, create the mapping (`mp[s[i]] = t[i]`) and mark `t[i]` as taken (`chk[t[i]] = true`).

* **Pass 2: Verify the mappings:**
  * Loop through the string again.
  * Check if `mp[s[i]]` equals `t[i]` for every position.
  * If any character maps to something different, immediately return `false`.

* **Return result:**
  * If all characters pass verification, return `true`.

---

## Time & Space Complexity

* **Time Complexity:** **O(N)** — where $N$ is the length of the strings. The code loops through the strings twice. Looking up items in array positions takes constant time ($O(1)$).
* **Space Complexity:** **O(1)** — the code creates fixed arrays of size 256. This memory does not grow regardless of how long the input strings are.

### Optimization Potential

**Is this already optimal?**  
The asymptotic complexity is already optimal (**O(N) Time** and **O(1) Space**), because you must inspect every character at least once. 

However, you can optimize the **runtime** by combining both loops into a **single pass**, checking for conflicts on the fly:

```cpp
// Single-pass optimization
for (int i = 0; i < s.size(); i++) {
    // Conflict 1: s[i] was already mapped to a different character
    if (mp[s[i]] != 0 && mp[s[i]] != t[i]) return false;
    
    // Conflict 2: t[i] was already claimed by a different character in s
    if (mp[s[i]] == 0 && chk[t[i]]) return false;
    
    mp[s[i]] = t[i];
    chk[t[i]] = true;
}
return true;
```

* **Improved Complexity:** Still **O(N) Time** and **O(1) Space**, but runs in roughly half the actual CPU operations by eliminating the second loop.
* **Theoretical Best:** **O(N) Time** and **O(1) Space**. The single-pass version hits this absolute limit.

---

## Edge Cases Handled

* **Two characters mapping to the same target:** e.g., `s = "badc"`, `t = "baba"`. Here, both 'a' and 'c' try to map to 'a'. The `chk` array prevents 'c' from claiming 'a', handling this correctly.
* **One character mapping to multiple targets:** e.g., `s = "foo"`, `t = "bar"`. 'o' maps to 'a' during loop 1, but fails validation against 'r' during loop 2.
* **Characters mapping to themselves:** e.g., `s = "paper"`, `t = "title"` (where 'e' maps to 'l' and 'r' maps to 'e'). Same-character mappings work without issue.
* **Full ASCII range:** Inputs containing digits, spaces, or punctuation work safely because array sizes are set to 256.
* **Single-character strings:** e.g., `s = "a"`, `t = "z"`. Loops run once, set the mapping, and return `true`.
