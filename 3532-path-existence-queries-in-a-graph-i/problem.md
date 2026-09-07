# 3532. Path Existence Queries in a Graph I

**Difficulty:** Medium
**Topics:** Array, Hash Table, Binary Search, Union-Find, Graph Theory

## Description
You are given an integer n representing the number of nodes in a graph, labeled from 0 to n - 1.

You are also given an integer array nums of length n sorted in non-decreasing order, and an integer maxDiff.

An undirected edge exists between nodes i and j if the absolute difference between nums[i] and nums[j] is at most maxDiff (i.e., |nums[i] - nums[j]| <= maxDiff).

You are also given a 2D integer array queries. For each queries[i] = [u_i, v_i], determine whether there exists a path between nodes u_i and v_i.

Return a boolean array answer, where answer[i] is true if there exists a path between u_i and v_i in the i^th query and false otherwise.

## Examples
**Example 1:**
```
Input: n = 2, nums = [1,3], maxDiff = 1, queries = [[0,0],[0,1]]
Output: [true,false]
Explanation:
```

**Example 2:**
```
Input: n = 4, nums = [2,5,6,8], maxDiff = 2, queries = [[0,1],[0,2],[1,3],[2,3]]
Output: [false,false,true,true]
Explanation:
The resulting graph is:
```

## Constraints
- 1 <= n == nums.length <= 10^5
- 0 <= nums[i] <= 10^5
- nums is sorted in non-decreasing order.
- 0 <= maxDiff <= 10^5
- 1 <= queries.length <= 10^5
- queries[i] == [u_i, v_i]
- 0 <= u_i, v_i < n
