# 486. Predict the Winner

**Difficulty:** Medium
**Topics:** Array, Math, Dynamic Programming, Recursion, Minimax, Game Theory, Zero-Sum Game

## Description
You are given an integer array nums.

Two players are playing a game with this array: Player 1 and Player 2.

Player 1 and Player 2 take turns, with Player 1 starting first. Both players start the game with a score of 0.

At each turn, the current player takes the number at either end of the array (i.e., nums[0] or nums[nums.length - 1]), removing it from the array and adding it to their own score. The game ends when there are no more elements in the array.

Return true if Player 1's final score is greater than or equal to Player 2's final score, and false otherwise.

Note that a tie counts as a win for Player 1. You may assume that both players play optimally.

## Examples
**Example 1:**
```
Input: nums = [1,5,2]
Output: false
Explanation:
```

**Example 2:**
```
Input: nums = [1,5,233,7]
Output: true
Explanation:
```

## Constraints
- 1 <= nums.length <= 20
- 0 <= nums[i] <= 10^7
