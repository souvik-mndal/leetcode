![Runtime](https://img.shields.io/badge/Runtime-27%20ms%20(beats%204.12%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-23.8%20MB%20(beats%2080.42%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums` containing distinct integers. The array has at least 3 elements.

Your task is to distribute all the numbers from `nums` into two separate temporary arrays, `arr1` and `arr2`, following these rules in order:

1. Put the first number (`nums[0]`) into `arr1`.
2. Put the second number (`nums[1]`) into `arr2`.
3. For every remaining number in `nums`:
   - Look at the last number currently in `arr1` and the last number currently in `arr2`.
   - If the last element of `arr1` is greater than the last element of `arr2`, append the current number to `arr1`.
   - Otherwise, append the current number to `arr2`.
4. At the end, combine both arrays by placing all elements of `arr2` right after `arr1`. Return this single combined array.

---

## Intuition

This problem is a direct **simulation**. You do not need any complex math tricks or advanced algorithms. You just need to follow the distribution rules step by step.

Since you only ever need to look at the **most recent (last) number** added to each array, using dynamic arrays (like `std::vector` in C++) is ideal. Checking the last element with `.back()` and adding a new element with `.push_back()` both happen instantly in O(1) constant time. Once all numbers are processed, you join the second array onto the end of the first array and return it.

---

## Approach

Here is how the code works line-by-line:

* `vector<int>ans;` and `vector<int>a2;`: Creates two dynamic arrays. `ans` acts as `arr1` (and will hold the final result), while `a2` acts as `arr2`.
* `for( auto i:nums )`: Loops through each number `i` in the input array `nums` from left to right.
* `if( ans.size()==0 )`: Checks if `ans` is empty. If so, this is the first element of `nums`. It pushes `i` into `ans` and moves to the next number using `continue`.
* `if( a2.size()==0 )`: Checks if `a2` is empty. If so, this is the second element of `nums`. It pushes `i` into `a2` and moves to the next number using `continue`.
* `if( ans.back()>a2.back() )`: For all remaining numbers, compares the last element of `ans` (`ans.back()`) with the last element of `a2` (`a2.back()`). If `ans`'s last element is strictly larger, it appends `i` to `ans`.
* `else`: If `a2`'s last element is greater than or equal to `ans`'s last element, it appends `i` to `a2`.
* `for( auto it:a2 ) { ans.push_back(it); }`: After distributing all numbers, loops through `a2` and appends every element onto the end of `ans`.
* `return ans;`: Returns the combined array `ans`.

---

## Dry Run

### Case 1: Standard input with 3 elements (`nums = [2, 1, 3]`)

| Step | `i` | `ans` before | `a2` before | Decision / Action | `ans` after | `a2` after |
|---|---|---|---|---|---|---|
| 1 | 2 | `[]` | `[]` | `ans` is empty -> push 2 to `ans` | `[2]` | `[]` |
| 2 | 1 | `[2]` | `[]` | `a2` is empty -> push 1 to `a2` | `[2]` | `[1]` |
| 3 | 3 | `[2]` | `[1]` | `ans.back()` (2) > `a2.back()` (1) -> push 3 to `ans` | `[2, 3]` | `[1]` |
| Final | - | `[2, 3]` | `[1]` | Append all elements of `a2` to `ans` | `[2, 3, 1]` | `[1]` |

**Output:** `[2, 3, 1]`

---

### Case 2: Multi-step distribution (`nums = [5, 4, 3, 8]`)

| Step | `i` | `ans` before | `a2` before | Decision / Action | `ans` after | `a2` after |
|---|---|---|---|---|---|---|
| 1 | 5 | `[]` | `[]` | `ans` is empty -> push 5 to `ans` | `[5]` | `[]` |
| 2 | 4 | `[5]` | `[]` | `a2` is empty -> push 4 to `a2` | `[5]` | `[4]` |
| 3 | 3 | `[5]` | `[4]` | `ans.back()` (5) > `a2.back()` (4) -> push 3 to `ans` | `[5, 3]` | `[4]` |
| 4 | 8 | `[5, 3]` | `[4]` | `ans.back()` (3) <= `a2.back()` (4) -> push 8 to `a2` | `[5, 3]` | `[4, 8]` |
| Final | - | `[5, 3]` | `[4, 8]` | Append all elements of `a2` to `ans` | `[5, 3, 4, 8]` | `[4, 8]` |

**Output:** `[5, 3, 4, 8]`

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — where n is the length of `nums`. The code processes each element once in the loop, and then iterates through `a2` to append elements to `ans`. 
* **Space Complexity:** **O(n)** — additional storage is used to store elements across `ans` and `a2`.

### Why the original code had low performance percentile
The original code includes debugging print statements (`cout << it << " ";`). Printing to standard output inside competitive programming platform benchmarks introduces heavy **I/O overhead**, which artificially slows runtime execution drastically.

### Optimization Strategy
To make this solution run as fast as possible:
1. **Remove `cout` statements**: Printing to standard output is very slow.
2. **Pre-assign initial elements**: Put `nums[0]` and `nums[1]` directly into `ans` and `a2` before starting the loop. This removes redundant `.size() == 0` checks on every iteration.
3. **Reserve vector capacity**: Use `.reserve(n)` on vectors to allocate memory upfront, preventing costly array re-allocations during insertions.
4. **Use bulk insert**: Use `ans.insert()` instead of a manual `for` loop to concatenate `a2` into `ans`.

Here is the optimized code:

```cpp
class Solution {
public:
    vector<int> resultArray(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans, a2;
        ans.reserve(n);
        a2.reserve(n);
        
        // Put first two elements directly
        ans.push_back(nums[0]);
        a2.push_back(nums[1]);
        
        // Process remaining elements starting at index 2
        for (int i = 2; i < n; ++i) {
            if (ans.back() > a2.back()) {
                ans.push_back(nums[i]);
            } else {
                a2.push_back(nums[i]);
            }
        }
        
        // Concatenate a2 onto ans
        ans.insert(ans.end(), a2.begin(), a2.end());
        return ans;
    }
};
```

* **Improved Complexity:** Still **O(n)** time and **O(n)** space, but with significantly lower runtime overhead.
* **Optimal Status:** Yes, this is the theoretical best performance for this problem. You must inspect all n elements to divide them, and you must store all n elements to build the result array.

---

## Edge Cases Handled

* **Minimum Constraint Size (`n = 3`)**: The problem guarantees length between 3 and 50. The logic places index 0 into `ans`, index 1 into `a2`, and compares index 2 against both. This works cleanly without index out-of-bounds errors.
* **Strictly Increasing or Decreasing Sequences**: Handled smoothly because the condition `ans.back() > a2.back()` dynamically evaluates the latest appended number every single turn regardless of sorting order.
* **All Distinct Elements**: The problem guarantees all elements in `nums` are distinct, so `ans.back()` will never equal `a2.back()`.
