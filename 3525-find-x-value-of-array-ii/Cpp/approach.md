![Runtime](https://img.shields.io/badge/Runtime-396%20ms%20(beats%2044.94%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-304.22%20MB%20(beats%2026.97%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums` and a number `k` (where `k` is between 1 and 5). 

You are also given a list of `queries`. Each query gives you four numbers: `[index_i, value_i, start_i, x_i]`.

For each query, you must perform two main steps:
1. **Update:** Change the element at index `index_i` in `nums` to `value_i`. This change stays permanently for all future queries.
2. **Count:** Focus only on the slice of the array starting at index `start_i` up to the end of the array. Within this slice, count how many prefixes (contiguous subsegments that start at index `start_i` and extend to some index `j`) have an overall product whose remainder when divided by `k` equals `x_i`.

### Small Example
Suppose `nums = [1, 2, 3]`, `k = 3`, and a query is `[1, 4, 0, 1]`.
* **Step 1 (Update):** Change `nums[1]` to `4`. The array becomes `[1, 4, 3]`.
* **Step 2 (Count):** Start at index `0` and look at all prefixes starting from index `0`:
  * Prefix `[1]`: Product is 1. Remainder when divided by 3 is `1 % 3 = 1`. (Matches `x = 1`)
  * Prefix `[1, 4]`: Product is 4. Remainder when divided by 3 is `4 % 3 = 1`. (Matches `x = 1`)
  * Prefix `[1, 4, 3]`: Product is 12. Remainder when divided by 3 is `12 % 3 = 0`. (Does not match)

There are 2 matching prefixes, so the answer for this query is `2`.

---

## Intuition

Calculating prefix products from scratch for every query would be too slow. Because the array elements keep changing, we need a data structure that can update values quickly and answer subrange questions quickly. A **Segment Tree** fits this purpose.

The key trick comes from the constraint that `k` is tiny (`1 <= k <= 5`).

Instead of calculating product values (which would grow huge and overflow), we only care about remainders modulo `k`. Since `k` is at most 5, there are only 5 possible remainders: `0, 1, 2, 3, 4`.

Each node in our Segment Tree represents a subsegment of `nums`. Each node keeps track of:
1. `prod`: The product of all numbers in this segment, modulo `k`.
2. `freq`: An array of size 5, where `freq[r]` tells us how many prefixes inside this segment have a product remainder of `r`.

When combining a left child node `L` and a right child node `R`:
* Every valid prefix inside `L` is already counted in `L.freq`.
* Every prefix that extends into `R` has its total product equal to `(L.prod * remainder_in_R) % k`. So, a prefix in `R` that had remainder `r` will shift to a new remainder `(L.prod * r) % k` when attached after `L`.

This allows us to merge two adjacent segments in `O(k)` time.

---

## Approach

Here is how the code works step-by-step:

* `struct Node { int prod; array<int, 5> freq; ... }`: Defines a segment tree node holding the total segment product modulo `k` (`prod`) and an array (`freq`) counting how many prefixes in this node's segment give each remainder from 0 to 4.
* `Node merge(const Node& L, const Node& R)`: Combines two adjacent tree nodes `L` (left) and `R` (right) into one merged result node.
* `res.prod = (1LL * L.prod * R.prod) % k;`: Calculates the combined product modulo `k` for the full merged segment.
* `res.freq = L.freq;`: Copies all prefix remainder counts from `L` directly into the merged node because prefixes ending inside `L` don't change.
* `int nr = (1LL * L.prod * r) % k;`: For each remainder `r` present in `R`, calculates the new combined remainder `nr` when that prefix in `R` is preceded by the full block `L`.
* `res.freq[nr] += R.freq[r];`: Adds the counts from `R.freq[r]` into the new combined remainder index `nr`.
* `void build(int v, int tl, int tr)`: Recursively builds the initial segment tree over the array `nums`. At a leaf (`tl == tr`), sets `prod = nums[tl] % k` and marks `freq[prod] = 1`.
* `void update(int v, int tl, int tr, int pos, int val)`: Recursively updates the single position `pos` to `val` in `O(log n)` time and updates all parent nodes on the path back up using `merge`.
* `Node query(int v, int tl, int tr, int l, int r)`: Returns a combined `Node` representing the range `[l, r]` by merging relevant tree segments.
* `ans.push_back(res.freq[x]);`: Executes the update for each query, queries the suffix range from `start` to `n - 1`, and retrieves the count of prefixes giving remainder `x`.

---

## Dry Run

### Case 1: Typical Query with k = 3
`nums = [1, 2, 3, 4, 5]`, `k = 3`. 
Query: `[2, 2, 0, 2]` (Update index 2 to value 2, then query range `[0, 4]` for remainder `x = 2`).

| Step / Action | `nums` state | Segment Tree Range `[0, 4]` Node (`prod`, `freq`) | Answer |
| :--- | :--- | :--- | :--- |
| Initial state | `[1, 2, 3, 4, 5]` | Built from initial elements | - |
| Update `idx = 2` to `val = 2` | `[1, 2, 2, 4, 5]` | Leaves updated; merged root recomputed | - |
| Leaf `[0,0]` (val 1) | `[1]` | `prod = 1`, `freq = [0: 0, 1: 1, 2: 0]` | - |
| Leaf `[1,1]` (val 2) | `[2]` | `prod = 2`, `freq = [0: 0, 1: 0, 2: 1]` | - |
| Merge `[0,0]` and `[1,1]` | `[1, 2]` | `prod = 2`, `freq = [0: 0, 1: 1, 2: 1]` | - |
| Full range `[0,4]` merged | `[1, 2, 2, 4, 5]` | `prod = 2`, `freq = [0: 0, 1: 3, 2: 2]` | - |
| Lookup `res.freq[2]` | `[1, 2, 2, 4, 5]` | Reads index 2 from `freq` array | **2** |

*Explanation of `freq = [0: 0, 1: 3, 2: 2]` for range `[0,4]`*:
* Prefix `[1]` -> product 1 % 3 = 1
* Prefix `[1, 2]` -> product 2 % 3 = 2
* Prefix `[1, 2, 2]` -> product 4 % 3 = 1
* Prefix `[1, 2, 2, 4]` -> product 16 % 3 = 1
* Prefix `[1, 2, 2, 4, 5]` -> product 80 % 3 = 2
Counts for remainders (0, 1, 2) are 0, 3, and 2 respectively.

---

### Case 2: Suffix Query starting at non-zero index
`nums` is now `[1, 2, 2, 4, 5]`, `k = 3`. 
Query: `[3, 3, 3, 0]` (Update index 3 to value 3, then query range `[3, 4]` starting at index `start = 3` for remainder `x = 0`).

| Step / Action | `nums` state | Target Range `[3, 4]` Node (`prod`, `freq`) | Answer |
| :--- | :--- | :--- | :--- |
| Update `idx = 3` to `val = 3` | `[1, 2, 2, 3, 5]` | Segment tree updated | - |
| Leaf `[3,3]` (val 3) | `[3]` | `prod = 0`, `freq = [0: 1, 1: 0, 2: 0]` | - |
| Leaf `[4,4]` (val 5) | `[5]` | `prod = 2`, `freq = [0: 0, 1: 0, 2: 1]` | - |
| Merge `[3,3]` and `[4,4]` | `[3, 5]` | `prod = 0`, `freq = [0: 2, 1: 0, 2: 0]` | - |
| Lookup `res.freq[0]` | `[3, 5]` | Reads index 0 from `freq` array | **2** |

*Explanation of `freq` for range `[3, 4]`*:
* Prefix starting at 3: `[3]` -> product 3 % 3 = 0
* Prefix starting at 3: `[3, 5]` -> product 15 % 3 = 0
Counts for remainders (0, 1, 2) are 2, 0, and 0 respectively.

---

## Time & Space Complexity

* **Time Complexity:** 
  * **Building Tree:** `O(n * k)` to construct the segment tree initially.
  * **Per Query Update:** `O(k * log n)` to traverse tree height `log n` and merge nodes of size `k`.
  * **Per Query Range Query:** `O(k * log n)` to collect and merge subranges.
  * **Total Time:** `O(n * k + Q * k * log n)` where `n` is array length, `Q` is number of queries, and `k <= 5`.
* **Space Complexity:** 
  * `O(n * k)` to store the segment tree of size `4 * n`, where each node stores a fixed array of size 5.

### Is this optimal or can it be improved?

The time complexity `O(n + Q * log n)` (since `k <= 5` is a constant) is **already optimal** for dynamic point updates and range queries.

However, we can optimize the **constant factor and memory usage** significantly:
1. **Iterative Segment Tree / Fenwick Tree:** We can use an 0-indexed flat array segment tree (size `2 * n`) instead of recursive dynamic calls. This removes recursive overhead and cuts memory by half.
2. **Pass-by-value / Pass-by-reference in merge:** Avoiding unnecessary node copying during stack returns.

Here is a short code snippet showing the optimized iterative merge approach:

```cpp
// Flat iterative segment tree node update
void update_iterative(int pos, int val) {
    pos += n;
    tree[pos].prod = val % k;
    tree[pos].freq.fill(0);
    tree[pos].freq[tree[pos].prod] = 1;
    for (pos /= 2; pos > 0; pos /= 2) {
        tree[pos] = merge(tree[2 * pos], tree[2 * pos + 1]);
    }
}
```

* **Why this helps:** Iterative updates do not make recursive function calls and use exactly `2 * n` array nodes instead of `4 * n`.

* **Resulting Complexity:** Time remains `O((n + Q * log n) * k)`, but runs roughly 2x-3x faster in practice. Space decreases from `4 * n * k` to `2 * n * k`.

---

## Edge Cases Handled

* **`start_i = n - 1` (Last Element):** The query asks for the range `[n - 1, n - 1]`. The segment tree returns a leaf node directly without extra merges.
* **`x_i = 0` (Product Divisible by `k`):** Correctly tracked whenever a prefix contains a number divisible by `k` or numbers whose product modulo `k` equals 0.
* **Large Input Values (`value_i <= 10^9`):** Safe against integer overflow because all node operations take values modulo `k` immediately (`val % k`) and use `1LL *` explicit 64-bit multiplication casting before taking `% k`.
* **Single Element Array (`n = 1`):** Tree height is 1, build and updates execute cleanly on index 0 without boundary errors.
* **`k = 1`:** Every number modulo 1 is 0. The code correctly records all prefix products under remainder index 0.
