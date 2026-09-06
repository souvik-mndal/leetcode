![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-11.7%20MB%20(beats%2011.40%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you have a row of colored balls. The balls are colored red, white, and blue. Right now, they are all mixed up. 

Your job is to sort them so that all the red balls come first, all the white balls come in the middle, and all the blue balls come at the very end. The problem uses the number 0 for red, 1 for white, and 2 for blue. 

You must do this sorting **in-place**, which means you rearrange the items inside the original list without making a copy of it. You are also not allowed to use the programming language's built-in sorting tool. You have to organize them using your own logic.

---

## Intuition

If you want to sort numbers that can only be 0, 1, or 2, a normal sorting algorithm (like Quicksort or Bubble Sort) is overkill. Those algorithms compare numbers against each other, which takes extra time. 

Instead, think of this like sorting dirty laundry into three separate baskets as you walk down a hallway: a basket for reds on the left, a basket for blues on the right, and a basket for whites in the middle. 

We can use **three pointers** (markers that track positions in the list) to do this in a single pass:
- A pointer at the left (`i`) to track where the next 0 should go.
- A pointer at the right (`j`) to track where the next 2 should go.
- A moving pointer (`index`) that walks through the list and inspects each number one by one.

When `index` finds a 0, it swaps it to the left side. When it finds a 2, it swaps it to the right side. When it finds a 1, it simply leaves it alone and moves forward. Because everything is swapped into its proper zone as we walk across, the array becomes fully sorted by the time `index` meets the right pointer.

---

## Approach

- `int i=0,j=nums.size()-1;` sets up two boundaries. The pointer `i` starts at the very beginning of the array to manage where zeros belong, and `j` starts at the very end of the array to manage where twos belong.
- `int index = 0;` creates our active scanning pointer that will inspect every element starting from the first position.
- `while( index <= j )` creates a loop that keeps running as long as our scanning pointer has not crossed past the right boundary pointer `j`.
- `if( nums[index] == 0 )` checks if the number we are currently looking at is a zero.
- `swap( nums[i] , nums[index]);` swaps the zero we found into the front section where zeros are supposed to live (at position `i`).
- `index++;i++;` moves both our scanning pointer and our zero-boundary pointer one step forward, because we successfully placed a zero.
- `else if( nums[index] == 2 )` checks if the number we are currently looking at is a two.
- `swap( nums[j] , nums[index] );` swaps this two over to the back section where twos belong (at position `j`).
- `j--;` moves the right boundary pointer one step inward, because we successfully placed a two at the back. (Notice we do *not* move `index` forward yet, because the number we just swapped from the back needs to be inspected too).
- `else{ index++; }` handles the case where the current number is a 1. Since 1s naturally belong in the middle, we just leave it where it is and move our scanning pointer forward.

---

## Dry Run

### Case 1: Typical case (Mixed values)
Input: `nums = [2, 0, 2, 1, 1, 0]`

| `index` | `i` | `j` | `nums` array state | Action taken |
| :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 5 | `[2, 0, 2, 1, 1, 0]` | Value is 2. Swap with `j` (index 5), decrement `j`. |
| 0 | 0 | 4 | `[0, 0, 2, 1, 1, 2]` | Value is 0. Swap with `i` (index 0), increment `index` and `i`. |
| 1 | 1 | 4 | `[0, 0, 2, 1, 1, 2]` | Value is 0. Swap with `i` (index 1), increment `index` and `i`. |
| 2 | 2 | 4 | `[0, 0, 2, 1, 1, 2]` | Value is 2. Swap with `j` (index 4), decrement `j`. |
| 2 | 2 | 3 | `[0, 0, 1, 1, 2, 2]` | Value is 1. Just increment `index`. |
| 3 | 2 | 3 | `[0, 0, 1, 1, 2, 2]` | Value is 1. Just increment `index`. |
| 4 | 2 | 3 | `[0, 0, 1, 1, 2, 2]` | Loop ends because `index` (4) is greater than `j` (3). |

### Case 2: Short array with three items
Input: `nums = [2, 0, 1]`

| `index` | `i` | `j` | `nums` array state | Action taken |
| :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 2 | `[2, 0, 1]` | Value is 2. Swap with `j` (index 2), decrement `j`. |
| 0 | 0 | 1 | `[1, 0, 2]` | Value is 1. Just increment `index`. |
| 1 | 0 | 1 | `[1, 0, 2]` | Value is 0. Swap with `i` (index 0), increment `index` and `i`. |
| 2 | 1 | 1 | `[0, 1, 2]` | Loop ends because `index` (2) is greater than `j` (1). |

---

## Time & Space Complexity

- **Time:** O(n) — The scanning pointer `index` starts at the beginning and moves forward one step at a time until it meets `j`. Every element is visited at most once, meaning the time grows in direct proportion to the size of the array (n).
- **Space:** O(1) — We only created a few single integer variables (`i`, `j`, `index`) to track positions. No extra arrays or data structures were built, so memory usage stays completely constant regardless of how large the input array is.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already completely optimal. 
- **Time Complexity:** You cannot sort an array faster than O(n) because you must at least look at every single element once to know what color it is. Since our code processes every element in a single pass, it achieves the absolute fastest possible time limit.
- **Space Complexity:** O(1) constant space is the best possible memory usage because the problem requires modifying the array in-place without extra storage.

---

## Edge Cases Handled

- **Already sorted arrays (e.g., `[0, 0, 1, 2, 2]`):** The pointers move smoothly across the array, recognize that elements are already in their correct places, and finish without making unnecessary changes.
- **Arrays with missing colors (e.g., only 0s and 2s, like `[2, 0, 2, 0]`):** The `else` block for 1s is simply skipped, and the 0 and 2 pointers handle the sorting correctly without crashing.
- **Smallest possible input size (`n = 1`):** With an array of length 1 (e.g., `[1]`), `index` equals `j` (0), the loop runs once, checks the value, and exits safely without errors.
