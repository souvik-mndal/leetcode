![Runtime](https://img.shields.io/badge/Runtime-155%20ms%20(beats%2048.30%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-262.6%20MB%20(beats%2053.49%25)-yellow?style=for-the-badge)

---

## Problem Explained

You are given a binary string `s` made up of `'0'`s (inactive sections) and `'1'`s (active sections). You are also given a list of `queries`, where each query is a range `[l, r]` specifying a substring `s[l...r]`.

For each query, you need to find the maximum possible number of `'1'`s in the entire string after performing at most one **trade** on the substring `s[l...r]`.

To perform a trade on `s[l...r]`:
1. Imagine placing a temporary `'1'` right before index `l` and another temporary `'1'` right after index `r` (this is called augmenting the substring).
2. Choose a block of consecutive `'1'`s that is completely surrounded by `'0'`s inside this range, and change all those `'1'`s to `'0'`s.
3. Then, choose a block of consecutive `'0'`s that is now surrounded by `'1'`s, and change all those `'0'`s to `'1'`s.
4. Remove the temporary `'1'`s added at the ends.

Each query is independent and does not permanently change `s`. You must return an array where each element is the answer for the corresponding query.

**Example:**
If `s = "0100"` and a query asks about range `[0, 3]`:
* The substring is `"0100"`. Augmenting it with `'1'` at both ends gives `"101001"`.
* The middle `'1'` is surrounded by `'0'`s. Turning it to `'0'` gives `"100001"`.
* Now, the four `'0'`s in the middle are surrounded by `'1'`s. Turning them all to `'1'` gives `"111111"`.
* Removing the augmented ends leaves `"1111"`, which has 4 active sections (`'1'`s).

---

## Intuition

What does a trade actually accomplish?

Suppose you have two blocks of `'0'`s separated by a block of `'1'`s, like `00 1 000`.
1. When you turn the middle `'1'` into `'0'`, the two zero-blocks merge into one big block of `'0'`s (`00000`).
2. Because this giant zero-block is now flanked by `'1'`s (or augmented `'1'`s at the query edges), the trade turns the entire combined block into `'1'`s (`11111`).

Notice what happened:
* The middle `'1'` started as `'1'`, briefly became `'0'`, and ended up as `'1'`. Its count did not change.
* The `'0'`s in the left zero-block and right zero-block ALL became `'1'`s!

This gives us the key realization: **trading at a block of `'1'`s between two zero-blocks increases the total count of `'1'`s by the sum of the lengths of those two zero-blocks.**

We can call the region of `'1'`s between two zero-blocks a **valley**. Trading at valley `j` gives a gain equal to `length(zero-block j) + length(zero-block j + 1)`.

When a query restricts us to a range `[l, r]`:
* Zero-blocks that extend past `l` on the left or `r` on the right get clipped (trimmed) by the boundary.
* The boundary valleys (the first and last usable ones in the range) might suffer partial clipping.
* Any valleys completely inside `[l, r]` experience no clipping at all.

To answer each query quickly:
1. Find the first and last usable valley in `[l, r]`.
2. Compute their clipped gains.
3. For all valleys fully inside `[l, r]`, quickly find the maximum unclipped gain using a **Sparse Table** (a structure that answers range maximum queries in constant time).

---

## Approach

Here is how the C++ solution works step-by-step:

* `int ones = ranges::count(s, '1');`: Counts the initial number of `'1'`s in `s`. Any gain from a trade will be added to this baseline count.
* `vector<int> zs, ze;`: Declares two vectors to store the starting index (`zs`) and ending index (`ze`) of every maximal block of `'0'`s in `s`.
* `for (int i = 0; i < n; ) { ... }`: Loops through `s` to find all zero-blocks, recording their start position `i` in `zs` and end position `j - 1` in `ze`.
* `int nblocks = zs.size();`: Stores the total number of zero-blocks found in `s`.
* `vector<int> V;`: Stores the potential gain for each valley `j`. Valley `j` sits between zero-block `j` and zero-block `j + 1`.
* `V.push_back((ze[j] - zs[j] + 1) + (ze[j + 1] - zs[j + 1] + 1));`: Fills `V[j]` with the sum of lengths of zero-block `j` and zero-block `j + 1`.
* `vector<vector<int>> sparse{V}; ...`: Builds a Sparse Table over `V`. A sparse table precomputes range maximums so we can look up the maximum value in any range in O(1) constant time.
* `auto rmq = [&](int lo, int hi) { ... };`: A helper lambda that returns the maximum value in `V[lo...hi]` in O(1) time using the precomputed Sparse Table.
* `auto clip = [&](int j, int l, int r) { ... };`: A helper lambda that calculates valley `j`'s gain clipped to range `[l, r]`. It takes the full gain `V[j]` and subtracts any part of zero-block `j` before index `l` or zero-block `j + 1` after index `r`.
* `auto gain = [&](int l, int r) { ... };`: A helper lambda that calculates the best gain achievable inside query range `[l, r]`:
  * `int ja = ranges::lower_bound(ze, l) - ze.begin();`: Finds `ja`, the index of the first valley whose left zero-block ends at or after index `l`.
  * `int jb = ranges::upper_bound(zs, r) - zs.begin() - 2;`: Finds `jb`, the index of the last valley whose right zero-block starts at or before index `r`.
  * `if (ja > jb) return 0;`: If `ja > jb`, no valid trade can be performed inside `[l, r]`, so gain is 0.
  * `return max({clip(ja, l, r), clip(jb, l, r), jb - ja >= 2 ? rmq(ja + 1, jb - 1) : 0});`: Evaluates the clipped gains at boundary valleys `ja` and `jb`, and uses `rmq` to check all unclipped valleys strictly between them. Returns the largest gain.
* `for (auto& q : queries) ans.push_back(ones + gain(q[0], q[1]));`: Computes the final answer for each query by adding the best gain to `ones` and returns `ans`.

---

## Dry Run

### Case 1: `s = "0100"`, `queries = [[0, 3], [0, 2]]`

Initial state:
* `ones` = 1
* Zero-blocks: Block 0 (`s[0...0]`, `zs[0]=0, ze[0]=0`), Block 1 (`s[2...3]`, `zs[1]=2, ze[1]=3`)
* `nblocks` = 2
* Valley array `V`: `V[0] = (0 - 0 + 1) + (3 - 2 + 1) = 1 + 2 = 3`. `V = [3]`

| Query | Step / Operation | Variables Tracked | Action / Result |
|---|---|---|---|
| `[0, 3]` | Find usable valley range | `l = 0, r = 3` | `ja = lower_bound(ze, 0) = 0`. `jb = upper_bound(zs, 3) - 2 = 2 - 2 = 0`. Usable range is `j = 0`. |
| `[0, 3]` | Evaluate gain | `ja = 0, jb = 0` | `clip(0, 0, 3) = 3 - max(0, 0-0) - max(0, 3-3) = 3`. Middle RMQ not needed (`jb - ja < 2`). `gain = 3`. |
| `[0, 3]` | Compute answer | `ones = 1, gain = 3` | Total = `1 + 3 = 4`. Output `4`. |
| `[0, 2]` | Find usable valley range | `l = 0, r = 2` | `ja = 0`. `jb = upper_bound(zs, 2) - 2 = 2 - 2 = 0`. |
| `[0, 2]` | Evaluate gain | `ja = 0, jb = 0` | `clip(0, 0, 2) = 3 - max(0, 0-0) - max(0, ze[1] - 2) = 3 - 0 - (3 - 2) = 2`. `gain = 2`. |
| `[0, 2]` | Compute answer | `ones = 1, gain = 2` | Total = `1 + 2 = 3`. Output `3`. |

### Case 2: `s = "01010"`, `queries = [[0, 3], [1, 3]]`

Initial state:
* `ones` = 2 (`'1'` at indices 1 and 3)
* Zero-blocks: Block 0 (`[0, 0]`), Block 1 (`[2, 2]`), Block 2 (`[4, 4]`)
* `zs = [0, 2, 4]`, `ze = [0, 2, 4]`, `nblocks` = 3
* Valley array `V`: `V[0] = 1 + 1 = 2`, `V[1] = 1 + 1 = 2`. `V = [2, 2]`

| Query | Step / Operation | Variables Tracked | Action / Result |
|---|---|---|---|
| `[0, 3]` | Find usable valley range | `l = 0, r = 3` | `ja = lower_bound(ze, 0) = 0`. `jb = upper_bound(zs, 3) - 2 = 2 - 2 = 0`. |
| `[0, 3]` | Evaluate gain | `ja = 0, jb = 0` | `clip(0, 0, 3) = 2 - max(0, 0-0) - max(0, ze[1]-3) = 2 - 0 - 0 = 2`. `gain = 2`. |
| `[0, 3]` | Compute answer | `ones = 2, gain = 2` | Total = `2 + 2 = 4`. Output `4`. |
| `[1, 3]` | Find usable valley range | `l = 1, r = 3` | `ja = lower_bound(ze, 1) = 1` (since `ze[1] = 2 >= 1`). `jb = upper_bound(zs, 3) - 2 = 0`. |
| `[1, 3]` | Check validity | `ja = 1, jb = 0` | `ja > jb` (1 > 0). No valley fits inside query range `[1, 3]`. `gain = 0`. |
| `[1, 3]` | Compute answer | `ones = 2, gain = 0` | Total = `2 + 0 = 2`. Output `2`. |

---

## Time & Space Complexity

### Current Solution Complexity
* **Time Complexity:**
  * **Preprocessing:** $O(N \log N)$ where $N$ is the length of string `s`. Finding zero-blocks takes $O(N)$ time. Building the Sparse Table over array `V` of size at most $N$ takes $O(N \log N)$ time.
  * **Per Query:** $O(\log N)$ time. Binary searching `zs` and `ze` takes $O(\log N)$ time. Looking up the range maximum using the Sparse Table (`rmq`) takes $O(1)$ constant time.
  * **Total Time:** $O(N \log N + Q \log N)$ where $Q$ is the number of queries.
* **Space Complexity:**
  * **Auxiliary Space:** $O(N \log N)$ space for storing the Sparse Table 2D vector, plus $O(N)$ for vectors `zs`, `ze`, and `V`.

---

### Can this be improved?

**Yes, the auxiliary space can be improved from $O(N \log N)$ down to $O(N)$.**

#### Why and How
Notice that in each query, we already spend $O(\log N)$ time performing binary searches (`lower_bound` / `upper_bound`) to find `ja` and `jb`. 

Using a **Sparse Table** gives $O(1)$ query time for range maximums, but costs $O(N \log N)$ space. If we replace the Sparse Table with a standard **Segment Tree**:
1. Building a Segment Tree takes only $O(N)$ space and $O(N)$ time.
2. Querying a range maximum in a Segment Tree takes $O(\log N)$ time.

Since $O(\log N)$ for the Segment Tree query combines with the $O(\log N)$ binary search, the per-query time remains $O(\log N)$, but the auxiliary space drops from $O(N \log N)$ to $O(N)$.

#### Code Snippet (Segment Tree Replacement)

```cpp
// Replaces the Sparse Table setup and rmq lambda
struct SegTree {
    int n;
    vector<int> tree;
    SegTree(const vector<int>& V) {
        n = V.size();
        tree.resize(4 * n + 1, 0);
        if (n > 0) build(V, 1, 0, n - 1);
    }
    void build(const vector<int>& V, int node, int l, int r) {
        if (l == r) { tree[node] = V[l]; return; }
        int mid = l + (r - l) / 2;
        build(V, 2 * node, l, mid);
        build(V, 2 * node + 1, mid + 1, r);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }
    int query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = l + (r - l) / 2;
        return max(query(2 * node, l, mid, ql, qr), query(2 * node + 1, mid + 1, r, ql, qr));
    }
    int query(int ql, int qr) { return (ql <= qr && ql >= 0 && qr < n) ? query(1, 0, n - 1, ql, qr) : 0; }
};
```

#### Improved Complexity
* **Improved Time:** $O(N + Q \log N)$ total time.
* **Improved Space:** $O(N)$ auxiliary space.

#### Theoretical Best Complexity
* **Theoretical Best Time:** $O(N + Q)$ time (linear preprocessing and $O(1)$ per query) using direct index mapping to avoid binary search, combined with an $O(1)$ RMQ data structure like a Disjoint Sparse Table.
* **Theoretical Best Space:** $O(N)$ space.
* The Segment Tree modification achieves the optimal $O(N)$ auxiliary space bound.

---

## Edge Cases Handled

* **String with no `'0'`s (e.g., `s = "111"`):** No zero-blocks exist (`nblocks = 0`). The helper `gain` returns 0 immediately, correctly leaving the active count as initial `ones = 3`.
* **String with no `'1'`s (e.g., `s = "000"`):** Initial `ones = 0` and `nblocks = 1`. Since there are fewer than 2 zero-blocks (`nblocks < 2`), no valley exists. `gain` returns 0, giving the correct total 0.
* **No valid valley in query range (`ja > jb`):** Occurs when the query range `[l, r]` is too narrow to contain a block of `'1'`s flanked by zero-blocks. Handled explicitly by checking `if (ja > jb) return 0;`.
* **Query boundary cuts through a zero-block:** Handled by the `clip` lambda, which subtracts the portion of the zero-block that falls outside `[l, r]`.
* **Queries spanning the entire string `[0, n-1]`:** Handled naturally because the augmented `'1'`s outside indices 0 and `n-1` allow boundary zero-blocks to be fully converted into `'1'`s.
