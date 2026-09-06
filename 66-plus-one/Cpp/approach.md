![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.1%20MB%20(beats%2098.74%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a number written down on a row of sticky notes, with one digit on each note. For example, the number 123 is spread across three sticky notes: 1, then 2, then 3. 

The problem asks us to take this array of digits, add 1 to the total number it represents, and return the new number as a new array of digits. 

If we add 1 to 123, we get 124, so we should return [1, 2, 4]. If the number is 9, adding 1 gives us 10, so we should return [1, 0]. The digits are always ordered normally, meaning the biggest place value (the hundreds or thousands) comes first, and there are never any unnecessary zero digits sitting at the very front.

## Intuition

If you were doing this math on paper, you would start at the rightmost digit (the ones place) and add 1. If that digit becomes 10, you write down a 0 and "carry" a 1 over to the next digit to the left. You keep moving left as long as there is a carry. 

The aha moment for this code is that we can simulate this exact right-to-left addition using a loop. We start at the very last index of the vector (the rightmost digit), add 1, and manage our carry. 

Because we are building our result backwards (from right to left), we push our calculated digits into a temporary storage vector and then reverse that vector at the very end so it reads correctly from left to right. If we hit a point where the carry becomes 0, we can actually stop early and just copy over the remaining untouched digits from the left side of the original array.

## Approach

Here is how the code executes step-by-step, using the actual variables and logic:

* `int size = digits.size()-1;` sets up our starting position at the very last index of the input vector, which represents the rightmost (least significant) digit.
* `vector<int>ans_vec;` creates a blank vector to hold our resulting digits as we calculate them.
* `int carry = 0;` initializes our carry value to 0, since we haven't carried anything over yet.
* `int sum;` declares a variable to hold the combined value of a digit, the carry, and our added 1.
* `while( size >= 0 ){` starts a loop that will walk backward through the digits array from right to left as long as our index is valid.
* `if( carry == 0 ){ sum = digits[size] + 1 + carry; } else { sum = digits[size] + carry ; }` calculates the sum for the current digit. On the very first step, because `carry` is 0, it adds 1 to the current digit. On later steps if a carry exists, it just adds the digit and the carry.
* `ans_vec.push_back( sum % 10 );` takes the last digit of our sum (for example, 4 from 14) and saves it into our answer vector.
* `carry = sum / 10 ;` calculates the new carry value (for example, 1 from 14) to pass along to the next digit to the left.
* `size--;` moves our index one position to the left.
* `if( carry == 0 ){ break; }` checks if our carry is gone. If we no longer have a carry to pass left, we can stop the addition loop early because the rest of the number will stay unchanged.
* `if( carry ){ ans_vec.push_back( carry ); }` runs after the loop if we finished walking through all digits and still have a leftover carry (like turning 9 into 10). It pushes that final 1 into our answer vector.
* `while( size >= 0 ){ ans_vec.push_back( digits[size--]); }` runs if we broke out of the first loop early. It grabs all the remaining untouched digits from the left side of the original array and copies them into our answer vector.
* `reverse( ans_vec.begin() , ans_vec.end());` flips our answer vector around because we built it backwards from right to left.
* `return ans_vec;` hands back the final correctly ordered array of digits.

## Dry Run

### Case 1: Typical case ([1, 2, 3])

| size | carry | sum | digits[size] | ans_vec (contents) | Action |
|---|---|---|---|---|---|
| 2 | 0 | 4 | 3 | [4] | Add 1 to last digit (3+1=4), carry is 0, break loop |
| 2 | 0 | - | 2 | [4, 2] | Second while loop copies remaining digit 2 |
| 1 | 0 | - | 1 | [4, 2, 1] | Second while loop copies remaining digit 1 |
| -1 | 0 | - | - | [1, 2, 4] | Vector reversed to [1, 2, 4] and returned |

### Case 2: Edge case with a single 9 ([9])

| size | carry | sum | digits[size] | ans_vec (contents) | Action |
|---|---|---|---|---|---|
| 0 | 0 | 10 | 9 | [0] | Add 1 to digit (9+1=10), push 0, new carry is 1, loop ends |
| -1 | 1 | - | - | [0, 1] | First loop ends, leftover carry 1 is pushed |
| -1 | 1 | - | - | [1, 0] | Vector reversed to [1, 0] and returned |

## Time & Space Complexity

**Time:** O(N) — where N is the number of digits in the array. In the worst-case scenario (like an array of all nines, such as 999), the code visits every single digit once during the addition loop. Reversing the vector at the end also takes linear time proportional to the number of digits.

**Space:** O(N) — because we create a new vector `ans_vec` to store the results, which grows linearly with the size of the input array. 

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

No further improvement is possible for time complexity. Any solution to this problem must look at least at the last digit, and in the worst-case scenario (handling numbers made entirely of nines), every single digit must be inspected and updated. Therefore, touching every element gives a mandatory lower bound of O(N) time. 

However, the space complexity can technically be optimized from O(N) down to O(1) auxiliary space. The current code allocates a brand-new vector of size N plus one. An in-place solution could modify the original input vector directly (or handle resizing if a carry spills past the front), avoiding the creation of a separate tracking vector and the final reversal step. 

That said, because the constraints cap the array length at 100, the current solution's memory usage is already microscopic and beats 98.74 percent of submissions. It is practically optimal for everyday use, even if a theoretical O(1) space variant exists.

## Edge Cases Handled

* **Single-digit numbers that turn into two digits (e.g., [9]):** Correctly handles the leftover carry after the loop finishes, producing [1, 0].
* **Numbers packed with nines that cascade carries (e.g., [9, 9, 9]):** Keeps the carry moving left across multiple positions until it runs out or builds a new leading digit.
* **Normal numbers where addition requires no carries (e.g., [1, 2, 3]):** Breaks out of the carry loop early and efficiently copies the remaining prefix digits.
* **Maximum constraint size (lengths up to 100 digits):** Easily handled well within standard execution time limits without integer overflow issues, because digits are stored individually in a vector rather than a primitive integer type.
