![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.1%20MB%20(beats%2098.91%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to determine if two strings, `s` and `t`, of the exact same length, are **isomorphic**. 

Two strings are isomorphic if every character in the first string can be systematically replaced to form the second string. This replacement must follow two strict rules:
1. **One-to-one mapping:** Every occurrence of a specific character in `s` must always map to the exact same character in `t`.
2. **No collisions:** Two different characters from `s` are not allowed to map to the same character in `t`. 

For example, if `s` is "egg" and `t` is "add", they are isomorphic because 'e' maps to 'a' and 'g' maps to 'd'. But if `s` is "f11" and `t` is "b23", they are not isomorphic because the character '1' in `s` would need to map to both '2' and '3' in `t`, which breaks the rules.

## Intuition

To check if two strings map to each other correctly, we need to track two things simultaneously: what each character in `s` is turning into, and which characters in `t` have already been claimed. 

The "aha" moment is using the ASCII character set to our advantage. Instead of using a heavy search-based map, we can use fixed-size lookup tables (like arrays) because standard ASCII has 256 possible characters. 
- We use an array called `mp` to record the mapping from each character in `s` to its partner in `t`.
- We use a boolean array called `chk` to make sure no two different characters in `s` accidentally point to the same target character in `t`.

By splitting the logic into two passes (or phases), we first build and enforce the mapping rules, and then we verify that the strings actually match those rules without any contradictions.

## Approach

- `vector<int>mp(256,0);` creates a translation table sized for all 256 ASCII characters, initialized to zero, to store which character from `t` each character in `s` maps to.
- `vector<bool>chk(256,0);` creates a tracking table initialized to false to record which characters in `t` have already been claimed by a mapping.
- `for( int i=0 ; i<s.size() ; i++ ){` starts a loop that looks at every character position in the strings from left to right.
- `if( mp[s[i]]==0 && chk[t[i]]==0 ){` checks if the current character in `s` has no mapping yet **and** the target character in `t` has not been claimed yet.
- `mp[s[i]] = t[i];` records the new mapping so that the character from `s` now points to the character from `t`.
- `chk[t[i]]=true;` marks the target character in `t` as officially claimed so no other character can map to it.
- `for( int i=0 ; i<s.size() ; i++ ){` starts a second loop to verify the entire string from the beginning.
- `if( mp[s[i]] != t[i] ){ return false; }` checks if the established mapping for the character in `s` matches the actual character in `t`. If there is a mismatch, it immediately rejects the strings and returns false.
- `return true;` finishes the function by returning true if every character pair successfully passes the verification check.

## Dry Run

### Case 1: Typical case (`s = "egg"`, `t = "add"`)

| i | s[i] | t[i] | mp[s[i]] before | chk[t[i]] before | Action |
|---|---|---|---|---|---|
| 0 | 'e' | 'a' | 0 | false | Unmapped. Set mp['e'] = 'a' and chk['a'] = true. |
| 1 | 'g' | 'd' | 0 | false | Unmapped. Set mp['g'] = 'd' and chk['d'] = true. |
| 2 | 'g' | 'd' | 'd' | true | Already mapped. First loop skips making changes. Second loop verifies matches. Returns true at the end. |

### Case 2: Edge case with collision (`s = "f11"`, `t = "b23"`)

| i | s[i] | t[i] | mp[s[i]] before | chk[t[i]] before | Action |
|---|---|---|---|---|---|
| 0 | 'f' | 'b' | 0 | false | Unmapped. Set mp['f'] = 'b' and chk['b'] = true. |
| 1 | '1' | '2' | 0 | false | Unmapped. Set mp['1'] = '2' and chk['2'] = true. |
| 2 | '1' | '3' | '2' | false | Character '1' already maps to '2', but loop 2 checks `mp['1']` against `t[2]` ('3'). They do not match (`'2' != '3'`). Returns false. |

## Time & Space Complexity

- **Time:** O(N) — where N is the length of the string. The code loops through the strings twice. Each loop runs N times, and array lookups take constant time.
- **Space:** O(1) — the memory usage does not grow with the size of the input strings because the `mp` and `chk` arrays are always fixed at a size of 256, regardless of how long `s` and `t` are.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already at the optimal possible complexity. 

- **Time complexity is O(N):** You must look at every character in the strings at least once to determine if they match the isomorphic rules, meaning any solution must take at least linear time.
- **Space complexity is O(1):** Because the character set is restricted to fixed-size ASCII (256 possibilities), storing our mapping and check tables requires a constant amount of memory that never scales up with larger inputs. 

No further improvements in asymptotic time or space are possible.

## Edge Cases Handled

- **Single character strings:** Handles strings of length 1 correctly because the loops evaluate the single index and pass safely.
- **Already mapped duplicate characters:** Correctly handles repeating characters in `s` (like the double 'g' in "egg") by ensuring they map consistently to their previously assigned partner without triggering a collision.
- **Many-to-one mapping violations:** Successfully catches cases where two different characters in `s` try to point to the exact same character in `t`, because the `chk` boolean array prevents a target character from being claimed twice.
