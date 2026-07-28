![Runtime](https://img.shields.io/badge/Runtime-10%20ms%20(beats%2096.65%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-60.12%20MB%20(beats%2092.89%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a list of unsorted numbers. Your task is to find the length of the longest chain of consecutive numbers (numbers that go up by 1, like 1, 2, 3, 4). 

The numbers in the chain do not need to be next to each other in the original list, and they can appear in any order.

**Example:**
If the input list is `[100, 4, 200, 1, 3, 2]`:
* The consecutive numbers you can form are `1, 2, 3, 4`.
* The length of this chain is `4`.

---

## Intuition

If you line up all the numbers in order from smallest to largest, consecutive numbers will sit right next to each other. 

Once the list is ordered:
1. You can walk through the list one number at a time.
2. If a number is exactly 1 greater than the previous number, your consecutive streak grows.
3. If a number is identical to the previous number, you just skip it.
4. If a number jumps ahead by more than 1, your current streak ends, and you start a new streak of 1.

By keeping track of the longest streak seen so far, you get the final answer.

---

## Approach

Here is how the provided C++ code executes this logic step-by-step:

* **Handle empty input:** Check if the input list has a size of `0`. If so, immediately return `0`.
* **Sort the list:** Sort all elements in ascending order so consecutive numbers are placed side-by-side.
* **Initialize tracking variables:**
  * `lng`: Stores the length of the longest sequence found so far (starts at `1`).
  * `cnt`: Tracks the length of the current consecutive sequence (starts at `1`).
  * `ele`: Stores the previous number seen (starts as the first element `nums[0]`).
* **Iterate through the array:** Start a loop from the second element (index `1`) to the end:
  * **Duplicate check:** If the current number equals `ele`, skip it and move to the next number.
  * **Consecutive check:** If the current number equals `ele + 1`, increment `cnt` by `1`, update `ele` to the current number, and set `lng` to the larger value between `lng` and `cnt`.
  * **Gap found:** Otherwise, reset `cnt` back to `1` and update `ele` to the current number to start tracking a new sequence.
* **Return result:** Return `lng` as the final length.

---

## Time & Space Complexity

* **Time Complexity:** **$O(n \log n)$** — Sorting an array of size $n$ using `std::sort` takes $O(n \log n)$ time. The subsequent single loop through the array takes $O(n)$ time. The sorting step dominates the runtime.
* **Space Complexity:** **$O(1)$** auxiliary space (or $O(\log n)$ internal stack space used by `std::sort`) — The code modifies the array in place and uses a few simple integer variables (`lng`, `cnt`, `ele`).

### Optimization Check

**Can this be improved?**
Yes. The problem constraints ask for an $O(n)$ time algorithm. Sorting takes $O(n \log n)$, which is slower than required.

To achieve **$O(n)$ time**, you can store all numbers in an **unordered hash set** (a fast lookup table). Then, only count a sequence if a number is the *start* of that sequence (meaning `num - 1` does not exist in the set).

```cpp
// Optimal O(n) approach using a hash set
unordered_set<int> numSet(nums.begin(), nums.end());
int longest = 0;

for (int num : numSet) {
    // Only start counting if 'num' is the beginning of a sequence
    if (!numSet.count(num - 1)) {
        int currentNum = num;
        int currentStreak = 1;

        while (numSet.count(currentNum + 1)) {
            currentNum++;
            currentStreak++;
        }
        longest = max(longest, currentStreak);
    }
}
return longest;
```

**Improved Complexity:**
* **Time Complexity:** **$O(n)$** — Inserting into a hash set takes $O(n)$ time. Looking up elements takes $O(1)$ on average. Because the `while` loop only runs for the starting elements of a sequence, each number is checked at most twice.
* **Space Complexity:** **$O(n)$** — Extra memory is needed to store all elements in the hash set.

**Theoretical Best:**
The theoretical best time complexity for this problem is **$O(n)$**, because you must inspect every number at least once. The hash set optimization reaches this optimal limit.

---

## Edge Cases Handled

* **Empty Array (`[]`):** Handled immediately at the start with `if (nums.size() == 0) return 0;`.
* **Single Element (`[5]`):** The loop condition `i < nums.size()` evaluates to false immediately, and the code returns `lng = 1`.
* **Duplicate Numbers (`[1, 0, 1, 2]`):** The condition `if (ele == nums[i]) continue;` ignores duplicate values so they don't reset the streak or count extra steps.
* **Negative Numbers (`[-3, -2, -1, 0]`):** Integer arithmetic (`ele + 1`) and vector sorting work seamlessly with negative values.
* **Large Gaps Between Chains (`[1, 2, 100, 101, 102]`):** The `else` block correctly resets `cnt` to `1` when a gap is detected, properly starting a new streak tracker.
