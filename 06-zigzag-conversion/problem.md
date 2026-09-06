# 6. Zigzag Conversion

**Difficulty:** Medium
**Topics:** String

## Description
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

And then read line by line: "PAHNAPLSIIGYIR"

Write the code that will take a string and make this conversion given a number of rows:

## Examples
**Example 1:**
```
P A H N
A P L S I I G
Y I R
```

**Example 2:**
```
string convert(string s, int numRows);
```

**Example 3:**
```
Input: s = &quot;PAYPALISHIRING&quot;, numRows = 3
Output: &quot;PAHNAPLSIIGYIR&quot;
```

**Example 4:**
```
Input: s = &quot;PAYPALISHIRING&quot;, numRows = 4
Output: &quot;PINALSIGYAHRPI&quot;
Explanation:
P I N
A L S I G
Y A H R
P I
```

**Example 5:**
```
Input: s = &quot;A&quot;, numRows = 1
Output: &quot;A&quot;
```

## Constraints
- 1 <= s.length <= 1000
- s consists of English letters (lower-case and upper-case), ',' and '.'.
- 1 <= numRows <= 1000
