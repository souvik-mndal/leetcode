![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2052.08%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.20%20MB%20(beats%209.70%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers named `nums` and a single target integer named `target`. Your goal is to find two different numbers in `nums` that add up to `target`, and return their array positions (indices).

You can assume that every input has exactly one correct pair, and you are not allowed to use the same element at the same index twice. You can return the two indices in any order.

For example, if `nums = [2, 7, 11, 15]` and `target = 9`:
- Look for two numbers that sum to `9`.
- `2` (at index `0`) plus `7` (at index `1`) equals `9`.
- The answer is `[0, 1]`.

## Intuition

Checking every possible pair of numbers using two loops works, but it is slow. 

Instead, as we walk through the array, we can figure out what number is missing to reach our `target`. For any number `nums[i]`, the missing complement is `rem = target - nums[i]`. 

If we keep a memory (a map) of numbers we have already seen alongside their indices, we can ask a quick question at each step: *"Have I already seen `rem` earlier in the array?"* 

If yes, we immediately have our pair! If no, we store the current number and its index in our map so a future number can find it.

## Approach

* `map<int,int>mp;`: Creates an ordered map named `mp` that stores key-value pairs, where each key is a number from `nums` and its value is that number's index.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts a loop that iterates through every index `i` of the array `nums` from start to end.
* `int rem = target-nums[i];`: Calculates `rem`, which represents the exact second number needed to add to `nums[i]` to reach `target`.
* `if( mp.find(rem)!= mp.end())`: Checks whether `rem` is already stored in `mp`. If `mp.find(rem)` does not equal `mp.end()`, it means `rem` was seen earlier.
* `return {i,mp[rem]};`: Returns an array containing the current index `i` and the index of `rem` found in `mp`.
* `if( mp.find(rem)== mp.end())`: Checks if `rem` is missing from `mp`.
* `mp[nums[i]]=i;`: Stores the current number `nums[i]` as a key in `mp` with its index `i` as the value so future iterations can find it.
* `return {1,1};`: A fallback return statement required by C++ to satisfy the function's return type, though it will never be reached because the problem guarantees a solution exists.

## Dry Run

### Case 1: Typical case (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `rem` (`9 - nums[i]`) | `mp` state before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 2 | 7 | `{}` | `7` is not in `mp`. Store `mp[2] = 0`. |
| 1 | 7 | 2 | `{2: 0}` | `2` is in `mp`! Return `{1, mp[2]}` which is `{1, 0}`. |

### Case 2: Case with duplicate values (`nums = [3, 3]`, `target = 6`)

| `i` | `nums[i]` | `rem` (`6 - nums[i]`) | `mp` state before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 3 | 3 | `{}` | `3` is not in `mp`. Store `mp[3] = 0`. |
| 1 | 3 | 3 | `{3: 0}` | `3` is in `mp`! Return `{1, mp[3]}` which is `{1, 0}`. |

## Time & Space Complexity

* **Time Complexity:** **O(N log N)** — The loop runs N times. The C++ `std::map` is implemented as a balanced binary search tree (Red-Black tree). Searching and inserting into `std::map` takes O(log N) time per operation.
* **Space Complexity:** **O(N)** — In the worst case, we store up to N elements inside `mp`.

### Can this be improved?

Yes, it can be improved! 

Using `std::map` forces elements to remain sorted, taking O(log N) time for lookups. However, we do not need sorted keys; we only need to look up keys quickly.

By changing `std::map` to `std::unordered_map` (a hash map), lookups and insertions drop from O(log N) to **O(1) average time**.

```cpp
unordered_map<int, int> mp;
for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {mp[rem], i};
    }
    mp[nums[i]] = i;
}
```

- Changing `map` to `unordered_map` switches to hash table lookups.
- Removing the second `if` check cleans up redundant operations.

* **Improved Complexity:** **Time:** O(N) on average, **Space:** O(N).
* **Theoretical Best Possible:** **O(N) time and O(N) space**. We must look at each element at least once to know its value, so O(N) time is optimal. The improved version reaches this theoretical limit.

## Edge Cases Handled

* **Duplicate Numbers:** For inputs like `nums = [3, 3]` and `target = 6`, the code checks `mp.find(rem)` *before* writing `nums[i]` into `mp`. At `i = 1`, it finds the first `3` at index `0` in `mp` without overwriting it.
* **Negative Numbers:** Arithmetic expressions like `rem = target - nums[i]` work seamlessly with negative numbers (e.g., `target = -5`, `nums[i] = -2` yields `rem = -3`).
* **Smallest Array Size:** The constraints state array length is at least 2. The algorithm handles minimal inputs cleanly by adding index 0 on step 1 and matching index 1 on step 2.
