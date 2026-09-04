![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-22.31%20MB%20(beats%207.67%25)-red?style=for-the-badge)

---

## Problem Explained

You are given an array of numbers called `nums`. The array contains `n` distinct (unique) numbers. These numbers are supposed to cover the full range from `0` all the way up to `n`. 

Because the range from `0` to `n` contains `n + 1` total numbers, but your array `nums` only has a length of `n`, exactly one number from that range is missing. Your task is to find and return that single missing number.

For example, if `nums = [3, 0, 1]`, the length `n` is `3`. The full range of numbers should be `[0, 1, 2, 3]`. Looking at `nums`, the number `2` is missing, so the answer is `2`.

---

## Intuition

Think of this like taking attendance from a checklist. 

If you have a group of students numbered `0` through `n`, you can create a tally sheet (a list of counters initialized to zero). You look at each student present in the room (`nums`), find their number on your tally sheet, and mark them present by adding 1.

Once you check everyone in the room, you look down your tally sheet from `0` to `n`. The student whose count is still `0` is the one missing from the room.

---

## Approach

Here is how the given code solves the problem step-by-step:

* `int n = nums.size();` — Finds the total count of numbers currently in the input array `nums`. This tells us that the full range of expected numbers goes from `0` to `n`.
* `vector<int>hash(n+1,0);` — Creates a frequency array (a checklist) named `hash` of size `n + 1`. Every slot starts initialized to `0`.
* `for( int i=0 ; i<n ; i++ )` — Starts a loop to examine every number inside `nums` one by one using the index `i`.
* `hash[nums[i]]++;` — Looks at the actual value stored at `nums[i]` and increases the count at that value's slot in `hash` by `1`. This marks that number as present.
* `for( int i=0 ; i<hash.size() ; i++ )` — Starts a second loop that checks every slot `i` in the `hash` array from `0` up to `n`.
* `if( hash[i] == 0 )` — Checks if the count at slot `i` is still `0`. If it is, this number was never seen in `nums`.
* `return i;` — Returns `i` immediately as the missing number.
* `return -1;` — Serves as a backup return statement required by C++ to complete the function structure, though it will never be reached given valid problem inputs.

---

## Dry Run

### Case 1: Standard unsorted array (`nums = [3, 0, 1]`)

Here, `n = 3`. `hash` starts as `[0, 0, 0, 0]` representing indices `0, 1, 2, 3`.

**Step 1: Counting frequencies in `nums`**

| `i` | `nums[i]` | `hash` State (`[0, 1, 2, 3]`) | Action |
| --- | --- | --- | --- |
| 0 | 3 | `[0, 0, 0, 1]` | Increment `hash[3]` |
| 1 | 0 | `[1, 0, 0, 1]` | Increment `hash[0]` |
| 2 | 1 | `[1, 1, 0, 1]` | Increment `hash[1]` |

**Step 2: Searching for missing number in `hash`**

| `i` | `hash[i]` | Condition (`hash[i] == 0`) | Action |
| --- | --- | --- | --- |
| 0 | 1 | False | Move to next index |
| 1 | 1 | False | Move to next index |
| 2 | 0 | **True** | **Found missing number! Return 2** |

---

### Case 2: Array missing the largest boundary number (`nums = [0, 1]`)

Here, `n = 2`. `hash` starts as `[0, 0, 0]` representing indices `0, 1, 2`.

**Step 1: Counting frequencies in `nums`**

| `i` | `nums[i]` | `hash` State (`[0, 1, 2]`) | Action |
| --- | --- | --- | --- |
| 0 | 0 | `[1, 0, 0]` | Increment `hash[0]` |
| 1 | 1 | `[1, 1, 0]` | Increment `hash[1]` |

**Step 2: Searching for missing number in `hash`**

| `i` | `hash[i]` | Condition (`hash[i] == 0`) | Action |
| --- | --- | --- | --- |
| 0 | 1 | False | Move to next index |
| 1 | 1 | False | Move to next index |
| 2 | 0 | **True** | **Found missing number! Return 2** |

---

## Time & Space Complexity

* **Time Complexity:** **O(n)** — We loop through `nums` of size `n` once, and then loop through `hash` of size `n + 1` once. Total operations are `n + (n + 1)`, which grows linearly with `n`.
* **Space Complexity:** **O(n)** — We allocate an additional vector `hash` of size `n + 1` to store counts.

### Can we improve this?

Yes! The code can be optimized to use **O(1) extra space** (constant space) while keeping **O(n) time**.

Instead of using an extra array to track seen numbers, we can use simple math. The sum of all integers from `0` to `n` can be calculated instantly using the math formula:

```
expected_sum = n * (n + 1) / 2
```

If we sum up all the actual numbers present in `nums`, the difference between `expected_sum` and `actual_sum` must be the missing number!

#### Optimized Code Snippet (Math Approach)

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        // Calculate expected sum of numbers from 0 to n
        int expectedSum = n * (n + 1) / 2;
        
        // Sum all elements in nums
        int actualSum = 0;
        for (int num : nums) {
            actualSum += num;
        }
        
        // The difference is the missing number
        return expectedSum - actualSum;
    }
};
```

* `expectedSum = n * (n + 1) / 2` calculates what the total sum *should* be if no number were missing.
* `actualSum += num` calculates the total sum of numbers present in `nums`.
* `expectedSum - actualSum` gives the missing value directly without needing extra memory arrays.

#### Improved Complexity
* **Improved Time Complexity:** **O(n)** — A single pass through `nums` to sum elements.
* **Improved Space Complexity:** **O(1)** — Only uses two integer variables (`expectedSum` and `actualSum`), regardless of input size.
* **Theoretical Best Complexity:** **O(n)** time and **O(1)** space. You must read every element in `nums` at least once (**O(n)** time), and you cannot use less memory than a few variables (**O(1)** space). The math approach reaches this theoretical limit.

---

## Edge Cases Handled

* **Missing number is `0`**: Correctly detected because array indexing starts at `0`.
* **Missing number is `n` (the boundary value)**: Handled because the `hash` vector is allocated with size `n + 1`, ensuring index `n` is tracked.
* **Unsorted arrays**: The logic relies on frequency counting by value index, so input element order does not affect the output.
* **Smallest array size (`n = 1`)**: E.g., `nums = [0]` returns `1`, and `nums = [1]` returns `0`. Vector sizing handles `n = 1` properly.
