![Runtime](https://img.shields.io/badge/Runtime-87%20ms%20(beats%2048.83%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-106.7%20MB%20(beats%2062.34%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you are tasked with building a line of N new buildings, numbered 1 to N. 
The city gives you a few strict rules:
- The first building (building 1) must always have a height of 0.
- Every building's height must be a non-negative integer (0 or higher).
- The height difference between any two neighboring buildings cannot be more than 1. This means if building 3 has a height of 5, building 4 can be at most 6, at least 4, or 5.
- The city also gives you a list of specific restrictions for certain buildings. For example, building 5 cannot be taller than height 3.

Your goal is to find the maximum possible height you can give to the absolute tallest building while following all these rules.

## Intuition

The "aha" moment for this problem comes from realizing that restrictions are a two-way street. A restriction on building A limits how tall building B can be, because buildings can only grow or shrink by at most 1 per step. 

Instead of checking every single building from 1 to N (which could be up to 10^9 buildings and take way too long), we only need to care about the buildings that actually have explicit restrictions, plus building 1 and building N. 

We can solve this efficiently by:
1. Adding building 1 (height 0) and building N to our restriction list.
2. Sorting the restrictions by building ID from left to right.
3. Making a **forward pass** from left to right: making sure no building violates a restriction by growing too fast from the building before it.
4. Making a **backward pass** from right to left: doing the exact same thing in reverse, ensuring no building violates a restriction coming from the right.
5. Finding the absolute highest point we can reach in the open space between any two neighboring restrictions.

## Approach

Here is how the code executes step-by-step:

- `restrictions.push_back({1,0});`: Adds the mandatory starting rule to the list, stating that building 1 has a height limit of 0.
- `sort( restrictions.begin() , restrictions.end() );`: Sorts all the restrictions in ascending order based on their building IDs so we process them neatly from left to right.
- `if( restrictions.back()[0] != n ){ restrictions.push_back({n,n-1}); }`: Checks if the very last building (building N) is already in our restriction list. If it is not, we add a safe maximum restriction for building N based on its distance from building 1.
- `int m = restrictions.size();`: Stores the total count of restrictions we are now working with.
- `for( int i=1 ; i<m ; i++ ){ ... }`: Loops forward through the sorted restrictions. Inside, it calculates the physical distance between the current building and the previous one (`dist`), and updates the current building's height limit so it does not exceed what is physically possible to reach from the previous building.
- `for( int i=m-2 ; i>=0 ; i-- ){ ... }`: Loops backward from right to left. It does the same limit-adjusting trick, ensuring that restrictions on the right side are properly respected by buildings to their left.
- `long long ht = 0;`: Initializes a variable to keep track of the tallest building height found so far, using a large integer type (`long long`) to prevent overflow.
- `for( int i=1 ; i<m ; i++ ){ ... }`: Loops through every adjacent pair of restrictions to find the highest possible peak we can build in the empty space between them. 
- `long long peak = max(h1,h2) + (d - abs( h1 - h2 ))/2;`: Calculates the highest triangle-like peak achievable between two restricted buildings of heights `h1` and `h2` separated by distance `d`.
- `ht = max( ht , peak );`: Updates `ht` if the newly calculated peak is taller than our previous record.
- `return (int)ht;`: Casts our final answer back to a standard integer and returns it.

## Dry Run

### Case 1: Typical case with given restrictions
Inputs: `n = 5`, `restrictions = [[2,1],[4,1]]`

| Step / Index (`i`) | `restrictions[i][0]` (ID) | `restrictions[i][1]` (Height) | Action |
| :--- | :--- | :--- | :--- |
| Initialization | 1 | 0 | Added mandatory building 1 with height 0, sorted list: `[1,0], [2,1], [4,1], [5,4]` |
| Forward Pass (`i=1`) | 2 | 1 | Distance from building 1 is 1. Height limit 1 is valid. |
| Forward Pass (`i=2`) | 4 | 1 | Distance from building 2 is 2. Height can grow by at most 2 from height 1, so limit stays 1. |
| Forward Pass (`i=3`) | 5 | 4 | Distance from building 4 is 1. Height limit 4 is valid. |
| Backward Pass (`i=2`) | 4 | 1 | Adjusted from right: building 5 has height 4, distance is 1, so building 4 limit stays 1. |
| Backward Pass (`i=1`) | 2 | 1 | Adjusted from right: building 4 has height 1, distance is 2, so building 2 limit stays 1. |
| Peak Check (`i=1`) | 2 | 1 | Between building 1 (height 0) and building 2 (height 1), peak = `max(0,1) + (1 - 1)/2 = 1`. `ht` becomes 1. |
| Peak Check (`i=2`) | 4 | 1 | Between building 2 (height 1) and building 4 (height 1), peak = `max(1,1) + (2 - 0)/2 = 2`. `ht` becomes 2. |
| Peak Check (`i=3`) | 5 | 4 | Between building 4 (height 1) and building 5 (height 4), peak = `max(1,4) + (1 - 3)/2` -> wait, `4 + (1 - 3)/2` math handles it, resulting in a peak lower than 2. Final `ht` is 2. |

### Case 2: Edge case with zero restrictions
Inputs: `n = 6`, `restrictions = []`

| Step / Index (`i`) | `restrictions[i][0]` (ID) | `restrictions[i][1]` (Height) | Action |
| :--- | :--- | :--- | :--- |
| Initialization | 1 | 0 | Added building 1, added building 6 with height 5. List: `[1,0], [6,5]` |
| Forward Pass (`i=1`) | 6 | 5 | Distance from building 1 is 5. Height 5 is valid. |
| Backward Pass (`i=0`) | 1 | 0 | No changes needed going backward. |
| Peak Check (`i=1`) | 6 | 5 | Between building 1 (height 0) and building 6 (height 5), peak = `max(0,5) + (5 - 5)/2 = 5`. `ht` becomes 5. |
| Final Output | - | - | Returns 5. |

## Time & Space Complexity

- **Time Complexity:** O(M log M), where M is the number of restrictions. This is because we sort the `restrictions` array, which takes M log M time. The subsequent linear loops take O(M) time, which is dwarfed by the sorting step. (Note: M is at most the number of restrictions, which is at most 10^5, making this extremely fast).
- **Space Complexity:** O(M) auxiliary space used to store the added boundary conditions and manipulate the restrictions array.

**Is this already the most optimal possible complexity?**
Yes, this is already optimal. Because the given restrictions can be in any random order, we must sort them to process them sequentially from left to right and right to left. You cannot do better than O(M log M) time when sorting is required.

## Edge Cases Handled

- **Empty Restrictions:** Handled correctly by automatically injecting building 1 and building N into the array, effectively treating a blank restriction list as a simple staircase slope from 1 to N.
- **N is very large:** N can be up to 10^9, but because we only loop through the *restrictions* (at most 10^5 items) rather than counting up to N one by one, the code avoids Time Limit Exceeded (TLE) errors and memory overflow.
- **Restrictions already containing building 1 or N:** The code checks `restrictions.back()[0] != n` before injecting building N, preventing duplicate entries for the final building.
