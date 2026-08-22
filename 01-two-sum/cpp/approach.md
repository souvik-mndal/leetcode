![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%2043.45%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.00%20MB%20(beats%2025.73%25)-orange?style=for-the-badge)

---

## Problem Explained

You are given a list of numbers called `nums` and a target number called `target`. Your task is to find two numbers inside `nums` that add up to `target`, and return their index positions (their 0-based locations in the array).

You can assume that:
* Every test case has **exactly one** correct answer.
* You cannot use the same element twice (meaning you cannot pick the number at index 0 twice to make the target).

**Example:**
If `nums = [2, 7, 11, 15]` and `target = 9`, the numbers `2` (at index 0) and `7` (at index 1) add up to `9`. The answer is `[0, 1]` or `[1, 0]`.

---

## Intuition

A simple way to solve this is to check every possible pair of numbers using two loops. But that is slow because it checks almost every combination.

Instead, we can do this in a single pass using a map (a key-value store). As we walk through the list:
1. We take the current number and figure out its needed partner: `needed_partner = target - current_number`.
2. We check our map to see if we already saw that `needed_partner` earlier in our walk.
3. If we **have** seen it, we immediately return the current index and the saved index of that partner.
4. If we **have not** seen it, we save the current number and its index in our map so a future number can find it.

Think of it like looking for a dance partner: instead of asking everyone in the room at once, each person enters the room, checks if their ideal partner is already waiting in the lobby, and if not, waits in the lobby for someone else to find them.

---

## Approach

Here is how the code executes line by line:

* `map<int,int>mp;`: Creates an ordered map called `mp`. The key is a number from `nums`, and the value is its index in the array.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts a loop that goes through each element of `nums` from left to right using index `i`.
* `int rem = target-nums[i];`: Calculates `rem` (the remaining amount needed). This is the exact number required to add up to `target` with `nums[i]`.
* `if( mp.find(rem)!= mp.end())`: Checks if `rem` is already present in our map `mp`.
* `return {i,mp[rem]};`: If `rem` exists in `mp`, we found our matching pair. We immediately return an array containing `i` and the stored index `mp[rem]`.
* `if( mp.find(rem)== mp.end())`: Checks if `rem` was not found in the map.
* `mp[nums[i]]=i;`: Stores the current number `nums[i]` as the key and its index `i` as the value in `mp` for future lookups.
* `return {1,1};`: A fallback return required by the function signature, though the problem guarantees a valid pair always exists before the loop ends.

---

## Dry Run

### Case 1: Typical case (`nums = [2, 7, 11, 15]`, `target = 9`)

| Step (`i`) | `nums[i]` | `rem` (`target - nums[i]`) | `mp` state before step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `2` | `9 - 2 = 7` | `{}` | `7` not in `mp`. Add `mp[2] = 0`. |
| `1` | `7` | `9 - 7 = 2` | `{2: 0}` | `2` is in `mp` at index `0`. Return `{1, 0}`. |

---

### Case 2: Out of order numbers (`nums = [3, 2, 4]`, `target = 6`)

| Step (`i`) | `nums[i]` | `rem` (`target - nums[i]`) | `mp` state before step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `0` | `3` | `6 - 3 = 3` | `{}` | `3` not in `mp`. Add `mp[3] = 0`. |
| `1` | `2` | `6 - 2 = 4` | `{3: 0}` | `4` not in `mp`. Add `mp[2] = 1`. |
| `2` | `4` | `6 - 4 = 2` | `{3: 0, 2: 1}` | `2` is in `mp` at index `1`. Return `{2, 1}`. |

---

## Time & Space Complexity

### Current Solution Performance
* **Time:** O(N log N) — The loop runs N times (where N is the number of elements in `nums`). In C++, `std::map` is backed by a balanced search tree (a red-black tree), which takes O(log N) time for lookups and insertions.
* **Space:** O(N) — In the worst case, we store up to N elements inside the map `mp`.

---

### Can this be improved?

**Yes.** We can improve the time complexity from **O(N log N)** down to **O(N)**.

#### How to optimize:
In C++, `std::map` keeps keys sorted, which costs O(log N) time per operation. We do not need our keys to be sorted. If we replace `std::map` with `std::unordered_map` (a hash table), lookups and insertions take **O(1) average time** instead of O(log N).

We can also clean up the second `if` statement because if `rem` was not found in the first check, we already know it is missing.

#### Improved Code Snippet:
```cpp
// Change std::map to std::unordered_map for O(1) average lookup
unordered_map<int, int> mp;

for (int i = 0; i < nums.size(); i++) {
    int rem = target - nums[i];
    
    if (mp.find(rem) != mp.end()) {
        return {mp[rem], i};
    }
    
    // No second if-check needed here
    mp[nums[i]] = i;
}
```

#### Improved Complexity:
* **Time:** O(N) average time — Each hash map lookup and insertion runs in O(1) average time across N elements.
* **Space:** O(N) — Storing up to N elements in the hash table.

#### Theoretical Best:
**O(N) time and O(N) space** is the theoretical best possible complexity for this problem. We must look at each element at least once (O(N) time), and we need a data structure to remember past elements (O(N) space). The improved version reaches this optimal limit.

---

## Edge Cases Handled

* **Duplicate numbers in input (e.g., `nums = [3, 3]`, `target = 6`):** Handled correctly. At index 1, the code checks if `rem` (`6 - 3 = 3`) exists in `mp` *before* inserting the second `3`. It finds the first `3` stored at index 0 and returns `{1, 0}` immediately without overwriting keys.
* **Negative numbers (e.g., `nums = [-3, 4, 3]`, `target = 1`):** Handled correctly. Simple subtraction works with negative numbers (`rem = 1 - (-3) = 4`).
* **Minimum array size (size = 2):** Handled correctly. The loop runs for index 0 (stores it) and index 1 (finds match and returns).
* **Target requires numbers from opposite ends of array:** Handled correctly since the map tracks all previously visited indices regardless of distance.
