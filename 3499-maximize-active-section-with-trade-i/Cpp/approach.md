![Runtime](https://img.shields.io/badge/Runtime-120%20ms%20(beats%2029.83%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-151.4%20MB%20(beats%2021.70%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given a binary string `s` containing only `'0'`s and `'1'`s. A `'1'` represents an active section, and a `'0'` represents an inactive section.

Imagine adding a virtual `'1'` at both the start and end of `s`. Your goal is to maximize the total number of `'1'`s in `s` by making at most one **trade**.

A trade consists of two steps done in order:
1. Choose a block of consecutive `'1'`s that is completely surrounded by `'0'`s, and change all those `'1'`s into `'0'`s.
2. Choose a block of consecutive `'0'`s that is completely surrounded by `'1'`s, and change all those `'0'`s into `'1'`s.

The extra virtual `'1'`s at the ends help surround sections at the boundaries, but they do not count toward your final total. You need to return the maximum number of `'1'`s possible in `s` after at most one trade.

For example, if `s = "0100"`:
- The string with virtual ends is `'1' + "0100" + '1'`.
- The middle `'1'` is surrounded by `'0'`s. In step 1, we turn it to `'0'`, making `s = "0000"`.
- Now all four `'0'`s form one single block surrounded by the virtual `'1'`s. In step 2, we turn all four `'0'`s into `'1'`s, getting `s = "1111"`.
- Final answer: 4.

---

## Intuition

What actually happens during a trade?

Suppose you have a pattern like `00` (left `'0'`s) - `11` (middle `'1'`s) - `000` (right `'0'`s).

1. Step 1 turns the middle `'1'`s into `'0'`s.
2. Now, the left `'0'`s, middle turned `'0'`s, and right `'0'`s merge into one giant block of `'0'`s.
3. Step 2 turns this entire merged block into `'1'`s!

Notice the net gain:
- You temporarily lost the middle `'1'`s, but then gained the left `'0'`s, middle `'1'`s, and right `'0'`s back as `'1'`s.
- Net gain in `'1'`s = **length of left `'0'` block + length of right `'0'` block**.

So, trading a middle `'1'` block effectively gives you extra `'1'`s equal to the sum of the two `'0'` blocks surrounding it!

Boundary `'1'`s (at the very left or right of `s`) touch the virtual `'1'`s, so they are not surrounded by `'0'`s and cannot be traded. We can safely ignore them by trimming leading and trailing `'1'`s.

---

## Approach

Here is how the code implements this step-by-step:

* `int ones = 0; for (char ch : s) { if (ch == '1') ones++; }`: Counts total `'1'`s originally present in `s`.
* `int l = 0, r = n - 1;`: Sets pointers at the start and end of `s`.
* `while (l < n && s[l] == '1') l++;`: Moves `l` right to skip leading `'1'`s because they cannot be traded.
* `while (r >= 0 && s[r] == '1') r--;`: Moves `r` left to skip trailing `'1'`s for the same reason.
* `if (l > r) return ones;`: If the string has no `'0'`s (all `'1'`s), no trade is possible. Returns `ones`.
* `vector<pair<int, char>> store;`: Stores compressed blocks of character counts for the trimmed substring `s[l...r]`.
* `for (int i = l + 1; i <= r; i++) { ... } store.push_back({len, s[r]});`: Groups consecutive identical characters together into block lengths.
* `int MaxRange = 0;`: Keeps track of the maximum combined length of two adjacent `'0'` blocks.
* `for (int i = 0; i + 2 < store.size(); i += 2)`: Iterates through all `'0'` blocks. Here, `store[i]` is a `'0'` block, `store[i + 1]` is a `'1'` block, and `store[i + 2]` is the next `'0'` block.
* `int range = store[i].first + store[i + 2].first; MaxRange = max(MaxRange, range);`: Calculates the gained `'0'` length if we trade the `'1'` block at `store[i + 1]`.
* `return ones + MaxRange;`: Adds the best possible gained count to the original count of `'1'`s.

---

## Dry Run

### Case 1: Standard case `s = "0100"`

- Initial `ones = 1`, `n = 4`.
- Find boundaries: `l = 0`, `r = 3` (no leading or trailing `'1'`s to skip).
- Substring `s[0...3]` is `"0100"`.
- Compress into `store`:
  - `store[0]` = `{1, '0'}`
  - `store[1]` = `{1, '1'}`
  - `store[2]` = `{2, '0'}`

| Step | `i` | `store[i]` (0-block) | `store[i+1]` (1-block) | `store[i+2]` (0-block) | `range` | `MaxRange` | Action / Explanation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | - | - | - | - | - | 0 | Initialize `MaxRange = 0` |
| 2 | 0 | `{1, '0'}` | `{1, '1'}` | `{2, '0'}` | 1 + 2 = 3 | 3 | Trade inner '1' block; gain both '0' blocks |

- Return `ones + MaxRange` = 1 + 3 = 4.

---

### Case 2: String with leading '1's `s = "1000100"`

- Initial `ones = 2`, `n = 7`.
- Find boundaries: `l = 1` (skips `s[0] == '1'`), `r = 6`.
- Substring `s[1...6]` is `"000100"`.
- Compress into `store`:
  - `store[0]` = `{3, '0'}`
  - `store[1]` = `{1, '1'}`
  - `store[2]` = `{2, '0'}`

| Step | `i` | `store[i]` (0-block) | `store[i+1]` (1-block) | `store[i+2]` (0-block) | `range` | `MaxRange` | Action / Explanation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | - | - | - | - | - | 0 | Initialize `MaxRange = 0` |
| 2 | 0 | `{3, '0'}` | `{1, '1'}` | `{2, '0'}` | 3 + 2 = 5 | 5 | Trade inner '1' block at index 4; gain left and right '0' blocks |

- Return `ones + MaxRange` = 2 + 5 = 7.

---

## Time & Space Complexity

- **Time Complexity:** **O(n)** — We iterate through `s` a constant number of times (counting `'1'`s, finding `l` and `r`, building `store`, and scanning `store`).
- **Space Complexity:** **O(n)** — The vector `store` can store up to `n` pairs in the worst case (e.g., alternating `"010101"`).

### Can we improve this?

Yes! We can optimize the **space complexity** from O(n) to **O(1)**.

Instead of building a `store` vector, we can iterate from `l` to `r` and track running lengths of `'0'` blocks in real time. We only need to remember the length of the *previous* `'0'` block (`prev_zero`) and the *current* `'0'` block (`curr_zero`). Whenever we finish a `'1'` block, we add `prev_zero + curr_zero` to update `MaxRange`.

```cpp
// Optimized snippet using O(1) extra space
int MaxRange = 0;
int prev_zero = 0, curr_zero = 0;
bool seen_one = false;

for (int i = l; i <= r; ) {
    if (s[i] == '0') {
        curr_zero = 0;
        while (i <= r && s[i] == '0') { curr_zero++; i++; }
        if (seen_one) {
            MaxRange = max(MaxRange, prev_zero + curr_zero);
        }
        prev_zero = curr_zero;
    } else {
        seen_one = true;
        while (i <= r && s[i] == '1') { i++; }
    }
}
```

- **Resulting Improved Complexity:** **Time:** O(n), **Space:** O(1).
- **Theoretical Best Complexity:** **O(n) time** (must read the string) and **O(1) space**. The improved version achieves this theoretical limit.

---

## Edge Cases Handled

- **All `'1'`s (e.g., `s = "111"`):** Handled by `if (l > r) return ones;`. Pointers cross each other, returning `3` directly without errors.
- **No inner `'1'` block (e.g., `s = "000"` or `s = "01"`):** `store.size()` will be less than 3. The loop condition `i + 2 < store.size()` fails immediately, leaving `MaxRange = 0`. Returns original count of `'1'`s.
- **Leading/Trailing `'1'`s (e.g., `s = "110010011"`):** Skipping boundary `'1'`s using `l` and `r` guarantees we only evaluate valid internal trades surrounded by `'0'` blocks.
