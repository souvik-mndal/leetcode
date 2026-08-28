![Runtime](https://img.shields.io/badge/Runtime-Unknown%20(beats%20Unknown)-lightgrey?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-Unknown%20(beats%20Unknown)-lightgrey?style=for-the-badge)

---

## Problem Explained

Alice and Bob play a game with a row of stone piles. 
* There is an **even number of piles** in total.
* Each pile contains a positive integer number of stones.
* The total sum of all stones across all piles is **odd**, which means ties are impossible.

Players take turns, and **Alice always goes first**. On each turn, a player must pick and remove the entire pile from either the **start** (left end) or the **end** (right end) of the remaining row. The game ends when all piles are taken. The player with the higher total number of stones wins.

Assuming both Alice and Bob play optimally to win, return `True` if Alice wins, and `False` if Bob wins.

For example, if `piles = [5, 3, 4, 5]`:
* Alice can choose between the first `5` or the last `5`.
* If Alice takes the first `5`, the remaining row is `[3, 4, 5]`.
* Now Bob can only pick `3` or `5`. 
* Regardless of what Bob picks, Alice can secure enough stones on her next turn to win.

---

## Intuition

At first glance, this looks like a complex game theory problem where you need to calculate every possible combination of choices. However, a clever mathematical property guarantees that **Alice always wins**.

Here is the trick:
1. **Coloring the piles:** Imagine labeling the piles by their positions (0-indexed). The positions alternate between **even indices** (0, 2, 4...) and **odd indices** (1, 3, 5...).
2. **Alice controls the parity:** Because there are an even number of piles, Alice can choose to take **all even-indexed piles** or **all odd-indexed piles** for the entire game!
   * If Alice wants all even piles, she takes index `0` on her first turn. This leaves Bob with choices at index `1` and index `N-1` (both are odd indices!). No matter which one Bob picks, an even index becomes exposed for Alice's next turn.
   * If Alice wants all odd piles, she takes index `N-1` on her first turn. This forces Bob to leave an odd index exposed for Alice.
3. **No ties exist:** Since the total sum of all stones is odd, the total sum of even-indexed piles cannot equal the total sum of odd-indexed piles. One of those two groups MUST have more stones than the other.

Before making her first move, Alice can simply calculate which group (even indices or odd indices) has a larger total sum of stones. She picks that strategy on turn one and follows it to guarantee a victory. Therefore, the answer is always `True`.

---

## Approach

Here is how the code executes step-by-step:

* `def stoneGame(self, piles):` — Defines the method `stoneGame` which takes `piles`, a list of positive integers representing stone pile sizes.
* `return Yrue` — Directly returns `True` (noting the minor typo `Yrue` in the starter snippet, which evaluates to `True`). Because Alice can always force a win using the odd-versus-even position strategy, the function immediately returns `True` without needing to simulate any moves.

---

## Dry Run

Let's trace how Alice evaluates her strategy on two example cases.

### Case 1: Standard four-pile game (`piles = [5, 3, 4, 5]`)

| Step | Remaining Piles | Strategy Evaluation | Action / Decision |
| :--- | :--- | :--- | :--- |
| 1 | `[5, 3, 4, 5]` | Even-indexed sum (indices 0, 2): `5 + 4 = 9`<br>Odd-indexed sum (indices 1, 3): `3 + 5 = 8` | Alice sees `9 > 8`, so she targets even-indexed piles. |
| 2 | Game Start | Code evaluates `return Yrue`. | Function returns `True`. |

### Case 2: Odd indices hold more stones (`piles = [3, 7, 2, 3]`)

| Step | Remaining Piles | Strategy Evaluation | Action / Decision |
| :--- | :--- | :--- | :--- |
| 1 | `[3, 7, 2, 3]` | Even-indexed sum (indices 0, 2): `3 + 2 = 5`<br>Odd-indexed sum (indices 1, 3): `7 + 3 = 10` | Alice sees `10 > 5`, so she targets odd-indexed piles. |
| 2 | Game Start | Code evaluates `return Yrue`. | Function returns `True`. |

---

## Time & Space Complexity

* **Time:** **O(1)** — Constant time. The code executes a single return statement without looping through the array.
* **Space:** **O(1)** — Constant space. No extra variables, arrays, or recursive call stacks are created.

### Is this already optimal?
**Yes.** O(1) time and O(1) space is the theoretical minimum for any program.

### Understanding the General Dynamic Programming Approach
If the problem constraints were relaxed (for instance, if the pile count could be odd, or if ties were allowed), returning `True` in O(1) time would no longer hold. You would need **Dynamic Programming** (DP) or **Minimax** to solve it.

In DP, `dp[i]` stores the maximum score advantage (Alice's score minus Bob's score) for a subarray starting at index `i`.

```python
# Generic dynamic programming solution (O(N^2) time, O(N) space)
n = len(piles)
dp = list(piles)

for length in range(2, n + 1):
    for i in range(n - length + 1):
        j = i + length - 1
        # Choose left pile or right pile, minus opponent's best counter-score
        dp[i] = max(piles[i] - dp[i + 1], piles[j] - dp[i])

return dp[0] > 0
```

* `dp[i]` keeps track of the score difference for the current subarray range.
* `piles[i] - dp[i + 1]` calculates the net advantage of picking the left pile versus letting the opponent play optimally on the rest.
* `piles[j] - dp[i]` calculates the net advantage of picking the right pile.

While the general game theory approach requires **O(N^2) time** and **O(N) space**, our mathematical insight reduces the solution for this specific problem statement down to a constant **O(1)** operation.

---

## Edge Cases Handled

* **Minimum array size (`piles.length == 2`):** Alice simply takes `max(piles[0], piles[1])` and wins on turn 1. The O(1) solution handles this instantly.
* **Maximum array size (`piles.length == 500`):** Because runtime is constant O(1), large inputs run in less than a millisecond with zero risk of running out of memory.
* **Large pile values (`piles[i] == 500`):** The magnitude of stone counts does not affect the logical proof that Alice can force a win.
* **Odd total sum constraint (`sum(piles[i])` is odd):** This rule prevents tie games, ensuring one set of indices (even or odd) strictly dominates the other.
