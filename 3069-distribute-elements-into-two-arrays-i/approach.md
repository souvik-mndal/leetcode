![Runtime](https://img.shields.io/badge/Runtime-27%20ms%20(beats%205.38%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-23.78%20MB%20(beats%2083.63%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given an array of distinct integers called `nums`. You need to distribute all elements of `nums` into two separate arrays, `arr1` and `arr2`, by following these rules:

1. Place the 1st element of `nums` into `arr1`.
2. Place the 2nd element of `nums` into `arr2`.
3. For every remaining element in `nums` (from the 3rd element onward):
   - Compare the last element currently in `arr1` with the last element currently in `arr2`.
   - If the last element of `arr1` is strictly greater than the last element of `arr2`, append the current number to `arr1`.
   - Otherwise, append the current number to `arr2`.
4. Finally, combine `arr1` and `arr2` end-to-end (all of `arr1` followed by all of `arr2`) and return this combined array as the final result.

**Example:**
If `nums = [2, 1, 3]`:
- 1st element `2` goes into `arr1` -> `arr1 = [2]`
- 2nd element `1` goes into `arr2` -> `arr2 = [1]`
- 3rd element `3`: compare last of `arr1` (`2`) and last of `arr2` (`1`). Since `2 > 1`, append `3` to `arr1` -> `arr1 = [2, 3]`
- Result is `arr1` + `arr2` -> `[2, 3, 1]`.

---

## Intuition

This is a direct **simulation** problem. You do not need any complex algorithms or advanced data structures. 

The main idea is to maintain two dynamic lists while iterating through `nums`:
1. Use the first number to seed the first list.
2. Use the second number to seed the second list.
3. For every number after that, check the end of both lists using `.back()`, make a simple comparison, and push the number into whichever list wins the comparison.
4. Join the second list onto the end of the first list and return it.

Because the array length `n` is small (between 3 and 50), directly building both lists element-by-element runs nearly instantly.

---

## Approach

Here is how the provided C++ code executes this step-by-step:

- `vector<int>ans; vector<int>a2;` — Creates two empty dynamic arrays (vectors). `ans` acts as `arr1` (and will hold the final concatenated result), while `a2` acts as `arr2`.
- `for( auto i:nums )` — Loops through every integer `i` in `nums` from left to right.
- `if( ans.size()==0 ) { ans.push_back(i); continue; }` — Checks if `ans` is empty. If it is, this is the very first element of `nums`, so it appends `i` to `ans` and skips to the next loop iteration.
- `if( a2.size()==0 ) { a2.push_back(i); continue; }` — Checks if `a2` is empty. If it is, this is the second element of `nums`, so it appends `i` to `a2` and skips to the next loop iteration.
- `if( ans.back()>a2.back() ) { ans.push_back(i); }` — For the remaining elements, checks if the last element of `ans` is strictly greater than the last element of `a2`. If true, appends `i` to `ans`.
- `else { a2.push_back(i); }` — If the last element of `ans` is less than or equal to the last element of `a2`, appends `i` to `a2`.
- `for( auto it:ans ){ cout<<it<<" "; } ...` — Prints the contents of both lists to the console for debugging purposes.
- `for( auto it:a2 ){ ans.push_back(it); }` — Iterates through `a2` and appends every element onto the end of `ans` to join the two arrays together.
- `return ans;` — Returns the combined array.

---

## Dry Run

### Case 1: Typical case (`nums = [2, 1, 3]`)

| Step / Index | Element `i` | `ans` content | `a2` content | Condition Checked | Action Taken |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | `[]` | `[]` | - | - |
| 1 (`nums[0]`) | `2` | `[]` | `[]` | `ans.size() == 0` is true | Push `2` to `ans` |
| 2 (`nums[1]`) | `1` | `[2]` | `[]` | `a2.size() == 0` is true | Push `1` to `a2` |
| 3 (`nums[2]`) | `3` | `[2]` | `[1]` | `ans.back() > a2.back()` (2 > 1) is true | Push `3` to `ans` |
| After Loop | - | `[2, 3]` | `[1]` | Concatenation pass | Push `a2` elements into `ans` |

**Final Result:** `[2, 3, 1]`

---

### Case 2: Multi-step distribution (`nums = [5, 4, 3, 8]`)

| Step / Index | Element `i` | `ans` content | `a2` content | Condition Checked | Action Taken |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | `[]` | `[]` | - | - |
| 1 (`nums[0]`) | `5` | `[]` | `[]` | `ans.size() == 0` is true | Push `5` to `ans` |
| 2 (`nums[1]`) | `4` | `[5]` | `[]` | `a2.size() == 0` is true | Push `4` to `a2` |
| 3 (`nums[2]`) | `3` | `[5]` | `[4]` | `ans.back() > a2.back()` (5 > 4) is true | Push `3` to `ans` |
| 4 (`nums[3]`) | `8` | `[5, 3]` | `[4]` | `ans.back() > a2.back()` (3 > 4) is false | Push `8` to `a2` |
| After Loop | - | `[5, 3]` | `[4, 8]` | Concatenation pass | Push `a2` elements into `ans` |

**Final Result:** `[5, 3, 4, 8]`

---

## Time & Space Complexity

- **Time Complexity:** **O(N)** — where N is the length of `nums`. The loop runs N times, doing O(1) checks and insertions at each step. Concatenating `a2` into `ans` takes another O(N) operations. 
  - *Note on Performance:* The low runtime performance score (beats ~5.38%) is caused by the extra `cout` printing loops inside the code. Console I/O in C++ is relatively slow.
- **Space Complexity:** **O(N)** — required to store the N elements across the vectors `ans` and `a2` before building the result array.

---

### Can this be improved?

**Algorithmically:** No, O(N) time and O(N) space is already optimal. Every element must be inspected at least once, and an output array of size N must be constructed and returned.

**Implementation-wise:** Yes, runtime can be optimized significantly.

1. **Remove console logging (`cout`):** Output streams slow down execution immensely in standard online judge platforms.
2. **Use pre-allocation or array insertion:** Use standard built-in functions like `insert()` to attach `a2` to `ans` in one fast memory copy rather than looping element by element.
3. **Avoid checking `size() == 0` every loop iteration:** Handle the first two elements outside the main loop since they are fixed defaults.

Here is the key snippet showing the cleaned-up approach:

```cpp
vector<int> resultArray(vector<int>& nums) {
    vector<int> ans = {nums[0]};
    vector<int> a2 = {nums[1]};
    
    for (int i = 2; i < nums.size(); ++i) {
        if (ans.back() > a2.back()) {
            ans.push_back(nums[i]);
        } else {
            a2.push_back(nums[i]);
        }
    }
    
    ans.insert(ans.end(), a2.begin(), a2.end());
    return ans;
}
```

- **Line 2-3:** Initializes `ans` and `a2` directly with `nums[0]` and `nums[1]`, skipping runtime size checks inside the loop.
- **Line 5:** Starts the loop from index 2 directly.
- **Line 13:** Uses `ans.insert(...)` to append all elements of `a2` in a single contiguous block memory operation.

- **Improved Complexity:**
  - **Time:** Still O(N), but with significantly lower constants and zero I/O overhead (reaches top performance / 0ms runtime).
  - **Space:** O(N).

---

## Edge Cases Handled

- **Minimum Length Array (`n = 3`):** Handled cleanly. The first element goes to `ans`, the second goes to `a2`, and the third element is evaluated against the rule.
- **Strictly Decreasing Inputs (e.g., `[5, 4, 3, 2]`):** Correctly handles cases where `ans.back()` stays larger, consistently routing elements into `ans`.
- **Strictly Increasing Inputs (e.g., `[1, 2, 3, 4]`):** Correctly routes elements to `a2` when `ans.back()` is smaller than `a2.back()`.
- **Distinct Values Constraint:** The problem guarantees all values in `nums` are distinct, meaning `ans.back() == a2.back()` is impossible, so tie-breaking behavior never needs to be handled.
