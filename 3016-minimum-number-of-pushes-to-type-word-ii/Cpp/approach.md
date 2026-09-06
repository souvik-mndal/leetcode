![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%2078.10%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-25.3%20MB%20(beats%2080.75%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a phone with keys numbered from 2 to 9. Each key can hold a collection of letters. Normally, key 2 holds letters "a", "b", and "c". Pressing "a" once types it, pressing "b" twice types it, and pressing "c" three times types it. 

The problem gives you a string called `word` and lets you rearrange (remap) which letters go to which keys, as long as every letter of the alphabet goes to exactly one key, and you only use keys 2 through 9 (which gives you 8 available keys). 

Your goal is to choose a mapping that makes the **total number of key presses** as small as possible when you type out the entire `word`. If a letter is assigned to require 2 presses, and it appears 5 times in your word, typing all those instances will cost you 2 * 5 = 10 presses. You want to minimize the grand total.

## Intuition

To make the total number of presses as small as possible, we should use a **greedy approach**, which means making the best local choice at each step to get the best overall result. 

The telephone keypad has 8 usable keys (keys 2 through 9). 
- The 8 letters that appear **most frequently** in our word should be placed on the 8 keys so that they only require **1 push** each.
- The next 8 most frequent letters should require **2 pushes** each (since each key now has 2 letters on it).
- The next 8 should require **3 pushes**, and whatever is left over will require **4 pushes**.

So, the core trick is:
1. Count how many times each letter appears in the word.
2. Sort those counts from smallest to largest.
3. Assign the highest frequencies to the cheapest key positions (1 push), the next highest to the next cheapest (2 pushes), and so on.

## Approach

- `vector<int> store(26);`
  Creates a frequency array with 26 slots, one for each lowercase English letter from "a" to "z", starting all counts at zero.
- `for(int i=0 ; i<word.size() ; i++) { store[word[i]-'a']++; }`
  Loops through every character in `word`, converts the character to an index from 0 to 25 by subtracting the character 'a', and increments that letter's count in `store`.
- `sort(store.begin(), store.end());`
  Sorts the 26 frequency counts in ascending order (from smallest to largest). This places the rarest letters at the beginning and the most frequent letters at the very end of the vector.
- `int j=0;`
  Initializes a tracking variable `j` starting at 0. This will help us count how many letters we have processed so far, so we know when to move to a higher push cost.
- `int ans=0;`
  Initializes our running total of key presses to 0.
- `for(int i=25 ; i>=0 ; i--) {`
  Loops backward through the sorted `store` array, starting from index 25 (the most frequent letter) down to index 0 (the rarest letter).
- `if( store[i] == 0 ) continue;`
  Skips any letter that never actually appeared in our word (frequency is zero), since it costs zero presses.
- `ans += store[i] * ((j / 8) + 1);`
  Multiplies the frequency of the current letter by its push cost. The push cost is determined by `(j / 8) + 1` because every group of 8 letters increases the push cost by 1. This adds the result to our total `ans`.
- `j++;`
  Increments `j` by 1 because we have now successfully assigned a letter to a slot.
- `return ans;`
  Returns the final calculated minimum number of key presses after looping through all valid letters.

## Dry Run

### Case 1: Typical case (`word = "abcde"`)

| i (loop index) | store[i] (frequency) | j | Calculation `store[i] * ((j/8) + 1)` | ans (running total) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 25 to 5 | 0 | 0 | Skipped | 0 | Skip unused letters (frequencies are 0) |
| 4 | 1 | 0 | `1 * ((0/8) + 1)` = 1 | 1 | Process letter 'e' (frequency 1, cost 1 push) |
| 3 | 1 | 1 | `1 * ((1/8) + 1)` = 1 | 2 | Process letter 'd' (frequency 1, cost 1 push) |
| 2 | 1 | 2 | `1 * ((2/8) + 1)` = 1 | 3 | Process letter 'c' (frequency 1, cost 1 push) |
| 1 | 1 | 3 | `1 * ((3/8) + 1)` = 1 | 4 | Process letter 'b' (frequency 1, cost 1 push) |
| 0 | 1 | 4 | `1 * ((4/8) + 1)` = 1 | 5 | Process letter 'a' (frequency 1, cost 1 push) |

Final output: `5`

### Case 2: Repeating characters case (`word = "xyzxyzxyzxyz"`)

| i (loop index) | store[i] (frequency) | j | Calculation `store[i] * ((j/8) + 1)` | ans (running total) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 25 to 25 | 0 | 0 | Skipped | 0 | Skip unused letters |
| 25 (letter z) | 4 | 0 | `4 * ((0/8) + 1)` = 4 | 4 | Process 'z' (frequency 4, cost 1 push) |
| 24 (letter y) | 4 | 1 | `4 * ((1/8) + 1)` = 4 | 8 | Process 'y' (frequency 4, cost 1 push) |
| 23 (letter x) | 4 | 2 | `4 * ((2/8) + 1)` = 4 | 12 | Process 'x' (frequency 4, cost 1 push) |

Final output: `12`

## Time & Space Complexity

- **Time:** O(N + K log K) where N is the length of the string and K is the number of unique alphabet letters (which is a constant 26). Counting character frequencies takes O(N) time. Sorting the 26-element frequency array takes a fixed time of O(26 log 26), which is effectively constant time, O(1). The final loop runs 26 times, which is also O(1). Therefore, the total time complexity simplifies to **O(N)**.
- **Space:** O(1) auxiliary space. We use a `store` vector of a fixed size of 26 integers, regardless of how long the input word is. 

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already optimal. 
- **Time-wise:** We must look at every character in the input string at least once to count its frequency, which inherently requires O(N) time. We cannot go faster than O(N).
- **Space-wise:** We only store counts for the 26 lowercase English letters, which uses a fixed, constant amount of memory (O(1)). 

No further improvements in time or space complexity are possible.

## Edge Cases Handled

- **Words with length 1:** The loop runs once, skips unused letters, processes the single character at 1 push, and correctly returns 1.
- **Words with all identical characters (e.g., "aaaaa"):** Only one letter frequency is non-zero, it gets assigned a cost of 1 push, and the total cost equals the word length.
- **Words containing all 26 distinct letters:** The code handles full alphabets correctly, smoothly pushing the 9th through 26th letters into higher-cost tiers `((j/8) + 1)` as `j` crosses multiples of 8.
- **Maximum length constraint (10^5):** The loop bounds and integer types easily handle words up to 100,000 characters without overflowing standard integer limits.
