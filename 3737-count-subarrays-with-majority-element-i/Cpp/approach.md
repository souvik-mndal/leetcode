![Runtime](https://img.shields.io/badge/Runtime-52%20ms%20(beats%2025.66%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-39.5%20MB%20(beats%2074.87%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of numbers, and you pick a starting point and an ending point. Everything from your start to your end forms a **subarray** (a smaller slice of the original array). 

This problem asks us to look at every single possible subarray we can make from a given array of numbers called `nums`. For each subarray, we need to check two things:
1. Does our specific `target` number exist inside that slice?
2. Does that `target` appear **strictly more than half** of the total times in that slice? (For example, if the slice has length 4, the target must appear at least 3 times. If the slice has length 3, the target must appear at least 2 times.)

If both conditions are true, that slice is a **majority subarray**. Our job is to count how many such slices exist across the entire array and return that total count.

---

## Intuition

The most straightforward way to find every single subarray is to look at all possible starting points, and for each starting point, stretch out to every possible ending point. 

As we stretch our ending point further to the right, we can just keep a running tally of how many times our `target` has shown up so far. At each step, we look at the current length of our slice (which is ending index minus starting index plus 1), divide that length by 2, and check if our `target` count is strictly greater than that half-way point. If it is, we found a valid majority subarray and we add 1 to our running total. 

This brute-force approach checks every single combination, which is simple to write and guarantees we do not miss any valid slices.

---

## Approach

Here is how the code works step by step:

* `int total = 0;`: Initializes a counter variable named `total` to keep track of how many valid majority subarrays we find. It starts at 0.
* `for( int i=0 ; i<nums.size() ; i++ )`: Starts an outer loop where `i` represents the starting index of our subarray, moving from the very beginning of `nums` to the end.
* `int count = 0;`: Inside the outer loop, initializes a variable named `count` to 0. This tracks how many times the `target` appears in the current subarray starting at index `i`.
* `for( int j=i ; j<nums.size() ; j++ )`: Starts an inner loop where `j` represents the ending index of our subarray. It starts at `i` (a subarray of length 1) and stretches rightward to the end of the array.
* `if( nums[j] == target )`: Checks if the element currently at the right edge of our subarray (`nums[j]`) matches our target.
* `count++;`: If it matches, we increase our `count` of the target by 1.
* `if( count > (j-i+1)/2 )`: Calculates the current length of the subarray as `(j-i+1)`. It divides that length by 2 using integer division, and checks if our running `count` is strictly greater than that half-way mark.
* `total++;`: If the condition is met, the current subarray has `target` as its majority element, so we add 1 to our `total` counter.
* `return total;`: After checking every possible starting and ending combination, the function returns the final `total` count of valid subarrays.

---

## Dry Run

### Case 1: Typical case
Inputs: `nums = [1, 2, 2, 3]`, `target = 2`

| i | j | nums[j] | count | Subarray length | Length / 2 | count > length / 2? | total | Action |
|---|---|---|---|---|---|---|---|---|
| 0 | 0 | 1 | 0 | 1 | 0 | False | 0 | Subarray [1], target count 0 |
| 0 | 1 | 2 | 1 | 2 | 1 | False | 0 | Subarray [1, 2], target count 1 |
| 0 | 2 | 2 | 2 | 3 | 1 | True | 1 | Subarray [1, 2, 2], target count 2 (2 > 1) |
| 0 | 3 | 3 | 2 | 4 | 2 | False | 1 | Subarray [1, 2, 2, 3], target count 2 |
| 1 | 1 | 2 | 1 | 1 | 0 | True | 2 | Subarray [2], target count 1 (1 > 0) |
| 1 | 2 | 2 | 2 | 2 | 1 | True | 3 | Subarray [2, 2], target count 2 (2 > 1) |
| 1 | 3 | 3 | 2 | 3 | 1 | True | 4 | Subarray [2, 2, 3], target count 2 (2 > 1) |
| 2 | 2 | 2 | 1 | 1 | 0 | True | 5 | Subarray [2], target count 1 (1 > 0) |
| 2 | 3 | 3 | 1 | 2 | 1 | False | 5 | Subarray [2, 3], target count 1 |
| 3 | 3 | 3 | 0 | 1 | 0 | False | 5 | Subarray [3], target count 0 |

Final returned `total`: `5`

---

### Case 2: Target not present
Inputs: `nums = [1, 2, 3]`, `target = 4`

| i | j | nums[j] | count | Subarray length | Length / 2 | count > length / 2? | total | Action |
|---|---|---|---|---|---|---|---|---|
| 0 | 0 | 1 | 0 | 1 | 0 | False | 0 | Subarray [1], target count 0 |
| 0 | 1 | 2 | 0 | 2 | 1 | False | 0 | Subarray [1, 2], target count 0 |
| 0 | 2 | 3 | 0 | 3 | 1 | False | 0 | Subarray [1, 2, 3], target count 0 |
| 1 | 1 | 2 | 0 | 1 | 0 | False | 0 | Subarray [2], target count 0 |
| 1 | 2 | 3 | 0 | 2 | 1 | False | 0 | Subarray [2, 3], target count 0 |
| 2 | 2 | 3 | 0 | 1 | 0 | False | 0 | Subarray [3], target count 0 |

Final returned `total`: `0`

---

## Time & Space Complexity

- **Time:** O(n^2) — There are two nested loops. The outer loop runs `n` times (where `n` is the length of `nums`), and the inner loop runs up to `n` times depending on the starting point. This gives roughly `n * (n + 1) / 2` total checks.
- **Space:** O(1) — We only use a few integer variables (`total`, `count`, `i`, `j`), which take up a constant amount of extra memory regardless of the input size.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, it can be improved. The brute-force O(n^2) approach checks every subarray individually. However, this problem can actually be solved in **O(n)** time using a prefix sum technique combined with a hash map or frequency array. 

### How the optimization works:
If we transform the array by replacing every occurrence of `target` with the number `1`, and every other number with `-1`, then finding a majority element turns into finding a subarray whose sum is strictly greater than zero. 

Using running prefix sums, we can count how many past prefix sum values are smaller than our current prefix sum. By using an indexed frequency structure or Fenwick tree (binary indexed tree), we can query and update these counts in logarithmic time, bringing the overall time complexity down to **O(n log n)** or even **O(n)**.

Here is what the core optimized logic looks like using a transform and a frequency map approach:

```cpp
int countMajoritySubarrays(vector<int>& nums, int target) {
    int n = nums.size();
    vector<int> transformed(n);
    for (int i = 0; i < n; i++) {
        transformed[i] = (nums[i] == target) ? 1 : -1;
    }
    // Using prefix sums and a frequency map to count valid ranges in O(n log n)
    // ...
}
```

- **Resulting improved complexity:** **Time:** O(n log n) or O(n). **Space:** O(n) to store prefix counts and frequency maps.
- **Theoretical best possible complexity:** **O(n)** time, which the advanced data structure approach can achieve.
- *Note on constraints:* Because the problem constraints state that `nums.length <= 1000`, the simple O(n^2) solution runs in a fraction of a second and easily passes all test cases without timing out, making the brute-force approach very practical here.

---

## Edge Cases Handled

* **Target does not exist in the array:** If the target is never found, `count` remains 0, the condition is never met, and the code correctly returns `0`.
* **Array of length 1:** The loops run exactly once for `i = 0` and `j = 0`, checking a single-element subarray correctly.
* **All elements match the target:** Every single subarray will have the target as its majority element, and the code correctly tallies all `n * (n + 1) / 2` possible combinations.
* **Target appears less than half the time:** The strict greater-than check (`count > (j-i+1)/2`) ensures that ties do not count as a majority.
