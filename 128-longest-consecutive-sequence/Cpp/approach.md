![Runtime](https://img.shields.io/badge/Runtime-7%20ms%20(beats%2098.91%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-60.2%20MB%20(beats%2085.27%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you have a messy pile of integer numbers, like [100, 4, 200, 1, 3, 2]. The problem asks you to find the longest chain of numbers that follow each other consecutively, meaning each number is bigger than the previous one by exactly 1. 

In that example, the numbers can be arranged into a consecutive sequence: 1, 2, 3, 4. That chain has a length of 4. Even though the numbers are scattered in the input list, we only care about finding the longest unbroken chain of numbers that exist in that list, regardless of their original order.

## Intuition

The "aha" moment for this specific solution is realizing that **sorting** the list makes finding consecutive numbers very easy. If you line up all the numbers from smallest to largest, any consecutive numbers will naturally end up sitting right next to each other in the line. 

Once sorted, you just walk down the list from left to right. You check if the current number is just 1 greater than the previous number. If it is, your current streak grows by 1. If it skips a number, your streak resets. If it is an exact duplicate, you simply skip over it. By keeping a running track of the longest streak you have ever seen while walking down the sorted list, you will find the answer at the end.

## Approach

Here is how the code executes step by step:

* `if( nums.size() == 0 ) { return 0; }`: Checks if the input list is completely empty. If it has zero numbers, it immediately returns 0 because no sequence can exist.
* `sort( nums.begin() , nums.end() );`: Rearranges all the numbers in the list from smallest to largest. This brings potential consecutive numbers side by side.
* `int lng = 1 , cnt = 1 , ele = nums[0];`: Initializes three tracking variables. `lng` keeps the record of the longest streak found so far. `cnt` keeps track of the current active streak. `ele` remembers the last valid number we looked at, starting with the very first number in the sorted list.
* `for( int i=1 ; i<nums.size() ; i++ )`: Loops through the sorted list starting from the second element (index 1) all the way to the end.
* `if( ele == nums[i] ) { continue; }`: Checks if the current number is a duplicate of the previous number (`ele`). If it is, it skips it (`continue`) so duplicates do not break or alter our streak count.
* `else if( ele+1 == nums[i] )`: Checks if the current number is exactly 1 greater than the previous number. This means our consecutive sequence is continuing.
* `cnt++; ele = nums[i]; lng = max( lng , cnt );`: Inside the consecutive check, we increase our current streak count (`cnt`), update our last seen element tracker (`ele`), and update our all-time longest streak (`lng`) if the current streak is now the largest we have seen.
* `else { ele = nums[i]; cnt=1; }`: Handles the case where the chain breaks (the number is neither a duplicate nor consecutive). It resets the last seen element (`ele`) to the current number and resets the current streak count (`cnt`) back to 1.
* `return lng;`: After the loop finishes checking all elements, it returns the final length of the longest consecutive sequence found (`lng`).

## Dry Run

### Case 1: Typical case with consecutive numbers and duplicates
**Input:** `nums = [100, 4, 200, 1, 3, 2]`
After sorting, `nums` becomes: `[1, 2, 3, 4, 100, 200]`

| `i` | `nums[i]` | `ele` | `cnt` | `lng` | Action |
| --- | --- | --- | --- | --- | --- |
| - | - | 1 | 1 | 1 | Initialization before loop |
| 1 | 2 | 2 | 2 | 2 | 1+1 equals 2; streak grows, `lng` updates to 2 |
| 2 | 3 | 3 | 3 | 3 | 2+1 equals 3; streak grows, `lng` updates to 3 |
| 3 | 4 | 4 | 4 | 4 | 3+1 equals 4; streak grows, `lng` updates to 4 |
| 4 | 100 | 100 | 1 | 4 | Chain breaks; `ele` becomes 100, `cnt` resets to 1 |
| 5 | 200 | 200 | 1 | 4 | Chain breaks; `ele` becomes 200, `cnt` resets to 1 |

*Final output:* `4`

### Case 2: Input with duplicate values
**Input:** `nums = [1, 0, 1, 2]`
After sorting, `nums` becomes: `[0, 1, 1, 2]`

| `i` | `nums[i]` | `ele` | `cnt` | `lng` | Action |
| --- | --- | --- | --- | --- | --- |
| - | - | 0 | 1 | 1 | Initialization before loop |
| 1 | 1 | 1 | 2 | 2 | 0+1 equals 1; streak grows, `lng` updates to 2 |
| 2 | 1 | 1 | 2 | 2 | `ele` (1) equals `nums[i]` (1); duplicate is ignored via `continue` |
| 3 | 2 | 2 | 3 | 3 | 1+1 equals 2; streak grows, `lng` updates to 3 |

*Final output:* `3`

## Time & Space Complexity

- **Time:** O(n log n) — The code relies heavily on the `sort` function to order the elements. In C++, sorting an array or vector of size n takes O(n log n) time. The subsequent loop runs through the array a single time, which takes O(n) time. Because O(n log n) grows faster than O(n), the total time complexity is dominated by the sorting step.
- **Space:** O(1) or O(log n) auxiliary space — The sorting algorithm uses a small amount of extra memory for its internal recursive stack, and we only use a few tracking variables (`lng`, `cnt`, `ele`), meaning no extra data structures like hash tables are created.

**Is this already the most optimal possible complexity, or can it be improved?**

Yes, it can be improved. While this sorting approach is very clean and fast in practice (often beating many submissions due to hardware cache efficiency), its theoretical time complexity is O(n log n) because of the sort step. 

The problem asks for an O(n) time solution. We can achieve true O(n) time complexity by abandoning sorting entirely and using a Hash Set instead. 

**How the optimized logic works:**
Instead of sorting, we throw all numbers into a hash set for instant O(1) lookups. Then, we loop through each number and ask: *"Is this number the start of a sequence?"* We know a number is the start of a sequence if its partner-minus-one (number - 1) is **not** in the set. If it is a starting number, we keep checking if number + 1, number + 2, and so on exist in the set, counting how long the streak goes. Because we only expand sequences from their actual starting points, each number is only inspected a constant number of times, dropping the time complexity down to pure O(n).

Here is what the optimized code snippet looks like:

```cpp
int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;
    unordered_set<int> num_set(nums.begin(), nums.end());
    int longestStreak = 0;

    for (int num : num_set) {
        // Check if this is the start of a sequence
        if (num_set.find(num - 1) == num_set.end()) {
            int currentNum = num;
            int currentStreak = 1;

            // Count consecutive numbers upwards
            while (num_set.find(currentNum + 1) != num_set.end()) {
                currentNum += 1;
                currentStreak += 1;
            }

            longestStreak = max(longestStreak, currentStreak);
        }
    }
    return longestStreak;
}
```

- **Resulting improved complexity:** **Time:** O(n), because hash set insertions and lookups take O(1) average time, and every number is visited at most twice. **Space:** O(n), because we store all unique numbers in the hash set.
- **Theoretical best possible complexity:** O(n) time is the theoretical best possible complexity for this problem, because you must at least look at every element in the input array at least once to know if it belongs to a sequence. The hash set approach successfully reaches this optimal limit.

## Edge Cases Handled

- **Empty input array:** Handled immediately by `if( nums.size() == 0 ) { return 0; }`, preventing crashes or out-of-bounds errors.
- **Duplicate values:** Handled by `if( ele == nums[i] ) { continue; }`, ensuring that repeating numbers do not falsely reset the sequence count or distort length calculations.
- **Single element array:** The loop does not execute because `i < nums.size()` evaluates to false (`1 < 1`), safely bypassing the loop and returning the initial default length of 1.
- **Negative numbers:** Handled naturally because C++ sorting and mathematical additions like `ele + 1` work correctly across negative ranges (for instance, moving from -2 to -1).
