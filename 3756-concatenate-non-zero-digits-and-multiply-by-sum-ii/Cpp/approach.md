![Runtime](https://img.shields.io/badge/Runtime-37%20ms%20(beats%2059.33%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-164.2%20MB%20(beats%2026.18%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a long string of digits, like "10203004". You are given a list of queries. Each query gives you a starting index and an ending index, telling you to look at a specific substring. 

For each substring, you must do three things:
1. Throw away all the zeros. Keep only the non-zero digits and push them together in their original order to form a brand-new number. If there are no non-zero digits at all, this new number is just 0.
2. Add up all the digits of that new number to get a sum.
3. Multiply the new number by that sum. 

Because the resulting numbers can get extremely large, you must return each answer modulo 10^9 + 7. 

For example, if the substring is "10203", you throw away the zeros to get "123". The sum of its digits is 1 + 2 + 3 = 6. Your final answer for that query is 123 * 6 = 738.

## Intuition

The main challenge is that the input string can be up to 100,000 characters long, and there can be up to 100,000 queries. If we processed each query by looping through the string from scratch, it would take far too long and time out.

The "aha" moment is realizing that we can filter out the zeros *once* at the beginning, and pack all the non-zero digits into a separate compact array. We can then precompute three things for this filtered array using **prefix sums** (running totals that let us quickly find the sum or value of any sub-section in constant time):
1. A running list of numbers formed by the digits, kept modulo 10^9 + 7, so we can reconstruct any substring's value instantly.
2. A running sum of the digits, so we can get the sum of any section instantly.
3. Powers of 10 modulo 10^9 + 7, which help us shift place values when gluing digits together.

To handle the original string indices mapping to our filtered array, we also keep a count array that tells us how many non-zero digits appear up to any given index in the original string. This lets us translate any query's start and end boundaries into the exact range we need in our filtered digit array.

## Approach

- `vector<int> nonZeroCount(n + 1, 0);`: Creates an array to track how many non-zero digits exist up to each index of the string, helping us map query ranges.
- `vector<int> digits;`: Creates an array that will hold only the non-zero digits extracted from the string in order.
- `for (int i = 0; i < n; i++) { ... }`: Loops through the original string, updating the non-zero count and pushing any non-zero digit (converted from character to integer) into our `digits` array.
- `int k = digits.size();`: Stores the total number of non-zero digits found.
- `vector<long long> prefixValue(k + 1, 0);`: Prepares an array to store running values of the digits as if they form a large number, kept modulo 10^9 + 7.
- `vector<long long> prefixSum(k + 1, 0);`: Prepares an array to store running sums of the digits.
- `vector<long long> power10(k + 1, 1);`: Prepares an array to store powers of 10 modulo 10^9 + 7, used for shifting place values.
- `for (int i = 0; i < k; i++) { ... }`: Fills up `prefixValue`, `prefixSum`, and `power10` step-by-step so we can answer any query in a single math step later.
- `vector<int> answer; answer.reserve(queries.size());`: Sets up the final results container and reserves memory to make it run faster.
- `for (const auto& query : queries) { ... }`: Loops through every query given in the input.
- `int l = query[0]; int r = query[1];`: Extracts the starting and ending indices of the current query from the original string.
- `int left = nonZeroCount[l];`: Finds the starting index in our filtered `digits` array corresponding to the query's start.
- `int right = nonZeroCount[r + 1];`: Finds the ending index in our filtered `digits` array corresponding to the query's end.
- `int len = right - left;`: Calculates how many non-zero digits are in this specific query range.
- `long long x = (...);`: Reconstructs the exact multi-digit number formed by the non-zero digits using our prefix values and power of 10, applying the modulo.
- `long long sum = prefixSum[right] - prefixSum[left];`: Quickly calculates the sum of the digits for this query range.
- `answer.push_back((int)((x * sum) % MOD));`: Multiplies the number by the sum, applies the modulo, and stores the final answer.

## Dry Run

### Case 1: Typical case
- **Input:** s = "10203004", queries = [[0,7]]
- **Setup Phase:** 
  - `digits` extracts non-zero elements: [1, 2, 3, 4], so k = 4.
  - `nonZeroCount` maps original indices to filtered indices.
  - `prefixValue` tracks running number values: [0, 1, 12, 123, 1234].
  - `prefixSum` tracks running sums: [0, 1, 3, 6, 10].
  - `power10` tracks powers of 10: [1, 10, 100, 1000, 10000].
- **Query Execution:**
  - Query [0,7]: `left = nonZeroCount[0] = 0`, `right = nonZeroCount[8] = 4`.
  - `len = 4 - 0 = 4`.
  - `x = prefixValue[4] - (prefixValue[0] * power10[4]) = 1234 - 0 = 1234`.
  - `sum = prefixSum[4] - prefixSum[0] = 10 - 0 = 10`.
  - Answer calculation: 1234 * 10 = 12340.
  - Result: [12340].

### Case 2: Edge case with only zeros
- **Input:** s = "000", queries = [[0,2]]
- **Setup Phase:**
  - `digits` finds no non-zero digits: [], so k = 0.
  - `prefixValue` = [0], `prefixSum` = [0], `power10` = [1].
- **Query Execution:**
  - Query [0,2]: `left = nonZeroCount[0] = 0`, `right = nonZeroCount[3] = 0`.
  - `len = 0 - 0 = 0`.
  - `x = 0`, `sum = 0`.
  - Answer calculation: 0 * 0 = 0.
  - Result: [0].

## Time & Space Complexity

- **Time:** O(N + Q) — where N is the length of the string `s` and Q is the number of queries. We make one pass over the string to filter digits and build our prefix arrays, taking O(N) time. Then, each of the Q queries is answered in O(1) constant time using simple arithmetic.
- **Space:** O(N + Q) — we store the filtered digits, prefix arrays, and non-zero counts, which all scale linearly with the length of the string N. The `answer` vector takes O(Q) space to store the results.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the optimal time and space complexity. 

Because we must read a string of length N and process Q queries, any correct algorithm must at minimum spend O(N + Q) time just to look at the input and produce the output. Since our solution runs in O(N + Q) time, it achieves the theoretical best possible time complexity. No further improvement is possible.

## Edge Cases Handled

- **Substrings with only zeros:** If a query points to a range containing only zeros (e.g., "000"), `digits` is empty for that range, `len` is 0, `x` evaluates to 0, and the sum evaluates to 0, correctly returning 0.
- **Single-digit queries:** Handles queries where length is 1 (e.g., `[1,1]` on "1000") without breaking index math.
- **Large numbers and modulo arithmetic:** Constantly applies modulo 10^9 + 7 during prefix value and power calculations to prevent integer overflow.
- **Full string queries:** Correctly handles queries that span the entire length of `s` from index 0 to n - 1.
