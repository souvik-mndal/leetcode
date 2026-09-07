# 3756. Concatenate Non-Zero Digits and Multiply by Sum II

**Difficulty:** Medium
**Topics:** Math, String, Prefix Sum

## Description
You are given a string s of length m consisting of digits. You are also given a 2D integer array queries, where queries[i] = [l_i, r_i].

For each queries[i], extract the substring s[l_i..r_i]. Then, perform the following:

Return an array of integers answer where answer[i] is the answer to the i^th query.

Since the answers may be very large, return them modulo 10^9 + 7.

## Examples
**Example 1:**
```
Input: s = "10203004", queries = [[0,7],[1,3],[4,6]]
Output: [12340, 4, 9]
Explanation:
```

**Example 2:**
```
Input: s = "1000", queries = [[0,3],[1,1]]
Output: [1, 0]
Explanation:
```

**Example 3:**
```
Input: s = "9876543210", queries = [[0,9]]
Output: [444444137]
Explanation:
```

## Constraints
- Form a new integer x by concatenating all the non-zero digits from the substring in their original order. If there are no non-zero digits, x = 0.
- Let sum be the sum of digits in x. The answer is x * sum.
- 1 <= m == s.length <= 10^5
- s consists of digits only.
- 1 <= queries.length <= 10^5
- queries[i] = [l_i, r_i]
- 0 <= l_i <= r_i < m
