![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.1%20MB%20(beats%2083.01%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

The problem asks us to find how many **unique** pairs of numbers in an array have a difference equal to `k`. 

A pair is defined by two numbers at different positions in the array. Their absolute difference must equal `k`. Because we need unique pairs, picking the numbers `(1, 3)` multiple times from different array positions still counts as only 1 pair.

For example, if `nums = [3, 1, 4, 1, 5]` and `k = 2`:
* Pairs with a difference of 2 are `(1, 3)` and `(3, 5)`.
* Even though there are two `1`s in the input, we count `(1, 3)` only once.
* The output is `2`.

## Intuition

If we sort the array first, two helpful things happen:
1. Equal numbers get grouped together, making duplicate values easy to skip.
2. The numbers are ordered from smallest to largest.

For any number `nums[i]`, we can look for its matching partner `nums[i] + k`. Because the array is sorted, any valid partner must lie somewhere to the right of index `i`. 

Instead of checking every element to the right one by one, we can use **binary search** to instantly find if `nums[i] + k` exists. To avoid counting duplicate pairs, if `nums[i]` is identical to `nums[i-1]`, we skip it completely because we already checked for its pair partner in the previous step.

## Approach

* `int n = nums.size();` — Store the total number of elements in `nums` in variable `n`.
* `int cnt = 0;` — Initialize `cnt` to 0 to keep track of the total count of valid unique pairs found.
* `sort( nums.begin() , nums.end());` — Sort `nums` in ascending order so duplicate numbers are adjacent and we can use binary search on remaining elements.
* `for( int i=0 ; i<n ; i++ )` — Loop through each element in the sorted array, using `nums[i]` as the smaller value of a potential pair.
* `int rem = nums[i] + k;` — Calculate `rem`, which is the exact larger value needed to form a pair with `nums[i]`.
* `if( i>0 && nums[i] == nums[i-1] ) { continue; }` — Check if `nums[i]` is identical to the previous element. If so, skip it to prevent counting duplicate pairs.
* `if( binary_search(nums.begin()+i+1 , nums.end() , rem )) { cnt++; }` — Perform a binary search for `rem` strictly in the portion of the array after index `i`. If found, increment `cnt` by 1.
* `return cnt;` — Return the final count of unique k-diff pairs.

## Dry Run

### Example 1: Standard case with duplicate values
`nums = [3, 1, 4, 1, 5]`, `k = 2`  
After sorting: `nums = [1, 1, 3, 4, 5]`, `n = 5`, `cnt = 0`

| `i` | `nums[i]` | `rem` (`nums[i] + k`) | `cnt` | Action |
| --- | --- | --- | --- | --- |
| 0 | 1 | 3 | 1 | Search for 3 in `[1, 3, 4, 5]`. Found! Increment `cnt`. |
| 1 | 1 | - | 1 | `nums[1] == nums[0]`, skip loop iteration (`continue`). |
| 2 | 3 | 5 | 2 | Search for 5 in `[4, 5]`. Found! Increment `cnt`. |
| 3 | 4 | 6 | 2 | Search for 6 in `[5]`. Not found. |
| 4 | 5 | 7 | 2 | Search for 7 in `[]`. Not found. |

Final `cnt` = 2.

### Example 2: Edge case with k = 0
`nums = [1, 3, 1, 5, 4]`, `k = 0`  
After sorting: `nums = [1, 1, 3, 4, 5]`, `n = 5`, `cnt = 0`

| `i` | `nums[i]` | `rem` (`nums[i] + k`) | `cnt` | Action |
| --- | --- | --- | --- | --- |
| 0 | 1 | 1 | 1 | Search for 1 in `[1, 3, 4, 5]`. Found! Increment `cnt`. |
| 1 | 1 | - | 1 | `nums[1] == nums[0]`, skip loop iteration (`continue`). |
| 2 | 3 | 3 | 1 | Search for 3 in `[4, 5]`. Not found. |
| 3 | 4 | 4 | 1 | Search for 4 in `[5]`. Not found. |
| 4 | 5 | 5 | 1 | Search for 5 in `[]`. Not found. |

Final `cnt` = 1.

## Time & Space Complexity

* **Time Complexity:** **O(N * log N)** — Sorting the array takes O(N * log N) time. The loop runs N times, and inside the loop, `binary_search` takes O(log N) time. Thus, the overall time is dominated by O(N * log N).
* **Space Complexity:** **O(1)** — Beyond the internal stack space used by `std::sort` (which is O(log N)), the algorithm uses a fixed set of integer variables (`n`, `cnt`, `rem`, `i`), requiring O(1) auxiliary space.

### Can it be improved?

**Yes.** We can reduce the time complexity to **O(N)** by using a **Hash Map** (or frequency map) instead of sorting and binary search.

#### How the optimization works:
1. Count the frequency of every number in `nums` using a hash map.
2. Iterate through each unique key `x` in the map:
   * **If `k > 0`:** Check if `x + k` exists as a key in the map. If it exists, we found a valid pair `(x, x + k)`.
   * **If `k == 0`:** Check if the frequency of `x` is greater than 1. If it is, the number appears at least twice, forming a valid pair `(x, x)`.

Because hash map lookups take O(1) time on average, this process avoids sorting entirely.

```cpp
int findPairs(vector<int>& nums, int k) {
    unordered_map<int, int> counts;
    for (int x : nums) counts[x]++;
    
    int cnt = 0;
    for (auto [x, freq] : counts) {
        if (k > 0 && counts.count(x + k)) {
            cnt++;
        } else if (k == 0 && freq > 1) {
            cnt++;
        }
    }
    return cnt;
}
```

* Line 2 creates a frequency map of all elements.
* Line 6 loops through unique values only, naturally avoiding duplicate pairs.
* Line 7 checks if `x + k` exists in O(1) time when `k > 0`.
* Line 9 checks if `x` appears at least twice when `k == 0`.

* **Improved Complexity:** **O(N) Time**, **O(N) Space**.
* **Theoretical Best Complexity:** **O(N) Time** — We must inspect each element at least once to know its value. The Hash Map approach achieves this optimal time limit.

## Edge Cases Handled

* **Duplicate Numbers in Input:** Handled by skipping indices where `nums[i] == nums[i-1]`.
* **`k = 0`:** Handled because binary search only looks in `nums.begin() + i + 1` onward. It strictly checks if a *second* copy of the same number exists without matching an element with itself.
* **Negative Numbers:** Handled cleanly because sorting and arithmetic operations (`nums[i] + k`) work identical for negative values.
* **No Valid Pairs Found:** Returns `cnt = 0` smoothly as binary search fails on every element.
