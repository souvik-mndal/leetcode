![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2052.16%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.13%20MB%20(beats%209.69%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single goal number called `target`. Your job is to find two different numbers in `nums` that add up to `target`. 

Instead of returning the numbers themselves, you must return their positions in the list (their indices). 

You can assume there is always exactly one correct pair, and you cannot use the same element twice (meaning you cannot pick the number at index 0 twice to make the target).

For example, if `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` and `7` add up to `9`. Their positions in the list are `0` and `1`, so you return `[0, 1]`.

## Intuition

Imagine you are looking for a partner to complete a task. If the total needed score is `9`, and you hold the number `2`, you know right away that you need a partner holding `7` (because `9 - 2 = 7`). 

Instead of searching the whole list back and forth for every number, you can walk through the list once while keeping a notepad. 

At each step:
1. Calculate the exact number you need to reach `target` (`rem = target - nums[i]`).
2. Check your notepad to see if you have already walked past `rem`.
3. If you see `rem` in your notepad, you are done! You found both numbers.
4. If you do not see `rem`, write your current number and its position in your notepad, then move to the next number.

## Approach

* `map<int,int>mp;` — Creates an ordered map (a lookup structure) named `mp` to store numbers we have seen as keys and their array positions as values.
* `for( int i=0 ; i<nums.size() ; i++ )` — Loops through the `nums` array one element at a time, keeping track of the current index `i`.
* `int rem = target-nums[i];` — Calculates `rem` (short for remainder), which is the complement value needed to reach `target` when added to `nums[i]`.
* `if( mp.find(rem)!= mp.end())` — Checks if `rem` is already stored in `mp`, meaning we already processed its matching partner earlier.
* `return {i,mp[rem]};` — If `rem` is found in `mp`, returns the current index `i` along with the stored index of `rem` from the map as the final answer.
* `if( mp.find(rem)== mp.end())` — Checks if `rem` was not found in the map.
* `mp[nums[i]]=i;` — Saves the current number `nums[i]` and its index `i` into `mp` so future numbers can look it up.
* `return {1,1};` — Provides a fallback return value required by C++ syntax, though the problem guarantees a solution always exists before reaching here.

## Dry Run

### Case 1: Typical case (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `rem` (`9 - nums[i]`) | Map `mp` before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 2 | 7 | `{}` | 7 is not in `mp`. Store `mp[2] = 0`. |
| 1 | 7 | 2 | `{2: 0}` | 2 is found in `mp` at index 0! Return `{1, 0}`. |

### Case 2: Matching numbers further in array (`nums = [3, 2, 4]`, `target = 6`)

| `i` | `nums[i]` | `rem` (`6 - nums[i]`) | Map `mp` before step | Action |
| --- | --- | --- | --- | --- |
| 0 | 3 | 3 | `{}` | 3 is not in `mp`. Store `mp[3] = 0`. |
| 1 | 2 | 4 | `{3: 0}` | 4 is not in `mp`. Store `mp[2] = 1`. |
| 2 | 4 | 2 | `{3: 0, 2: 1}` | 2 is found in `mp` at index 1! Return `{2, 1}`. |

## Time & Space Complexity

* **Time Complexity:** O(n log n) — The loop runs up to `n` times. Inside the loop, looking up or inserting into `std::map` (a red-black tree) takes O(log n) time.
* **Space Complexity:** O(n) — In the worst case, we store up to `n` elements in `mp`.

### Can it be improved?

Yes, it can be improved. 

Right now, the code uses `std::map`, which keeps keys sorted using a balanced search tree. Tree operations take O(log n) time. 

We do not need the keys to be sorted. We only care about looking up values instantly. By switching to `std::unordered_map` (which uses a hash table), key lookups and insertions take O(1) average time instead of O(log n).

We can also remove the redundant second check `if( mp.find(rem)== mp.end())` by using a simple `else` or placing the insertion after the first `if` check.

```cpp
unordered_map<int, int> mp;
for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {i, mp[rem]};
    }
    mp[nums[i]] = i;
}
```

* Line 1 uses `unordered_map` for fast hash table lookups.
* Line 4-6 checks if the complement `rem` exists in average O(1) time.
* Line 7 runs directly if `rem` was not found, inserting `nums[i]` into `mp` without performing a second search.

* **Improved Time Complexity:** O(n) average time.
* **Improved Space Complexity:** O(n).
* **Theoretical Best Complexity:** O(n) time is the absolute best possible because we must inspect every number at least once in the worst case to find the solution. The improved hash map approach achieves this optimal bound.

## Edge Cases Handled

* **Duplicate Values (`nums = [3, 3]`, `target = 6`):** The code safely handles duplicate values. When evaluating the second `3`, the first `3` is already stored in `mp`, allowing it to find the pair correctly without overwriting issues.
* **Negative Numbers (`nums = [-3, 4, 3, 90]`, `target = 0`):** Works seamlessly because simple arithmetic (`target - nums[i]`) correctly calculates negative complements (e.g., `0 - (-3) = 3`).
* **Minimum Array Length (`nums.length = 2`):** Works correctly on the smallest valid array size allowed by the constraints.
* **Solution at the Ends:** Works whether the target pair is at the beginning, middle, or end of the array.
