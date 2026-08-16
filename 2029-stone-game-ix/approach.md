![Runtime](https://img.shields.io/badge/Runtime-1%20ms%20(beats%2082.74%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-131.27%20MB%20(beats%2069.29%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with a row of stones. Each stone has an integer value.

* **Turns:** Alice goes first. Players take turns picking and removing any single stone from the remaining stones.
* **Running Sum:** The game keeps track of the sum of all removed stone values so far.
* **Losing Condition:** If a player removes a stone that makes the total sum evenly divisible by 3, that player **loses immediately**.
* **Bob's Default Win:** If all stones are removed and the total sum was never divisible by 3, Bob wins automatically.

Alice wants to win, and Bob wants Alice to lose. Both players make the smartest possible move on every turn. Your job is to return `true` if Alice can guarantee a win, or `false` if Bob wins.

For example, if `stones = [2, 1]`:
* Alice picks `2` (sum = 2, not divisible by 3).
* Bob must pick `1` (sum = 2 + 1 = 3, which is divisible by 3).
* Bob loses, so Alice wins (`true`).

---

## Intuition

Since losing depends entirely on whether the sum is divisible by 3, the exact numbers on the stones do not matter. Only their **remainder when divided by 3** matters (`stone % 3`).

Every stone falls into one of three groups:
1. **Group 0:** Stones with remainder 0 (value % 3 == 0).
2. **Group 1:** Stones with remainder 1 (value % 3 == 1).
3. **Group 2:** Stones with remainder 2 (value % 3 == 2).

Playing a **Group 0** stone adds 0 to the sum's remainder. It does not change whether the sum is divisible by 3. It acts like a "pass turn" button, passing the move to the other player.

Playing a **Group 1** or **Group 2** stone changes the sum's remainder:
* If current sum remainder is 1, picking a 1-stone changes it to 2 (safe). Picking a 2-stone makes it 3, which is 0 mod 3 (instant loss).
* If current sum remainder is 2, picking a 2-stone changes it to 1 (safe). Picking a 1-stone makes it 3, which is 0 mod 3 (instant loss).

The key insight breaks down into two cases based on **Group 0** stones:

1. **Even number of Group 0 stones (`count[0] % 2 == 0`):**
   * Pairwise, 0-stones cancel out because whatever one player passes, the other can pass right back.
   * Alice can win as long as **both** Group 1 and Group 2 have at least 1 stone (`count[1] > 0 && count[2] > 0`). Alice picks the smaller group first, forcing Bob into a trap where he runs out of safe stones first.

2. **Odd number of Group 0 stones (`count[0] % 2 != 0`):**
   * Bob gets an extra "pass turn" advantage.
   * Alice can only win if one non-zero group is significantly larger than the other — specifically, if the difference between `count[1]` and `count[2]` is **3 or more** (`abs(count[1] - count[2]) >= 3`).

---

## Approach

Here is how the solution works step-by-step:

* `int count[3] = {0, 0, 0};`  
  Creates an array of size 3 to store the count of stones belonging to Group 0, Group 1, and Group 2.

* `for (int stone : stones) { count[stone % 3]++; }`  
  Loops through each stone in the input vector `stones`, finds its remainder modulo 3 (`stone % 3`), and increments the corresponding slot in `count`.

* `if (count[0] % 2 == 0) { return count[1] > 0 && count[2] > 0; }`  
  Checks if the number of Group 0 stones is even. If even, Alice wins if and only if both Group 1 and Group 2 have at least one stone available.

* `return abs(count[1] - count[2]) >= 3;`  
  If the number of Group 0 stones is odd, checks if the difference between the count of Group 1 stones and Group 2 stones is at least 3. If so, Alice wins; otherwise, Bob wins.

---

## Dry Run

### Case 1: `stones = [2, 1]`

* Remainder counts calculated: `count[0] = 0`, `count[1] = 1`, `count[2] = 1`.

| Step | Code Expression / Action | count[0] | count[1] | count[2] | Result / Decision |
| --- | --- | --- | --- | --- | --- |
| 1 | `int count[3] = {0, 0, 0};` | 0 | 0 | 0 | Array initialized |
| 2 | Count remainders for `[2, 1]` | 0 | 1 | 1 | Finished counting |
| 3 | `if (count[0] % 2 == 0)` | 0 | 1 | 1 | `0 % 2 == 0` is `true` |
| 4 | `return count[1] > 0 && count[2] > 0;` | 0 | 1 | 1 | `1 > 0 && 1 > 0` is `true`. Returns `true` |

**Final Result:** `true` (Alice wins).

---

### Case 2: `stones = [5, 1, 2, 4, 3]`

* Remainders: `5 % 3 = 2`, `1 % 3 = 1`, `2 % 3 = 2`, `4 % 3 = 1`, `3 % 3 = 0`.
* Remainder counts calculated: `count[0] = 1`, `count[1] = 2`, `count[2] = 2`.

| Step | Code Expression / Action | count[0] | count[1] | count[2] | Result / Decision |
| --- | --- | --- | --- | --- | --- |
| 1 | `int count[3] = {0, 0, 0};` | 0 | 0 | 0 | Array initialized |
| 2 | Count remainders for `[5, 1, 2, 4, 3]` | 1 | 2 | 2 | Finished counting |
| 3 | `if (count[0] % 2 == 0)` | 1 | 2 | 2 | `1 % 2 == 0` is `false` |
| 4 | `return abs(count[1] - count[2]) >= 3;` | 1 | 2 | 2 | `abs(2 - 2) = 0 >= 3` is `false`. Returns `false` |

**Final Result:** `false` (Bob wins).

---

## Time & Space Complexity

* **Time Complexity:** **O(N)** — where N is the number of stones. We make a single pass through the array to count remainders. The final decision takes constant time **O(1)**.
* **Space Complexity:** **O(1)** — we only store a fixed array `count[3]` of size 3, which uses constant memory regardless of input size.

### Is this optimal?

**Yes, this solution is fully optimal.** 
To know the game outcome, every stone must be inspected at least once to determine its remainder, requiring at least **O(N)** time. The extra space used is **O(1)**, which is the absolute theoretical minimum. No faster time or smaller space complexity is possible.

---

## Edge Cases Handled

* **No Group 0 stones present (`count[0] == 0`):** Since `0 % 2 == 0` is true, the code correctly evaluates the even branch.
* **Only one type of non-zero stone present (e.g., `stones = [1, 1, 1]` or `stones = [2]`):** If `count[1] == 0` or `count[2] == 0`, `count[1] > 0 && count[2] > 0` returns `false`, correctly recognizing that Alice cannot win without choices from both groups.
* **Single stone array (e.g., `stones = [2]`):** Alice must pick the only stone. Total sum becomes 2 (not divisible by 3). Since no stones remain, Bob wins by rule. The code returns `false` (`count[1] > 0 && count[2] > 0` is `0 > 0 && 1 > 0` = `false`).
* **Balanced vs imbalanced odd Group 0 cases:** If `count[0]` is odd, Alice only wins if `abs(count[1] - count[2]) >= 3`. The code directly checks this arithmetic condition.
