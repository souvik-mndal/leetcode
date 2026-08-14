![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.95%20MB%20(beats%2095.50%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `s` made of lowercase English letters. You need to find the length of the longest contiguous piece of the string (a substring) where **no character appears more than 2 times**.

For example, if `s = "bcbbbcba"`, the answer is `4`. The substring `"bcba"` has two `b`s, one `c`, and one `a`. No character appears 3 or more times, and its length is 4.

If `s = "aaaa"`, the answer is `2` because any valid substring can only contain at most two `a`s (like `"aa"`).

## Intuition

Think of this like a flexible frame or "window" sliding over the string from left to right. 

We expand the right end of our window one character at a time and keep count of how many times each letter appears inside the window. If adding a new character makes its count hit 3, our window becomes invalid. 

To fix it, we shrink the window from the left end until that character's count is back down to 2. At every valid step, we measure the window size and keep track of the largest size seen so far.

## Approach

Here is how the code works step-by-step:

* `int freq[26]={0};`: Creates an array of size 26 initialized with zeros. This acts as our count tracker for each letter from 'a' to 'z' in the current window.
* `int l=0, n=s.size(), len=0;`: Sets the left pointer `l` at index 0, stores the length of string `s` in `n`, and initializes the maximum answer `len` to 0.
* `for(int r=0; r<n; r++)`: Loops through the string using a right pointer `r` from the start to the end, expanding the window one step at a time.
* `int x=s[r]-'a';`: Converts the current character `s[r]` into a number from 0 to 25 (where 'a' becomes 0, 'b' becomes 1, etc.).
* `freq[x]++;`: Increments the stored count for letter `x` because it has entered our window.
* `while(l<r && freq[x]>2)`: Checks if adding `s[r]` caused letter `x` to appear more than 2 times. If so, it enters a loop to shrink the window from the left until `freq[x]` is back to 2 or lower.
* `freq[s[l]-'a']--;`: Decrements the letter count for the character at the left edge `l` as it leaves the window.
* `l++;`: Shifts the left pointer one position to the right to complete removing that character from the window.
* `len=max(len, r-l+1);`: Calculates the current valid window length (`r - l + 1`) and updates `len` if this window is larger than any seen before.
* `return len;`: Returns the length of the longest valid substring found.

## Dry Run

### Case 1: `s = "bcbbbcba"`

| `r` | `s[r]` | `freq[x]` | `l` | Action | Window | `len` |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 'b' | `freq['b'] = 1` | 0 | Letter 'b' added. Count <= 2. Valid. | "b" | 1 |
| 1 | 'c' | `freq['c'] = 1` | 0 | Letter 'c' added. Count <= 2. Valid. | "bc" | 2 |
| 2 | 'b' | `freq['b'] = 2` | 0 | Letter 'b' added. Count <= 2. Valid. | "bcb" | 3 |
| 3 | 'b' | `freq['b'] = 3` | 1 | Count of 'b' is 3! Shrink from left: remove `s[0]` ('b'). `freq['b']` becomes 2, `l` becomes 1. | "cbb" | 3 |
| 4 | 'b' | `freq['b'] = 3` | 3 | Count of 'b' is 3! Shrink left twice: remove `s[1]` ('c') then `s[2]` ('b'). `l` becomes 3. | "bb" | 3 |
| 5 | 'c' | `freq['c'] = 1` | 3 | Letter 'c' added. Count <= 2. Valid. | "bbc" | 3 |
| 6 | 'b' | `freq['b'] = 3` | 4 | Count of 'b' is 3! Shrink from left: remove `s[3]` ('b'). `freq['b']` becomes 2, `l` becomes 4. | "bcb" | 3 |
| 7 | 'a' | `freq['a'] = 1` | 4 | Letter 'a' added. Count <= 2. Valid. | "bcba" | 4 |

Final Answer: `4`

---

### Case 2: `s = "aaaa"`

| `r` | `s[r]` | `freq[x]` | `l` | Action | Window | `len` |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 'a' | `freq['a'] = 1` | 0 | Letter 'a' added. Count <= 2. Valid. | "a" | 1 |
| 1 | 'a' | `freq['a'] = 2` | 0 | Letter 'a' added. Count <= 2. Valid. | "aa" | 2 |
| 2 | 'a' | `freq['a'] = 3` | 1 | Count of 'a' is 3! Shrink from left: remove `s[0]` ('a'). `l` becomes 1. | "aa" | 2 |
| 3 | 'a' | `freq['a'] = 3` | 2 | Count of 'a' is 3! Shrink from left: remove `s[1]` ('a'). `l` becomes 2. | "aa" | 2 |

Final Answer: `2`

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where n is the length of string `s`. The right pointer `r` visits each character once. The left pointer `l` moves forward and visits each character at most once. Because both pointers only move from left to right, total steps are bounded by 2 * n, which simplifies to O(n).
* **Space Complexity:** **O(1)** — The memory used is fixed. The `freq` array always takes exactly 26 integer slots regardless of how long the string `s` is.

**Is this optimal?**
Yes, this solution is already optimal. 
* To know the maximum valid substring, we must inspect every character in the string at least once, so time cannot be better than O(n). 
* Since the alphabet is fixed at 26 lowercase English letters, using a 26-element array uses O(1) auxiliary space, which is the absolute minimum required to track frequencies.

## Edge Cases Handled

* **All characters are identical** (e.g., `s = "aaaa"`): The while loop triggers immediately on the 3rd duplicate, maintaining a maximum window size of 2.
* **Smallest valid string length** (e.g., `s.length = 2`): The algorithm processes both characters smoothly and returns 2.
* **All characters are distinct or repeat at most twice** (e.g., `s = "abcabc"`): The frequency of any letter never exceeds 2, so `l` stays at 0, correctly evaluating the answer as the full string length.
* **Tightly packed repeats** (e.g., `s = "abbba"`): The left pointer shifts twice in a single loop iteration to handle back-to-back duplicate overruns properly.
