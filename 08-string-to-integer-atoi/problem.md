# 8. String to Integer (atoi)

**Difficulty:** Medium
**Topics:** String

## Description
Implement the myAtoi(string s) function, which converts a string to a 32-bit signed integer.

The algorithm for myAtoi(string s) is as follows:

Return the integer as the final result.

## Examples
**Example 1:**
```
Input: s = "42"
Output: 42
Explanation:
```

**Example 2:**
```
Input: s = " -042"
Output: -42
Explanation:
```

**Example 3:**
```
Input: s = "1337c0d3"
Output: 1337
Explanation:
```

**Example 4:**
```
Input: s = "0-1"
Output: 0
Explanation:
```

**Example 5:**
```
Input: s = "words and 987"
Output: 0
Explanation:
Reading stops at the first non-digit character 'w'.
```

## Constraints
- 0 <= s.length <= 200
- s consists of English letters (lower-case and upper-case), digits (0-9), ' ', '+', '-', and '.'.
