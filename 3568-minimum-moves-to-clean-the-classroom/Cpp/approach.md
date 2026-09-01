![Runtime](https://img.shields.io/badge/Runtime-125%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-67.12%20MB%20(beats%20100.00%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a classroom represented as a grid of size `m x n`. The classroom contains:
*   `'S'`: The starting position of a student volunteer.
*   `'L'`: A piece of litter that needs to be collected.
*   `'R'`: A energy reset area that completely restores the student's energy.
*   `'X'`: An obstacle that cannot be walked through.
*   `'.'`: An empty walkable tile.

The student starts at `'S'` with a fixed maximum energy level called `energy`. Moving to any adjacent tile (up, down, left, right) costs 1 unit of energy. If the energy reaches 0, the student cannot move any further unless they land on or are already standing on a reset area `'R'`. Reset areas can be used as many times as needed.

Your goal is to find the **minimum number of moves** needed to collect all pieces of litter `'L'`. If it is impossible to collect all litter, return `-1`.

---

## Intuition

Since we need the *minimum* number of moves, a **Breadth-First Search (BFS)** is the standard approach. BFS explores all paths layer by layer, guaranteeing that the first time we collect all litter, we used the fewest moves possible.

However, standard BFS only tracks cell coordinates. Here, our state depends on three key pieces of information:
1.  **Where we are** on the grid (`row, col`).
2.  **Which litter items we have collected so far**.
3.  **How much energy we have left**.

Because there are at most 10 litter items (`'L'`), we can use a **bitmask** (an integer where each binary bit represents whether a specific piece of litter was collected) to track collected litter. 

To avoid infinite loops and redundant exploration, we keep track of the maximum energy we had when visiting a specific grid cell with a specific litter bitmask. If we reach the same cell with the exact same collected litter but with *less or equal* energy than before, we skip processing it.

---

## Approach

Here is how the C++ code solves the problem step-by-step:

*   `char A[400]; int L[10]; int dir[5]={0, 1, 0, -1, 0}; int bestE[400][1024];`  
    Global memory arrays are set up. `A` flattens the 2D grid into a 1D array of size up to 400 (`20 x 20`). `L` tracks cell positions of litter. `dir` allows clean iteration across 4 adjacent directions. `bestE[cell][mask]` stores the highest remaining energy recorded at a given cell index with a specific collection mask.

*   `const int r=classroom.size(), c=classroom[0].size(), rc=r*c;`  
    Extracts the rows `r`, columns `c`, and total grid size `rc`.

*   `for(int i=0; i<r; i++) { ... if (ch=='L') A[key]=Lidx, L[Lidx++]=key; ... }`  
    Scans the grid. Each `'L'` tile is assigned a unique small ID number `0, 1, 2...` stored directly inside `A[key]`. The starting tile `'S'` is recorded into `S0`.

*   `LMask=(1<<Lidx)-1;`  
    Creates a target bitmask where all bits representing the found litter items are set to 1. For example, if there are 3 litter items, `LMask` becomes `2^3 - 1 = 7` (binary `111`).

*   `memset(bestE[i], -1, sizeof(int)*(1<<Lidx));`  
    Initializes `bestE` to `-1`, meaning no states have been visited yet.

*   `queue<int4> q; q.emplace(S0, 0, energy, 0); bestE[S0][0]=energy;`  
    Initializes the BFS queue using a 4-element tuple `(cell_index, mask, remaining_energy, step_count)`. It starts at position `S0` with mask `0`, full `energy`, and `0` steps.

*   `auto [ij, mask, en, step]=q.front(); q.pop();`  
    Pulls the next state from the front of the BFS queue.

*   `if (mask==LMask) return step;`  
    If our current `mask` equals `LMask`, all litter has been collected! Since BFS guarantees the shortest path first, we return `step` immediately.

*   `if (en==0) continue;`  
    If remaining energy `en` is 0, we cannot move any further from this tile, so we stop expanding this path.

*   `for(int a=0; a<4; a++) { ... }`  
    Checks all 4 neighboring cells (up, right, down, left).

*   `if (isOutside(s, t, r, c) || A[k]=='X') continue;`  
    Skips the neighbor if it goes off the grid or hits an obstacle `'X'`.

*   `if (A[k]<Lidx) mask2|=(1<<A[k]);`  
    If the neighbor contains litter (identifiable because its value stored in `A` is a valid litter ID `< Lidx`), update `mask2` by setting that litter's bit to 1.

*   `en2=(A[k]=='R')?energy:en-1;`  
    Calculates remaining energy for the next step. If the new cell is a reset area `'R'`, energy resets back to max `energy`. Otherwise, subtract 1.

*   `if (en2>bestE[k][mask2]) { bestE[k][mask2]=en2; q.emplace(k, mask2, en2, step+1); }`  
    Pruning step: only push the new state to the queue if this new energy `en2` is strictly greater than the previously recorded energy for cell `k` with litter mask `mask2`.

---

## Dry Run

### Case 1: Standard Shortest Path
`classroom = ["S.", "XL"], energy = 2`

*   Grid size: `2 x 2` (`r=2, c=2`).
*   `S0 = 0` (at `0,0`).
*   Litter `'L'` at cell `3` (`1,1`), assigned `Lidx = 0`. Target `LMask = 1`.

| Queue Pop (ij, mask, en, step) | Action / Processing | Next States Pushed |
| :--- | :--- | :--- |
| `(0, 0, 2, 0)` | Start at (0,0). Neighbors: (1,0) is 'X' (skip); (0,1) is empty. | Push `(1, 0, 1, 1)` |
| `(1, 0, 1, 1)` | Cell (0,1). Neighbors: (0,0) ignored (en=0 <= bestE); (1,1) is 'L'. | Mask becomes 1. Push `(3, 1, 0, 2)` |
| `(3, 1, 0, 2)` | Cell (1,1). `mask == LMask` (1 == 1). | **Return 2** |

---

### Case 2: Using Energy Reset Area
`classroom = ["LS", "RL"], energy = 4`

*   Grid size: `2 x 2`.
*   Start `S0 = 1` (`0,1`).
*   Litter 0 at cell `0` (`0,0`), Litter 1 at cell `3` (`1,1`). Target `LMask = 3` (binary `11`).

| Queue Pop (ij, mask, en, step) | Action / Processing | Next States Pushed |
| :--- | :--- | :--- |
| `(1, 0, 4, 0)` | Start at (0,1). Moves to (0,0) 'L' or (1,1) 'L'. | Push `(0, 1, 3, 1)` ('L0'), `(3, 2, 3, 1)` ('L1') |
| `(0, 1, 3, 1)` | Cell (0,0), mask=1. Move down to (1,0) 'R'. | Reset energy to 4. Push `(2, 1, 4, 2)` |
| `(3, 2, 3, 1)` | Cell (1,1), mask=2. Move left to (1,0) 'R'. | Reset energy to 4. Push `(2, 2, 4, 2)` |
| `(2, 1, 4, 2)` | Cell (1,0), mask=1, en=4. Move right to (1,1) 'L1'. | Mask becomes 3. Push `(3, 3, 3, 3)` |
| `(2, 2, 4, 2)` | Cell (1,0), mask=2, en=4. Move up to (0,0) 'L0'. | Mask becomes 3. Push `(0, 3, 3, 3)` |
| `(3, 3, 3, 3)` | Cell (1,1), `mask == LMask` (3 == 3). | **Return 3** |

---

## Time & Space Complexity

*   **Time Complexity:** `O(N * 2^L)` where `N = m * n` is the total number of cells in the grid (at most 400), and `L` is the number of litter items (at most 10).
    *   *Why:* There are at most `N` positions and `2^L` bitmask combinations, creating at most `N * 2^L` distinct states. For each state, we inspect up to 4 neighbors in `O(1)` time. Max total states = `400 * 1024 = 409,600`, which runs well within milliseconds.
*   **Space Complexity:** `O(N * 2^L)`
    *   *Why:* The matrix `bestE` has dimensions `400 x 1024` integers, and the BFS queue can hold up to `N * 2^L` elements at worst.

### Is this optimal?
**Yes, this is already optimal.** Any algorithm solving this problem must track both the current location and the subset of collected items (which is equivalent to the Traveling Salesperson Problem on a grid with energy constraints). State space of size `N * 2^L` is theoretically required, and standard BFS runs in time proportional to the state space size.

---

## Edge Cases Handled

*   **No litter in grid (`Lidx == 0`):** `LMask` is 0, so `mask == LMask` evaluates to true immediately on the first step, returning `0`.
*   **Energy reaching exactly 0 upon landing on 'R':** Energy is restored back to max capacity `energy` immediately, allowing the student to continue moving.
*   **Energy reaching 0 on a non-reset cell:** Handled by `if (en == 0) continue;`, stopping dead-end paths from expanding further.
*   **Revisiting cells with better energy:** Handled by `if (en2 > bestE[k][mask2])`. If we reach a cell with the same collected litter but strictly more remaining energy, we re-enqueue it to explore potentially longer remaining routes.
*   **Blocked or unreachable litter:** If obstacles `'X'` block access to any piece of litter, the queue eventually empties, and the function returns `-1`.
