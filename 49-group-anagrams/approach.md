![Runtime](https://img.shields.io/badge/Runtime-22%20ms%20(beats%2030.99%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-25.02%20MB%20(beats%2068.41%25)-green?style=for-the-badge)

---

## Problem Explained

The goal is to group words that are **anagrams** of each other into separate lists. 

An **anagram** is a word formed by rearranging the exact same letters of another word. For example, `"eat"`, `"tea"`, and `"ate"` are all anagrams because they use the exact same letters: one 'a', one 'e', and one 't'.

You are given a list of words. You need to gather all words that share the exact same set of letters and put them together into their own group. You can return the final groups in any order.

**Example:**
* Input: `["eat", "tea", "tan", "ate", "nat", "bat"]`
* Output: `[["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]`

---

## Intuition

The "aha!" moment comes from asking: *How can we easily tell if two words are anagrams?*

If you sort the letters of any word alphabetically, all of its anagrams will turn into the exact same string!
* `"eat"` sorted becomes `"aet"`
* `"tea"` sorted becomes `"aet"`
* `"ate"` sorted becomes `"aet"`

This sorted string acts as a unique **signature** (or key) for that entire group of anagrams. 

We can use a **hash map** (a structure that stores key-value lookup pairs, like a dictionary). The **key** will be the sorted word (`"aet"`), and the **value** will be a list holding all original words that match that key (`["eat", "tea", "ate"]`).

---

## Approach

Here is how the given code solves the problem step-by-step:

1. **Create a hash map:** Define an `unordered_map` where the key is a `string` (the sorted signature) and the value is a `vector<string>` (a list of matching words).
2. **Loop through each word:** Take one word at a time from the input array `strs`.
3. **Sort the word:** Make a copy of the word and sort its letters alphabetically.
4. **Group in map:** Use the sorted word as the map key, and add the original unsorted word to that key's list.
5. **Collect results:** Create a 2D list (`vector<vector<string>>`). Loop through the map and add every list of grouped words into this final result.
6. **Return:** Output the collected groups.

---

## Time & Space Complexity

Let **$N$** be the number of strings in the array, and **$K$** be the maximum length of a string.

* **Time Complexity:** **$O(N \cdot K \log K)$**
  * We loop through $N$ strings.
  * For each string of length $K$, sorting takes $O(K \log K)$ time. 
  * Map lookups take $O(K)$ time on average to hash the string key.
* **Space Complexity:** **$O(N \cdot K)$**
  * We store all $N$ original strings of length $K$ in the hash map and the output array.

### Can this be improved?

**Yes!** We can optimize the **Time Complexity** from $O(N \cdot K \log K)$ to **$O(N \cdot K)$**.

Instead of sorting each string (which costs $O(K \log K)$), we can count character frequencies. Since the problem guarantees lowercase English letters ('a' through 'z'), we can build a string key based on the count of each character (a frequency array of size 26).

**Code snippet for the optimized loop:**

```cpp
unordered_map<string, vector<string>> mp;
for (auto &s : strs) {
    // Count frequencies of each letter 'a' through 'z'
    vector<int> count(26, 0);
    for (char c : s) count[c - 'a']++;
    
    // Convert character counts into a unique string key
    string key = "";
    for (int i = 0; i < 26; i++) {
        key += "#" + to_string(count[i]);
    }
    
    mp[key].push_back(s);
}
```

* **Improved Complexity:**
  * **Time:** **$O(N \cdot K)$** — Generating the count key takes $O(K)$ time instead of $O(K \log K)$.
  * **Space:** **$O(N \cdot K)$** — Holds the map keys and result groups.
* **Theoretical Best:** **$O(N \cdot K)$** is the absolute limit because you must read every character of every word at least once. The character-counting fix reaches this optimal performance.

---

## Edge Cases Handled

* **Empty Strings (`strs = [""]`):** An empty string sorted is still `""`. It gets stored under the key `""` without crashing or throwing errors.
* **Single Character Strings (`strs = ["a"]`):** A string of length 1 stays unchanged when sorted. It forms a single group correctly.
* **List with One Word (`strs = ["a"]`):** The code loops once, populates one key in the map, and returns a group with 1 element.
* **Duplicate Words (`strs = ["eat", "eat"]`):** Both instances sort to `"aet"` and append to the same key, resulting in `[["eat", "eat"]]`.
* **No Anagrams Present (`strs = ["abc", "def"]`):** Each word creates its own distinct sorted key, returning each word in its own single-item group.
