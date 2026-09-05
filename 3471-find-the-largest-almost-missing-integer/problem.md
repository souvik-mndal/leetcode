# 3471. Find the Largest Almost Missing Integer

**Difficulty:** Easy
**Topics:** Array, Hash Table

## Description
You are given an integer array nums and an integer k.

An integer x is almost missing from nums if x appears in exactly one subarray of size k within nums.

Return the largest almost missing integer from nums. If no such integer exists, return -1.

## Examples
**Example 1:**
```
Input: nums = [3,9,2,1,7], k = 3
Output: 7
Explanation:
We return 7 since it is the largest integer that appears in exactly one subarray of size k.
```

**Example 2:**
```
Input: nums = [3,9,7,2,1,7], k = 4
Output: 3
Explanation:
We return 3 since it is the largest and only integer that appears in exactly one subarray of size k.
```

**Example 3:**
```
Input: nums = [0,0], k = 1
Output: -1
Explanation:
There is no integer that appears in only one subarray of size 1.
```

## Constraints
- 1 <= nums.length <= 50
- 0 <= nums[i] <= 50
- 1 <= k <= nums.length
