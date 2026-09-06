![Runtime](https://img.shields.io/badge/Runtime-144%20ms%20(beats%208.49%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-116.3%20MB%20(beats%205.05%25)-red?style=for-the-badge)

---

## Problem Explained

We are given a list of whole numbers called `nums`. Our job is to look through this list and figure out if any number appears more than once. 

- If we find **any duplicate**, we must return `true`.
- If **every single number is completely unique**, we must return `false`.

For example, if our list is `[1, 2, 3, 1]`, the number `1` shows up twice (at the very beginning and at the very end). Because of that, the answer is `true`. But if our list is `[1, 2, 3, 4]`, every number is different, so the answer is `false`.

## Intuition

The core trick here is using a special container called a **set** (specifically `std::set` in C++). A set is a collection that automatically throws away any duplicate values you try to put into it. 

If you hand a set a pile of numbers that includes repeats, it will quietly keep only the unique ones. Because of this rule, we can just dump every number from our input list into a set. Once we are done, we can compare how many items we started with (`nums.size()`) against how many items the set kept (`st.size()`). If the set is smaller than our original list, it means some numbers were duplicates and got squashed. If both sizes are an exact match, every number was unique to begin with.

This is a great memory-jog for a returning programmer: grab a set, throw everything in, and check if the size shrank.

## Approach

Here is how the code builds and executes this logic step-by-step:

- `set<int>st;` — Creates an empty **set** named `st` that will hold integer values and automatically filter out any duplicates.
- `for( auto i:nums ){` — Starts a loop that looks at every single number in the `nums` list one by one, temporarily naming the current number `i`.
- `st.insert( i );` — Takes the current number `i` and attempts to add it into our set `st`. If `i` is already inside the set, nothing happens; if it is new, it gets added.
- `if( st.size()!=nums.size()){` — Checks if the final count of unique items in our set (`st.size()`) is different from the total number of items we started with (`nums.size()`).
- `return true;` — Runs if the sizes do not match, meaning duplicates were found during the process.
- `return false;` — Runs if the sizes are equal, proving that every element in the original list was unique.

## Dry Run

### Case 1: Typical case with a duplicate
Input: `nums = [1, 2, 3, 1]`

| Loop Step / Action | Current Number (`i`) | Contents of `st` After Insertion | Set Size (`st.size()`) |
| :--- | :--- | :--- | :--- |
| Start loop, first item | `1` | `{1}` | `1` |
| Second loop iteration | `2` | `{1, 2}` | `2` |
| Third loop iteration | `3` | `{1, 2, 3}` | `3` |
| Fourth loop iteration | `1` (duplicate) | `{1, 2, 3}` (size unchanged) | `3` |
| Size comparison check | N/A | `{1, 2, 3}` | `3` (which is not equal to `nums.size()` of `4`) |
| Result | N/A | N/A | Returns `true` |

### Case 2: Edge case with all distinct values
Input: `nums = [1, 2, 3, 4]`

| Loop Step / Action | Current Number (`i`) | Contents of `st` After Insertion | Set Size (`st.size()`) |
| :--- | :--- | :--- | :--- |
| Start loop, first item | `1` | `{1}` | `1` |
| Second loop iteration | `2` | `{1, 2}` | `2` |
| Third loop iteration | `3` | `{1, 2, 3}` | `3` |
| Fourth loop iteration | `4` | `{1, 2, 3, 4}` | `4` |
| Size comparison check | N/A | `{1, 2, 3, 4}` | `4` (which equals `nums.size()` of `4`) |
| Result | N/A | N/A | Returns `false` |

## Time & Space Complexity

- **Time:** O(N * log N) — The code loops through N elements. Inside the loop, it inserts each element into a standard C++ `set`. Under the hood, a standard `set` is built as a balanced binary search tree, meaning every single insertion takes O(log N) time. Doing this N times results in N * log N total time.
- **Space:** O(N) — In the worst-case scenario where all elements are unique, the set must store all N elements, taking up linear memory space.

**Can this be improved?**
Yes, it can be made faster! The slowdown here comes from using `std::set`, which keeps elements sorted using a tree structure, making lookups and insertions take O(log N) time. 

If we swap `std::set` for a hash-based container like `std::unordered_set`, insertions drop from O(log N) down to an average time of O(1) (constant time). Furthermore, we can optimize this even further by stopping the loop **the exact moment** we find a duplicate, rather than inserting everything blindly and checking sizes at the very end. 

Here is what the optimized code looks like:

```cpp
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> st;
        for (int i : nums) {
            if (st.count(i)) {
                return true; // Found a duplicate early, stop immediately
            }
            st.insert(i);
        }
        return false;
    }
};
```

- **Resulting improved complexity:** **Time:** O(N) on average using a hash table. **Space:** O(N).
- **Theoretical best possible complexity:** O(N) time and O(N) space. You must look at every element at least once in the worst case to know if a duplicate exists, so O(N) is the absolute ceiling of efficiency. Our optimized version using `unordered_set` successfully reaches this theoretical limit.

## Edge Cases Handled

- **Minimum length array (1 element):** The loop runs once, the set size is `1`, it matches `nums.size()`, and safely returns `false`.
- **Negative numbers:** C++ sets handle negative integers just fine, treating them like any other number.
- **Large numbers (up to 10^9):** Standard integer types easily accommodate the full range of values specified in the constraints without overflowing.
- **All elements identical:** The set size will instantly collapse to `1`, immediately flagging a mismatch and returning `true`.
