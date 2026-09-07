# 1967. Number of Strings That Appear as Substrings in Word

**Difficulty:** Easy
**Topics:** Array, String

## Description
Given an array of strings patterns and a string word, return the number of strings in patterns that exist as a substring in word.

A substring is a contiguous sequence of characters within a string.

## Examples
**Example 1:**
```
Input: patterns = [&quot;a&quot;,&quot;abc&quot;,&quot;bc&quot;,&quot;d&quot;], word = &quot;abc&quot;
Output: 3
Explanation:
- &quot;a&quot; appears as a substring in &quot;abc&quot;.
- &quot;abc&quot; appears as a substring in &quot;abc&quot;.
- &quot;bc&quot; appears as a substring in &quot;abc&quot;.
- &quot;d&quot; does not appear as a substring in &quot;abc&quot;.
3 of the strings in patterns appear as a substring in word.
```

**Example 2:**
```
Input: patterns = [&quot;a&quot;,&quot;b&quot;,&quot;c&quot;], word = &quot;aaaaabbbbb&quot;
Output: 2
Explanation:
- &quot;a&quot; appears as a substring in &quot;aaaaabbbbb&quot;.
- &quot;b&quot; appears as a substring in &quot;aaaaabbbbb&quot;.
- &quot;c&quot; does not appear as a substring in &quot;aaaaabbbbb&quot;.
2 of the strings in patterns appear as a substring in word.
```

**Example 3:**
```
Input: patterns = [&quot;a&quot;,&quot;a&quot;,&quot;a&quot;], word = &quot;ab&quot;
Output: 3
Explanation: Each of the patterns appears as a substring in word &quot;ab&quot;.
```

## Constraints
- 1 <= patterns.length <= 100
- 1 <= patterns[i].length <= 100
- 1 <= word.length <= 100
- patterns[i] and word consist of lowercase English letters.
