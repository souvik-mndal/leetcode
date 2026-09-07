![Runtime](https://img.shields.io/badge/Runtime-41%20ms%20(beats%2054.55%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-127.8%20MB%20(beats%2046.98%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers, like `[2, 3, 4]`. You want to look at every possible pair of numbers in that list. For our example, the pairs are `(2, 3)`, `(2, 4)`, and `(3, 4)`.

For each pair, you find their **GCD** (Greatest Common Divisor), which is the largest number that divides evenly into both numbers in the pair. 
- For `(2, 3)`, the GCD is `1`.
- For `(2, 4)`, the GCD is `2`.
- For `(3, 4)`, the GCD is `1`.

If you collect all these GCD values together, you get a new list: `[1, 2, 1]`. Then, you sort that new list from smallest to largest, giving you `[1, 1, 2]`.

The problem gives you a list of **queries**. Each query is just a number representing an index in your sorted list of GCDs. For example, if a query asks for index `2`, you look at index `2` of `[1, 1, 2]`, which gives you `2`. You need to return the answer for every query in the input.

## Intuition

If you have N numbers, there are about N * N / 2 total pairs. If N is up to 10^5, that means around 5 billion pairs! Listing them all out, finding the GCD of each, and sorting them would take way too long and run out of memory. 

Instead of looking at individual pairs, we flip the problem around: **how many pairs have a GCD that is a multiple of a certain number `d`?**

Instead of working up from pairs, we work down from the maximum possible number in our array using **number theory** (specifically, a sieve-like inclusion-exclusion method):
1. First, we count how many times each number appears in the array using a frequency list.
2. Then, we loop backwards from the largest number down to `1`. For any number `d`, we can easily count how many numbers in our array are multiples of `d`.
3. If we have `m` numbers that are multiples of `d`, then the number of pairs we can form from them is `m * (m - 1) / 2`. 
4. However, some of those pairs will have a GCD that is a multiple of `2d`, `3d`, and so on. By working backwards (from largest `d` down to `1`), we can subtract out the pairs that belong to larger multiples. This leaves us with the exact count of pairs whose GCD is *strictly* a multiple of `d` (or specifically, starts untangling exact counts).
5. Once we know how many pairs have a GCD equal to `d` (or multiples of it), we build a cumulative prefix sum array. This tells us how many total pairs have a GCD **less than or equal to** a given value.
6. Finally, for each query, we use **binary search** (`upper_bound`) on our prefix sum array to instantly find which GCD value corresponds to that query index.

## Approach

* `int mx = ranges::max(nums); vector<int> freq(mx + 1);` -> Finds the largest number in `nums` and creates a frequency array where `freq[v]` stores how many times the number `v` appears in `nums`.
* `for (int v : nums) ++freq[v];` -> Populates the frequency array by looping through every number in `nums` and incrementing its count.
* `vector<long long> g(mx + 1);` -> Creates an array `g` to store the count of pairs for each GCD value.
* `for (int d = mx; d > 0; --d) {` -> Loops backwards from the maximum number down to `1`. Working backwards is crucial because it lets us use the results of larger multiples (like `2d`, `3d`) to fix the counts for `d`.
* `for (int k = d; k <= mx; k += d) { m += freq[k]; g[d] -= g[k]; }` -> Sums up how many numbers in `nums` are multiples of `d` (stored in `m`), and subtracts the pair counts of proper multiples `g[k]` (where `k` is `2d`, `3d`, etc.) to isolate the correct count.
* `g[d] += m * (m - 1) / 2;` -> Calculates all possible pairs that can be formed using the `m` multiples found, adds them to `g[d]` after the subtraction adjustments.
* `vector<long long> s(mx + 1); partial_sum(g.begin(), g.end(), s.begin());` -> Creates a prefix sum array `s`, where `s[d]` holds the cumulative total number of pairs whose GCD is less than or equal to `d`.
* `vector<int> ans(queries.size());` -> Prepares the final answer vector with the same size as the input queries list.
* `ranges::transform(queries, ans.begin(), [&](long long q) { return int(ranges::upper_bound(s, q) - s.begin()); });` -> Uses binary search (`upper_bound`) for each query `q` to find its position in the cumulative sum array `s`, instantly revealing the matching GCD value.

## Dry Run

### Case 1: Typical case
Inputs: `nums = [2, 3, 4]`, `queries = [0, 2, 2]`
`mx` = `4`. `freq` counts: `freq[2]=1`, `freq[3]=1`, `freq[4]=1` (others 0).

| Loop `d` | Multiples checked (`k`) | `m` (multiples count) | `m * (m-1) / 2` | Adjustments (`g[d] -= g[k]`) | Resulting `g[d]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `d = 4` | `4` | `1` (`freq[4]`) | `0` | None | `0` | Only one multiple of 4, zero pairs possible. |
| `d = 3` | `3` | `1` (`freq[3]`) | `0` | None | `0` | Only one multiple of 3, zero pairs possible. |
| `d = 2` | `2, 4` | `2` (`freq[2], freq[4]`) | `1` | `g[2] -= g[4]` (`0`) | `1` | Two multiples of 2, forms 1 pair `(2, 4)`. |
| `d = 1` | `1, 2, 3, 4` | `3` (`freq[1,2,3,4]`) | `3` | Subtract `g[2]` (`1`), `g[3]` (`0`), `g[4]` (`0`) | `2` | All numbers are multiples of 1. Total pairs minus pairs accounted for by higher GCDs leaves 2 pairs with GCD 1. |

After computing `g = [0, 2, 1, 0, 0]`, prefix sum `s` becomes `[0, 2, 3, 3, 3]`.
- Query `0`: `upper_bound` finds index for count `0`, giving GCD `1`.
- Query `2`: `upper_bound` finds index for count `2`, giving GCD `2`.
- Query `2`: gives GCD `2`.
Output: `[1, 2, 2]`.

### Case 2: Edge case with identical values
Inputs: `nums = [2, 2]`, `queries = [0, 0]`
`mx` = `2`. `freq` counts: `freq[2]=2`.

| Loop `d` | Multiples checked (`k`) | `m` (multiples count) | `m * (m-1) / 2` | Adjustments | Resulting `g[d]` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `d = 2` | `2` | `2` (`freq[2]`) | `1` | None | `1` | Two elements equal to 2 form 1 pair with GCD 2. |
| `d = 1` | `1, 2` | `2` (`freq[2]`) | `1` | Subtract `g[2]` (`1`) | `0` | Both elements are multiples of 1, but their pair is already accounted for at GCD 2. |

Prefix sum `s` becomes `[0, 0, 1]`.
- Query `0`: matches index `2`, giving GCD `2`.
- Query `0`: gives GCD `2`.
Output: `[2, 2]`.

## Time & Space Complexity

- **Time:** O(M log M + Q log M), where `M` is the maximum value in `nums` (up to 5 * 10^4) and `Q` is the number of queries (up to 10^5). 
  - Why: The harmonic series loop runs in roughly `M log M` total steps because we visit multiples (`d, 2d, 3d...`). Building the prefix sum takes `O(M)`. Each of the `Q` queries performs a binary search (`upper_bound`) on an array of size `M`, taking `O(log M)` time per query.
- **Space:** O(M + Q)
  - Why: We store arrays (`freq`, `g`, `s`) sized up to the maximum value in `nums` (`mx`), plus space for the output answer vector of size `Q`.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this approach is already extremely efficient and optimal for the given constraints. Because the maximum value of `nums[i]` is capped at `5 * 10^4`, `M` is small enough that `M log M` operations and `Q log M` binary searches easily run well within time limits (executing in a fraction of a second), outperforming any direct pair-generation approach. No further algorithmic improvements are required.

## Edge Cases Handled

- **Duplicate elements:** Handled correctly because the frequency array counts exact occurrences, allowing proper combination math (`m * (m - 1) / 2`) for identical numbers like `[2, 2]`.
- **Minimum constraints (`n = 2`):** Correctly handles the smallest possible array length without crashing or breaking loop bounds.
- **Queries requesting high indices:** Handled safely by `upper_bound` which bounds searches within the prefix sum array limits.
