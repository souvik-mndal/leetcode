# 3513. Number of Unique XOR Triplets I

**Difficulty:** Medium
**Topics:** Array, Math, Bit Manipulation

## Description
You are given an integer array nums of length n, where nums is a permutation of the numbers in the range [1, n].

A XOR triplet is defined as the XOR of three elements nums[i] XOR nums[j] XOR nums[k] where i <= j <= k.

Return the number of unique XOR triplet values from all possible triplets (i, j, k).

## Examples
**Example 1:**
```
Input: nums = [1,2]
Output: 2
Explanation:
The possible XOR triplet values are:
The unique XOR values are {1, 2}, so the output is 2.
```

**Example 2:**
```
Input: nums = [3,1,2]
Output: 4
Explanation:
The possible XOR triplet values include:
The unique XOR values are {0, 1, 2, 3}, so the output is 4.
```

## Constraints
- 1 <= n == nums.length <= 10^5
- 1 <= nums[i] <= n
- nums is a permutation of integers from 1 to n.
