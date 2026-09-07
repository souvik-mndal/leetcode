![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.6%20MB%20(beats%2051.28%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a secret code for words. A "pattern" is a word made of letters, like "abb". The letters tell you a rule for how characters must map to each other:
- The first letter ('a') maps to some new letter.
- The second letter ('b') maps to a different new letter.
- The third letter ('b') must map to the exact same letter that 'b' just mapped to, because the character repeats.

The main rule is a **bijection**, which is just a fancy way of saying: every letter must map to one and only one letter, and no two different letters can map to the same letter. For example, "abc" matches "abb" if 'a' goes to 'm' and 'b' goes to 'e'. But "ccc" does *not* match "abb" because both 'a' and 'b' would have to point to 'c', which breaks the rule that different pattern letters cannot share the same target.

We are given a list of words and a single pattern. Our job is to look through the list and return only the words that follow the exact same structural pattern.

## Intuition

The core trick here is **normalization** (or canonical form). Instead of trying to compare every word directly to the pattern while keeping track of complex mapping rules on the fly, we can translate *both* the pattern and every word into a universal shape. 

If we convert every string into a standard format where the first unique letter encountered becomes 'a', the second unique letter becomes 'b', the third becomes 'c', and so on, then matching becomes as simple as checking for text equality! 
- The pattern "abb" becomes "abc" (first unique letter 'a' becomes 'a', second unique letter 'b' becomes 'b', third unique letter 'b' becomes 'c'). Wait, let's trace: 'a' -> 'a', 'b' -> 'b', second 'b' -> 'b'. So "abb" normalizes to "abb".
- The word "mee" normalizes: 'm' -> 'a', 'e' -> 'b', second 'e' -> 'b'. So "mee" also normalizes to "abb".
- The word "ccc" normalizes: 'c' -> 'a', second 'c' -> 'a', third 'c' -> 'a'. So "ccc" normalizes to "aaa".

Since "abb" does not equal "aaa", "ccc" fails. By converting everything into this standard alphabet-indexed format first, we turn a tricky mapping problem into a basic string comparison.

## Approach

- `string change( string s )`: This helper function takes a string and translates it into its normalized form using a hash table.
- `char ch = 'a';`: Initializes a character tracker starting at 'a', which will be assigned to each new unique letter we encounter.
- `unordered_map<char,char>mp;`: Creates a hash table (a lookup dictionary) to remember which original letter maps to which normalized replacement character.
- `if( mp.find(it) == mp.end()){ mp[it] = ch++; }`: Inside a loop over each character in the string, if we see a character for the first time, we map it to the current value of `ch` and then increment `ch` so the next unique letter gets the next alphabet character.
- `for( int i=0 ; i<s.size() ; i++ ){ s[i] = mp[s[i]]; }`: Replaces every character in the string with its mapped version from our hash table, then returns the transformed string.
- `pattern = change(pattern);`: Normalizes the target pattern right at the start of `findAndReplacePattern`.
- `vector<string>chk;`: Prepares a list to store the normalized versions of all the input words.
- `string temp = change(it); chk.push_back(temp);`: Loops through every word in `words`, normalizes it using our helper function, and saves it into `chk`.
- `if( chk[i].compare(pattern) == 0 ){ ans.push_back(words[i]); }`: Compares each normalized word against the normalized pattern. If they match completely, the original word is added to our final answer list.

## Dry Run

### Case 1: Typical matching case
Inputs: `words = ["mee", "ccc"]`, `pattern = "abb"`

| Step / Action | `pattern` / `words[i]` | `ch` | `mp` contents | `chk` / `ans` |
| :--- | :--- | :--- | :--- | :--- |
| Call `change("abb")` | `"abb"` | `'a'` -> `'c'` | `{'a':'a', 'b':'b'}` | `pattern` becomes `"abb"` |
| Process word 0: `"mee"` | `"mee"` | `'a'` -> `'c'` | `{'m':'a', 'e':'b'}` | `chk` adds `"abb"` |
| Process word 1: `"ccc"` | `"ccc"` | `'a'` -> `'b'` | `{'c':'a'}` | `chk` adds `"aaa"` |
| Compare `chk[0]` to `pattern` | `"abb"` vs `"abb"` | - | - | Match! `ans` adds `"mee"` |
| Compare `chk[1]` to `pattern` | `"aaa"` vs `"abb"` | - | - | No match. |

### Case 2: Single letter edge case
Inputs: `words = ["a", "b"]`, `pattern = "a"`

| Step / Action | `pattern` / `words[i]` | `ch` | `mp` contents | `chk` / `ans` |
| :--- | :--- | :--- | :--- | :--- |
| Call `change("a")` | `"a"` | `'a'` -> `'b'` | `{'a':'a'}` | `pattern` becomes `"a"` |
| Process word 0: `"a"` | `"a"` | `'a'` -> `'b'` | `{'a':'a'}` | `chk` adds `"a"` |
| Process word 1: `“b”` | `"b"` | `'a'` -> `'b'` | `{'b':'a'}` | `chk` adds `"a"` |
| Compare `chk[0]` & `chk[1]` | `"a"`, `"a"` vs `"a"` | - | - | Both match! `ans` adds `"a"`, `"b"` |

## Time & Space Complexity

- **Time:** O(W * L) — where W is the number of words and L is the length of the pattern (and words). For each word and the pattern, we loop through its length L to build the hash map and transform the string, which takes O(L) time per string. Comparing the transformed strings also takes O(L) time. With W words, total time scales linearly with the total number of characters across all words and the pattern.
- **Space:** O(L) — the hash map (`mp`) only stores up to 26 key-value pairs because the input is restricted to lowercase English letters. Thus, the extra space used for normalization per string is bounded by a constant size (at most 26), plus the space for storing the transformed strings.

### Is this optimal?
Yes, this is already optimal. To determine if a word matches the pattern, you must inspect every character in the pattern and every word at least once, meaning any correct algorithm must take at least O(W * L) time. Our approach processes each character a constant number of times and uses minimal extra space. No further improvement is possible.

## Edge Cases Handled

- **Single character strings:** Handled correctly because the loop runs once, assigns `'a'`, and successfully compares 1-letter strings.
- **Completely unique characters:** (e.g., pattern `"abc"`, word `"deg"`) Normalizes to `"abc"` for both, matching correctly.
- **All identical characters:** (e.g., pattern `"aaa"`, word `"ccc"`) Normalizes pattern to `"aaa"` and word to `"aaa"`, matching correctly while rejecting mismatched structures like `"abb"` vs `"ccc"`.
- **Constraint boundaries:** Since word lengths are up to 20 and word counts up to 50, operations complete well within time limits without risking integer overflows or excessive memory usage.
