![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.9%20MB%20(beats%2082.15%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to find every possible combination of numbers we can make from a given list of unique numbers. This collection of all possible combinations is called the **power set**. 

For example, if you are given the numbers 1, 2, and 3, you need to return:
- An empty set
- Sets with one number: [1], [2], [3]
- Sets with two numbers: [1, 2], [1, 3], [2, 3]
- A set with all three numbers: [1, 2, 3]

The order of the subsets in your final answer does not matter, but every subset must be unique.

---

## Intuition

The core "aha" moment for this solution is that making a subset is very much like flipping a row of light switches. 

Imagine you have 3 numbers in your input array. You can picture every possible subset as a 3-digit binary number (using 1s and 0s), where each digit decides whether you include a specific number or leave it out:
- 000 means include nothing: []
- 001 means include the first number: [1]
- 010 means include the second number: [2]
- 111 means include all three numbers: [1, 2, 3]

If you have N numbers, there are 2 to the power of N possible combinations. By counting up from 0 to (2^N - 1) in binary, you naturally generate every single combination without missing any or making duplicates.

---

## Approach

Here is how the code builds every subset step-by-step:

- `vector<vector<int>> ans;`: Creates a master container called `ans` that will hold all of our generated subsets.
- `int total = 1 << nums.size();`: Calculates the total number of subsets possible by shifting the binary number 1 to the left by `nums.size()` places. This equals 2 to the power of `nums.size()`.
- `for( int i=0 ; i<total ; i++ )`: Loops through every integer from 0 up to `total - 1`. Each integer `i` represents a unique binary pattern (a distinct subset choice).
- `int num = i;`: Copies the current loop number into a variable named `num` so we can look at its individual bits.
- `vector<int> temp;`: Creates a temporary list called `temp` to store the numbers that belong to the current subset.
- `for( int j=0 ; j<nums.size() ; j++ )`: Loops through every index `j` of the original `nums` array to check which numbers should be included in our current subset.
- `if( num & ( 1 << j )){`: Uses a **bitwise AND** operation and a bit shift to check if the bit at position `j` in `num` is turned on (set to 1).
- `temp.push_back(nums[j]);`: Adds the number at `nums[j]` into our `temp` subset if its matching bit was turned on.
- `ans.push_back(temp);`: Adds our finished `temp` subset into the master `ans` list once the inner loop finishes checking all positions.
- `return ans;`: Returns the final list containing all generated subsets after the outer loop finishes.

---

## Dry Run

### Case 1: Typical case with 1 element `nums = [0]`

| i (num) | Binary of i | j | 1 << j | num & (1 << j) | Action | temp | ans |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 0 | 1 | 0 | Bit 0 is off, skip `nums[0]` | [] | [[]] |
| 1 | 1 | 0 | 1 | 1 | Bit 0 is on, add `nums[0]` (which is 0) | [0] | [[], [0]] |

### Case 2: Multi-element case `nums = [1, 2]`

| i (num) | Binary of i | j | 1 << j | num & (1 << j) | Action | temp | ans |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 00 | 0, 1 | 1, 2 | 0, 0 | No bits are on | [] | [[]] |
| 1 | 01 | 0<br>1 | 1<br>2 | 1<br>0 | Bit 0 is on (add 1), Bit 1 is off | [1] | [[], [1]] |
| 2 | 10 | 0<br>1 | 1<br>2 | 0<br>2 | Bit 0 is off, Bit 1 is on (add 2) | [2] | [[], [1], [2]] |
| 3 | 11 | 0<br>1 | 1<br>2 | 1<br>2 | Both bits are on (add 1 and 2) | [1, 2] | [[], [1], [2], [1, 2]] |

---

## Time & Space Complexity

- **Time:** O(N * 2^N) — There are 2^N total subsets, and for each subset, the inner loop runs N times to check the bit positions.
- **Space:** O(N * 2^N) — We store 2^N subsets, and in the worst case, those subsets hold up to N elements each. (Note: This space is strictly required to hold the output answer itself).

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

This code is already optimal in terms of time and space complexity. Because the problem *requires* you to output every single one of the 2^N subsets, your code must take at least O(N * 2^N) time just to generate and write them down. A backtracking approach achieves the exact same optimal time and space complexity. No further improvement is possible.

---

## Edge Cases Handled

- **Minimum size constraint (`nums.length == 1`):** Correctly produces 2 subsets (an empty set and a set with the single element).
- **Negative numbers:** The bitwise math only cares about indices and binary flags, so negative numbers inside `nums` are treated identically to positive ones and pushed correctly.
- **Maximum constraint size (`nums.length == 10`):** The total number of subsets is 2^10 = 1024, which runs instantly without any risk of integer overflow or time limits.
