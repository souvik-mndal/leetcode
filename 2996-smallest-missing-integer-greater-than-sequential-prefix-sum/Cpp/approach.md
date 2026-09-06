![Runtime](https://img.shields.io/badge/Runtime-5%20ms%20(beats%205.50%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-23.1%20MB%20(beats%2028.92%25)-orange?style=for-the-badge)

---

## Problem Explained

The problem asks us to find a specific missing integer from an array of numbers called `nums`. 

Here is how we find it:
1. Look at the very start of the array (index 0).
2. Find the **longest sequential prefix**. This is the starting sequence of numbers where every number is exactly 1 greater than the number right before it. 
3. Calculate the sum of this sequence.
4. Find the smallest integer that is **greater than or equal to** this sum, but **does not exist anywhere** in `nums`.

For example, if `nums = [1, 2, 3, 2, 5]`:
* The sequence starting at index 0 is `1, 2, 3`. (The next number is 2, which breaks the rule because 2 is not 3 + 1).
* The sum of `1 + 2 + 3` is `6`.
* Now look for the smallest number greater than or equal to `6` that is missing from `nums`. 
* Is 6 in `nums`? No. So 6 is the answer.

## Intuition

The solution breaks down into two straightforward steps:

1. **Calculate the sequential prefix sum**: Iterate through `nums` from left to right starting at index 1. As long as each element is equal to the previous element plus 1, keep adding it to a running sum. The moment this consecutive pattern breaks, stop checking.
2. **Find the smallest missing number**: Start with a candidate answer equal to the sum. Check if this answer exists anywhere in `nums`. To make lookups fast, store all elements of `nums` in a hash set. If the candidate answer is in the set, increase it by 1 and check again. Repeat this until you find a candidate that is not in the set.

## Approach

* `int sum = nums[0];`: Initialize `sum` with the first element of `nums`. Every non-empty array has a sequential prefix of at least length 1.
* `for (int i = 1; i < nums.size(); i++)`: Start a loop from index 1 to inspect the rest of the array.
* `if (nums[i] == nums[i - 1] + 1)`: Check if the current element is exactly 1 larger than the element immediately before it.
* `sum += nums[i];`: If it is sequential, add the current element's value to `sum`.
* `break;`: If the current element breaks the sequential order, stop the loop immediately.
* `unordered_set<int> seen(nums.begin(), nums.end());`: Copy all elements of `nums` into an `unordered_set` so we can look up any number in constant time.
* `int answer = sum;`: Set `answer` to start at `sum`.
* `while (seen.count(answer))`: Check if `answer` is currently present inside the set `seen`.
* `answer++;`: If `answer` is found in the set, increase `answer` by 1 to test the next number.
* `return answer;`: Return `answer` once we find the first value that does not exist in `seen`.

## Dry Run

### Case 1: Standard case (`nums = [1, 2, 3, 2, 5]`)

| Step | Loop index `i` | `nums[i]` | `sum` | `answer` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | 1 | - | Set `sum = nums[0] = 1`. |
| 1 | 1 | 2 | 3 | - | `nums[1] == nums[0] + 1` (2 == 1 + 1). Add 2 to `sum`. |
| 2 | 2 | 3 | 6 | - | `nums[2] == nums[1] + 1` (3 == 2 + 1). Add 3 to `sum`. |
| 3 | 3 | 2 | 6 | - | `nums[3] != nums[2] + 1` (2 != 3 + 1). Break loop. |
| Set | - | - | 6 | - | Populate `seen` set: `{1, 2, 3, 5}`. |
| Check | - | - | 6 | 6 | `seen.count(6)` is false. Exit loop. |
| Result | - | - | 6 | 6 | Return `6`. |

### Case 2: Sum already exists in array (`nums = [3, 4, 5, 1, 12, 14, 13]`)

| Step | Loop index `i` | `nums[i]` | `sum` | `answer` | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Start | - | - | 3 | - | Set `sum = nums[0] = 3`. |
| 1 | 1 | 4 | 7 | - | `nums[1] == nums[0] + 1` (4 == 3 + 1). Add 4 to `sum`. |
| 2 | 2 | 5 | 12 | - | `nums[2] == nums[1] + 1` (5 == 4 + 1). Add 5 to `sum`. |
| 3 | 3 | 1 | 12 | - | `nums[3] != nums[2] + 1` (1 != 5 + 1). Break loop. |
| Set | - | - | 12 | - | Populate `seen` set: `{1, 3, 4, 5, 12, 13, 14}`. |
| Check 1 | - | - | 12 | 12 | `seen.count(12)` is true. Increment `answer` to 13. |
| Check 2 | - | - | 12 | 13 | `seen.count(13)` is true. Increment `answer` to 14. |
| Check 3 | - | - | 12 | 14 | `seen.count(14)` is true. Increment `answer` to 15. |
| Check 4 | - | - | 12 | 15 | `seen.count(15)` is false. Exit loop. |
| Result | - | - | 12 | 15 | Return `15`. |

## Time & Space Complexity

* **Time Complexity:** **O(N)** average, where N is the length of `nums`. Finding the sum takes at most N iterations. Inserting N elements into an `unordered_set` takes O(N) average time. The `while` loop runs at most N times because `answer` can only match elements present in `nums`.
* **Space Complexity:** **O(N)** extra space to store elements in the `unordered_set`.

### Can this be improved?

Yes, memory usage and runtime overhead can both be improved. Creating an `unordered_set` allocates dynamic heap memory, which causes low performance scores on submission platforms.

Instead of a set, we can calculate `sum` first, then **sort** `nums` in ascending order. After sorting, we iterate through `nums`. Whenever we see an element equal to `answer`, we increment `answer` by 1. Because the array is sorted, matching values appear in order, so we will never miss a number.

Here is the key change:

```cpp
// Sort nums after calculating prefix sum
sort(nums.begin(), nums.end());

int answer = sum;
for (int num : nums) {
    if (num == answer) {
        answer++;
    }
}
return answer;
```

* `sort(nums.begin(), nums.end());`: Sorts the array in place so elements appear in increasing order.
* `if (num == answer) answer++;`: Increments `answer` whenever the current array element matches `answer`.

* **Improved Time Complexity:** **O(N log N)** due to sorting `nums`.
* **Improved Space Complexity:** **O(1)** auxiliary space (excluding space used by sorting algorithm stack), avoiding hash table allocations completely.
* **Theoretical Best Complexity:** **O(N)** time and **O(1)** extra space. Because the maximum value in `nums` is constrained to 50, we can also use a small boolean frequency array of fixed size (size 2550 to handle maximum possible sum) to achieve O(N) time and true O(1) space.

## Edge Cases Handled

* **Single-element array (`nums.length == 1`)**: The prefix loop does not execute because `i < nums.size()` is false at `i = 1`. `sum` defaults to `nums[0]`.
* **Sequential prefix of length 1**: If `nums[1]` is not `nums[0] + 1`, the loop breaks instantly, leaving the prefix sum as `nums[0]`.
* **Entire array is sequential**: If the whole array forms a single sequence (e.g., `[1, 2, 3, 4]`), the loop processes every element without breaking.
* **Duplicate elements in `nums`**: Duplicate values are either merged into single entries by `unordered_set` or safely passed over in the sorted loop without double-incrementing `answer` incorrectly.
* **Sum matching a number later in the array**: If the prefix sum equals a value located later in the array, the `while` loop correctly detects it and increments past it until an unused number is found.
