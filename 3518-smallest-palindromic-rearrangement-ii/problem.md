# 3518. Smallest Palindromic Rearrangement II

**Difficulty:** Hard
**Topics:** Hash Table, Math, String, Combinatorics, Counting

## Description
Return the k-th lexicographically smallest palindromic permutation of s. If there are fewer than k distinct palindromic permutations, return an empty string.

Note: Different rearrangements that yield the same palindromic string are considered identical and are counted once.

## Examples
**Example 1:**
```
Input: s = "abba", k = 2
Output: "baab"
Explanation:
```

**Example 2:**
```
Input: s = "aa", k = 2
Output: ""
Explanation:
```

**Example 3:**
```
Input: s = "bacab", k = 1
Output: "abcba"
Explanation:
```

## Constraints
- 1 <= s.length <= 10^4
- s consists of lowercase English letters.
- s is guaranteed to be palindromic.
- 1 <= k <= 10^6
