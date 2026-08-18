![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2066.44%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.18%20MB%20(beats%209.66%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a single target number called `target`. Your goal is to find two different positions (indices) in `nums` whose values add up to `target`.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the answer is `[0, 1]` because the number at index 0 (which is 2) plus the number at index 1 (which is 7) equals 9.

Key rules:
- Every input has exactly one correct pair.
- You cannot use the exact same position twice to make the sum.
- You can return the two positions in any order.

## Intuition

A simple way to solve this is to check every possible pair of numbers. But if the list has many numbers, checking every pair takes too long.

Instead, think of it like this: as you read each number `nums[i]`, you can immediately figure out the exact missing partner number you need. That missing partner is `rem = target - nums[i]`.

For example, if `target` is 9 and you are currently looking at 2, your missing partner is `9 - 2 = 7`.

If you keep a notebook (a map) of every number you have already seen along with its index, you can just open your notebook and ask: *"Have I seen a 7 earlier?"*
- If **yes**, you found your pair right away!
- If **no**, you write down the current number 2 and its index in your notebook, then move to the next number.

This trick lets you find the answer in a single walk through the list.

## Approach

Here is how the code works step-by-step:

* `map<int,int>mp;`: Creates an ordered map named `mp`. It acts as our memory notebook. It stores numbers as keys and their index positions as values.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts a loop that goes through `nums` from left to right, using `i` as the current index.
* `int rem = target-nums[i];`: Calculates `rem` (short for remainder/remaining value), which is the exact partner number needed to reach `target`.
* `if( mp.find(rem)!= mp.end())`: Checks if `rem` is already stored in our map `mp`.
* `return {i,mp[rem]};`: Executes if `rem` was found. It immediately returns the current index `i` and the stored index of `rem` from `mp`.
* `if( mp.find(rem)== mp.end())`: Checks if `rem` was not found in `mp`.
* `mp[nums[i]]=i;`: Saves the current number `nums[i]` into `mp` with its index `i`, so future numbers can look it up.
* `return {1,1};`: Returns a fallback pair if no solution was found (this line is never actually reached because the problem guarantees one solution).

## Dry Run

### Case 1: Standard case (`nums = [2, 7, 11, 15]`, `target = 9`)

| Step (`i`) | `nums[i]` | `rem` (`9 - nums[i]`) | Is `rem` in `mp`? | Action | `mp` state after step |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 2 | 7 | No | Add `2` to `mp` | `{2: 0}` |
| 1 | 7 | 2 | Yes (`mp[2]` is 0) | Return `{1, 0}` | `{2: 0}` |

### Case 2: Out of order numbers (`nums = [3, 2, 4]`, `target = 6`)

| Step (`i`) | `nums[i]` | `rem` (`6 - nums[i]`) | Is `rem` in `mp`? | Action | `mp` state after step |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 3 | 3 | No | Add `3` to `mp` | `{3: 0}` |
| 1 | 2 | 4 | No | Add `2` to `mp` | `{3: 0, 2: 1}` |
| 2 | 4 | 2 | Yes (`mp[2]` is 1) | Return `{2, 1}` | `{3: 0, 2: 1}` |

## Time & Space Complexity

- **Time Complexity:** **O(n log n)** — The loop runs up to `n` times (where `n` is the length of `nums`). Inside the loop, `std::map` lookups and insertions take **O(log n)** time because `std::map` uses a balanced search tree under the hood.
- **Space Complexity:** **O(n)** — In the worst case, we store up to `n` elements in `mp`.

### Can this be improved?

**Yes.** We can improve the time complexity from **O(n log n)** to **O(n)**.

The `std::map` container in C++ keeps keys sorted, which causes **O(log n)** search times. We do not need the keys to be sorted. Switching to `std::unordered_map` uses a hash table instead. A hash table provides **O(1)** average lookup and insertion times.

We can also simplify the code by replacing the second `if` check with a clean `else` pattern to avoid searching the map twice.

```cpp
unordered_map<int, int> mp; // Using hash map for O(1) average lookup
for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {i, mp[rem]}; // Found partner, return indices
    }
    mp[nums[i]] = i; // Save current number and index
}
```

- **Improved Time Complexity:** **O(n)** average — Each loop step now takes O(1) time on average.
- **Improved Space Complexity:** **O(n)** — We still store up to `n` elements.
- **Theoretical Best Complexity:** **O(n)** time and **O(n)** space. We must inspect each element at least once, so we cannot do better than O(n) time. The improved code reaches this optimal limit.

## Edge Cases Handled

- **Duplicate numbers (e.g., `nums = [3, 3]`, `target = 6`):** The code works correctly. For the second `3`, `rem` is `3`. It finds the first `3` in `mp` before it overwrites `mp[3]`, returning indices `{1, 0}`.
- **Negative numbers (e.g., `nums = [-3, 4, 3]`, `target = 0`):** Subtraction handles negative values naturally. When `nums[i] = 3`, `rem = 0 - 3 = -3`, which finds `-3` in `mp`.
- **Minimum array size (`nums.length = 2`):** Works correctly on the smallest valid input size without out-of-bounds errors.
- **Self-matching prevention:** Because we check `mp.find(rem)` *before* adding `mp[nums[i]] = i`, an element can never match with itself.
