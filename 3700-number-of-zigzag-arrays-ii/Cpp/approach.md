![Runtime](https://img.shields.io/badge/Runtime-167%20ms%20(beats%2092.15%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-17%20MB%20(beats%2085.95%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you need to build a sequence of numbers of length n. Every number in this sequence must fall somewhere between two given bounds, l and r. 

There are two strict rules your sequence must follow:
1. No two numbers right next to each other can be equal.
2. The sequence must zig and zag. This means it cannot keep going up forever (strictly increasing) and it cannot keep going down forever (strictly decreasing) for any three numbers in a row. Every three adjacent numbers must change direction — going up then down, or down then up.

Your goal is to count how many valid sequences of length n can be formed. Because this number can get extremely large, you must return the final count modulo 1,000,000,007.

The constraint on length n can be as huge as 10^9. This massive number is the biggest clue in the problem. It means we cannot use a standard loop to build the sequence step-by-step, because a loop running 1,000,000,000 times would time out. We need a math shortcut.

## Intuition

The "aha" moment for this problem comes from two realizations: state transitions and matrix exponentiation.

First, think about how a zigzag sequence works. If a number goes up, the next number must go down. If it goes down, the next number must go up. We can track the transitions between any possible value in our range [l, r]. Let m be the total number of valid values, which is r - l + 1. 

We can represent the choices of going up or down using two matrices, U (for up) and L (for down). 
- Matrix U has a 1 at position [i, j] if value j is greater than value i.
- Matrix L has a 1 at position [i, j] if value j is less than value i.

Because a zigzag sequence alternates (Up then Down, or Down then Up), doing two steps of our sequence means multiplying our choices. Specifically, a two-step block of "Up followed by Down" is represented by multiplying the matrices U * L. 

Second, because n can be as large as 10^9, we can use a technique called matrix exponentiation. Instead of multiplying matrices step-by-step one million times, we can raise our combined matrix (U * L) to a massive power using binary exponentiation (doubling the power at each step, much like binary search). This lets us compute the result in logarithmic time, slashing billions of steps down to just around 30 operations.

## Approach

- `m=r-l+1;`
  Calculates the total number of available values in the range from l to r, storing it in the static variable m.

- `matrix U(m*m, 0), L(m*m, 0);`
  Initializes two flat matrices of size m * m, filled with zeros, to represent upward and downward transitions.

- `for(int i=0; i<m; i++){ for(int j=i+1; j<m; j++) U[i*m+j]=1; }`
  Populates the upper matrix U with 1s everywhere a transition goes from value i to a strictly greater value j.

- `for(int i=0; i<m; i++){ for(int j=0; j<i; j++) L[i*m+j]=1; }`
  Populates the lower matrix L with 1s everywhere a transition goes from value i to a strictly smaller value j.

- `n--;`
  Decrements n by 1 because tracking transitions between elements means we look at n - 1 steps.

- `const int n0=n>>1;`
  Divides the remaining steps by 2 using a bitshift, since our transition matrix represents a two-step pair (Up-Down).

- `const matrix UL=U*L;`
  Multiplies matrix U and matrix L together to form a single combined transition matrix representing a complete two-step zigzag unit.

- `matrix P=pow(UL, n0);`
  Raises the combined transition matrix UL to the power of n0 using fast binary matrix exponentiation.

- `if (n&1) P=L*P;`
  Checks if n was odd by looking at the last bit. If it is odd, we multiply by L one extra time to account for the leftover step.

- `return 2LL*reduce(P.begin(), P.end(), 0LL)%mod;`
  Sums up all values in the final matrix P, multiplies the result by 2 (to account for both starting directions, Up-first and Down-first), and applies the modulo 10^9 + 7.

## Dry Run

### Case 1: Typical case (n = 3, l = 4, r = 5)
Here, values range from 4 to 5. So m = 2 (values are 4 and 5). n = 3.

| Variable / Step | Value | Action |
| :--- | :--- | :--- |
| `m` | 2 | Range size: 5 - 4 + 1 = 2. Matrix size is 2 * 2. |
| `U` | [0, 1, 0, 0] | Matrix U allows going from 4 to 5. |
| `L` | [0, 0, 1, 0] | Matrix L allows going from 5 to 4. |
| `n` | 2 | Decremented from 3 to 2. |
| `n0` | 1 | n divided by 2 (2 >> 1 = 1). |
| `UL` | [1, 0, 0, 1] | Multiplying U and L gives the identity-like 2x2 matrix. |
| `P` | [1, 0, 0, 1] | UL raised to the power 1. |
| `n & 1` | 0 | n is even, so no extra L multiplication needed. |
| Return | 2 | Sum of P elements is 2. Multiply by 2, modulo 10^9 + 7 gives 2. |

### Case 2: Edge case with larger range (n = 3, l = 1, r = 3)
Here, values range from 1 to 3. So m = 3 (values are 1, 2, 3). n = 3.

| Variable / Step | Value | Action |
| :--- | :--- | :--- |
| `m` | 3 | Range size: 3 - 1 + 1 = 3. Matrix size is 3 * 3. |
| `U` | 3x3 matrix | Represents upward steps (e.g., 1 to 2, 1 to 3, 2 to 3). |
| `L` | 3x3 matrix | Represents downward steps (e.g., 3 to 2, 3 to 1, 2 to 1). |
| `n` | 2 | Decremented from 3 to 2. |
| `n0` | 1 | n divided by 2 (2 >> 1 = 1). |
| `UL` | 3x3 matrix | Combined two-step transition matrix. |
| `P` | 3x3 matrix | UL raised to the power 1 remains UL. |
| `n & 1` | 0 | n is even, so no extra L multiplication. |
| Return | 10 | Sum of elements in P equals 5. Multiply by 2 to account for directions, yielding 10. |

## Time & Space Complexity

- **Time:** O(m^3 * log n) — Matrix multiplication takes O(m^3) operations for an m * m matrix, and we repeat it O(log n) times during binary exponentiation. Since m is at most 75 (derived from l and r constraints where r - l <= 74), m^3 is roughly 421,875 operations per multiplication, which runs exceptionally fast combined with a log(n) exponentiation step.
- **Space:** O(m^2) — We store matrices of size m * m, where m is at most 75, requiring minimal fixed memory.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the optimal approach. Because n can reach 10^9, any solution iterating linearly would fail with a time limit exceeded error. Matrix exponentiation is the standard and mathematically optimal way to solve linear recurrence relations and path-counting problems over massive step counts in logarithmic time. No further algorithmic improvements are possible.

## Edge Cases Handled

- **Minimum length (n = 3):** Handled correctly as the base constraint boundary, ensuring our decrement and bit-shifts process properly without underflowing.
- **Smallest range size (l = 1, r = 2, so m = 2):** Handled correctly by building a small 2 * 2 transition matrix.
- **Odd vs Even n:** Correctly handled by checking `if (n&1)` to conditionally multiply an extra downward matrix L when the step count is odd.
- **Large n (up to 10^9):** Handled efficiently using binary exponentiation, keeping the number of multiplication steps proportional to the logarithm of n rather than n itself.
