![Runtime](https://img.shields.io/badge/Runtime-11%20ms%20(beats%2075.39%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.74%20MB%20(beats%2098.43%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `s` and a positive integer `k`. 

Your goal is to find the maximum number of non-overlapping substrings you can pick from `s` such that every picked substring satisfies two conditions:
1. It is a **palindrome** (it reads the same backward and forward, like `"aba"` or `"racecar"`).
2. Its length is at least `k`.

"Non-overlapping" means no two selected substrings can share the same character index in `s`.

For example, if `s = "abaccdbbd"` and `k = 3`:
- `"aba"` (length 3) is a palindrome.
- `"dbbd"` (length 4) is a palindrome.
- They do not overlap.
- The answer is `2`.

---

## Intuition

This problem combines **Center Expansion** (for finding palindromes) and a **Greedy Strategy** (for maximizing the total count).

1. **Greedy Strategy**: To fit as many non-overlapping substrings as possible, we should always pick the valid palindrome that finishes as early as possible. Finishing early leaves the maximum amount of remaining string for future palindromes.
2. **Center Expansion**: Palindromes expand symmetrically from a center. We can iterate through each character in the string and expand outward to look for palindromes of both odd lengths (centered at one character) and even lengths (centered between two characters).
3. **Stop Early**: A longer palindrome contains a shorter palindrome around the same center. Since we want to finish as early as possible, the moment our expanding palindrome reaches a length of at least `k` and starts after our last picked palindrome, we grab it immediately, update our end position, and stop expanding from that center.

---

## Approach

Here is how the code executes step-by-step:

- `int n = s.size(), ans = 0, end = -1;`: We store string length in `n`, set our palindrome counter `ans` to `0`, and set `end` to `-1` (representing the ending index of the last chosen palindrome).
- `for(int i = 0; i < n; i++)`: We loop through every index `i` in the string, treating `i` as the center of potential palindromes.
- `for(int l : {i - 1, i})`: We check two types of center expansions for each `i`. First `l = i - 1` (even-length center between `i - 1` and `i`), then `l = i` (odd-length center at `i`).
- `int r = i;`: We set the right pointer `r` to start at `i`.
- `while(l >= 0 && r < n && s[l] == s[r])`: We expand outward by moving `l` left and `r` right as long as both pointers stay inside the string bounds and characters at `l` and `r` match.
- `if(r - l + 1 >= k && l > end)`: We check if the current palindrome length (`r - l + 1`) is at least `k`, and if its starting position `l` is strictly greater than `end` (ensuring no overlap with previously selected palindromes).
- `ans++;`: If both conditions are met, we increment our valid palindrome count.
- `end = r;`: We update `end` to `r` so future palindromes know where this chosen substring ends.
- `break;`: We break out of the expansion immediately. Expanding further from this center would only give a longer palindrome that ends later, which is worse for our greedy strategy.
- `return ans;`: After checking all centers, we return the maximum count of non-overlapping palindromes found.

---

## Dry Run

### Case 1: Typical case (`s = "abaccdbbd"`, `k = 3`)

| i | l | r | Substring | ans | end | Action |
|---|---|---|---|---|---|---|
| 1 | 0 | 2 | "aba" | 1 | 2 | Length is 3 (>= 3) and start index 0 > -1. Found palindrome! Set `ans = 1`, `end = 2`. Break loop. |
| 4 | 3 | 4 | "cc" | 1 | 2 | Length is 2 (< 3). Keep expanding. Next expansion ('a' vs 'd') fails. |
| 7 | 5 | 8 | "dbbd" | 2 | 8 | Length is 4 (>= 3) and start index 5 > 2. Found palindrome! Set `ans = 2`, `end = 8`. Break loop. |

**Final Output:** `2`

---

### Case 2: No valid palindromes of required length (`s = "adbcda"`, `k = 2`)

| i | l | r | Substring | ans | end | Action |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | "a" | 0 | -1 | Length 1 (< 2). Next step out of bounds. |
| 1 | 0 | 1 | "ad" | 0 | -1 | Characters 'a' and 'd' do not match. Expansion stops. |
| 2..5 | - | - | - | 0 | -1 | No matching character pairs form a palindrome of length >= 2. |

**Final Output:** `0`

---

## Time & Space Complexity

- **Time Complexity:** O(N * k) — where N is the length of string `s` and `k` is the minimum length. For each of the N centers, expansion stops as soon as a valid length (at most `k + 1`) is reached or a character mismatch occurs. In the worst case, center expansion takes at most O(k) operations, leading to O(N * k) overall runtime.
- **Space Complexity:** O(1) — only a few scalar variables (`n`, `ans`, `end`, `i`, `l`, `r`) are used. No additional arrays or dynamic memory are allocated.

### Is this optimal?

The current code is fast in practice, but its theoretical worst-case time complexity is O(N * k), which can reach O(N^2) when `k` is proportional to N.

We can improve the time complexity to **O(N)** by updating the center index `i` when a palindrome is found. 

**Why it works:**
Once a valid palindrome is selected ending at `end = r`, any future valid palindrome must start at index `l > r`. Any center `i <= r` cannot form a valid palindrome starting at `l > r`. Therefore, we can jump `i` directly to `r` after finding a palindrome, skipping all redundant centers inside the chosen substring.

```cpp
if(r - l + 1 >= k && l > end){
    ans++;
    end = r;
    i = r; // Jump 'i' forward to skip centers inside the selected palindrome
    break;
}
```

- **Improved Complexity:** **Time:** O(N) — each character index in `s` is processed a constant number of times. **Space:** O(1).
- **Theoretical Best Complexity:** O(N) — since we must inspect characters in the string at least once, O(N) time and O(1) space is the theoretical optimal complexity for this problem.

---

## Edge Cases Handled

- **`k = 1`**: Every single character is a valid palindrome of length 1. The algorithm selects every character without overlapping, returning `n`.
- **`k > s.length()`**: The condition `r - l + 1 >= k` is never met. The code returns `0` correctly.
- **Overlapping Palindromes**: The check `l > end` prevents selecting any palindrome substring that overlaps with previously chosen substrings.
- **Even vs. Odd Palindromes**: By checking both `l = i - 1` (even center) and `l = i` (odd center), the algorithm correctly catches all palindrome types (such as `"cc"` or `"aba"`).
