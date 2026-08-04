![Runtime](https://img.shields.io/badge/Runtime-4%20ms%20(beats%2035.49%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-32.30%20MB%20(beats%2075.54%25)-green?style=for-the-badge)

---

## Problem Explained

You are given an array of unique integers named `nums`. 

Originally, `nums` contained every single consecutive integer starting from its smallest number up to its largest number, without leaving any gaps. However, some of those numbers were removed. The smallest and largest numbers are guaranteed to still be present in `nums`.

Your task is to find all the numbers that are missing between the smallest and largest values, and return them as a list sorted from smallest to largest. If no numbers are missing, return an empty list.

**Example:**
* Input: `nums = [1, 4, 2, 5]`
* Smallest number: `1`
* Largest number: `5`
* Full range should be: `1, 2, 3, 4, 5`
* Numbers present: `1, 2, 4, 5`
* Output: `[3]`

---

## Intuition

If we sort the array from smallest to largest, the numbers should ideally go up by `1` at each step (for example: `1, 2, 3, 4, 5`).

We can track what number we expect to see next, starting with the smallest number. As we look at the sorted array:
* If the current number in the array matches what we expect, everything is fine. We move to the next expected number and the next array item.
* If the current number in the array is larger than what we expect, it means our expected number was skipped. That expected number is missing! We record it, increment our expected number, but **stay on the same array item** so we can test it against the new expected number.

---

## Approach

Here is how the provided code works step-by-step:

* **Create an answer list:** Initialize an empty list called `ans` to store any missing numbers we find.
* **Sort the input array:** Sort `nums` in ascending order. This guarantees that numbers appear sequentially from smallest to largest.
* **Initialize tracking variables:** Set `toFind` to `nums[0]` (the smallest number, which is our starting expected value). Set `counter` (our current index in `nums`) to `0`.
* **Loop through the array:** While `counter` is less than the total length of `nums`:
  * **If `nums[counter]` equals `toFind`:** The expected number exists in the array. Move to the next number in `nums` by doing `counter++`, and increment `toFind++` for the next check.
  * **If `nums[counter]` does not match `toFind`:** The number `toFind` is missing from `nums`. Add `toFind` to `ans`, and increment `toFind++` to check for the next number. Do **not** increment `counter`, because we still need to match `nums[counter]` against the updated `toFind`.
* **Return result:** Once all items in `nums` are processed, return `ans`.

---

## Time & Space Complexity

### Current Solution Complexity

* **Time Complexity:** **O(N log N)** — where **N** is the number of elements in `nums`. Sorting the array takes **O(N log N)** time. The `while` loop runs in **O(N + M)** time, where **M** is the count of missing numbers. Since **M** is bounded by the difference between the minimum and maximum numbers (at most 100 based on constraints), sorting dominates the overall time.
* **Space Complexity:** **O(1)** auxiliary space — excluding the space required for the output list `ans`. Depending on the C++ implementation, `std::sort` may use **O(log N)** internal stack space.

### Can We Improve This?

**Yes.** We can avoid sorting entirely by using a **Hash Table** (or Hash Set).

**Why the change works:**
Instead of sorting the array to check numbers in order, we can:
1. Scan `nums` once to find both the smallest (`min_val`) and largest (`max_val`) numbers in **O(N)** time.
2. Store all numbers from `nums` into a **Hash Set** in **O(N)** time.
3. Loop through every integer from `min_val` to `max_val`. If a number is not found in the Hash Set, we add it directly to our answer. Because we iterate from `min_val` to `max_val` in increasing order, our answer list is automatically sorted without needing to run a sorting algorithm.

Here is the key logic update:

```cpp
vector<int> findMissingElements(vector<int>& nums) {
    // Put all array elements in a hash set for quick O(1) lookups
    unordered_set<int> present(nums.begin(), nums.end());
    
    // Find smallest and largest bounds in one pass
    int min_val = *min_element(nums.begin(), nums.end());
    int max_val = *max_element(nums.begin(), nums.end());

    vector<int> ans;
    // Iterate sequentially through the full expected range
    for (int num = min_val; num <= max_val; num++) {
        // If the number is not in our set, it is missing
        if (present.find(num) == present.end()) {
            ans.push_back(num);
        }
    }
    return ans;
}
```

* **Improved Time Complexity:** **O(N + K)** — where **N** is the size of `nums` and **K** is the range span (`max_val - min_val + 1`). Since constraints state elements are between 1 and 100, **K** is at most 100, making this effectively **O(N)** linear time.
* **Improved Space Complexity:** **O(N)** — to store elements inside the hash set.
* **Theoretical Best Complexity:** **O(N + K)** time. Every element in `nums` must be inspected at least once to determine the boundaries and present elements, making **O(N + K)** the theoretical limit. The Hash Set approach achieves this optimal bound.

---

## Edge Cases Handled

* **No missing numbers (e.g., `[6, 7, 8, 9]`):** The code compares `nums[counter]` with `toFind` at every step, finding a match each time. `ans` remains empty `[]`.
* **Multiple consecutive missing numbers (e.g., `[1, 5]`):** When `1` is processed, `toFind` becomes `2`. In the next loop iteration, `nums[1]` is `5`, which does not match `2`. The code appends `2`, increments `toFind` to `3`, and keeps `counter` at `1`. It repeats this for `3` and `4` before finally matching `5`.
* **Smallest valid input size (2 elements, e.g., `[5, 1]`):** Works correctly regardless of initial order because `sort()` places the minimum element at index `0`.
* **Unsorted initial input (e.g., `[1, 4, 2, 5]`):** The call to `sort()` fixes the order before the logic loop runs.
