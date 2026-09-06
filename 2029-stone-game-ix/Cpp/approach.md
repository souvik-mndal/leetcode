![Runtime](https://img.shields.io/badge/Runtime-1%20ms%20(beats%2083.84%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-131.3%20MB%20(beats%2065.32%25)-green?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a turn-based game with a collection of stones. Alice always takes the first turn. On each turn, a player removes one stone from the remaining stones and adds its value to a running total sum.

There are two ways to lose or win:
1. **Instant Loss:** If a player removes a stone and the new running total sum becomes divisible by 3 (sum remainder 0 when divided by 3), that player **loses immediately**, and the other player wins.
2. **Game End Loss:** If all stones are removed and the running total was never divisible by 3, **Bob wins automatically** (Alice loses).

Both players play perfectly to win. You need to return `true` if Alice can guarantee a win, or `false` if Bob can guarantee a win.

For example, if `stones = [2, 1]`:
- Alice picks 1 (running total = 1).
- Bob is forced to pick 2 (running total = 1 + 2 = 3).
- Since 3 is divisible by 3, Bob loses instantly. Alice wins (`true`).

---

## Intuition

The actual numbers on the stones do not matter—only their **remainder when divided by 3** matters (0, 1, or 2).

We can categorize all stones into three counts:
- `count[0]`: Stones with remainder 0 (e.g., 3, 6, 9)
- `count[1]`: Stones with remainder 1 (e.g., 1, 4, 7)
- `count[2]`: Stones with remainder 2 (e.g., 2, 5, 8)

Here is the key trick to understanding the game:
1. **Remainder-0 stones are turn flippers:** Picking a stone with remainder 0 does not change the total sum's remainder modulo 3. It simply passes the turn to the other player.
2. **Alice cannot start with a remainder-0 stone:** If Alice picks a 0-remainder stone on turn 1, the total sum is 0, which is divisible by 3, so she loses immediately. Alice MUST start by picking a stone with remainder 1 or remainder 2.
3. **Forced moves after the start:** Once a starting stone (say 1) is picked, the running sum remainder is 1. The next player cannot pick remainder 2 (1 + 2 = 3, divisible by 3). They must pick remainder 1 or remainder 0. This creates a strictly forced sequence of moves.

The game outcome depends entirely on whether `count[0]` is **even** or **odd**:

- **If `count[0]` is even:** The turn flippers cancel out in pairs and do not change who gets trapped. Alice can win as long as both remainder-1 and remainder-2 stones exist (`count[1] > 0` and `count[2] > 0`). Alice can choose to start with whichever group is smaller, forcing Bob to run out of safe moves first.
- **If `count[0]` is odd:** Bob gains a free turn flip that breaks Alice's standard sequence. Alice can only win if one type of stone (1 or 2) heavily outweighs the other by at least 3 stones (`abs(count[1] - count[2]) >= 3`). This extra buffer gives Alice enough extra moves to survive Bob's turn flip and still force Bob into a losing move.

---

## Approach

Here is how the code implements this strategy step-by-step:

- `int count[3] = {0, 0, 0};`: Create a small frequency array of size 3 to store the count of stones with remainders 0, 1, and 2.
- `for (int stone : stones)` and `count[stone % 3]++;`: Loop through every stone in the input vector, compute its remainder when divided by 3 (`stone % 3`), and increment the corresponding counter.
- `if (count[0] % 2 == 0)`: Check if the number of remainder-0 stones is even.
- `return count[1] > 0 && count[2] > 0;`: If `count[0]` is even, return `true` if at least one remainder-1 stone AND at least one remainder-2 stone exist. Otherwise, return `false`.
- `return abs(count[1] - count[2]) >= 3;`: If `count[0]` is odd, return `true` if the absolute difference between `count[1]` and `count[2]` is 3 or greater. Otherwise, return `false`.

---

## Dry Run

### Case 1: `stones = [2, 1]` (Even zero-remainder count)

Counting phase:

| Step | `stone` | `stone % 3` | `count[0]` | `count[1]` | `count[2]` | Action / Decision |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | 0 | 0 | 0 | Initialize counters |
| 1 | 2 | 2 | 0 | 0 | 1 | Process stone 2 |
| 2 | 1 | 1 | 0 | 1 | 1 | Process stone 1 |

Evaluation phase:
- `count[0] % 2 == 0` is `0 % 2 == 0` (**True**, 0 is even).
- Check `count[1] > 0 && count[2] > 0`: `1 > 0 && 1 > 0` is **True**.
- Output: `true` (Alice wins).

---

### Case 2: `stones = [5, 1, 2, 4, 3]` (Odd zero-remainder count)

Counting phase:

| Step | `stone` | `stone % 3` | `count[0]` | `count[1]` | `count[2]` | Action / Decision |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | 0 | 0 | 0 | Initialize counters |
| 1 | 5 | 2 | 0 | 0 | 1 | Process stone 5 |
| 2 | 1 | 1 | 0 | 1 | 1 | Process stone 1 |
| 3 | 2 | 2 | 0 | 1 | 2 | Process stone 2 |
| 4 | 4 | 1 | 0 | 2 | 2 | Process stone 4 |
| 5 | 3 | 0 | 1 | 2 | 2 | Process stone 3 |

Evaluation phase:
- `count[0] % 2 == 0` is `1 % 2 == 0` (**False**, 1 is odd).
- Calculate `abs(count[1] - count[2])`: `abs(2 - 2) = 0`.
- Check `0 >= 3`: **False**.
- Output: `false` (Bob wins).

---

## Time & Space Complexity

- **Time:** **O(N)** — where N is the number of stones in the input array. We loop through the array once to count remainders. The final modulo and mathematical checks run in O(1) constant time.
- **Space:** **O(1)** — We only use a fixed array of size 3 (`count[3]`), which uses constant memory regardless of input size.

### Is this solution optimal?

**Yes, this code is already fully optimal.** 

To determine who wins, any valid algorithm must look at every stone at least once to determine its remainder modulo 3. Thus, O(N) time is the theoretical minimum bound. O(1) space is also minimal since we only keep track of three counts. No further optimizations in time or space are possible.

---

## Edge Cases Handled

- **All stones are divisible by 3 (e.g., `stones = [3, 6, 9]`):** `count[1]` and `count[2]` will both be 0. Alice has no safe starting stone and loses instantly (`false`).
- **Only one remainder group available (e.g., `stones = [1, 4, 7]`):** `count[2]` is 0. If `count[0]` is even, `count[1] > 0 && count[2] > 0` returns `false`, correctly identifying that Alice will get trapped when forced to complete the sequence.
- **Smallest inputs (e.g., `stones = [2]`):** `count[0] = 0`, `count[1] = 0`, `count[2] = 1`. `count[1] > 0` is false, returning `false` (Bob wins because Alice removes all stones without anyone reaching a multiple of 3).
- **Large numbers on stones:** Using `stone % 3` handles arbitrarily large values without integer overflow.
