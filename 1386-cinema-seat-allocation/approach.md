![Runtime](https://img.shields.io/badge/Runtime-76%20ms%20(beats%2034.39%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-86.67%20MB%20(beats%2018.53%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine a movie theater with `n` rows of seats. Each row has 10 seats side-by-side, numbered from 1 to 10. 

Some seats are already booked, given in an array called `reservedSeats`.

We need to seat 4-person families together in a single row. A family must sit in 4 consecutive seats. However, seats 1 and 10 are on the outer aisles, so a 4-person group can only sit in one of three possible 4-seat blocks in a row:
1. **Left block:** Seats 2, 3, 4, 5
2. **Middle block:** Seats 4, 5, 6, 7
3. **Right block:** Seats 6, 7, 8, 9

Notice that a single row can hold at most **2 families** (the Left block and the Right block at the same time). The Middle block overlaps with both the Left and Right blocks, so if you place a family in the Middle block, you cannot place any other family in that row.

Our goal is to find the **maximum total number of four-person families** we can seat across all `n` rows.

---

## Intuition

If a row has no reserved seats at all, we can fit 2 families in it (Left block and Right block). 

So, we can start with an initial assumption: every row can host 2 families, giving us `2 * n` total potential spots. Then, we only look at the rows that actually have reserved seats and figure out how many families we lose in those rows.

Seats 1 and 10 do not matter because no 4-person block uses them. 

For seats 2 through 9, any reserved seat will ruin one or more of the 3 possible blocks (Left, Middle, Right):
- A seat between 2 and 5 ruins the **Left block** (index 0).
- A seat between 4 and 7 ruins the **Middle block** (index 1).
- A seat between 6 and 9 ruins the **Right block** (index 2).

For any row with reserved seats, we count how many of these 3 blocks are ruined:
- If **all 3 blocks** are ruined, 0 families can sit in this row. Since we originally assumed 2 families could sit here, we subtract 2 from our total.
- If **1 or 2 blocks** are ruined, at least 1 valid block remains (either Left, Right, or Middle). So 1 family can sit here. We subtract 1 from our total.

---

## Approach

Here is how the code implements this logic step-by-step:

* `unordered_map<int, unordered_set<int>> seats;`  
  Creates a hash map where each key is a row number, and the value is a hash set storing which block regions (0 for Left, 1 for Middle, 2 for Right) are ruined by reserved seats in that row.
* `for (auto& r : reservedSeats)`  
  Loops through each reserved seat in the input list.
* `int row = r[0]; int seat = r[1];`  
  Extracts the row number and seat number for the current reserved seat.
* `if (seat >= 2 && seat <= 5) { seats[row].insert(0); }`  
  If the reserved seat is between 2 and 5, it marks block region 0 (Left block) as ruined for this row.
* `if (seat >= 4 && seat <= 7) { seats[row].insert(1); }`  
  If the reserved seat is between 4 and 7, it marks block region 1 (Middle block) as ruined for this row.
* `if (seat >= 6 && seat <= 9) { seats[row].insert(2); }`  
  If the reserved seat is between 6 and 9, it marks block region 2 (Right block) as ruined for this row.
* `int total = 2 * n;`  
  Initializes the total count assuming every single row starts with 2 valid family spots.
* `for (auto& [row, groups] : seats)`  
  Iterates through only the rows that had at least one seat blocking a region.
* `if (groups.size() == 3) { total -= 2; }`  
  If all 3 block regions (Left, Middle, Right) are ruined, 0 families can sit in this row. Deducts 2 spots from `total`.
* `else { total -= 1; }`  
  If only 1 or 2 regions are ruined, exactly 1 family can still sit in this row. Deducts 1 spot from `total`.
* `return total;`  
  Returns the final maximum number of families that can be seated.

---

## Dry Run

### Case 1: Typical case with multiple rows
Input: `n = 3`, `reservedSeats = [[1,2],[1,3],[1,8],[2,6],[3,1],[3,10]]`

Initial state: `total = 2 * 3 = 6`, `seats = {}`

1. Process `reservedSeats`:
   - `[1,2]`: seat 2 blocks region 0 -> `seats[1] = {0}`
   - `[1,3]`: seat 3 blocks region 0 -> `seats[1] = {0}`
   - `[1,8]`: seat 8 blocks region 2 -> `seats[1] = {0, 2}`
   - `[2,6]`: seat 6 blocks region 1 and 2 -> `seats[2] = {1, 2}`
   - `[3,1]`: seat 1 is ignored (not in range 2..9)
   - `[3,10]`: seat 10 is ignored (not in range 2..9)

2. Evaluate affected rows in `seats`:

| Row (`row`) | Ruined Regions (`groups`) | `groups.size()` | Action | Total Families (`total`) |
| :--- | :--- | :--- | :--- | :--- |
| Start | - | - | Initialized `2 * 3` | 6 |
| Row 1 | `{0, 2}` | 2 | `groups.size() != 3`, subtract 1 | 5 |
| Row 2 | `{1, 2}` | 2 | `groups.size() != 3`, subtract 1 | 4 |

Final Return: `4`

---

### Case 2: Row where all blocks are ruined
Input: `n = 4`, `reservedSeats = [[4,3],[1,4],[4,6],[1,7]]`

Initial state: `total = 2 * 4 = 8`, `seats = {}`

1. Process `reservedSeats`:
   - `[4,3]`: seat 3 blocks region 0 -> `seats[4] = {0}`
   - `[1,4]`: seat 4 blocks region 0 and 1 -> `seats[1] = {0, 1}`, `seats[4] = {0}`
   - `[4,6]`: seat 6 blocks region 1 and 2 -> `seats[4] = {0, 1, 2}`
   - `[1,7]`: seat 7 blocks region 1 and 2 -> `seats[1] = {0, 1, 2}`

2. Evaluate affected rows in `seats`:

| Row (`row`) | Ruined Regions (`groups`) | `groups.size()` | Action | Total Families (`total`) |
| :--- | :--- | :--- | :--- | :--- |
| Start | - | - | Initialized `2 * 4` | 8 |
| Row 1 | `{0, 1, 2}` | 3 | `groups.size() == 3`, subtract 2 | 6 |
| Row 4 | `{0, 1, 2}` | 3 | `groups.size() == 3`, subtract 2 | 4 |

Final Return: `4`

---

## Time & Space Complexity

- **Time Complexity:** **O(M)**, where M is the length of `reservedSeats`. We loop through all reserved seats once, doing O(1) hash map and set insertions. Then we loop through the modified rows in our map, which is at most M rows.
- **Space Complexity:** **O(M)**. We store up to M entries in our `unordered_map` and nested `unordered_set`.

### Can this be improved?

Yes, memory overhead and execution speed can both be improved.

The original code uses nested hash sets (`unordered_map<int, unordered_set<int>>`), which causes extra memory allocations and hash lookups. 

Instead of storing ruined block numbers in a set, we can store reserved seats directly using **Bit Manipulation** (a bitmask integer) for each row. 

Each seat in a row (from seat 1 to 10) can be represented by a bit in a 10-bit integer.
- Seat `s` corresponds to the bit position `1 << (s - 1)`.
- **Left block** (seats 2..5) bitmask: bits 2, 3, 4, 5 active -> binary `0b0000111100` (value 60).
- **Middle block** (seats 4..7) bitmask: bits 4, 5, 6, 7 active -> binary `0b0001111000` (value 120).
- **Right block** (seats 6..9) bitmask: bits 6, 7, 8, 9 active -> binary `0b0111100000` (value 480).

We check if a block is available using bitwise AND (`mask & block_mask`). If the result is `0`, none of those seats are reserved!

Here is the key improved code snippet:

```cpp
unordered_map<int, int> seats;

// Set bit for reserved seats
for (auto& r : reservedSeats) {
    seats[r[0]] |= (1 << (r[1] - 1));
}

int total = 2 * n;

for (auto& [row, mask] : seats) {
    bool left  = (mask & 60) == 0;   // seats 2..5 free
    bool mid   = (mask & 120) == 0;  // seats 4..7 free
    bool right = (mask & 480) == 0;  // seats 6..9 free

    if (left && right) {
        // Both left and right fit; no reduction needed
    } else if (left || right || mid) {
        total -= 1; // Exactly 1 family fits
    } else {
        total -= 2; // 0 families fit
    }
}
```

- **Improved Time Complexity:** **O(M)** — still linear, but runs much faster due to cheap bitwise operations instead of set operations.
- **Improved Space Complexity:** **O(K)**, where K is the number of distinct rows with reserved seats (at most M). Each row uses just one integer instead of a heap-allocated hash set.

This bitmask solution achieves the theoretical best complexity for both time and space.

---

## Edge Cases Handled

- **Seats 1 and 10 reserved:** Seats 1 and 10 never overlap with any 4-person block. The code correctly ignores their impact on family placement counts.
- **Large number of total rows (`n` up to 10^9):** The algorithm does not construct an array of size `n`. It only uses a hash map to process rows that actually have reserved seats, preventing memory limits from being exceeded.
- **Empty or untouched rows:** Any row not listed in `reservedSeats` automatically contributes 2 family spots toward the initial `total = 2 * n`.
- **Duplicate reserved seats in same row/region:** Duplicate entries for the same region insert the same key into the `unordered_set` (or OR the same bit in bitmask), maintaining correct state without double counting.
