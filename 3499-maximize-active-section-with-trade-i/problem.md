# 3499. Maximize Active Section with Trade I

**Difficulty:** Medium
**Topics:** String, Enumeration

## Description
You are given a binary string s of length n, where:

You can perform at most one trade to maximize the number of active sections in s. In a trade, you:

Return the maximum number of active sections in s after making the optimal trade.

Note: Treat s as if it is augmented with a '1' at both ends, forming t = '1' + s + '1'. The augmented '1's do not contribute to the final count.

## Examples
**Example 1:**
```
Input: s = "01"
Output: 1
Explanation:
Because there is no block of '1's surrounded by '0's, no valid trade is possible. The maximum number of active sections is 1.
```

**Example 2:**
```
Input: s = "0100"
Output: 4
Explanation:
```

**Example 3:**
```
Input: s = "1000100"
Output: 7
Explanation:
```

**Example 4:**
```
Input: s = "01010"
Output: 4
Explanation:
```

## Constraints
- '1' represents an active section.
- '0' represents an inactive section.
- Convert a contiguous block of '1's that is surrounded by '0's to all '0's.
- Afterward, convert a contiguous block of '0's that is surrounded by '1's to all '1's.
- 1 <= n == s.length <= 10^5
- s[i] is either '0' or '1'
