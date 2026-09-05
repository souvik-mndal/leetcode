# 4036. Lexicographically Largest String After Pair Transformations

**Difficulty:** Medium
**Topics:** N/A

## Description
You are given an integer array nums.

For each integer x in nums, start with a string consisting of exactly x lowercase 'a' characters.

You may perform the following operation any number of times (including zero):

For example, "aa" can be replaced with "b", and "bb" can be replaced with "c". The pair "zz" cannot be replaced.

For each x, determine the lexicographically largest string that can be obtained.

Return an array of strings where the i^th string is the answer for nums[i].

A string a is lexicographically larger than a string b if, at the first position where they differ, a contains a letter that appears later in the alphabet than the corresponding letter in b. If the first min(a.length, b.length) characters are equal, the longer string is lexicographically larger.

## Examples
**Example 1:**
```
Input: nums = [2,5,7]
Output: ["b","ca","cba"]
Explanation:
```

**Example 2:**
```
Input: nums = [3,9,1]
Output: ["ba","da","a"]
Explanation:
```

## Constraints
- Choose two adjacent equal letters and replace them with the next letter in the alphabet.
- 1 <= nums.length <= 10^5
- 1 <= nums[i] <= 10^8
