![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-10.1%20MB%20(beats%2070.28%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you have a square photo or digital image represented as a grid of numbers, where the width and height are the exact same size, meaning an N by N matrix. The problem asks you to rotate this entire image 90 degrees clockwise. 

For example, if the top row has the numbers 1, 2, and 3, after a 90-degree clockwise rotation, those numbers should now form the rightmost column, going down from top to bottom. 

The catch is that you must do this **in-place**. This means you are not allowed to create a second grid to copy the rotated numbers into. You have to rearrange the numbers inside the exact same grid given to you, modifying it directly.

## Intuition

Rotating a matrix directly by 90 degrees in-place can feel tricky because items have to move along circular paths. The "aha" moment for this solution is realizing you can break a 90-degree rotation down into two simpler, much easier steps:

1. **Transpose the matrix**: Swap every number across the diagonal line that runs from the top-left to the bottom-right. If a number is at row 2 and column 3, you swap it with the number at row 3 and column 2. 
2. **Reverse each row**: Take every individual row and flip it horizontally so the left elements swap with the right elements.

If you take any grid, first flip it across its diagonal, and then reverse every horizontal row, the resulting grid will be perfectly rotated 90 degrees clockwise. This geometric trick completely avoids complex multi-element circular swaps.

## Approach

* `void rev( vector<vector<int>>& matrix,int s,int e,int i)`: Defines a helper function named `rev` that takes the matrix, a start pointer `s`, an end pointer `e`, and a row index `i`, used to flip a specific row horizontally.
* `while(s<e)`: Loops as long as the start pointer is to the left of the end pointer, meaning we have not reached the middle of the row yet.
* `swap(matrix[i][s++],matrix[i][e--]);`: Swaps the elements at the start and end of the row, then moves the start pointer right and the end pointer left.
* `for( int i=0 ; i<matrix.size() ; i++)`: Loops through every row index `i` of the outer matrix.
* `for( int j=i+1 ; j<matrix.size() ; j++)`: Loops through the column index `j` starting at `i + 1` to target elements strictly above the diagonal, ensuring we only swap each pair once during the transpose step.
* `swap( matrix[i][j],matrix[j][i]);`: Swaps the element at row `i`, column `j` with the element at row `j`, column `i`, completing the transpose.
* `rev(matrix,0,matrix[i].size()-1,i);`: Calls the `rev` function for every row from the first column index 0 to the last column index to reverse each row horizontally and finish the rotation.

## Dry Run

### Case 1: Typical 3x3 matrix (Given Example)
Input: matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

| Step | i | j | matrix state after action | Action |
| --- | --- | --- | --- | --- |
| 1 | 0 | 1 | [[1, 4, 3], [2, 5, 6], [7, 8, 9]] | Swap matrix[0][1] (2) with matrix[1][0] (4) |
| 2 | 0 | 2 | [[1, 4, 7], [2, 5, 6], [3, 8, 9]] | Swap matrix[0][2] (3) with matrix[2][0] (7) |
| 3 | 1 | 2 | [[1, 4, 7], [2, 5, 8], [3, 6, 9]] | Swap matrix[1][2] (6) with matrix[2][1] (8) |
| 4 | 0 | - | [[7, 4, 1], [2, 5, 8], [3, 6, 9]] | Reverse row 0 using `rev()` |
| 5 | 1 | - | [[7, 4, 1], [8, 5, 2], [3, 6, 9]] | Reverse row 1 using `rev()` |
| 6 | 2 | - | [[7, 4, 1], [8, 5, 2], [9, 6, 3]] | Reverse row 2 using `rev()` |

### Case 2: Minimum size 1x1 matrix (Edge case)
Input: matrix = [[5]]

| Step | i | j | matrix state after action | Action |
| --- | --- | --- | --- | --- |
| 1 | 0 | 1 | [[5]] | Inner loop for transpose does not run because `j` (1) is not less than `matrix[0].size()` (1) |
| 2 | 0 | - | [[5]] | `rev` runs, but `s` (0) is not less than `e` (0), so row 0 remains unchanged |

## Time & Space Complexity

**Time:** O(N^2) — The transpose step loops through roughly half of the total elements in an N by N matrix, and the row reversal step visits every element once. Both operations scale with the total number of cells, which is N squared.
**Space:** O(1) — The rotation is done completely in-place using a few variables for swapping, requiring no extra data structures or extra 2D arrays.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already the most optimal possible complexity. 

Because the problem requires you to touch and move every single one of the N squared elements in the matrix, you must spend at least O(N^2) time just to visit them. A time complexity of O(N^2) matches the size of the input, making it theoretically impossible to go any faster. Similarly, an auxiliary space complexity of O(1) is the absolute best possible because the rules state you cannot allocate another 2D matrix.

## Edge Cases Handled

* **Minimum size matrix (N = 1):** Handled correctly because loops and pointers for swapping and reversing will simply not execute or skip immediately without crashing.
* **Negative numbers and zero:** Handled correctly because integer values inside the matrix have no impact on the coordinate math and swapping logic.
* **Maximum constraints (N = 20):** Handled efficiently with well under the time limit since N squared operations for N = 20 is only 400 operations.
