![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.7%20MB%20(beats%2070.70%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a big container of smaller word pieces, which we call **patterns**. You also have a single main text called a **word**. Your job is to check every single piece in your container and count how many of them can be found hiding inside the main word as a **substring**. 

A substring means a chunk of characters that appear right next to each other in a continuous block, without any breaks. For example, if your main word is "abc", then "a", "b", "c", "ab", "bc", and "abc" are all valid substrings, but "ac" is not because the letters are split apart. You just need to count how many items from your patterns list exist somewhere inside that word.

## Intuition

The core idea is simple: go through the list of patterns one by one, and for each pattern, ask C++ to search for it inside the main word. C++ has a built-in string search function that can instantly tell us if a smaller piece lives inside a larger string. Every time we find a match, we add one to our running counter. 

For someone revisiting this code, the key takeaway is that we do not need any fancy data structures like trees or hash tables. The constraints are tiny—both the patterns list and the word lengths are at most 100 characters. Because the data is so small, a straightforward brute-force check runs instantly and easily beats almost everything else.

## Approach

* `int count = 0;`: Creates a variable named `count` starting at zero to keep track of how many matching patterns we find.
* `for( int i=0 ; i<patterns.size() ; i++ )`: Starts a loop using the index variable `i` that visits every single string inside the `patterns` list from the first one to the last one.
* `if( word.find(patterns[i]) != string::npos )`: Uses the built-in `find` method to look for the current pattern inside `word`. If `find` successfully locates the pattern, it returns the starting index of where it was found. If it fails, it returns a special constant value called `string::npos`, which means "not found." Checking that it does not equal `string::npos` means "yes, the pattern was found."
* `count++;`: Increases our running total by one whenever the `if` condition confirms the pattern exists inside the word.
* `return count;`: Sends the final total back after checking all the patterns in the list.

## Dry Run

### Case 1: Typical case with multiple matches
**Inputs:** `patterns = ["a","abc","bc","d"]`, `word = "abc"`

| i | patterns[i] | word.find(patterns[i]) | Action | count |
|---|---|---|---|---|
| 0 | "a" | 0 | "a" is found at index 0 (not npos). Increment count. | 1 |
| 1 | "abc" | 0 | "abc" is found at index 0 (not npos). Increment count. | 2 |
| 2 | "bc" | 1 | "bc" is found at index 1 (not npos). Increment count. | 3 |
| 3 | "d" | string::npos | "d" is not found (returns npos). Do nothing. | 3 |

*Final output:* `3`

### Case 2: Case with duplicate patterns
**Inputs:** `patterns = ["a","a","a"]`, `word = "ab"`

| i | patterns[i] | word.find(patterns[i]) | Action | count |
|---|---|---|---|---|
| 0 | "a" | 0 | "a" is found at index 0 (not npos). Increment count. | 1 |
| 1 | "a" | 0 | "a" is found at index 0 (not npos). Increment count. | 2 |
| 2 | "a" | 0 | "a" is found at index 0 (not npos). Increment count. | 3 |

*Final output:* `3`

## Time & Space Complexity

**Time:** `O(P * W)` where `P` is the number of strings in `patterns` and `W` is the length of `word`. 
*Why:* For each of the `P` patterns, the `word.find()` function scans through the `word` string of length `W` in the worst case. Given the tiny constraint that both `P` and `W` are at most 100, this takes a tiny fraction of a millisecond.

**Space:** `O(1)` auxiliary space.
*Why:* We only store a single integer variable `count`, regardless of how many patterns we are given. We do not create any new data structures or copy strings.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is already extremely efficient for the given constraints. However, from a strict theoretical computer science view, string searching functions like `word.find()` use a basic substring scan which can be slow if the strings are massive. 

If `patterns` and `word` were thousands of times larger, we could optimize the time complexity by inserting all possible substrings of `word` into a Hash Set, or by building a suffix tree/trie. But because the constraints limit every string length to a maximum of 100 characters, those advanced algorithms add overhead and complexity for zero practical gain. For this problem size, this straightforward solution is already optimal in practice and hits the best possible runtime bracket.

## Edge Cases Handled

* **Duplicate patterns:** The code checks every index independently, so if the input has the exact same pattern three times and it matches, it counts it three times correctly.
* **Patterns longer than the word:** If a pattern has 10 characters and the word only has 3, `word.find()` safely returns `string::npos` without crashing.
* **Single-character inputs:** Works correctly when both the pattern and the word are only one character long.
