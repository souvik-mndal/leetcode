![Runtime](https://img.shields.io/badge/Runtime-141%20ms%20(beats%206.15%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-9.6%20MB%20(beats%2086.34%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine two players taking turns picking numbers from either the very beginning or the very end of an array of integers. 

Player 1 goes first. When a player picks a number from an end, that number is removed from the array and permanently added to that player's running score. The game ends when no numbers are left. 

Your goal is to figure out if Player 1 can guarantee a win. Player 1 wins if their final score is greater than or equal to Player 2's final score. A tie also counts as a win for Player 1. Both players play perfectly, meaning they always make the smartest possible move to maximize their own score or block the other person.

For example, if the input array is [1, 5, 2]:
- Player 1 can pick 1 or 2. 
- If Player 1 picks 2, Player 1 gets 2, leaving [1, 5]. 
- Then Player 2 can pick 5, getting 5 and leaving [1] for Player 1. 
- Final scores: Player 1 gets 2 + 1 = 3, Player 2 gets 5. Player 2 wins, so the answer is false.

## Intuition

The core trick here is **Minimax**, a classic concept in game theory for **zero-sum games** (where one player's gain is the other player's loss). 

Instead of tracking both players' full scores separately all the way down, we can track just Player 1's total score as we simulate the game recursively. A boolean flag called `turn` tells us whose turn it is: `true` means it is Player 1's turn, and `false` means it is Player 2's turn.

Because both players are playing **optimally** (smartly):
- When it is Player 1's turn (`turn == true`), Player 1 wants to maximize their final score. They will look at the two choices (left end or right end) and pick the path that gives them the largest possible outcome (`max`).
- When it is Player 2's turn (`turn == false`), Player 2 is also playing smartly to maximize *their* score—which means they will force Player 1 into the worst possible outcome. From Player 1's perspective, Player 2 will pick the move that results in the smallest possible score for Player 1 (`min`).

At the very end, we check if Player 1's accumulated score is at least half of the total sum of all numbers in the array. If it is, Player 1 wins.

## Approach

- `for (int num : nums) sum += num;`: Loops through every number in the array and adds it up to find the total sum of all elements, stored in `sum`.
- `int playerOneMax = getPlayer1Max(0, 0, nums.size() - 1, nums, true);`: Calls the helper function to simulate the game starting with Player 1's score at 0, using the full array bounds from index 0 to `nums.size() - 1`, with `turn` set to `true` (Player 1's turn).
- `if (sum % 2) return playerOneMax >= (sum + 1) / 2;`: If the total sum is odd, Player 1 needs a strict majority. Since integer division rounds down, `(sum + 1) / 2` calculates the exact threshold needed to win.
- `return playerOneMax >= sum / 2;`: If the total sum is even, Player 1 wins if their score is greater than or equal to half of the total sum.
- `if (start > end) return playerOneMax;`: Base case for the recursion. When the left pointer passes the right pointer, no numbers are left, so it returns Player 1's accumulated score.
- `if (turn) { ... }`: If it is Player 1's turn, Player 1 tries both ends. They add the chosen number to `player1`, move the pointer inward, flip `turn` to `false`, and use `max` to pick the best outcome.
- `else { ... }`: If it is Player 2's turn, Player 2 does not add to Player 1's score. They pick the move that results in the minimum possible score for Player 1, flip `turn` to `true`, and use `min`.

## Dry Run

### Case 1: nums = [1, 5, 2] (Given Example 1)

| player1 | start | end | turn | Action |
| :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 2 | true | Player 1's turn. Branches into picking left (1) or right (2). |
| 1 | 1 | 2 | false | Left branch: Player 1 picks 1. Now Player 2's turn on [5, 2]. |
| 1 | 2 | 2 | true | Player 2 picks 5 (leaving 2 for Player 1). Now Player 1's turn on [2]. |
| 3 | 2 | 1 | false | Player 1 picks 2. Pointers cross (`start > end`), returns score 3. |
| 2 | 0 | 1 | false | Right branch from start: Player 1 picks 2. Now Player 2's turn on [1, 5]. |
| 2 | 0 | 0 | true | Player 2 picks 5 (leaving 1 for Player 1). Now Player 1's turn on [1]. |
| 3 | 0 | -1 | false | Player 1 picks 1. Pointers cross, returns score 3. |

*Note:* Both main branches lead to Player 1 getting a max score of 3. The total sum is 8. Half of 8 is 4. Player 1's score (3) is less than 4, so the function returns `false`.

### Case 2: nums = [10, 2] (Edge case with two elements)

| player1 | start | end | turn | Action |
| :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 1 | true | Player 1's turn. Choices are 10 (index 0) or 2 (index 1). |
| 10 | 1 | 1 | false | Left branch: Player 1 picks 10. Player 2's turn on [2]. |
| 10 | 2 | 1 | true | Player 2 takes 2. Pointers cross, returns score 10. |
| 2 | 0 | 0 | false | Right branch: Player 1 picks 2. Player 2's turn on [10]. |
| 2 | 0 | -1 | true | Player 2 takes 10. Pointers cross, returns score 2. |

*Note:* Player 1 chooses the maximum between 10 and 2, which is 10. Total sum is 12, half is 6. Since 10 >= 6, it returns `true`.

## Time & Space Complexity

**Time:** O(2^n) — At every step of the recursion, the function branches into two recursive calls (taking from the left or taking from the right). For an array of length N, this creates a binary tree of calls up to depth N, resulting in up to 2^N operations.
**Space:** O(n) — The maximum depth of the recursion call stack is equal to the length of the array, N.

**Can this be improved?**
Yes, dramatically. The current recursive solution recomputes the exact same array slices (`start` and `end` indices) many times over, which causes massive redundant work. We can fix this using **Dynamic Programming** (specifically memoization, or saving past results in a lookup table/grid) or by switching to a bottom-up table. 

By caching results for every pair of `start` and `end` indices, we ensure each sub-problem is solved only once.

Here is how you can optimize it using a memoization table (a 2D vector):

```cpp
int memo[20][20]; // Constraints say length is up to 20

int getPlayer1Max(int start, int end, vector<int>& nums) {
    if (start > end) return 0;
    if (memo[start][end] != -1) return memo[start][end];
    
    int pickStart = nums[start] - getPlayer1Max(start + 1, end, nums);
    int pickEnd = nums[end] - getPlayer1Max(start, end - 1, nums);
    
    return memo[start][end] = max(pickStart, pickEnd);
}
```
*Why this works:* Instead of tracking turns explicitly, we can change the perspective: at any range, a player's optimal score from that sub-array is the current number they pick *minus* whatever optimal score the other player can get from the remaining sub-array. 

- **Resulting improved complexity:** **Time:** O(n^2), **Space:** O(n^2) to store the memo table.
- **Theoretical best possible complexity:** O(n^2) time and O(n^2) space (or O(n) space if optimized using rolling arrays) is the standard optimal bound for interval dynamic programming problems of this type. The memoized version reaches this optimal time complexity.

## Edge Cases Handled

- **Single element array (length 1):** The loop calculates the sum, the recursion immediately picks the single element, pointers cross, and it correctly evaluates as a win for Player 1.
- **Even vs. Odd total sums:** The code explicitly handles rounding with `(sum + 1) / 2` when the total sum is odd, ensuring ties correctly favor Player 1 as specified in the rules.
- **Array with identical values:** Handled naturally because the `max` and `min` choices properly explore symmetric paths.
- **Maximum constraint size (length 20):** While the unoptimized code runs slowly on larger arrays due to exponential time complexity, the constraints state length is between 1 and 20, allowing it to pass within time limits despite the redundant calculations.
