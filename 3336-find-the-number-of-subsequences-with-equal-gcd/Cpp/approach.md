![Runtime](https://img.shields.io/badge/Runtime-1229%20ms%20(beats%2020.98%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-386.9%20MB%20(beats%2011.05%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have a list of numbers (called nums). You need to pick two separate groups of numbers from this list. A group of numbers picked from a list by keeping them in the same order (without necessarily being next to each other) is called a **subsequence**. 

The two groups must not share any positions from the original list—they are **disjoint** (completely separate indices). 

For each of the two groups, you calculate the **Greatest Common Divisor (GCD)** of all its numbers. The GCD is the largest positive integer that divides evenly into all the numbers in that group. For example, the GCD of 4 and 6 is 2.

Your goal is to count how many pairs of non-empty subsequences you can form such that:
1. The two subsequences do not overlap in their original positions.
2. The GCD of the first subsequence equals the GCD of the second subsequence.

Because this count can be massive, you must return the final answer modulo 10^9 + 7 (which means you keep taking the remainder after dividing by 1,000,000,007). 

The constraints say that the length of the list can be up to 200, and each number inside the list is also between 1 and 200.

---

## Intuition

The "aha" moment for this problem comes from noticing two major constraints: 
1. The numbers in the list are small (at most 200).
2. The GCD of any group of numbers can never exceed the maximum possible value of the numbers themselves (which is 200).

Because the possible GCD values only range from 1 to 200, we can track the state of both subsequences simultaneously using a 2D grid (or table) where both rows and columns represent possible GCD values from 0 to 200. Here, a GCD of 0 means a subsequence is still completely empty.

As we look at each number one by one from our input list, we have three choices for that number:
- Add it to the first subsequence.
- Add it to the second subsequence.
- Ignore it completely.

When we add a number to a subsequence, the running GCD of that subsequence updates using the **Euclidean Algorithm** (which is just a fast way to find the GCD, built into C++ as `std::gcd`). By updating our table of all possible GCD pairs for every number we process, we can build up all combinations. At the very end, we look at all cases where the first subsequence GCD equals the second subsequence GCD (where row index equals column index) and sum them up.

---

## Approach

Here is the step-by-step walkthrough of what the code does:

- `const int MOD=1000000007;` and `const int M=200;`: Defines our modulo number to keep numbers from overflowing, and sets the maximum possible value (and maximum possible GCD) to 200 based on the problem constraints.
- `vector<vector<int>>dp(M+1,vector<int>(M+1));`: Creates a 2D table filled with zeros, where rows represent the current GCD of the first subsequence and columns represent the current GCD of the second subsequence.
- `dp[0][0]=1;`: Initializes the table by setting the state where both subsequences are empty (GCD is 0) to 1 way of doing things.
- `for(int x:nums){`: Loops through every single number `x` in our input list `nums`.
- `vector<vector<int>> ndp=dp;`: Creates a copy of our current table called `ndp` (next dynamic programming table) so we can update states based on the current number without messing up calculations mid-loop.
- `for(int g1=0;g1<=M;g1++){` and `for(int g2=0; g2<=M;g2++){`: Loops through every possible pair of current GCDs (`g1` for the first subsequence, `g2` for the second subsequence).
- `int cur=dp[g1][g2]; if(!cur) continue;`: Grabs the number of ways to reach this specific GCD pair. If there are zero ways (`!cur`), we skip it to save time.
- `int ng1=g1 == 0 ? x:std::gcd(g1,x);`: Figures out the new GCD for the first subsequence if we add the current number `x` to it. If it was empty (`g1 == 0`), the new GCD is just `x`. Otherwise, it finds the GCD of the old GCD and `x`.
- `int &a=ndp[ng1][g2]; a +=cur; if(a>=MOD) a-=MOD;`: Adds the ways (`cur`) to the new table location where the first subsequence's GCD has changed to `ng1`, keeping it bounded by `MOD`.
- `int ng2=g2==0 ? x:std::gcd(g2,x);`: Figures out the new GCD for the second subsequence if we add the current number `x` to it, using the same logic as above.
- `int &b=ndp[g1][ng2]; b+=cur; if(b>=MOD) b-=MOD;`: Adds the ways to the new table location where the second subsequence's GCD has changed to `ng2`.
- `dp.swap(ndp);`: Replaces the old table with the newly updated table for the next iteration of the loop.
- `long long ans=0;`: Initializes a variable to keep track of our final answer.
- `for(int g=1;g<=M;g++){ ans+=dp[g][g]; }`: Loops through all valid matching GCD values (where the first and second subsequence share the exact same GCD `g`, from 1 to 200) and adds up all those valid ways.
- `return ans%MOD;`: Returns the total sum modulo 10^9 + 7.

---

## Dry Run

### Case 1: Minimal example (nums = [10, 20])
Let us trace a smaller version of the input where `nums = [10, 20]`, with `M = 20`.

| Step | x | Action / State Change |
| :--- | :--- | :--- |
| Initialization | - | `dp[0][0] = 1`, all other cells are 0. |
| Process first number | 10 | We can place 10 into subseq 1 (`ndp[10][0] += 1`) or subseq 2 (`ndp[0][10] += 1`). |
| Process second number | 20 | For existing states, we add 20. For example, starting from `dp[10][0] = 1`, adding 20 to subseq 2 gives `ndp[10][20] = 1`. Adding 20 to subseq 1 gives `ndp[gcd(10,20)][0] = ndp[10][0] = 1`. |
| Final summation | - | We check matching diagonal cells like `dp[10][10]`. In this case, combinations where both end up with a GCD of 10 are tallied up. |

### Case 2: Array with identical elements (nums = [1, 1])

| Step | x | Action / State Change |
| :--- | :--- | :--- |
| Initialization | - | `dp[0][0] = 1`. |
| Process first number | 1 | We can place 1 into subseq 1 (`ndp[1][0] = 1`) or subseq 2 (`ndp[0][1] = 1`). |
| Process second number | 1 | Taking the state where subseq 1 has a GCD of 1 (`dp[1][0] = 1`), we can add the second 1 into subseq 2, resulting in `ndp[1][1] += 1`. |
| Final summation | - | The cell `dp[1][1]` holds our successful pairs where both subsequences have a GCD of 1. |

---

## Time & Space Complexity

- **Time:** O(N * M^2) — where N is the length of the input array `nums` and M is the maximum possible value in the array (200). For each number in the array, we iterate through a 201 by 201 grid of possible GCD pairs.
- **Space:** O(M^2) — because we maintain two 201 by 201 tables (`dp` and `ndp`) to store our counts.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

This code is already well-optimized for the given constraints. Because N is up to 200 and the maximum number value is 200, an approach based on the maximum possible GCD value is standard and executes within milliseconds. The grid dimensions are fixed at 200x200 regardless of how large N grows beyond that, making it ideal for these specific constraints. No further structural improvements are necessary.

---

## Edge Cases Handled

- **Duplicate values:** The code correctly handles arrays with repeating numbers (like `[1, 1, 1, 1]`) by multiplying pathways dynamically through the nested loops instead of treating identical numbers as distinct choices.
- **Smallest possible inputs:** Handles single-element arrays or small inputs cleanly, though the problem implies non-empty disjoint pairs can be formed based on constraints (minimum length 2 for valid pairs).
- **Numbers that do not share common factors:** Handled smoothly by `std::gcd`, which correctly evaluates to 1 when numbers are coprime.
- **Modulo overflow prevention:** The code checks `if(a >= MOD) a -= MOD;` continuously inside the inner loops, avoiding integer overflow before it can happen.
