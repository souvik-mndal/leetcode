![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2093.80%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-21.17%20MB%20(beats%2020.16%25)-orange?style=for-the-badge)

---

## Problem Explained

The problem asks us to find the **k-th smallest number** that can be formed as a positive multiple of any single coin in a given list.

You are given:
* An array of distinct positive integers `coins` representing coin values.
* An integer `k`.

Rules:
* You can pick any single coin denomination and multiply it by any positive integer (1, 2, 3, ...).
* You **cannot** combine different coin denominations together (for example, you cannot add coin 2 and coin 5 to make 7).
* All generated numbers from all coins are merged into one combined list of unique numbers in sorted order.
* We need to find the number at position `k` (1-indexed) in this merged list.

### Small Example
If `coins = [3, 5]` and `k = 4`:
* Coin `3` produces: 3, 6, 9, 12, 15, 18, ...
* Coin `5` produces: 5, 10, 15, 20, ...
* Combined distinct sorted amounts: 3, 5, 6, 9, 10, 12, 15, ...
* The 4th smallest amount is **9**.

---

## Intuition

If someone gives us a target number `x`, can we count how many valid coin amounts exist that are less than or equal to `x`? 

If we have a fast function `count(x)` that returns the number of valid amounts `<= x`, we can use **Binary Search**:
* If `count(x) < k`, then `x` is too small. We must look higher.
* If `count(x) >= k`, then `x` might be our answer, but a smaller `x` could also produce at least `k` amounts. So we try smaller values.

### How to calculate `count(x)`: Inclusion-Exclusion Principle
How many multiples of coin `c` are less than or equal to `x`? Simply `x / c` (using integer division).

If we have multiple coins, just adding `x / c1 + x / c2` counts numbers that are multiples of both coins twice (like 15 for coins 3 and 5). 

To fix double-counting, we use the **Inclusion-Exclusion Principle** (PIE):
* **Add** counts from single coins (e.g., multiples of 3, multiples of 5).
* **Subtract** counts from pairs of coins using their **Least Common Multiple** (LCM) (e.g., multiples of LCM(3, 5) = 15).
* **Add** counts from triplets of coins using their combined LCM.
* **Subtract** counts from quadruplets, and so on.

In short: add subsets with an odd number of coins, subtract subsets with an even number of coins.

### Smart Optimizations
1. **Remove redundant coins**: If coin 6 is in the array alongside coin 3, every multiple of 6 is already a multiple of 3. Coin 6 adds zero new values, so we can toss it out completely.
2. **Small subset count**: After removing redundant coins, the number of coins `m` is at most 15. The total number of subsets is `2^m` (at most 32,768). Because `2^m` is so small, we can precompute the LCM and sign (+1 or -1) for every non-empty subset before doing the binary search.

---

## Approach

Here is the step-by-step breakdown of the code logic:

* `sort(coins.begin(), coins.end());`: Sorts the coins in ascending order so smaller coins come first.
* `vector<long long> useful;` and `if (coin % prev == 0)`: Filters out redundant coins. If a coin is divisible by any smaller coin already stored in `useful`, it is skipped. Otherwise, it is added to `useful`.
* `long long high = useful[0] * 1LL * k;`: Sets the upper bound for binary search. The k-th amount will never exceed `k` times the smallest useful coin.
* `long long low = 1;`: Sets the lower bound for binary search to 1.
* `int m = useful.size();` and `int totalMasks = 1 << m;`: Calculates the number of non-redundant coins `m` and the total number of coin subsets `2^m` using bitmasks.
* `vector<long long> lcms(totalMasks, 1);` and `vector<int> signs(totalMasks, 1);`: Pre-allocates arrays to store the precomputed LCM and inclusion-exclusion sign (+1 for odd subset size, -1 for even) for each subset bitmask.
* `for (int mask = 1; mask < totalMasks; ++mask)`: Loops through every non-empty subset of coins (represented as binary bitmasks).
* `long long g = std::gcd(currentLCM, useful[i]);`: Computes the greatest common divisor to calculate the new running LCM.
* `if (currentLCM > high / useful[i])`: Prevents integer overflow. If the LCM grows larger than `high`, we clamp it to `high + 1` because its count `x / LCM` will just evaluate to 0 anyway.
* `signs[mask] = (bits % 2 == 1) ? 1 : -1;`: Assigns a positive sign (+1) if the subset contains an odd number of coins, or a negative sign (-1) if even.
* `auto count = [&](long long x)`: Defines a helper function that calculates the total unique coin multiples `<= x` by summing `signs[mask] * (x / lcms[mask])` for all subsets.
* `while (low < high)`: Standard binary search loop searching for the smallest value `mid` such that `count(mid) >= k`.
* `return low;`: Returns the smallest number that reaches at least `k` valid amounts.

---

## Dry Run

### Case 1: Redundant coins present (`coins = [3, 6, 9]`, `k = 3`)

1. **Filter Redundant Coins**:
   * Coin 3: kept (`useful = [3]`).
   * Coin 6: `6 % 3 == 0` -> skipped.
   * Coin 9: `9 % 3 == 0` -> skipped.
   * `useful = [3]`, `m = 1`.

2. **Precomputation**:
   * `high = 3 * 3 = 9`, `low = 1`.
   * Mask 1 (`useful[0] = 3`): `lcms[1] = 3`, `signs[1] = +1`.

3. **Binary Search**:

| Step | `low` | `high` | `mid` | `count(mid)` calculation | `count(mid)` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 1 | 9 | 5 | `+ (5 / 3) = 1` | 1 | `1 < 3` -> `low = mid + 1 = 6` |
| 2 | 6 | 9 | 7 | `+ (7 / 3) = 2` | 2 | `2 < 3` -> `low = mid + 1 = 8` |
| 3 | 8 | 9 | 8 | `+ (8 / 3) = 2` | 2 | `2 < 3` -> `low = mid + 1 = 9` |
| 4 | 9 | 9 | - | Loop terminates (`low == high`) | - | Return `low = 9` |

---

### Case 2: Multiple non-redundant coins (`coins = [5, 2]`, `k = 7`)

1. **Filter Redundant Coins**:
   * Sorted `coins = [2, 5]`.
   * Neither divides the other -> `useful = [2, 5]`, `m = 2`.

2. **Precomputation**:
   * `high = 2 * 7 = 14`, `low = 1`.
   * Mask 1 (binary `01`, coin 2): `lcms[1] = 2`, `signs[1] = +1`.
   * Mask 2 (binary `10`, coin 5): `lcms[2] = 5`, `signs[2] = +1`.
   * Mask 3 (binary `11`, coins 2 & 5): `lcms[3] = 10`, `signs[3] = -1`.

3. **Binary Search**:

| Step | `low` | `high` | `mid` | `count(mid)` calculation | `count(mid)` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 1 | 14 | 7 | `(7/2) + (7/5) - (7/10) = 3 + 1 - 0` | 4 | `4 < 7` -> `low = 8` |
| 2 | 8 | 14 | 11 | `(11/2) + (11/5) - (11/10) = 5 + 2 - 1` | 6 | `6 < 7` -> `low = 12` |
| 3 | 12 | 14 | 13 | `(13/2) + (13/5) - (13/10) = 6 + 2 - 1` | 7 | `7 >= 7` -> `high = 13` |
| 4 | 12 | 13 | 12 | `(12/2) + (12/5) - (12/10) = 6 + 2 - 1` | 7 | `7 >= 7` -> `high = 12` |
| 5 | 12 | 12 | - | Loop terminates (`low == high`) | - | Return `low = 12` |

---

## Time & Space Complexity

### Current Solution Analysis
* **Time Complexity:** 
  * Filtering redundant coins: `O(N^2)` where `N` is `coins.size()`.
  * Precomputing LCMs: `O(M * 2^M)` where `M` is the number of useful coins (`M <= N <= 15`).
  * Binary Search: Runs `O(log(useful[0] * k))` iterations. Inside each iteration, `count(x)` iterates over all `2^M` masks.
  * Total Time: `O(N^2 + M * 2^M + 2^M * log(useful[0] * k))`.
  * With `M <= 15`, `2^15 = 32,768` operations per binary search step, and `log2(25 * 2 * 10^9) ≈ 46` steps. Total operations are around `1.5 * 10^6`, which easily passes.

* **Space Complexity:**
  * `O(2^M)` space to store `lcms` and `signs` arrays for all bitmasks.

---

### Can We Optimize It Further?

**Yes, we can optimize both runtime and space!**

#### Why & How:
1. **Filter out useless masks during precomputation**: Many subsets produce an LCM strictly greater than `high`. For these masks, `x / LCM` will always evaluate to `0` throughout the entire binary search. We can skip storing them entirely!
2. **Compact storage**: Instead of allocating fixed vectors of size `2^M` (32,768 elements), we can store only active `(LCM, sign)` pairs in a single dynamic array.
3. **Use built-in bit counting**: Replace the inner bit-counting loop with `__builtin_popcount(mask)` to speed up subset precomputation.

#### Optimized Snippet:
```cpp
struct SubSet {
    long long lcm;
    int sign;
};

// ... inside findKthSmallest ...

vector<SubSet> validSubsets;

for (int mask = 1; mask < totalMasks; ++mask) {
    long long currentLCM = 1;
    bool overflow = false;

    for (int i = 0; i < m; ++i) {
        if (mask & (1 << i)) {
            long long g = std::gcd(currentLCM, useful[i]);
            currentLCM = currentLCM / g;
            if (currentLCM > high / useful[i]) {
                overflow = true;
                break;
            }
            currentLCM *= useful[i];
        }
    }

    if (!overflow && currentLCM <= high) {
        int sign = (__builtin_popcount(mask) % 2 == 1) ? 1 : -1;
        validSubsets.push_back({currentLCM, sign});
    }
}

auto count = [&](long long x) {
    long long result = 0;
    for (const auto& sub : validSubsets) {
        if (sub.lcm <= x) {
            result += sub.sign * (x / sub.lcm);
        }
    }
    return result;
};
```

#### Line-by-Line Connection:
* `struct SubSet`: Packs LCM and sign together to reduce memory overhead and cache misses.
* `if (!overflow && currentLCM <= high)`: Only keeps terms that can actually contribute a non-zero count to `count(x)`.
* `__builtin_popcount(mask)`: Fast CPU instruction to count set bits, eliminating the `bits` variable loop increment.
* `for (const auto& sub : validSubsets)`: Iterates only over useful subsets in `count(x)`, drastically reducing iteration work during binary search.

#### Improved Complexity:
* **Time:** `O(N^2 + M * 2^M + S * log(useful[0] * k))` where `S` is the count of valid subsets whose LCM is `<= high` (`S <= 2^M`).
* **Space:** `O(S)` where `S <= 2^M`, using significantly less memory.

#### Theoretical Limits:
Because the Inclusion-Exclusion Principle requires inspecting subset LCMs to eliminate duplicate counts, `O(2^M)` is the theoretical minimum bound for exact counting on general coin sets without additional mathematical structure. The optimized version reaches this theoretical limit.

---

## Edge Cases Handled

* **Redundant Denominations**: Multiples of smaller coins (e.g., `coins = [2, 4, 8]`) are filtered out early to avoid unnecessary exponential work in subset generation.
* **Large Integer Overflow**: Calculating LCMs of numbers can easily exceed standard 64-bit integers (`long long`). The code guards against this by checking `currentLCM > high / useful[i]` before multiplying.
* **Large `k` Values**: `k` can be up to `2 * 10^9`, producing results up to `5 * 10^10`. The search bounds and counts all use `long long` to prevent arithmetic overflow.
* **Single Coin Input**: Works seamlessly when `coins.size() == 1` (`m = 1`), reducing inclusion-exclusion to a simple division `x / coin`.
* **Coins with No Common Factors**: Works correctly when coin values are pairwise coprime, accurately accounting for products via LCMs.
