![Runtime](https://img.shields.io/badge/Runtime-3%20ms%20(beats%2093.41%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-84.4%20MB%20(beats%2020.81%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a fixed amount of pocket money, called coins. You walk into a store that sells ice cream bars. Every ice cream bar has a different price listed in the array costs. 

Your goal is simple: buy as many ice cream bars as you possibly can without spending more money than the total coins you brought with you. You can buy the bars in any order you like, and the store has plenty of stock so multiple bars can cost the same amount. 

For example, if your costs are [1, 3, 2, 4, 1] and you have 7 coins, you want to pick the largest possible quantity of bars whose prices add up to 7 or less. You can buy the bars priced at 1, 1, 2, and 3, which totals 1 + 1 + 2 + 3 = 7. That is 4 ice cream bars, which is the maximum possible.

## Intuition

If you want to buy the *maximum* number of items with a limited budget, the smartest strategy is to always buy the cheapest available items first. If you spend your coins on expensive items, your money runs out quickly, and you miss out on buying many cheaper items. This is a classic **greedy** strategy—making the locally best choice at each step to achieve the globally best result.

Normally, sorting the prices from lowest to highest takes N * log(N) time. But here, the problem constraints allow us to do something even faster using **Counting Sort**. Instead of sorting every single item, we can just count *how many* ice cream bars exist for each price. Because the maximum possible price is capped (up to 10^5), we can create a frequency list where the index represents the price, and the value at that index represents how many bars cost that much.

Once we have our counts, we can just walk through the prices sequentially from cheapest (1 coin) to most expensive. For each price, we buy as many as we can afford and as many as the store actually has. This avoids sorting entirely and makes our code blazing fast.

## Approach

Here is how the C++ code implements this logic step-by-step:

*   `int cnt = 0;` — Initializes a counter variable to track the total number of ice cream bars successfully bought, starting at zero.
*   `vector<int>arr(*(max_element(costs.begin(),costs.end()))+1);` — Finds the maximum price in the costs array using `max_element`, adds 1 to it, and creates a frequency array (named `arr`) of that size. This array will store how many times each price appears.
*   `for( int i=0 ; i<costs.size() ; i++ ) { arr[costs[i]]++; }` — Loops through every price in the original costs array and increments the matching index in our frequency array `arr` to count how many ice cream bars cost that exact amount.
*   `for( int i=0 ; i<arr.size() ; i++ ) {` — Loops through the frequency array starting from price index 0 up to the maximum price. Because indices are naturally ordered, this automatically visits prices from cheapest to most expensive.
*   `if( arr[i] == 0 ) continue;` — Skips any price that has zero ice cream bars available in the store.
*   `if( i > coins ){ break; }` — Stops the loop entirely if the current price `i` is greater than our remaining coins, because we cannot afford this price or any price after it.
*   `int d = min(coins / i , arr[i]);` — Calculates how many bars of price `i` we should buy. It takes the smaller of two numbers: how many we can afford (`coins / i`) versus how many actually exist in the store (`arr[i]`).
*   `coins = coins - ( d * i );` — Deducts the total cost of the bars we just bought from our remaining coins.
*   `cnt+=d;` — Adds the number of newly bought bars (`d`) to our running total counter.
*   `return cnt;` — Returns the final maximum number of ice cream bars bought once all loops finish.

## Dry Run

Let us trace two examples.

### Case 1: Typical case (costs = [1, 3, 2, 4, 1], coins = 7)

*   **Frequency array setup:** `arr` is sized up to index 4. `arr[1] = 2`, `arr[2] = 1`, `arr[3] = 1`, `arr[4] = 1`.
*   **Loop `i = 1` (Price 1):** `arr[1]` is 2. `i` (1) <= `coins` (7). `d = min(7 / 1, 2) = 2`. `coins = 7 - (2 * 1) = 5`. `cnt = 2`.
*   **Loop `i = 2` (Price 2):** `arr[2]` is 1. `i` (2) <= `coins` (5). `d = min(5 / 2, 1) = 1`. `coins = 5 - (1 * 2) = 3`. `cnt = 3`.
*   **Loop `i = 3` (Price 3):** `arr[3]` is 1. `i` (3) <= `coins` (3). `d = min(3 / 3, 1) = 1`. `coins = 3 - (1 * 3) = 0`. `cnt = 4`.
*   **Loop `i = 4` (Price 4):** `i` (4) > `coins` (0). The loop breaks.
*   **Output:** Returns `4`.

| Step / Price index (i) | Coins Left | Store Count (arr[i]) | Amount to Buy (d) | Total Count (cnt) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial Setup | 7 | N/A | N/A | 0 | Frequency array built |
| i = 1 | 5 | 2 | 2 | 2 | Bought 2 bars of price 1 |
| i = 2 | 3 | 1 | 1 | 3 | Bought 1 bar of price 2 |
| i = 3 | 0 | 1 | 1 | 4 | Bought 1 bar of price 3 |
| i = 4 | 0 | 1 | Break | 4 | Price exceeds remaining coins, stop |

### Case 2: Edge case with insufficient coins (costs = [10, 6, 8], coins = 5)

*   **Frequency array setup:** `arr` sized up to index 10. `arr[6] = 1`, `arr[8] = 1`, `arr[10] = 1`.
*   **Loop `i = 6` (Price 6):** `i` (6) > `coins` (5). The loop breaks immediately.
*   **Output:** Returns `0`.

| Step / Price index (i) | Coins Left | Store Count (arr[i]) | Amount to Buy (d) | Total Count (cnt) | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial Setup | 5 | N/A | N/A | 0 | Frequency array built |
| i = 6 | 5 | 1 | Break | 0 | Price 6 is higher than 5 coins, stop |

## Time & Space Complexity

*   **Time Complexity:** **O(N + M)**, where N is the number of ice cream bars in the costs array, and M is the maximum price found in the costs array. We loop through the costs array once to build the frequency counts (taking N time), and then we loop through the frequency array up to its maximum value M to buy the bars. Because the maximum price constraint is 10^5, this linear pass over price indices is extremely fast and avoids the O(N log N) penalty of traditional sorting.
*   **Space Complexity:** **O(M)**, where M is the maximum price in the costs array. We allocate a frequency vector `arr` whose size equals the highest price value plus one. Given constraints, this requires at most 10^5 integers of memory, which is well within safe limits.

### Is this optimal?
Yes, this code is already at the optimal time complexity of **O(N + M)**. Counting sort is the most efficient sorting approach here because the range of values (maximum price) is bounded by a reasonable constraint (10^5). You cannot achieve a faster time complexity than scanning the input array and the price range once. No further improvements are necessary.

## Edge Cases Handled

*   **Not enough coins for any item:** Handled cleanly when `i > coins` on the very first available price, immediately breaking out of the loop and returning 0.
*   **Duplicate prices:** Handled correctly because the frequency array `arr[costs[i]]++` accumulates all occurrences of identical prices, allowing us to buy multiple bars of the exact same price.
*   **More coins than total cost of everything:** Handled naturally because the store stock limit (`arr[i]`) acts as a cap via `min(coins / i, arr[i])`, ensuring we do not try to buy more items than actually exist.
