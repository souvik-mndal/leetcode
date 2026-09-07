![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9%20MB%20(beats%2037.49%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a big pile of letters cut out from a magazine, and you want to spell out the word "balloon" as many times as you possibly can. 

The catch is:
- You can only use the letters provided in the input string named `text`.
- Each individual letter in `text` can only be used once. You cannot reuse a single 'b' to spell "balloon" twice.

The problem asks us to return a single number: the maximum number of full "balloon" words we can build using the letters available.

For example, if `text = "nlaebolko"`, we have one 'b', one 'a', two 'l's, two 'o's, and one 'n'. That is just enough letters to form the word "balloon" exactly once. So the answer is 1. If `text = "leetcode"`, we don't even have the right letters to spell "balloon", so the answer is 0.

---

## Intuition

The core idea is simple: to make the word "balloon", we need specific letters in specific quantities. The word "balloon" breaks down into these required letters and counts per single word:
- 'b': 1 needed
- 'a': 1 needed
- 'l': 2 needed
- 'o': 2 needed
- 'n': 1 needed

If we count how many times each character appears in the entire input `text`, we can figure out how many "balloons" we can build based on that letter. For instance, if `text` has 4 'l's, since each "balloon" needs 2 'l's, those 4 'l's can support up to 4 / 2 = 2 "balloons". 

We do this math for every unique letter in "balloon" and take the smallest result among them. Why the smallest? Because that is our bottleneck. If we have enough letters to make 10 "balloons" of almost every letter, but only enough letters to make 2 "balloons" of the letter 'o', we can only ever make 2 "balloons" total before we run out of 'o's.

---

## Approach

- `unordered_map<char,int>mp;`: Creates a **hash table** (a data structure that stores key-value pairs for fast lookups) named `mp` to keep track of how many times each character appears in `text`.
- `char ch[] = {'b','a','l','o','n'};`: Declares an array of the unique characters that make up the word "balloon".
- `for( auto &it:text ){ mp[it]++; }`: Loops through every character in the input string `text` and increases its count inside our hash table `mp`.
- `int count = 1e9;`: Sets up a variable named `count` starting at a very large number (one billion) so it can safely be shrunk down by our minimum checks.
- `for( auto &it:ch ){`: Loops through each of the essential "balloon" characters one by one.
- `if( mp.find(it) == mp.end() ){ return 0; }`: Checks if one of the required characters is completely missing from our hash table. If even one required letter is missing, we cannot make a single "balloon", so we immediately return 0.
- `if( it == 'l' || it == 'o' ){ count = min( count , mp[it]/2 ); }`: For the letters 'l' and 'o', we need two of them per word, so we divide the total count of that letter in `text` by 2, and update `count` to hold the smaller value between our running total and this new limit.
- `else{ count = min( count , mp[it] ); }`: For the other required letters ('b', 'a', 'n'), we need only one per word, so we compare the full count directly against our running total.
- `return count;`: Returns the final bottleneck number, which represents the maximum number of "balloon" words we can form.

---

## Dry Run

### Case 1: Typical case (`text = "nlaebolko"`)

| `it` (Current letter) | `mp[it]` (Letter count in map) | `count` before update | `count` after update | Action |
| :--- | :--- | :--- | :--- | :--- |
| (Loop on text) | Counts all chars | - | - | Fills map: b:1, a:1, l:2, o:2, n:1, e:1, k:1 |
| `'b'` | 1 | 1000000000 | 1 | Letter 'b' needs 1, min(1000000000, 1) = 1 |
| `'a'` | 1 | 1 | 1 | Letter 'a' needs 1, min(1, 1) = 1 |
| `'l'` | 2 | 1 | 1 | Letter 'l' needs 2, min(1, 2/2) = 1 |
| `'o'` | 2 | 1 | 1 | Letter 'o' needs 2, min(1, 2/2) = 1 |
| `'n'` | 1 | 1 | 1 | Letter 'n' needs 1, min(1, 1) = 1 |
| (End) | - | - | 1 | Returns final answer: 1 |

### Case 2: Edge case with multiple words (`text = "loonbalxballpoon"`)

| `it` (Current letter) | `mp[it]` (Letter count in map) | `count` before update | `count` after update | Action |
| :--- | :--- | :--- | :--- | :--- |
| (Loop on text) | Counts all chars | - | - | Fills map: l:4, o:4, b:2, a:2, n:2, x:1 |
| `'b'` | 2 | 1000000000 | 2 | Letter 'b' needs 1, min(1000000000, 2) = 2 |
| `'a'` | 2 | 2 | 2 | Letter 'a' needs 1, min(2, 2) = 2 |
| `'l'` | 4 | 2 | 2 | Letter 'l' needs 2, min(2, 4/2) = 2 |
| `'o'` | 4 | 2 | 2 | Letter 'o' needs 2, min(2, 4/2) = 2 |
| `'n'` | 2 | 2 | 2 | Letter 'n' needs 1, min(2, 2) = 2 |
| (End) | - | - | 2 | Returns final answer: 2 |

---

## Time & Space Complexity

**Time:** O(N) — where N is the length of the string `text`. We loop through the string once to count all the characters, which takes O(N) time. Then we loop a fixed number of 5 times for the characters in "balloon", which takes constant time. Thus, the overall time is driven entirely by reading the input string.

**Space:** O(1) — because the input only ever consists of lowercase English letters. Our hash table will hold at most 26 keys (one for each letter of the alphabet), which is a constant upper limit that never grows no matter how long the input string gets.

**Is this already optimal?**
Yes, this is already the most optimal possible time and space complexity for this problem. 
- **Why?** To count the letters, we must look at every character in the input string at least once, which requires O(N) time. You cannot read a string faster than looking at all of its characters. Similarly, tracking a fixed alphabet size of 26 letters uses O(1) constant space. No further improvement is possible.

---

## Edge Cases Handled

- **Missing required letters:** If the text does not contain even one of the required letters ('b', 'a', 'l', 'o', or 'n'), `mp.find(it) == mp.end()` triggers and safely returns 0 immediately.
- **Extra unrelated letters:** Letters like 'x', 'e', or 'k' that are not part of "balloon" are counted into the hash table during the first loop, but they are safely ignored during the calculation loop because they are not checked.
- **Minimum string length (length 1):** If `text` has a length of 1 (e.g., `"a"`), it will correctly find that required letters are missing or insufficient, and return 0.
- **Repeated letters needing division:** Handles words where 'l' and 'o' appear in even counts, ensuring we divide by 2 so we do not overestimate how many words can be formed.
