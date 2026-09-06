![Runtime](https://img.shields.io/badge/Runtime-104%20ms%20(beats%2097.32%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-72.4%20MB%20(beats%2096.81%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `s`. You also receive a series of queries. Each query specifies an index in `s` and a new character to put at that index.

After applying each character update to `s`, you need to find the length of the longest substring made up of a single repeating character (for example, `"bbb"` has length 3).

Your goal is to return an array containing the maximum repeating substring length after each query is processed.

**Example:**
* Initial string: `s = "babacc"`
* Query 1: Change index 1 to `'b'` -> `s` becomes `"bbbacc"`. The longest repeating run is `"bbb"` (length 3).
* Query 2: Change index 3 to `'c'` -> `s` becomes `"bbbccc"`. Longest run is `"bbb"` or `"ccc"` (length 3).
* Query 3: Change index 3 to `'b'` -> `s` becomes `"bbbbcc"`. Longest run is `"bbbb"` (length 4).
* Output: `[3, 3, 4]`

---

## Intuition

Checking the whole string from scratch after every update would take too long.

Instead, notice that a continuous block of identical characters always lies between two **transition boundaries** — indices where `s[i - 1] != s[i]`. If we keep track of all boundary indices in an ordered set, the length of any repeating block is simply `next_boundary - prev_boundary`.

When you change a single character at index `idx`:
1. It only affects transitions at two places: between `idx - 1` and `idx`, and between `idx` and `idx + 1`.
2. Changing the character either creates a boundary (splitting one block into two smaller ones) or removes a boundary (merging two smaller blocks into one larger block).
3. By storing boundary positions in an ordered set (`boundaries`), we can find surrounding boundaries in fast logarithmic time.
4. By keeping track of all active block lengths in another ordered set (`lengths`), the maximum repeating length at any moment is simply the largest number in that set (`*lengths.rbegin()`).

Using C++ polymorphic memory resources (`pmr`) gives us a custom memory pool so set operations run faster without standard allocation overhead.

---

## Approach

Here is how the code works step-by-step:

* `pmr::unsynchronized_pool_resource pool;` and `pmr::set<int> boundaries{&pool}, lengths{&pool};`: Creates a fast memory resource pool and sets up two ordered sets (`boundaries` for index transitions, `lengths` for current block lengths).
* `vector<int> count;`: Allocates a frequency array where `count[len]` tracks how many blocks in `s` currently have length `len`.
* `auto addlen = [&](int len) { if (++count[len] == 1) lengths.insert(len); };`: Helper lambda that increases the count for `len`. If this is the first block of this length, it adds `len` to `lengths`.
* `auto removelen = [&](int len) { if (--count[len] == 0) lengths.erase(len); };`: Helper lambda that decreases the count for `len`. If no blocks of this length remain, it removes `len` from `lengths`.
* `boundaries.insert(0); boundaries.insert(n);`: Inserts virtual boundaries at the start (0) and end (`n`) of string `s`.
* `for (int i = 1; i < n; ++i) { if (s[i - 1] != s[i]) { boundaries.insert(i); addlen(i - prev); prev = i; } }`: Scans `s` initially to register all character transition indices into `boundaries` and track initial block lengths using `addlen`.
* `addlen(n - prev);`: Adds the length of the final block at the end of the initial scan.
* `auto update = [&](int i, bool set)`: Helper function to either add a boundary at index `i` (`set == true`) or remove a boundary at index `i` (`set == false`).
* `if (set) { ... }`: When adding a boundary at `i`, finds the surrounding boundaries `prev` and `next`. Removes the old combined length `next - prev`, adds the two new smaller block lengths `i - prev` and `next - i`, and inserts `i` into `boundaries`.
* `else { ... }`: When removing a boundary at `i`, finds `prev` (before `i`) and `next` (after `i`). Removes the two smaller lengths `i - prev` and `next - i`, adds the new merged length `next - prev`, and erases `i` from `boundaries`.
* `for (int i = 0; i < indices.size(); ++i)`: Loops through each query to update the string and collect results.
* `if (idx > 0) { ... }`: Compares whether the character transition status between `idx - 1` and `idx` changed (`old != now`). If it changed, calls `update(idx, now)` to add or remove the boundary at `idx`.
* `if (idx + 1 < n) { ... }`: Compares whether the character transition status between `idx` and `idx + 1` changed (`old != now`). If it changed, calls `update(idx + 1, now)` to add or remove the boundary at `idx + 1`.
* `s[idx] = c;`: Updates character at index `idx` in string `s`.
* `indices[i] = *lengths.rbegin();`: Reads the largest element currently in `lengths` and writes it into `indices[i]` as the answer for query `i`.
* `return move(indices);`: Returns the vector of results.

---

## Dry Run

### Case 1: Typical case with multiple queries

Given: `s = "babacc"`, `chars = "bcb"`, `indices = [1, 3, 3]`

**Initial Setup:**
* `boundaries` = `{0, 1, 2, 3, 4, 6}`
* `lengths` = `{1, 2}` (block lengths are 1, 1, 1, 1, 2)
* `count[1]` = 4, `count[2]` = 1

| Query `i` | `idx` | `c` | String `s` | Boundary Changes | `boundaries` state | `lengths` state | Result `*lengths.rbegin()` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0** | 1 | `'b'` | `"bbbacc"` | Transition at 1 removed (`s[0]==s[1]`); Transition at 2 removed (`s[1]==s[2]`) | `{0, 3, 4, 6}` | `{1, 2, 3}` | **3** |
| **1** | 3 | `'c'` | `"bbbccc"` | Transition at 4 removed (`s[3]==s[4]`) | `{0, 3, 6}` | `{3}` | **3** |
| **2** | 3 | `'b'` | `"bbbbcc"` | Transition at 3 removed (`s[2]==s[3]`); Transition at 4 added (`s[3]!=s[4]`) | `{0, 4, 6}` | `{2, 4}` | **4** |

---

### Case 2: Consecutive character update

Given: `s = "abyzz"`, `chars = "aa"`, `indices = [2, 1]`

**Initial Setup:**
* `boundaries` = `{0, 1, 2, 3, 5}`
* `lengths` = `{1, 2}` (block lengths are 1, 1, 1, 2)
* `count[1]` = 3, `count[2]` = 1

| Query `i` | `idx` | `c` | String `s` | Boundary Changes | `boundaries` state | `lengths` state | Result `*lengths.rbegin()` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0** | 2 | `'a'` | `"abazz"` | No transition changes at 2 or 3 | `{0, 1, 2, 3, 5}` | `{1, 2}` | **2** |
| **1** | 1 | `'a'` | `"aaazz"` | Transition at 1 removed (`s[0]==s[1]`); Transition at 2 removed (`s[1]==s[2]`) | `{0, 3, 5}` | `{2, 3}` | **3** |

---

## Time & Space Complexity

* **Time Complexity:**
  * **Initialization:** `O(n log n)` — scanning string `s` of length `n` and inserting initial boundary indices and block lengths into `boundaries` and `lengths`.
  * **Per Query:** `O(log n)` — each query modifies at most 2 boundaries, performing a constant number of `lower_bound`, `find`, `insert`, and `erase` operations on ordered sets.
  * **Total Time:** `O((n + k) log n)` where `n` is `s.length` and `k` is the number of queries.

* **Space Complexity:** `O(n)` — storing up to `n + 1` boundaries in `boundaries`, up to `n` block lengths in `lengths`, and size `n + 1` in `count`.

### Can this be improved?

**Yes.** We can improve the initialization time from `O(n log n)` down to `O(n)` by replacing the set-based approach with a **Segment Tree**.

A Segment Tree represents ranges of the string. Each tree node stores:
1. `max_len`: length of the longest repeating substring in its range.
2. `prefix_len`: length of repeating characters starting at the left boundary of its range.
3. `suffix_len`: length of repeating characters ending at the right boundary of its range.
4. `left_char` and `right_char`: the boundary characters of its range.

Building a Segment Tree takes `O(n)` time. Updating a single character takes `O(log n)` time.

```cpp
struct Node {
    int max_len, prefix_len, suffix_len;
    char left_char, right_char;
    int size;
};

Node merge(const Node& left, const Node& right) {
    Node res;
    res.size = left.size + right.size;
    res.left_char = left.left_char;
    res.right_char = right.right_char;
    
    res.prefix_len = left.prefix_len;
    if (left.prefix_len == left.size && left.right_char == right.left_char)
        res.prefix_len += right.prefix_len;
        
    res.suffix_len = right.suffix_len;
    if (right.suffix_len == right.size && left.right_char == right.left_char)
        res.suffix_len += left.suffix_len;
        
    res.max_len = max(left.max_len, right.max_len);
    if (left.right_char == right.left_char)
        res.max_len = max(res.max_len, left.suffix_len + right.prefix_len);
        
    return res;
}
```

* **Improved Complexity:**
  * **Time Complexity:** `O(n + k log n)` (`O(n)` build time + `O(log n)` per query).
  * **Space Complexity:** `O(n)` to store tree nodes.

* **Is `O(n + k log n)` optimal?** Yes. Since each query can fundamentally alter substring boundaries and we must report results after every query, `O(log n)` per query is the theoretical best time complexity for comparison-based dynamic range updates.

---

## Edge Cases Handled

* **Updates at Boundaries (`idx = 0` or `idx = n - 1`):** Handled safely by condition checks `idx > 0` and `idx + 1 < n`, preventing out-of-bounds boundary updates.
* **No Boundary Status Change:** If changing `s[idx]` to character `c` doesn't change whether adjacent characters match (e.g., changing `'a'` to `'c'` in `"a b a"` leaves both sides unequal), `old != now` evaluates to `false`, avoiding unnecessary updates.
* **Duplicate Lengths:** Multiple distinct repeating blocks can have identical lengths. The `count` vector tracks frequencies, ensuring a length is only removed from `lengths` when zero blocks of that size remain.
* **Entire String Made of One Character:** Virtual boundaries at `0` and `n` ensure that a single contiguous block spanning the whole string is accurately recorded with length `n`.
