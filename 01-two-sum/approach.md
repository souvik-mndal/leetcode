![Runtime](https://img.shields.io/badge/Runtime-1%20ms%20(beats%2076.77%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.24%20MB%20(beats%208.22%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single goal integer called `target`. Your job is to find two different positions (indices) in `nums` whose values add up to `target`.

Key rules to keep in mind:
* Exactly one pair of numbers will work for every test case.
* You cannot use the number at the exact same position twice.
* You can return the two positions in any order.

**Example:**
* Input: `nums = [2, 7, 11, 15]`, `target = 9`
* Output: `[0, 1]`
* Explanation: The number at index `0` is `2`, and the number at index `1` is `7`. Since `2 + 7 = 9`, we return `[0, 1]`.

---

## Intuition

Checking every pair of numbers using two nested loops works, but it is slow.

Instead of asking, "Do these two numbers add up to target?", ask a smarter question as you look at each number: **"What companion number do I need to reach target?"**

If `target = 9` and your current number is `2`, the companion number you need is `9 - 2 = 7`.

As you step through the array one element at a time, you keep a running memory (a map) of numbers you have already seen and where you saw them. For each new number:
1. Calculate its needed companion (`target - current_number`).
2. Check if that companion is already in your memory.
3. If it is, you found your answer!
4. If it is not, store the current number in memory and move to the next number.

---

## Approach

* `map<int,int>mp;` — Creates a map called `mp` to store numbers we have seen as keys, and their array positions (indices) as values.
* `for( int i=0 ; i<nums.size() ; i++ )` — Loops through `nums` from start to end, using `i` as the current index.
* `int rem = target-nums[i];` — Calculates the missing partner value `rem` (remainder) needed to sum up to `target`.
* `if( mp.find(rem)!= mp.end())` — Searches `mp` to see if `rem` was already saved in a previous step.
* `return {i,mp[rem]};` — Runs if `rem` is found. It returns an array with current index `i` and the companion's saved index `mp[rem]`.
* `if( mp.find(rem)== mp.end())` — Checks if `rem` was **not** found in the map.
* `mp[nums[i]]=i;` — Saves the current number `nums[i]` and its index `i` into `mp` for future checks.
* `return {1,1};` — A fallback return required by C++ so the function always returns a value, though the loop will always find a solution first.

---

## Dry Run

### Case 1: Standard input (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | State of `mp` before step | Action |
|---|---|---|---|---|
| `0` | `2` | `9 - 2 = 7` | `{}` | `7` is not in `mp`. Save `mp[2] = 0`. |
| `1` | `7` | `9 - 7 = 2` | `{2: 0}` | `2` is in `mp` at index `0`! Return `{1, 0}`. |

---

### Case 2: Duplicate numbers (`nums = [3, 3]`, `target = 6`)

| `i` | `nums[i]` | `rem` (`target - nums[i]`) | State of `mp` before step | Action |
|---|---|---|---|---|
| `0` | `3` | `6 - 3 = 3` | `{}` | `3` is not in `mp`. Save `mp[3] = 0`. |
| `1` | `3` | `6 - 3 = 3` | `{3: 0}` | `3` is in `mp` at index `0`! Return `{1, 0}`. |

---

## Time & Space Complexity

* **Time Complexity:** `O(n log n)` — The code loops `n` times. Inside the loop, `std::map` performs lookups and insertions in `O(log n)` time because it uses a balanced search tree under the hood.
* **Space Complexity:** `O(n)` — In the worst case, we store up to `n` numbers inside `mp`.

### Can this be improved?

**Yes.** We can improve the time complexity from `O(n log n)` down to **`O(n)`**.

**Why the change works:**
Standard `std::map` keeps keys ordered, which costs `O(log n)` time per operation. We do not care about ordering; we only care about fast lookups.

By switching from `std::map` to `std::unordered_map` (a hash table), lookups and insertions take **`O(1)` average time** instead of `O(log n)`. Additionally, we can use a simple `else` block instead of calling `find` a second time on line 11.

```cpp
unordered_map<int, int> mp; // Use hash map for O(1) average operations
for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    if (mp.find(rem) != mp.end()) {
        return {i, mp[rem]}; // Companion found
    }
    mp[nums[i]] = i; // Save current number if companion not found
}
```

* `unordered_map<int, int> mp;` — Replaces `map` with a hash table for constant time `O(1)` average lookups.
* We removed the redundant `if (mp.find(rem) == mp.end())` check. If the first `if` fails, we simply insert into `mp`.

**Improved Complexity:**
* **Improved Time:** `O(n)` average time.
* **Improved Space:** `O(n)` space.

**Theoretical Best:** `O(n)` time and `O(n)` space is the absolute best theoretical complexity for an unsorted array. You must inspect every element at least once to find the pair, making `O(n)` time optimal.

---

## Edge Cases Handled

* **Duplicate Numbers in Array:** Handles inputs like `nums = [3, 3]` and `target = 6`. The code checks if the needed partner exists in `mp` *before* adding the current number. This prevents a number from matching with itself.
* **Negative Integers:** Works correctly when `nums` contains negative values or `target` is negative (e.g., `nums = [-3, 4, 3]`, `target = 0`). Arithmetic subtraction handles signs automatically.
* **Smallest Input Size:** Works on arrays of size 2 (the minimum constraint).
* **Large Values:** Numbers can be up to `10^9` or `-10^9`. Standard 32-bit signed C++ `int` types handle subtraction cleanly without integer overflow.
