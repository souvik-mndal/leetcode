![Runtime](https://img.shields.io/badge/Runtime-84%20ms%20(beats%2055.17%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-122.6%20MB%20(beats%2097.95%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of positive numbers. You want to pick a special group of numbers from this list and arrange them in a very specific pattern that goes up and then back down symmetrically. 

The pattern looks like a mountain: it starts at some number `x`, goes up through its squares (`x^2`, `x^4`, `x^8`, and so on), reaches a peak at the very top, and then comes back down in the exact reverse order (`x^8`, `x^4`, `x^2`, `x`). 

For example, if you pick `2`, `4`, and `2`, you can arrange them as `[2, 4, 2]`. This works because `2^2 = 4`, matching the peak. Another valid arrangement is `[3, 9, 3]` because `3^2 = 9`. 

Your goal is to find the largest possible group (subset) of numbers from your starting list that can form this mountain pattern, and return how many numbers are in that group.

---

## Intuition

The core trick to solving this is recognizing that any valid mountain pattern is completely defined by its starting number `x` and how many times we can square it. 

If we have a number `x`, the next step up the mountain is `x^2`, then `x^4`, then `x^8`, and so on. To build a valid mountain that goes up and back down, we need pairs of almost every number to form the sides of the mountain, plus one extra number at the very peak if the path has to stop. 

To make our code efficient, we first count how many times each number appears in our input list using a **hash table** (a data structure that lets us look up items instantly by their value, named `mp` in our code). 

Once we have our counts, we can test every unique number in our map as a potential starting point `x`. We check if its square is also in the map, and if we have at least two of them (so one can go on the way up, and one on the way down). We keep squaring our number and climbing up as long as we find pairs. If we ever find only one copy of a number, we hit the peak of the mountain, add that single number to our count, and stop climbing. 

There is one special exception: the number `1`. Because `1 * 1 = 1`, squaring `1` never makes it bigger. If `1` is in our list, we can just grab as many `1`s as we want to form a flat mountain, so we handle `1` as a special case right away.

---

## Approach

* `unordered_map<long long , int >mp;` — Creates a hash map named `mp` to store every unique number from the input as a key and its frequency (how many times it appears) as the value. We use `long long` for the key because squaring numbers quickly results in very large integers.
* `for( auto &it:nums ){ mp[it]++; }` — Loops through every number in the input array `nums` and increases its count in the map `mp`.
* `int maxi = 0;` — Initializes a variable named `maxi` to track the maximum valid subset length found so far, starting at zero.
* `for( auto &it:mp ){` — Loops through each unique number and its count stored in the map `mp` to test each one as a starting point for our mountain pattern.
* `int count = 0;` — Resets a local variable named `count` to zero for the current starting number.
* `long long curr = it.first;` — Sets a tracking variable named `curr` to the current number being tested from the map.
* `while( mp.find(curr) != mp.end() ){` — Starts a loop that continues as long as our current number `curr` exists in the map.
* `if( curr == 1 ){ count = it.second; maxi = max(count , maxi); break; }` — Handles the special case for the number `1`. Since `1` squared is always `1`, we cannot square our way out of it. We take all available copies of `1`, update our maximum length, and stop looking for this path.
* `if( mp[curr] >= 2 ){ count+=2; curr = curr * curr; }` — Checks if we have at least two copies of the current number. If we do, we add `2` to our length (one for the way up, one for the way down) and square the number to move up to the next level of the mountain.
* `else{ count++; maxi = max(count , maxi); break; }` — Runs if we only have a single copy of the current number. This means we have reached the peak of our mountain. We add `1` to our count, update our maximum length, and stop climbing.
* `maxi = max(count , maxi);` — Updates `maxi` with the largest subset length found after each step of the climb.
* `if( !(maxi & 1) ){ return maxi-1; }` — Checks if the final maximum length is an even number using a bitwise operation (`!(maxi & 1)` checks if `maxi` is even). A valid mountain pattern must have an odd total length because it has a single peak and symmetric pairs on both sides. If it is even, we subtract `1` to make it a valid symmetric pattern.
* `return maxi;` — Returns the final maximum length found.

---

## Dry Run

### Case 1: Typical case with squares (`nums = [5, 4, 1, 2, 2]`)

| it.first (curr) | mp[curr] | Action | count | maxi |
| :--- | :--- | :--- | :--- | :--- |
| `1` | `1` | Special case for `1`: takes all copies of `1`. | `1` | `1` |
| `2` | `2` | Has `>= 2` copies: adds `2` to count, squares `curr` to `4`. | `2` | `2` |
| `4` | `1` | Has `< 2` copies (peak): adds `1` to count, breaks loop. | `3` | `3` |
| `5` | `1` | Has `< 2` copies (peak): adds `1` to count, breaks loop. | `1` | `3` |

*Final check:* `maxi` is `3` (which is odd), so the code returns `3`.

### Case 2: Edge case with single elements (`nums = [1, 3, 2, 4]`)

| it.first (curr) | mp[curr] | Action | count | maxi |
| :--- | :--- | :--- | :--- | :--- |
| `1` | `1` | Special case for `1`: takes single copy of `1`. | `1` | `1` |
| `2` | `1` | Has `< 2` copies (peak): adds `1` to count, breaks loop. | `1` | `1` |
| `3` | `1` | Has `< 2` copies (peak): adds `1` to count, breaks loop. | `1` | `1` |
| `4` | `1` | Has `< 2` copies (peak): adds `1` to count, breaks loop. | `1` | `1` |

*Final check:* `maxi` is `1` (which is odd), so the code returns `1`.

---

## Time & Space Complexity

**Time:** O(n log n) — Storing elements in an unordered map takes O(n) time on average. The outer loop runs for each unique element, and the inner squaring loop runs very few times per element (at most 5 times for numbers up to 10^9, because squaring grows numbers exponentially). However, because we use a hash map, worst-case lookups can degrade, or if implemented with ordered maps, lookups take O(log n). Across the board, the time complexity remains dominated by map operations and stays efficient well within the limits.

**Space:** O(n) — We store up to n elements in the hash map `mp`.

**Is this already optimal?**
Yes, this is effectively optimal for this problem. Because the values can be as large as 10^9, we cannot allocate an array of size 10^9 to count frequencies directly (that would cause a memory limit error). Using a hash map to count frequencies and traversing the exponential chains takes minimal time and space, matching the theoretical best approach for handling sparse, large-valued inputs.

---

## Edge Cases Handled

* **Single occurrence of numbers:** When a number appears only once, it correctly acts as the peak of the mountain, adding `1` to the count and stopping.
* **The number 1:** Handled specially so it doesn't get stuck in an infinite squaring loop (`1 * 1 = 1`).
* **Even vs. Odd lengths:** The final check `if(!(maxi & 1)) return maxi-1;` ensures that if an even number of elements are accidentally accumulated, we trim it down to form a valid symmetric mountain.
* **Large numbers up to 10^9:** The use of `long long` prevents integer overflow when numbers are squared repeatedly.
