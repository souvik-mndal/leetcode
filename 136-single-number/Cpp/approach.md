![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-20.7%20MB%20(beats%2058.64%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of items. Almost every item has an identical twin sitting somewhere else in the row. However, there is one single item that has no partner; it appears only once. Your job is to find that lonely item and output it.

For example, if your list of numbers is [4, 1, 2, 1, 2], the numbers 1 and 2 both show up twice. But the number 4 shows up only once. Your program needs to inspect the list and return 4.

## Intuition

The classic way to solve this might be using a hash map to count how many times each number appears, or sorting the list to check neighbors. But those methods take extra memory or extra time. 

The clever trick here uses **bit manipulation** — a way of looking at numbers at the binary level (ones and zeros) using computer logic gates. Specifically, we use the **XOR operator** (written as ^ in C++). 

XOR has two magical properties that make this problem trivial:
1. If you XOR a number with itself, it completely cancels out and becomes zero (for example, 5 ^ 5 = 0).
2. If you XOR a number with zero, it stays the same (for example, 5 ^ 0 = 5).

Because XOR is also commutative and associative, the order of numbers does not matter. If you take every single number in the array and XOR them all together, the pairs will cancel each other out and turn into zeros. The only thing left standing at the end will be the lonely single number that had no partner to cancel it out!

## Approach

Here is how the code executes this logic, step by step:

- `int ans = 0;`: Creates an integer variable named `ans` and sets it to zero. This variable will hold our running result. Because anything XORed with zero stays the same, starting at zero is a safe baseline.
- `for( int i=0 ; i<nums.size() ; i++ )`: Starts a standard loop that looks at every single number in the `nums` array, starting from index 0 all the way to the last element.
- `ans ^= nums[i];`: Takes the current value of `ans`, combines it with the current array element `nums[i]` using the XOR operator, and stores the updated result back into `ans`. Pairs cancel out to zero here, while the single number survives.
- `return ans;`: After the loop finishes checking every number, the function returns the final value stored in `ans`, which is our lonely single number.

## Dry Run

### Case 1: Typical case with multiple pairs
**Input:** `nums = [4, 1, 2, 1, 2]`

| Loop Index `i` | Current Element `nums[i]` | Running `ans` Before | Action | Running `ans` After |
| :--- | :--- | :--- | :--- | :--- |
| (Start) | - | 0 | Initialize `ans` to 0 | 0 |
| 0 | 4 | 0 | Calculate 0 ^ 4 | 4 |
| 1 | 1 | 4 | Calculate 4 ^ 1 | 5 |
| 2 | 2 | 5 | Calculate 5 ^ 2 | 7 |
| 3 | 1 | 7 | Calculate 7 ^ 1 (the twin of 1 cancels out) | 6 |
| 4 | 2 | 6 | Calculate 6 ^ 2 (the twin of 2 cancels out) | 4 |

**Final Output:** `4`

---

### Case 2: Minimal array with a single element
**Input:** `nums = [1]`

| Loop Index `i` | Current Element `nums[i]` | Running `ans` Before | Action | Running `ans` After |
| :--- | :--- | :--- | :--- | :--- |
| (Start) | - | 0 | Initialize `ans` to 0 | 0 |
| 0 | 1 | 0 | Calculate 0 ^ 1 | 1 |

**Final Output:** `1`

## Time & Space Complexity

- **Time:** O(N) — The code uses a single loop that visits every element in the `nums` array exactly once, where N is the length of the array.
- **Space:** O(1) — The code only creates one integer variable `ans`, using a fixed amount of extra memory regardless of how large the input array gets.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is already completely optimal. 
- You cannot improve the time complexity past O(N) because you must at least look at every element in the array to know if it has a pair. 
- You cannot improve the space complexity past O(1) because storing extra tracking structures would require additional memory that scales with the input size. 

No further improvement is possible.

## Edge Cases Handled

- **Single-element arrays:** The constraints say the array length starts at 1 (`1 <= nums.length`). If `nums = [1]`, the loop runs once, `ans` becomes `1 ^ 0`, which correctly returns `1` without errors.
- **Negative numbers:** The constraints allow negative numbers (`-3 * 10^4 <= nums[i] <= 3 * 10^4`). Because XOR works on the binary bit level of numbers regardless of whether they are positive or negative, negative pairs cancel out just like positive ones do.
- **Unsorted order:** The array is not required to be sorted. Because XOR is commutative (meaning order does not matter, like A ^ B is the same as B ^ A), numbers can appear anywhere in the array and their pairs will still successfully cancel out.
