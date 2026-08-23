![Runtime](https://img.shields.io/badge/Runtime-7%20ms%20(beats%2040.93%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.16%20MB%20(beats%209.69%25)-red?style=for-the-badge)

---

## Problem Explained

You are given a list of integers called `nums` and a single integer called `target`. Your goal is to find **two different positions (indices)** in `nums` whose numbers add up to `target`. 

You must return these two positions as a pair. 

**Rules:**
* Every input will have **exactly one** valid answer.
* You **cannot** use the same element twice (you cannot add a number at index 0 to itself).
* You can return the two indices in any order.

**Example:**
Suppose `nums = [2, 7, 11, 15]` and `target = 9`.
* The number at index 0 is `2`.
* The number at index 1 is `7`.
* `2 + 7 = 9`, which equals `target`.
* Output: `[0, 1]`

---

## Intuition

The naive way to solve this is to check every possible pair of numbers using two loops. That takes a lot of time because you keep re-checking pairs.

Instead, we can flip the problem around:
If we are looking at a number `x`, we know the exact partner number we need. That partner is `target - x`. 

As we walk through the list item by item:
1. We check our "notebook" (a map data structure) to see if we already ran into our needed partner earlier.
2. If we find the partner in our notebook, we are done! We return our current position and the partner's stored position.
3. If we do not find it, we write down our current number and its position in our notebook so future numbers can find us.

---

## Approach

Here is step-by-step how the provided code executes this idea:

* `map<int,int>mp;` — Creates an ordered map (a key-value lookup table) called `mp`. The key is a number from the array, and the value is its index position.
* `for( int i=0 ; i<nums.size() ; i++ )` — Starts a loop that inspects each element in `nums` one by one, using `i` as the current index tracker.
* `int rem = target-nums[i];` — Calculates `rem` (the remaining amount needed). This is the exact number we need to find to make a pair that sums to `target`.
* `if( mp.find(rem)!= mp.end())` — Searches `mp` to see if `rem` was saved during a previous step.
* `return {i,mp[rem]};` — If `rem` is already in `mp`, we immediately stop and return a vector containing the current index `i` and the index of `rem` stored in `mp`.
* `if( mp.find(rem)== mp.end())` — Checks if `rem` was not found in `mp`.
* `mp[nums[i]]=i;` — Saves the current number `nums[i]` and its index `i` into `mp` so it can be looked up by numbers later in the loop.
* `return {1,1};` — Serves as a fallback return statement to satisfy the C++ function requirements. Because the problem guarantees a solution exists, execution never actually reaches this line.

---

## Dry Run

### Case 1: Typical case (`nums = [2, 7, 11, 15]`, `target = 9`)

| `i` | `nums[i]` | `rem` (`9 - nums[i]`) | `mp` state (before check) | Action |
|---|---|---|---|---|
| `0` | `2` | `7` | `{}` | `7` not in `mp`. Store `mp[2] = 0`. |
| `1` | `7` | `2` | `{2: 0}` | `2` is in `mp` at index `0`! Return `{1, 0}`. |

---

### Case 2: Match is not adjacent (`nums = [3, 2, 4]`, `target = 6`)

| `i` | `nums[i]` | `rem` (`6 - nums[i]`) | `mp` state (before check) | Action |
|---|---|---|---|---|
| `0` | `3` | `3` | `{}` | `3` not in `mp`. Store `mp[3] = 0`. |
| `1` | `2` | `4` | `{3: 0}` | `4` not in `mp`. Store `mp[2] = 1`. |
| `2` | `4` | `2` | `{3: 0, 2: 1}` | `2` is in `mp` at index `1`! Return `{2, 1}`. |

---

## Time & Space Complexity

### Current Code Complexity
* **Time Complexity:** **O(n log n)** — The code loops through the array of length `n` once. Inside the loop, it uses C++ `std::map`. A standard `std::map` is built as a balanced binary search tree, making lookups (`find`) and insertions take **O(log n)** time.
* **Space Complexity:** **O(n)** — In the worst case, we store up to `n` elements in `mp`.

---

### Can this be improved?

**Yes.** We can improve time complexity from **O(n log n)** to **O(n) average time**.

#### Why and How
`std::map` keeps keys in sorted order, which forces every lookup to take **O(log n)** time. However, we do not care about sorted order here; we only care about fast lookups. 

If we swap `std::map` for `std::unordered_map` (a **hash table**), lookups and insertions take **O(1)** time on average instead of **O(log n)**. 

Additionally, the original code performs `mp.find(rem)` twice in every iteration. We can search once, save the result in an iterator variable, and check if it reached the end.

#### Improved Code Snippet
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mp; // Use hash table for O(1) average lookups
        for (int i = 0; i < nums.size(); i++) {
            int rem = target - nums[i];
            auto it = mp.find(rem); // Perform lookup once
            if (it != mp.end()) {
                return {i, it->second}; // Found matching index
            }
            mp[nums[i]] = i; // Store seen number and index
        }
        return {};
    }
};
```

* `unordered_map<int, int> mp;` replaces `map` so insertion and lookup run in constant average time **O(1)**.
* `auto it = mp.find(rem);` avoids looking up `rem` twice in the map.

#### Resulting Complexity
* **Time Complexity:** **O(n)** average — Each lookup in `unordered_map` takes **O(1)** average time, done `n` times.
* **Space Complexity:** **O(n)** — We still store up to `n` items in memory.

#### Is this theoretical best?
**Yes.** We must look at each element at least once to find a solution, which requires **O(n)** time. Therefore, **O(n)** time is the optimal time complexity.

---

## Edge Cases Handled

* **Duplicate Numbers in Input (e.g., `nums = [3, 3]`, `target = 6`):** The code checks `mp` for `rem` *before* inserting `nums[i]`. At `i = 1`, it successfully finds the first `3` (stored at `i = 0`) inside `mp` before overwriting it.
* **Negative Numbers (e.g., `nums = [-3, 4, 3, 90]`, `target = 0`):** Simple subtraction `target - nums[i]` works correctly with negative numbers (e.g., `0 - (-3) = 3`).
* **Minimum Sized Array (2 elements):** The loop runs for `i = 0`, adds the element to `mp`, then at `i = 1` immediately finds the complement and returns.
