![Runtime](https://img.shields.io/badge/Runtime-75%20ms%20(beats%2049.08%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-155.5%20MB%20(beats%2073.10%25)-green?style=for-the-badge)

---

## Problem Explained

We are given an array of integers called `nums`. From this array, we need to build a new array called `prefixGcd` of the exact same length. 

For every position `i` in the array, the value in `prefixGcd` is calculated by finding the Greatest Common Divisor (let's call it GCD) between `nums[i]` and the largest number seen so far from the start of the array up to index `i`. 

Once we have built the entire `prefixGcd` array, we must sort it from smallest to largest. Then, we pair up the numbers: we take the smallest unpaired number and the largest unpaired number, compute their GCD, and add that GCD to a running total. We repeat this pairing process until no more pairs can be formed. If the array length is an odd number, the leftover element right in the middle has no partner, so it is ignored. Finally, we return the sum of all those pair GCD values.

For example, given `nums = [2, 6, 4]`:
- At index 0: largest so far is 2, gcd(2, 2) = 2.
- At index 1: largest so far is 6, gcd(6, 6) = 6.
- At index 2: largest so far is 6, gcd(4, 6) = 2.
- `prefixGcd` becomes `[2, 6, 2]`.
- After sorting, it becomes `[2, 2, 6]`.
- We pair the smallest (2) and largest (6): gcd(2, 6) = 2. The middle 2 is left alone.
- The total sum is 2.

---

## Intuition

The problem maps out a very clear, step-by-step recipe. It asks us to simulate building an array based on running maximums and GCDs, sort it, and then match elements from the outside edges inward. 

The "aha" moment is realizing that we do not need any clever tricks or advanced data structures here. The constraints say `n` goes up to 10^5, which means an approach using sorting is completely fine. Sorting takes `n log n` time, easily passing within time limits. 

Once `prefixGcd` is sorted, matching the smallest and largest remaining elements is a classic **Two Pointers** pattern. We place one pointer (`l`) at the very beginning of the sorted array and another pointer (`r`) at the very end. We compute the GCD of the numbers at both pointers, add it to our answer, and then move `l` one step right and `r` one step left until they meet. If `n` is odd, the middle element is naturally skipped because the `while` loop condition `l < r` stops before they land on the same index.

---

## Approach

* `int n = nums.size(); vector<int> a(n); int mx = 0;`: Stores the length of the input array, creates our `prefixGcd` array named `a`, and initializes a tracking variable `mx` to keep track of the largest number seen so far.
* `for(i=0; i<n; i++)`: Loops through every element in `nums` from left to right to build the `prefixGcd` array step-by-step.
* `mx = max(mx, nums[i]);`: Updates the running maximum by comparing the current maximum with the current number in `nums`.
* `a[i] = std::gcd(nums[i], mx);`: Calculates the Greatest Common Divisor between the current number and the running maximum, then stores it in our `a` array at index `i`.
* `sort(a.begin(), a.end());`: Sorts the entire `a` array in non-decreasing order so that the smallest elements are on the left and the largest are on the right.
* `long long ans = 0; int l = 0, r = n - 1;`: Initializes the total sum to zero and sets up our two pointers: `l` starts at index 0 (the smallest element) and `r` starts at index `n - 1` (the largest element).
* `while(l < r)`: Loops as long as the left pointer is strictly to the left of the right pointer, ensuring we only pair distinct elements and stop before reaching the middle element on odd lengths.
* `ans += std::gcd(a[l], a[r]); l++; r--;`: Computes the GCD of the elements at the left and right pointers, adds it to `ans`, moves the left pointer one step right, and moves the right pointer one step left.
* `return ans;`: Returns the final accumulated sum of all formed pair GCDs.

---

## Dry Run

### Case 1: Typical case with sorted pairing
Input: `nums = [2, 6, 4]`

| Step / Index | mx | nums[i] | a[i] (after loop) | a (after sort) | l | r | Action / Calculation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Loop i=0 | 2 | 2 | 2 | - | - | - | mx = max(0, 2) = 2; a[0] = gcd(2, 2) = 2 |
| Loop i=1 | 6 | 6 | 6 | - | - | - | mx = max(2, 6) = 6; a[1] = gcd(6, 6) = 6 |
| Loop i=2 | 6 | 4 | 2 | - | - | - | mx = max(6, 4) = 6; a[2] = gcd(4, 6) = 2 |
| Post-loop | - | - | - | `[2, 2, 6]` | 0 | 2 | Array sorted. Pointers set at ends. |
| While l<r | - | - | - | `[2, 2, 6]` | 0 | 2 | ans += gcd(2, 6) = 2. l becomes 1, r becomes 1. Loop ends. |
| Return | - | - | - | `[2, 2, 6]` | 1 | 1 | Returns final answer: `2`. |

---

### Case 2: Four elements with multiple pairs
Input: `nums = [3, 6, 2, 8]`

| Step / Index | mx | nums[i] | a[i] (after loop) | a (after sort) | l | r | Action / Calculation |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Loop i=0 | 3 | 3 | 3 | - | - | - | mx = 3; a[0] = gcd(3, 3) = 3 |
| Loop i=1 | 6 | 6 | 6 | - | - | - | mx = 6; a[1] = gcd(6, 6) = 6 |
| Loop i=2 | 6 | 2 | 2 | - | - | - | mx = 6; a[2] = gcd(2, 6) = 2 |
| Loop i=3 | 8 | 8 | 8 | - | - | - | mx = 8; a[3] = gcd(8, 8) = 8 |
| Post-loop | - | - | - | `[2, 3, 6, 8]` | 0 | 3 | Array sorted. Pointers set at ends. |
| While l<r (1st) | - | - | - | `[2, 3, 6, 8]` | 0 | 3 | ans += gcd(2, 8) = 2. l becomes 1, r becomes 2. |
| While l<r (2nd) | - | - | - | `[2, 3, 6, 8]` | 1 | 2 | ans += gcd(3, 6) = 3 (Total ans = 5). l becomes 2, r becomes 1. Loop ends. |
| Return | - | - | - | `[2, 3, 6, 8]` | 2 | 1 | Returns final answer: `5`. |

---

## Time & Space Complexity

- **Time Complexity:** O(n log n). Building the `prefixGcd` array takes O(n) time because computing the GCD of two numbers takes a constant amount of time. However, sorting the array `a` takes O(n log n) time, which dominates the overall runtime. The two-pointer pairing loop runs in O(n) time. Therefore, the total time complexity is O(n log n).
- **Space Complexity:** O(n). We create a new vector `a` of size `n` to store the prefix GCD values. Aside from a few scalar tracking variables, no extra data structures are used.

**Is this already the most optimal possible complexity, or can it be improved?**

This code is already optimal in terms of asymptotic time complexity. Because the problem explicitly requires us to sort the `prefixGcd` array before pairing elements, any comparison-based sorting algorithm requires at least O(n log n) time in the worst case. 

---

## Edge Cases Handled

- **Single element array (`n = 1`):** The loop `for(int i=0; i<n; i++)` runs once, `prefixGcd` has 1 element, the sort does nothing, and the `while(l < r)` loop is skipped entirely because `l` (0) is not less than `r` (0). The function correctly returns `0`.
- **Odd length arrays:** When `n` is odd, the two-pointer loop stops when `l` meets `r` in the center. The middle element is naturally skipped, matching the rule that the middle element remains unpaired and should be ignored.
- **Duplicate values:** Duplicate numbers inside `nums` or duplicate values inside `prefixGcd` are handled smoothly by both the sorting step and the GCD function (e.g., gcd(2, 2) = 2).
- **Large numbers:** The constraints state that `nums[i]` can be up to 10^9. The code uses `long long ans` to accumulate the total sum, preventing integer overflow if many large GCD values are added together.
