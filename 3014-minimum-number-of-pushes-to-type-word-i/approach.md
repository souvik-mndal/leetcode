![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.16%20MB%20(beats%2095.83%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `word` made of distinct lowercase English letters. 

Imagine an old telephone keypad with 8 usable number keys (keys 2 through 9). You want to assign the letters of `word` to these 8 keys. 

* The **first** letter placed on a key takes **1 push** to type.
* The **second** letter on the same key takes **2 pushes**.
* The **third** letter takes **3 pushes**, and so on.

Your goal is to remapped the keypad so that typing the entire word takes the **minimum total number of key pushes**.

**Example:**
If `word = "abcde"`, you can place each letter on a different key (e.g., 'a' on key 2, 'b' on key 3, etc.). Since every letter is the 1st letter on its key, each takes 1 push. 
Total pushes = 1 + 1 + 1 + 1 + 1 = **5**.

---

## Intuition

We have **8 keys** available (2 through 9). Each key has a "1-push slot", a "2-push slot", a "3-push slot", and a "4-push slot".

Because every letter in `word` appears only once (all letters are distinct), we should always fill the cheapest available slots first:
1. The **first 8 letters** should each take **1 push** (one per key).
2. The **next 8 letters** (letters 9 to 16) should each take **2 pushes**.
3. The **next 8 letters** (letters 17 to 24) should each take **3 pushes**.
4. The **last 2 letters** (letters 25 and 26) should each take **4 pushes**.

Instead of manually counting letters, the code tracks the cumulative push count directly:
* 8 letters at 1 push each = total reaches **8**.
* 8 letters at 2 pushes each = total increases by 16, reaching **24**.
* 8 letters at 3 pushes each = total increases by 24, reaching **48**.

---

## Approach

Here is how the code builds the total push count character by character:

* **Initialize a tracker**: Start `count = 0` to keep track of total pushes.
* **Loop through `word`**: Process each letter one at a time.
* **Check the current total (`count`)**:
  * **First 8 letters (`count < 8`)**: Add **1 push** to `count`.
  * **Letters 9 to 16 (`count < 24`)**: Add **2 pushes** to `count`.
  * **Letters 17 to 24 (`count < 48`)**: Add **3 pushes** to `count`.
  * **Letters 25 to 26 (`else`)**: Add **4 pushes** to `count`.
* **Return total**: After processing all characters, return `count`.

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where **n** is the length of `word`. The code loops through each character in the string once.
* **Space Complexity:** **O(1)** — no extra data structures are used. Only a single integer variable `count` is stored in memory.

### Can this be improved?

Yes! Because all characters in `word` are guaranteed to be **distinct**, we don't actually need to loop through the characters at all. We only need the **length** of the string (`n`) to calculate the answer directly in **O(1)** time using basic math.

**Optimized snippet:**
```cpp
int minimumPushes(string word) {
    int n = word.length();
    if (n <= 8) return n;
    if (n <= 16) return 8 + (n - 8) * 2;
    if (n <= 24) return 24 + (n - 16) * 3;
    return 48 + (n - 24) * 4;
}
```

* **Improved Time Complexity:** **O(1)** — standard constant time operations using direct mathematical formulas.
* **Theoretical Best:** **O(1)** — achieved, because string length is bounded by 26 English letters.

---

## Edge Cases Handled

* **Short Words ($1 \le \text{length} \le 8$):** Correctly assigns 1 push to every letter using the `count < 8` branch.
* **Medium Words ($9 \le \text{length} \le 16$):** Fills all 8 single-push slots first, then adds 2 pushes for each remaining character using the `count < 24` branch.
* **Long Words ($17 \le \text{length} \le 24$):** Correctly transitions into 3-push slots using the `count < 48` branch.
* **Maximum Length Word ($\text{length} = 25 \text{ or } 26$):** Correctly routes remaining letters to 4-push slots in the `else` block.
* **Single Letter Word ($\text{length} = 1$):** Returns 1 without issue.
