![Runtime](https://img.shields.io/badge/Runtime-99%20ms%20(beats%2080.97%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-87.21%20MB%20(beats%2084.19%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a row of stones, each with a integer value printed on it. Two players, Alice and Bob, play a turn-based game using these stones. Alice always goes first.

On any player's turn, if there is more than one stone left, the player must:
1. Choose a number of stones from the left (at least 2 stones, up to all remaining stones).
2. Remove those chosen stones from the row.
3. Add the sum of the removed stones' values to their score.
4. Place a single new stone back on the far-left side of the row. The value of this new stone is equal to the sum of the removed stones.

The game ends when only 1 stone remains in the row.

The final score difference is calculated as: **(Alice's score) - (Bob's score)**. 

Alice wants to make this difference as large as possible. Bob wants to make this difference as small as possible (or as negative as possible). Assuming both players play perfectly to reach their goals, return the final score difference.

---

## Intuition

The key insight comes from noticing what happens when you combine stones from the left:

When a player takes the first `k` stones, the value of the new replacement stone is equal to the **prefix sum** (the sum of all original stones from index 0 up to index `k - 1`). 

Because the replacement stone's value is always equal to that original prefix sum, taking stones does **not** change the prefix sums of the remaining stones on the right!

This means:
* Choosing to take stones up to index `i` (where `i >= 1`) earns the player `prefix[i]` points.
* The remaining board state is identical to the original board starting after index `i`.
* The next player is now forced to choose a future index `j` that is strictly greater than `i` (`j > i`).

Because this is a **zero-sum game** (one player's gain is the other's loss), if the current player picks index `i`, they get `prefix[i]` points, but their opponent will get the optimal score difference available from the remaining indices (`i + 1` to `n - 1`). 

So, if a player chooses index `i`, their relative score advantage over the opponent is:
`prefix[i] - (opponent's best relative score from index i + 1 onwards)`

Alternatively, the player can choose **not** to pick index `i` and instead pass the option to the best move available at index `i + 1` or later.

By working **backwards** from the right side of the array to the left, we can track the maximum relative score difference a player can get at each step.

---

## Approach

Here is how the code works step-by-step:

* `int n = stones.size();`: Stores the total number of stones in the initial array.
* `for (int i = 1; i < n; ++i) { stones[i] += stones[i - 1]; }`: Replaces the original stone values in-place with running prefix sums. After this loop, `stones[i]` represents the sum of all original stones from index 0 to index `i`.
* `int max_diff = stones[n - 1];`: Initializes `max_diff` with the prefix sum of taking all stones (index `n - 1`). If a player takes all remaining stones, no stones are left for the opponent, so the relative score difference is simply the sum of all stones.
* `for (int i = n - 2; i >= 1; --i)`: Iterates backwards from index `n - 2` down to `1`. Index 0 is skipped because a player must take at least 2 stones (which corresponds to picking at least index 1).
* `max_diff = std::max(max_diff, stones[i] - max_diff);`: Updates `max_diff` at index `i`. The player compares two choices:
  1. Skip index `i` and keep the best outcome available from later indices (`max_diff`).
  2. Choose index `i`, earning `stones[i]` points and subtracting the opponent's best future advantage (`stones[i] - max_diff`).
* `return max_diff;`: Returns the final maximum score difference Alice can secure starting from the first valid move (index 1).

---

## Dry Run

### Case 1: Typical case with mixed values
`stones = [-1, 2, -3, 4, -5]`

1. **Prefix sum conversion (in-place):**
   * `stones[0] = -1`
   * `stones[1] = -1 + 2 = 1`
   * `stones[2] = 1 + (-3) = -2`
   * `stones[3] = -2 + 4 = 2`
   * `stones[4] = 2 + (-5) = -3`
   * Array becomes: `[-1, 1, -2, 2, -3]`

2. **Initialize:** `max_diff = stones[4] = -3`

3. **Backward Loop:**

| `i` | `stones[i]` | Option 1: Skip (`max_diff`) | Option 2: Take (`stones[i] - max_diff`) | New `max_diff` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `3` | `2` | `-3` | `2 - (-3) = 5` | `5` | Player prefers taking at index 3 |
| `2` | `-2` | `5` | `-2 - 5 = -7` | `5` | Player prefers skipping index 2 |
| `1` | `1` | `5` | `1 - 5 = -4` | `5` | Player prefers skipping index 1 |

* **Final Return Value:** `5`

---

### Case 2: Minimum size input (Edge Case)
`stones = [-10, -12]`

1. **Prefix sum conversion (in-place):**
   * `stones[0] = -10`
   * `stones[1] = -10 + (-12) = -22`
   * Array becomes: `[-10, -22]`

2. **Initialize:** `max_diff = stones[1] = -22`

3. **Backward Loop:**
   * Loop starts at `i = n - 2 = 0`.
   * Loop condition requires `i >= 1`, which is false (`0 >= 1` is false).
   * Loop body does not run.

| Step | Variable State | Action |
| :--- | :--- | :--- |
| Initialization | `max_diff = -22` | Takes all stones at index 1 |
| Loop | Skipped | Only 1 valid move exists |

* **Final Return Value:** `-22`

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — The algorithm makes one forward pass of length `n` to compute prefix sums, and one backward pass of length `n - 2` to update `max_diff`. Both passes do simple O(1) arithmetic at each element.
* **Space Complexity:** **O(1)** auxiliary space — The code mutates the input `stones` array in-place to store prefix sums and uses only two single-integer variables (`n` and `max_diff`).

### Is this optimal?
**Yes, this is optimal.**
* **Time:** We must examine every element at least once to know its value, so O(n) is the theoretical lower bound.
* **Space:** O(1) extra space is the theoretical best possible memory usage.

No further optimization in time or space is possible.

---

## Edge Cases Handled

* **Minimum array length (`n = 2`):** The backward loop condition `i >= 1` naturally prevents out-of-bounds access or invalid moves, returning the total sum directly.
* **Negative numbers:** The algorithm works seamlessly with negative stone values because `std::max` handles positive and negative score comparisons correctly.
* **Large total sums:** Individual stone values range from -10,000 to 10,000 with up to 100,000 stones. The maximum possible total sum fits within standard 32-bit signed integers (between -1,000,000,000 and 1,000,000,000), avoiding integer overflow.
* **Optimal skipping:** The dynamic choice `std::max(max_diff, stones[i] - max_diff)` correctly models a player passing on bad options to force a better outcome later in the array.
