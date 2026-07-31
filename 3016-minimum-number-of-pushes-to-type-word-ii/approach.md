![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%2079.89%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-25.33%20MB%20(beats%2084.45%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine an old phone keypad with 8 usable number keys (keys `2` through `9`). You need to map the 26 lowercase English letters to these 8 keys to type a given word with the **fewest total button pushes**.

Key rules:
- You can place multiple letters on a single key.
- The **first letter** assigned to a key takes **1 push** to type.
- The **second letter** assigned to the *same* key takes **2 pushes**.
- The **third letter** takes **3 pushes**, the **fourth** takes **4 pushes**, and so on.
- Every letter present in your word must be assigned to exactly one key.

**Goal:** Arrange the letters on the 8 keys so that typing out the entire `word` requires the absolute minimum total number of key presses.

---

## Intuition

This problem can be solved using a **greedy approach** (making the best choice at each step). 

If a letter appears very often in the word, we want it to be as easy to type as possible. That means giving high-frequency letters the 1st position on a key (1 push).

Since we have **8 available keys**, we can fit up to 8 different letters into position 1 across all keys!

The optimal placement strategy is:
1. The **top 8 most frequent letters** get position 1 (1 push each).
2. The **next 8 most frequent letters** get position 2 (2 pushes each).
3. The **next 8 most frequent letters** get position 3 (3 pushes each).
4. Any **remaining letters** get position 4 (4 pushes each).

---

## Approach

Here is how the C++ code carries out this strategy step-by-step:

- **Step 1: Count letter frequencies.**  
  Create an array `store` of size 26 (initialized to 0) to count how many times each letter from `'a'` to `'z'` appears in `word`.
- **Step 2: Sort frequencies.**  
  Sort the `store` array in ascending order. After sorting, the most frequent letters end up at the end of the array (indices 25, 24, 23, etc.).
- **Step 3: Assign push costs.**  
  Iterate backward through `store` from index 25 down to 0:
  - Skip any letter count that is `0` (letters not present in `word`).
  - Use a placement counter `j` (starting at 0) to track how many unique letters have been mapped so far.
  - Calculate the cost per letter using integer division: `(j / 8) + 1`.
    - For `j` from 0 to 7: cost is `1` push.
    - For `j` from 8 to 15: cost is `2` pushes.
    - For `j` from 16 to 23: cost is `3` pushes.
    - For `j` from 24 to 25: cost is `4` pushes.
  - Multiply the letter's frequency by its cost and add it to the total answer `ans`.
  - Increment `j` after each unique letter.
- **Step 4: Return result.**  
  Return `ans` as the minimum total key pushes.

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where $N$ is the length of the string `word`. 
  - Counting character frequencies takes $O(N)$ time to read through `word`. 
  - Sorting the array takes $O(26 \log 26)$ time. Because 26 is a fixed constant, this sorting step takes a fixed $O(1)$ amount of time. 
  - Thus, the overall runtime is dominated purely by reading the string: **O(N)**.

- **Space Complexity:** **O(1)** — memory used does not grow with the input size. 
  - We only allocate a single array of size 26 to count letter frequencies, which uses fixed constant memory.

### Is this optimal?
**Yes, this code is already fully optimal.** 
- Any valid algorithm must inspect every character of `word` at least once, requiring at least $O(N)$ time. 
- The space used is $O(1)$, which is the theoretical minimum. 
- No further optimizations to big-O time or space complexity are possible.

---

## Edge Cases Handled

- **Letters with 0 occurrences:** The check `if (store[i] == 0) continue;` skips any letters that do not appear in the word, avoiding unnecessary calculations.
- **Words with 8 or fewer unique letters:** All letters fit into position 1, charging 1 push per character typed.
- **Words with all 26 unique letters:** The logic correctly transitions push multipliers from 1 to 2, 3, and 4 seamlessly as `j` increases.
- **Large input length ($10^5$ characters):** The maximum total pushes cannot exceed $10^5 \times 4 = 400,000$, which easily fits inside a standard integer (`int`) without risk of overflow.
