![Runtime](https://img.shields.io/badge/Runtime-104%20ms%20(beats%2098.66%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-72.35%20MB%20(beats%2097.32%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a string `s` made of lowercase letters. You are also given `k` updates (queries). Each query specifies:
1. An index in the string (`queryIndices[i]`).
2. A new character to put at that index (`queryCharacters[i]`).

After each query, you update the string by placing the new character at the given index. Then, you need to find the length of the longest contiguous block of identical characters anywhere in the modified string.

Finally, you return an array containing the answer for each query in order.

### Example
Suppose `s = "babacc"`.
- **Query 1:** Change index `1` to `'c'`. The string becomes `"bcbacc"`. The longest contiguous block of identical characters is `"cc"` (or `"b"`/`"c"` blocks), but wait: index 1 was `'a'`, changing it to `'c'` makes `"bcbacc"`. If query 1 changes index 1 to `'c'`, the blocks are `"b"`, `"c"`, `"b"`, `"a"`, `"cc"`. The longest repeating run is 2 (`"cc"`). 
*(Note: In the prompt's example, updating index 1 to `'b'` makes `"bbbacc"`, giving a length of 3).*

---

## Intuition

If we scan the entire string from left to right after every single query to find the longest repeating sequence, it will take too long when the string and query count are large (up to 100,000 each).

Instead of looking at every character, notice that a string is made of contiguous "blocks" of identical characters. For example, in `"aaabbc"`, the blocks are `"aaa"` (length 3), `"bb"` (length 2), and `"c"` (length 1).

A block boundary happens wherever two adjacent characters are different (`s[i - 1] != s[i]`).
- Changing a single character at position `idx` can only affect two boundary positions:
  1. The boundary between `idx - 1` and `idx`.
  2. The boundary between `idx` and `idx + 1`.

When a boundary is removed, two adjacent blocks merge into one larger block. When a new boundary is created, one block splits into two smaller blocks.

To solve this efficiently:
1. **Track boundary locations:** Use an ordered set (`boundaries`) to store all indices where adjacent characters differ, plus virtual boundaries at index `0` and index `n`.
2. **Track block lengths:** Use a frequency count array (`count`) and an ordered set (`lengths`) containing all currently existing block lengths. This lets us read the maximum block length in instant O(1) time by taking the largest element in `lengths`.

---

## Approach

Here is the step-by-step breakdown of how the code executes:

- `pmr::unsynchronized_pool_resource pool;`: Creates a custom memory pool. Standard C++ sets allocate memory node-by-node on the heap, which is slow. Using PMR (Polymorphic Memory Resource) speeds up set insertions and deletions significantly.
- `pmr::set<int> boundaries{&pool}, lengths{&pool};`: Declares `boundaries` to store split indices in sorted order, and `lengths` to store unique active block lengths in sorted order.
- `vector<int> count;`: Declares a frequency array where `count[len]` tracks how many contiguous blocks currently have a length equal to `len`.
- `auto addlen = [&](int len) { if (++count[len] == 1) lengths.insert(len); };`: A helper function to record a block of size `len`. It increments the frequency count for `len`. If this is the first block of this length, it adds `len` into the `lengths` set.
- `auto removelen = [&](int len) { if (--count[len] == 0) lengths.erase(len); };`: A helper function to remove a block of size `len`. It decrements the frequency count for `len`. If no blocks of this length remain, it removes `len` from the `lengths` set.
- `boundaries.insert(0); boundaries.insert(n);`: Inserts the string endpoints (0 and `n`) as fixed start and end boundaries.
- `for (int i = 1; i < n; ++i) { if (s[i - 1] != s[i]) { boundaries.insert(i); addlen(i - prev); prev = i; } }`: Iterates through the original string. Whenever adjacent characters differ, it marks index `i` as a boundary and records the length of the block that just ended (`i - prev`).
- `addlen(n - prev);`: Records the length of the final block at the end of the string.
- `auto update = [&](int i, bool set)`: Helper function to handle adding or removing a boundary position `i`:
  - `if (set)`: When `set` is true, a new boundary is created at index `i`. It finds the surrounding existing boundaries `prev` and `next`, removes the old full block length (`next - prev`), adds the two new split lengths (`i - prev` and `next - i`), and inserts `i` into `boundaries`.
  - `else`: When `set` is false, an existing boundary at `i` is removed. It finds the previous boundary (`prev`) and next boundary (`next`), removes the two split lengths (`i - prev` and `next - i`), adds the merged block length (`next - prev`), and erases `i` from `boundaries`.
- `for (int i = 0; i < indices.size(); ++i)`: Loops through each query:
  - `if (idx > 0)`: Checks if the boundary status between `idx - 1` and `idx` changed. If it did (`old != now`), it calls `update(idx, now)` to add or remove that boundary.
  - `if (idx + 1 < n)`: Checks if the boundary status between `idx` and `idx + 1` changed. If it did (`old != now`), it calls `update(idx + 1, now)` to add or remove that boundary.
  - `s[idx] = c;`: Updates character at index `idx` in string `s`.
  - `indices[i] = *lengths.rbegin();`: Reads the largest block length from `lengths` (`*lengths.rbegin()`) and stores it directly into the result vector (reusing `indices` to save memory).

---

## Dry Run

### Case 1: Standard query updates
**Input:** `s = "abyzz"`, `chars = "aa"`, `indices = [2, 1]`

**Initial Setup:**
- `n = 5`
- Boundaries created at: `0`, `1` (`'a'!='b'`), `2` (`'b'!='y'`), `3` (`'y'!='z'`), `5`.
- Block lengths created: `1 - 0 = 1`, `2 - 1 = 1`, `3 - 2 = 1`, `5 - 3 = 2`.
- `count` array: `count[1] = 3`, `count[2] = 1`.
- `lengths` set: `{1, 2}`.

| Step / Query | Variable Values | Boundary Changes | `lengths` set contents | Result (`*lengths.rbegin()`) |
| :--- | :--- | :--- | :--- | :--- |
| **Start** | `s = "abyzz"` | `boundaries = {0, 1, 2, 3, 5}` | `{1, 2}` | - |
| **Query 1** | `idx = 2`, `c = 'a'` | Boundary at 2: `old` ('b'!='y' -> true), `now` ('b'!='a' -> true). Unchanged.<br>Boundary at 3: `old` ('y'!='z' -> true), `now` ('a'!='z' -> true). Unchanged. | `{1, 2}` | `2` |
| **String update** | `s` becomes `"abazz"` | No boundaries added or removed. | `{1, 2}` | `2` |
| **Query 2** | `idx = 1`, `c = 'a'` | Boundary at 1: `old` ('a'!='b' -> true), `now` ('a'!='a' -> false). **Remove 1**.<br>Boundary at 2: `old` ('b'!='a' -> true), `now` ('a'!='a' -> false). **Remove 2**. | `{2, 3}` | `3` |
| **String update** | `s` becomes `"aaazz"` | Boundary 1 removed: merges blocks `[0,1]` and `[1,2]` into `[0,2]`.<br>Boundary 2 removed: merges blocks `[0,2]` and `[2,3]` into `[0,3]` (length 3). | `{2, 3}` | `3` |

**Final Output:** `[2, 3]`

---

### Case 2: Multi-character query updates
**Input:** `s = "babacc"`, `chars = "bcb"`, `indices = [1, 3, 3]`

**Initial Setup:**
- `n = 6`
- Initial `s = "babacc"`.
- Initial boundaries: `0, 1, 2, 3, 4, 6`.
- Initial block lengths: `1` (from "b"), `1` (from "a"), `1` (from "b"), `1` (from "a"), `2` (from "cc").
- `count` array: `count[1] = 4`, `count[2] = 1`.
- `lengths` set: `{1, 2}`.

| Step / Query | Variable Values | Boundary Changes | `lengths` set contents | Result (`*lengths.rbegin()`) |
| :--- | :--- | :--- | :--- | :--- |
| **Start** | `s = "babacc"` | `boundaries = {0, 1, 2, 3, 4, 6}` | `{1, 2}` | - |
| **Query 1** | `idx = 1`, `c = 'b'` | Boundary 1: `old` ('b'!='a' -> true), `now` ('b'!='b' -> false). **Remove 1**.<br>Boundary 2: `old` ('a'!='b' -> true), `now` ('b'!='b' -> false). **Remove 2**. | `{1, 2, 3}` | `3` |
| **String update**| `s` becomes `"bbbacc"`| Merges `[0,1]`, `[1,2]`, and `[2,3]` into a single block `[0,3]` of length 3 ("bbb"). | `{1, 2, 3}` | `3` |
| **Query 2** | `idx = 3`, `c = 'c'` | Boundary 3: `old` ('b'!='a' -> true), `now` ('b'!='c' -> true). Unchanged.<br>Boundary 4: `old` ('a'!='c' -> true), `now` ('c'!='c' -> false). **Remove 4**. | `{1, 3}` | `3` |
| **String update**| `s` becomes `"bbbccc"`| Boundary 4 removed: merges block `[3,4]` ("c") and `[4,6]` ("cc") into `[3,6]` of length 3 ("ccc"). | `{1, 3}` | `3` |
| **Query 3** | `idx = 3`, `c = 'b'` | Boundary 3: `old` ('b'!='c' -> true), `now` ('b'!='b' -> false). **Remove 3**.<br>Boundary 4: `old` ('c'!='c' -> false), `now` ('b'!='c' -> true). **Insert 4**. | `{2, 4}` | `4` |
| **String update**| `s` becomes `"bbbbcc"`| Boundary 3 removed: merges `[0,3]` ("bbb") and `[3,4]` into `[0,4]` ("bbbb", length 4). Boundary 4 inserted: splits `[4,6]` ("cc", length 2). | `{2, 4}` | `4` |

**Final Output:** `[3, 3, 4]`

---

## Time & Space Complexity

### Current Solution Analysis
- **Time Complexity:** 
  - **Initial construction:** `O(N log N)` where `N` is the length of string `s`. Scanning takes `O(N)` time, and inserting up to `N` boundaries into `std::set` takes `O(N log N)` time.
  - **Per Query:** `O(log N)` where `K` is the number of queries. For each query, `lower_bound`, `insert`, and `erase` operations on `boundaries` and `lengths` take `O(log N)` time.
  - **Total Time Complexity:** `O((N + K) log N)`.

- **Space Complexity:** 
  - `boundaries` set stores at most `N + 1` integers: `O(N)` space.
  - `lengths` set stores at most `N` distinct length values: `O(N)` space.
  - `count` array has size `N + 1`: `O(N)` space.
  - **Total Space Complexity:** `O(N)`.

---

### Is this solution optimal?
Yes, this complexity `O((N + K) log N)` time and `O(N)` space is optimal.

An alternative standard approach uses a **Segment Tree**, which also achieves `O((N + K) log N)` total time complexity.
- In a Segment Tree, each node stores:
  1. `max_len`: maximum repeating character substring length in the node's interval.
  2. `prefix_len`: length of repeating characters starting from the left edge.
  3. `suffix_len`: length of repeating characters ending at the right edge.
  4. `left_char` and `right_char`: the characters at the left and right endpoints.

When merging two child nodes in a Segment Tree, if `left_child.right_char == right_child.left_char`, the combined boundary length is `left_child.suffix_len + right_child.prefix_len`.

**Comparison:**
- Both std::set boundary tracking and Segment Tree run in `O((N + K) log N)` time.
- The `std::pmr::set` approach used in this code has small code overhead and runs extremely fast due to custom memory pooling, matching the theoretical best possible performance.

---

## Edge Cases Handled

1. **Single character string (`N = 1`):** Handled gracefully. `boundaries` contains `{0, 1}`, `lengths` contains `{1}`, and updates work without out-of-bound errors due to index condition checks (`idx > 0` and `idx + 1 < n`).
2. **No-op updates:** If a query updates a character to the exact same character already present, `old != now` evaluates to `false`, avoiding unnecessary updates to the set.
3. **Updates at endpoints (`idx = 0` or `idx = N - 1`):** Guarded by `if (idx > 0)` and `if (idx + 1 < n)`, preventing illegal array lookups outside string bounds.
4. **Duplicate block lengths:** Handled correctly by the `count` vector. If multiple blocks have length `3`, `count[3]` will be greater than 1. The value `3` is only erased from `lengths` when `count[3]` reaches `0`.
5. **Merging multiple blocks at once:** If changing a character connects a block to both its left and right neighbors simultaneously (e.g., changing `'a'` to `'b'` in `"b a b"`), both boundary checks (`idx` and `idx + 1`) trigger sequentially, merging all three sections into one block.
