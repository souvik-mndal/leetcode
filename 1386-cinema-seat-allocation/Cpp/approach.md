![Runtime](https://img.shields.io/badge/Runtime-76%20ms%20(beats%2027.90%25)-orange?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-86.7%20MB%20(beats%2019.04%25)-red?style=for-the-badge)

---

## Problem Explained

A movie theater has `n` rows. Each row has 10 seats numbered 1 to 10. You are given a list of already reserved seats, where `[row, seat]` tells you which seat is taken. 

You need to place four-person family groups in the theater. Each four-person group must sit together in four adjacent seats in the same row. There are only three valid seat arrangements for a group in a single row:
1. **Left block:** Seats 2, 3, 4, 5
2. **Middle block:** Seats 4, 5, 6, 7
3. **Right block:** Seats 6, 7, 8, 9

Note that seats 1 and 10 cannot be part of any group. Also, notice that the Middle block overlaps with both the Left block (seats 4, 5) and the Right block (seats 6, 7). That means a single row can hold at most **2 groups** (Left block and Right block).

Your goal is to return the maximum total number of four-person groups that can be seated.

---

## Intuition

If no seats were reserved at all, every row could hold 2 groups. For `n` rows, the starting answer would simply be `2 * n`.

Since `n` can be as large as 1,000,000,000 (1 billion), we cannot iterate row by row. Instead, we only care about rows that actually have reserved seats.

For any row that has reservations, we want to know which of the 3 candidate blocks (Left, Middle, Right) are blocked:
* If a seat is in range 2 to 5, it blocks the **Left** option.
* If a seat is in range 4 to 7, it blocks the **Middle** option.
* If a seat is in range 6 to 9, it blocks the **Right** option.

After checking all reservations for a given row:
* If all 3 candidate blocks are ruined, that row can fit **0 groups** instead of 2. So we subtract 2 from our running total.
* If 1 or 2 candidate blocks are ruined, that row can still fit **1 group** (either Left, Right, or Middle). So we subtract 1 from our running total.

---

## Approach

Here is how the given code executes this idea step-by-step:

* `unordered_map<int, unordered_set<int>> seats;`: Creates a map where each key is a row number, and the value is a set tracking which block choices (0 for Left, 1 for Middle, 2 for Right) are ruined by reserved seats.
* `for (auto& r : reservedSeats)`: Loops through every reserved seat pair in the input list.
* `int row = r[0]; int seat = r[1];`: Extracts the row number and seat number for the current reservation.
* `if (seat >= 2 && seat <= 5) { seats[row].insert(0); }`: If the seat is between 2 and 5, it inserts `0` to mark the Left block as blocked.
* `if (seat >= 4 && seat <= 7) { seats[row].insert(1); }`: If the seat is between 4 and 7, it inserts `1` to mark the Middle block as blocked.
* `if (seat >= 6 && seat <= 9) { seats[row].insert(2); }`: If the seat is between 6 and 9, it inserts `2` to mark the Right block as blocked.
* `int total = 2 * n;`: Calculates the theoretical maximum number of groups assuming all rows were completely empty.
* `for (auto& [row, groups] : seats)`: Iterates through only the rows that had at least one relevant reservation.
* `if (groups.size() == 3) { total -= 2; }`: If all 3 block choices are ruined for this row, 0 groups can sit here. We reduce `total` by 2.
* `else { total -= 1; }`: If 1 or 2 block choices are ruined, we can still fit 1 group instead of 2. We reduce `total` by 1.
* `return total;`: Returns the final count of max groups.

---

## Dry Run

### Case 1: Standard case with multiple reservations
Input: `n = 3`, `reservedSeats = [[1,2],[1,3],[1,8],[2,6],[3,1],[3,10]]`

Initial state: `total = 2 * 3 = 6`. Map `seats` is empty.

**Phase 1: Building the `seats` map**

| Step | Current Seat `r` | Row `row` | Seat `seat` | Action on `seats[row]` | Map State |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `[1,2]` | 1 | 2 | Insert `0` | `seats[1] = {0}` |
| 2 | `[1,3]` | 1 | 3 | Insert `0` | `seats[1] = {0}` |
| 3 | `[1,8]` | 1 | 8 | Insert `2` | `seats[1] = {0, 2}` |
| 4 | `[2,6]` | 2 | 6 | Insert `1` and `2` | `seats[1] = {0, 2}`, `seats[2] = {1, 2}` |
| 5 | `[3,1]` | 3 | 1 | Seat 1 outside 2..9, ignore | `seats[1] = {0, 2}`, `seats[2] = {1, 2}` |
| 6 | `[3,10]` | 3 | 10 | Seat 10 outside 2..9, ignore | `seats[1] = {0, 2}`, `seats[2] = {1, 2}` |

**Phase 2: Adjusting `total`**

| Row | `groups.size()` | Decision | `total` Change | New `total` |
| :--- | :--- | :--- | :--- | :--- |
| Row 1 | 2 (`{0, 2}`) | `size != 3` -> subtract 1 | `6 - 1` | 5 |
| Row 2 | 2 (`{1, 2}`) | `size != 3` -> subtract 1 | `5 - 1` | 4 |

Final output: `4`

---

### Case 2: Minimal reservations with corner seats
Input: `n = 2`, `reservedSeats = [[2,1],[1,8],[2,6]]`

Initial state: `total = 2 * 2 = 4`. Map `seats` is empty.

**Phase 1: Building the `seats` map**

| Step | Current Seat `r` | Row `row` | Seat `seat` | Action on `seats[row]` | Map State |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `[2,1]` | 2 | 1 | Seat 1 outside 2..9, ignore | Map is empty |
| 2 | `[1,8]` | 1 | 8 | Insert `2` | `seats[1] = {2}` |
| 3 | `[2,6]` | 2 | 6 | Insert `1` and `2` | `seats[1] = {2}`, `seats[2] = {1, 2}` |

**Phase 2: Adjusting `total`**

| Row | `groups.size()` | Decision | `total` Change | New `total` |
| :--- | :--- | :--- | :--- | :--- |
| Row 1 | 1 (`{2}`) | `size != 3` -> subtract 1 | `4 - 1` | 3 |
| Row 2 | 2 (`{1, 2}`) | `size != 3` -> subtract 1 | `4 - 2` -> `3 - 1` | 2 |

Final output: `2`

---

## Time & Space Complexity

Let `K` be the number of elements in `reservedSeats`.

* **Time Complexity:** **O(K)** average time. Processing each reserved seat involves constant-time lookups and set insertions on average. The second loop visits at most `K` row entries in the map.
* **Space Complexity:** **O(K)** auxiliary space. The `unordered_map` stores at most `K` rows, and each row's set stores at most 3 numbers.

### Can this be improved?

**Yes, both runtime and memory overhead can be significantly reduced!**

The current implementation uses `unordered_map<int, unordered_set<int>>`. Constructing and inserting into inner `unordered_set` objects triggers many dynamic memory allocations on the heap, which causes poor cache locality and slow performance (beats ~28% runtime, ~19% memory).

Instead of a set, we can use a **bitmask** stored inside a simple integer:
* Bit 0 (value 1) represents whether the Left block is ruined.
* Bit 1 (value 2) represents whether the Middle block is ruined.
* Bit 2 (value 4) represents whether the Right block is ruined.

Using bitwise OR operations (`|=`), we can store all block states in a single `unordered_map<int, int>`. If the mask equals `7` (binary `111`), all 3 blocks are ruined.

#### Optimized Code Snippet:

```cpp
unordered_map<int, int> seats;

for (auto& r : reservedSeats) {
    int row = r[0], seat = r[1];
    if (seat >= 2 && seat <= 5) seats[row] |= 1; // Bit 0: Left block ruined
    if (seat >= 4 && seat <= 7) seats[row] |= 2; // Bit 1: Middle block ruined
    if (seat >= 6 && seat <= 9) seats[row] |= 4; // Bit 2: Right block ruined
}

int total = 2 * n;
for (auto& [row, mask] : seats) {
    if (mask == 7) { // 1 | 2 | 4 = 7 (all 3 options ruined)
        total -= 2;
    } else {
        total -= 1;
    }
}
return total;
```

* **Improved Time Complexity:** **O(K)** with much smaller constant factor (no set hashing or dynamic allocations).
* **Improved Space Complexity:** **O(K)** with minimal memory usage (storing plain `int` values).
* **Theoretical Best Complexity:** **O(K)** time and **O(K)** space. We must examine all `K` reserved seats, and since `n` can be up to 10^9, we cannot allocate an array for all rows. Therefore, this bitmask solution reaches the theoretical optimal complexity.

---

## Edge Cases Handled

* **Large row count (`n` up to 10^9):** The solution only tracks rows that appear in `reservedSeats`. Unreserved rows are never stored in memory, preventing out-of-memory errors.
* **Seats 1 and 10 reserved:** Seats 1 and 10 fall outside ranges 2..5, 4..7, and 6..9. The `if` conditions automatically ignore them because they do not interfere with 4-person blocks.
* **Duplicate reservations in the same block:** If multiple seats in the same row block the same region (e.g., seats 2 and 3 both block Left), the set (or bitwise OR) deduplicates the block flag safely.
* **Empty `reservedSeats` array:** If there are no reservations, the loop over `seats` does not run, and the function correctly returns `2 * n`.
* **Middle block survival:** If both Left and Right blocks are ruined by reservations at seats 2 and 9, `mask` will equal `1 | 4 = 5` (not 7). The code correctly subtracts only 1, allowing the Middle block (seats 4, 5, 6, 7) to be used.
