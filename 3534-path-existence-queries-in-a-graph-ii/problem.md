# 3534. Path Existence Queries in a Graph II

**Difficulty:** Hard
**Topics:** Array, Two Pointers, Binary Search, Dynamic Programming, Greedy, Bit Manipulation, Graph Theory, Sorting

## Description
You are given an integer n representing the number of nodes in a graph, labeled from 0 to n - 1.

You are also given an integer array nums of length n and an integer maxDiff.

An undirected edge exists between nodes i and j if the absolute difference between nums[i] and nums[j] is at most maxDiff (i.e., |nums[i] - nums[j]| <= maxDiff).

You are also given a 2D integer array queries. For each queries[i] = [u_i, v_i], find the minimum distance between nodes u_i and v_i_. If no path exists between the two nodes, return -1 for that query.

Return an array answer, where answer[i] is the result of the i^th query.

Note: The edges between the nodes are unweighted.

Thus, the output is [1, 2, -1, 1].

## Examples
**Example 1:**
```
Input: n = 5, nums = [1,8,3,4,2], maxDiff = 3, queries = [[0,3],[2,4]]
Output: [1,1]
Explanation:
The resulting graph is:
Thus, the output is [1, 1].
```

**Example 2:**
```
Input: n = 5, nums = [5,3,1,9,10], maxDiff = 2, queries = [[0,1],[0,2],[2,3],[4,3]]
Output: [1,2,-1,1]
Explanation:
The resulting graph is:
```

**Example 3:**
```
Input: n = 3, nums = [3,6,1], maxDiff = 1, queries = [[0,0],[0,1],[1,2]]
Output: [0,-1,-1]
Explanation:
There are no edges between any two nodes because:
Thus, no node can reach any other node, and the output is [0, -1, -1].
```

## Constraints
- 1 <= n == nums.length <= 10^5
- 0 <= nums[i] <= 10^5
- 0 <= maxDiff <= 10^5
- 1 <= queries.length <= 10^5
- queries[i] == [u_i, v_i]
- 0 <= u_i, v_i < n
