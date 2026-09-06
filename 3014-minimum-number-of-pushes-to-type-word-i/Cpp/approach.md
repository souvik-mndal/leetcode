![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.4%20MB%20(beats%2020.94%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine an old phone keypad where keys 2 through 9 can each hold letters. You want to type a given word by pressing these keys. 

- Pressing a key once types its first letter (cost: 1 push).
- Pressing a key twice types its second letter (cost: 2 pushes).
- Pressing a key three times types its third letter (cost: 3 pushes), and so on.

The problem tells us that all letters in the input word are **distinct** (no letter repeats). We are allowed to rearrange which letters go to which keys, and we can put as many letters on a key as we want. Our goal is to assign the letters to keys so that the total number of button presses needed to type the entire word is kept as small as possible.

For example, if the word is "abcde", we have 5 letters. We can assign each letter to a different key and press each key 1 time. That gives a total cost of 5 pushes.

---

## Intuition

This solution relies on a **greedy** approach, which means making the best local choice at each step to get the best overall result. 

Because we want to minimize total button presses, we want letters that appear in our word to cost as few pushes as possible. 
- There are **8 available keys** (keys 2 through 9). The first 8 letters we type should cost **1 push** each because we can place one letter on each of the 8 keys.
- Once those 8 keys are full, any extra letters must go on those same keys as a second letter. That means the next 16 letters will each cost **2 pushes**.
- After that, any further letters will cost **3 pushes**, and then **4 pushes**.

The problem description notes that `word` consists of distinct lowercase English letters and its length is at most 26. Since the input characters are unique, the loop simply looks at each character in the string one by one, tracks how many characters we have already assigned, and adds the correct cost for the current character based on how crowded the keys are getting.

---

## Approach

Here is how the code works, step by step:

- `int count = 0;`: Initializes a running total variable named `count` to store the total number of button presses. It also doubles as our position tracker for how many letters we have processed so far.
- `for( auto it:word ):`: Loops through every character (`it`) in the string `word` one by one.
- `if( count < 8 ) count++;`: Checks if we have processed fewer than 8 letters. If so, this letter fits on its own fresh key as the first choice, so we add `1` push to `count`.
- `else if( count < 24) { count += 2; }`: If we already placed 8 letters, the next group of 16 letters (from index 8 up to 23) must go as a second letter on the existing keys, so we add `2` pushes for each.
- `else if( count < 48) { count += 3; }`: If we have processed 24 or more letters, any further letters require 3 pushes, so we add `3` to `count`.
- `else { count += 4; }`: Adds `4` pushes if we somehow go beyond 48 letters (though the maximum word length is 26, so this branch is never actually reached for valid inputs).
- `return count;`: Returns the final accumulated total of button presses after the loop finishes.

---

## Dry Run

### Case 1: Typical case (`word = "abcde"`)

| `it` (Character) | `count` (Before check) | Branch taken | `count` (After update) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `'a'` | 0 | `count < 8` | 1 | First letter, adds 1 push |
| `'b'` | 1 | `count < 8` | 2 | Second letter, adds 1 push |
| `'c'` | 2 | `count < 8` | 3 | Third letter, adds 1 push |
| `'d'` | 3 | `count < 8` | 4 | Fourth letter, adds 1 push |
| `'e'` | 4 | `count < 8` | 5 | Fifth letter, adds 1 push |

Final return value: `5`.

---

### Case 2: Longer word filling multiple tiers (`word = "abcdefghijkl"`)

| `it` (Character) | `count` (Before check) | Branch taken | `count` (After update) | Action |
| :--- | :--- | :--- | :--- | :--- |
| `'a'` through `'h'` (8 chars) | 0 through 7 | `count < 8` | 1 through 8 | First 8 letters each cost 1 push |
| `'i'` | 8 | `count < 24` | 10 | 9th letter overflows to tier 2, adds 2 pushes |
| `'j'` | 10 | `count < 24` | 12 | 10th letter, adds 2 pushes |
| `'k'` | 12 | `count < 24` | 14 | 11th letter, adds 2 pushes |
| `'l'` | 14 | `count < 24` | 16 | 12th letter, adds 2 pushes |

Final return value: `16` (8 letters at 1 push + 4 letters at 2 pushes = 8 + 8 = 16).

---

## Time & Space Complexity

- **Time:** O(n), where n is the length of the string `word`. The code uses a single `for` loop that iterates through each character of the string exactly once.
- **Space:** O(1). The code only uses a single integer variable (`count`) regardless of how long the input word is, meaning it uses a constant amount of extra memory.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is **already the most optimal possible complexity**. 

To solve this problem, you must at least read every character in the input string once, which takes O(n) time. Since this solution visits each character once in a single pass and uses zero extra data structures, it achieves the absolute best possible time complexity of O(n) and space complexity of O(1). No further improvements can be made.

---

## Edge Cases Handled

- **Minimum length word (length 1):** The loop runs once, hits the `count < 8` condition, adds 1, and correctly returns 1.
- **Maximum length word (length 26):** Because the alphabet only has 26 letters and all characters in `word` are distinct, the loop will run at most 26 times. The first 8 letters add 1 push each, and the remaining 18 letters fall into the second tier and add 2 pushes each, handling the full alphabet gracefully without out-of-bounds errors.
- **Order of characters:** Since the problem states all letters in `word` are distinct and we only care about *how many* total letters there are (not which specific letters they are), the exact alphabetical order does not matter; counting the length via iteration gives the correct total cost every time.
