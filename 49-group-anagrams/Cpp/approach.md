![Runtime](https://img.shields.io/badge/Runtime-16%20ms%20(beats%2059.16%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-25.1%20MB%20(beats%2072.42%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a big pile of words. Some words are made out of the exact same letters, just scrambled around. For example, the letters in "eat" can be rearranged to spell "tea" or "ate". 

This problem asks you to take a list of strings and sort them into separate groups. Every word in a single group must be an anagram of the others—meaning they share the exact same letters in the exact same quantities. You can return the groups in any order you like.

For instance, if your input list is ["eat", "tea", "tan", "ate", "nat", "bat"], your output should group the matching ones together: ["bat"] in its own group, ["nat", "tan"] together, and ["ate", "eat", "tea"] in another group.

## Intuition

If you have a pile of scrambled words, how do you instantly know which ones belong together? 

The **"aha" moment** is this: if you sort the letters of any word alphabetically, all of its anagrams will turn into the exact same word. 
- "eat" sorted becomes "aet"
- "tea" sorted becomes "aet"
- "ate" sorted becomes "aet"

Because they all turn into "aet", we can use that sorted version as a **key** (like a label on a filing cabinet drawer) in a **hash table** (a data structure that lets us quickly look up values using a label). Whenever we look at a word, we sort its letters, check our hash table for that sorted key, and toss the original word into that key's matching bucket. Once we process all the words, we just collect all the buckets.

## Approach

Here is the step-by-step breakdown of how the code makes this happen:

* `unordered_map<string,vector<string>>mp;`: Creates a hash map named `mp` where the key is a sorted string (like `"aet"`) and the value is a list of original words that match that key (like `["eat", "tea", "ate"]`).
* `for( auto &it:strs ):`: Starts a loop that looks at every single word in our input list `strs` one by one, using `it` to represent the current word.
* `string temp = it;`: Makes a copy of the current word and stores it in a new variable named `temp`. We do this so we can scramble or sort `temp` without ruining the original word `it`.
* `sort(temp.begin() , temp.end());`: Sorts the letters inside `temp` alphabetically (turning `"tea"` into `"aet"`).
* `mp[temp].push_back(it);`: Uses our sorted string `temp` as the map key, and adds the original word `it` into the list of words stored at that key.
* `vector<vector<string>>str;`: Creates a new 2D list named `str` that will hold our final grouped answer.
* `for( auto &it:mp ):`: Loops through every key-value pair stored inside our hash map `mp`.
* `str.push_back(it.second);`: Takes just the list of grouped words (found in `it.second`) and adds it into our final answer list `str`.
* `return str;`: Sends the final grouped answer back out.

## Dry Run

### Case 1: Typical case with multiple matches
**Input:** `strs = ["eat", "tea", "bat"]`

| `it` (Current word) | `temp` (Before sort) | `temp` (After sort) | Hash Map `mp` state after step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `"eat"` | `"eat"` | `"aet"` | `{"aet": ["eat"]}` | Sorted `"eat"` to `"aet"`, created new bucket in `mp`. |
| `"tea"` | `"tea"` | `"aet"` | `{"aet": ["eat", "tea"]}` | Sorted `"tea"` to `"aet"`, added original word to existing bucket. |
| `"bat"` | `"bat"` | `"abt"` | `{"aet": ["eat", "tea"], "abt": ["bat"]}` | Sorted `"bat"` to `"abt"`, created a brand new bucket. |

### Case 2: Edge case with a single empty string
**Input:** `strs = [""]`

| `it` (Current word) | `temp` (Before sort) | `temp` (After sort) | Hash Map `mp` state after step | Action |
| :--- | :--- | :--- | :--- | :--- |
| `""` | `""` | `""` | `{"" : [""]}` | Sorted empty string remains empty, placed into an empty-string key bucket. |

## Time & Space Complexity

- **Time:** O(N * K log K) — Here, N is the number of strings in the array, and K is the maximum length of a single string. For each of the N strings, we sort its characters, which takes O(K log K) time. Inserting and looking things up in the hash map takes O(1) time on average.
- **Space:** O(N * K) — We store every string from the input inside our hash map `mp` and our final output vector `str`.

### Is this optimal?

**No, it can be optimized slightly.** 

Right now, we spend time sorting the characters of each string, which takes O(K log K) time. We can completely remove the sorting step by instead counting how many times each letter appears in a word (for example, keeping track that "eat" has one 'a', one 'e', and one 't'). We can turn that letter-count into a unique string key or tuple. Because words have a maximum length of 100 characters, counting letters takes O(K) time instead of O(K log K) time.

Here is how you can write that optimized counting logic:

```cpp
string count = "";
for (int i = 0; i < 26; i++) {
    count += "#" + to_string(charCount[i]);
}
mp[count].push_back(it);
```
*(In this snippet, we tally up the 26 lowercase English letters into an array `charCount`, then stitch those counts together into a unique signature string to use as our hash map key).*

- **Resulting improved complexity:** O(N * K) time.
- **Theoretical best possible complexity:** O(N * K) time, because you must at least look at every character of every string to group them. Our optimization successfully reaches the theoretical limit.

## Edge Cases Handled

- **Empty strings (`""`):** Handled safely. An empty string sorts to an empty string, creating its own bucket with `[""]` inside.
- **Single character strings (`"a"`):** Handled smoothly. It sorts to itself and forms a single-item group.
- **Duplicate strings:** If the input contains identical strings (like `["eat", "eat"]`), they sort to the exact same key and are correctly bundled into the same vector list.
- **Words with no anagrams:** Words that have no matches (like `"bat"`) simply create a hash map entry containing a list with just themselves, which outputs correctly.
