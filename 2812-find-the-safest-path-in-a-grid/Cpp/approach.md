![Runtime](https://img.shields.io/badge/Runtime-385%20ms%20(beats%2062.54%25)-green?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-131.6%20MB%20(beats%2075.88%25)-green?style=for-the-badge)

---

## Problem Explained

Imagine you are trying to walk from the top-left corner of a grid (position 0, 0) to the bottom-right corner (position n minus 1, n minus 1). You can only move up, down, left, or right. 

However, some cells in the grid contain thieves, marked with a 1. Empty cells are marked with a 0. As you walk along your path, you want to stay as far away from thieves as possible. 

The distance between any cell on your path and the nearest thief is measured using Manhattan distance (which means counting steps horizontally plus steps vertically, without moving diagonally). The "safeness factor" of your entire path is the **minimum** distance you ever get to *any* thief while walking that path. 

Your goal is to find a path from start to finish that makes this minimum distance (the safeness factor) as large as possible, and return that maximum safeness factor.

---

## Intuition

This problem can be broken down into two distinct phases:

1. **Find out how close everything is to danger:** Before we even think about walking from start to finish, we need to know the safeness value of every single cell in the grid. The safeness value of a cell is its distance to the nearest thief. We can calculate this for all cells at once using a **Multi-Source Breadth-First Search (BFS)** — a search that spreads outward level-by-level from all thieves simultaneously.

2. **Find the safest path:** Once every cell has a safeness score, we need to find a path from (0, 0) to (n minus 1, n minus 1) that keeps our lowest safeness score as high as possible. This is a best-first search problem. We use a **Max-Heap (Priority Queue)** to always explore the neighbor with the currently highest safeness score. As we walk, the safeness of our path is limited by the single weakest link (the minimum safeness value among the cells we visit). By greedily picking the highest available safeness at each step, we ensure we reach the end with the best possible minimum score.

---

## Approach

* `n = grid.size();`: Stores the dimension of the grid (since it is an n by n square) so we can check boundaries.
* `queue<pair<int,int>> q; vector<vector<int>> dist(n, vector<int>(n, 0)); vector<vector<bool>> vis(n, vector<bool>(n, false));`: Initializes the queue and tracking grids for the first phase (Multi-Source BFS). `dist` will store the distance to the nearest thief, and `vis` prevents revisiting cells.
* `for(int r = 0; r < n; r++){ for(int c = 0; c < n; c++){ if(grid[r][c] == 1){ vis[r][c] = true; q.push({r, c}); } } }`: Scans the entire grid to find all thieves (cells with value 1). Marks them as visited and pushes all of them into the queue to start our distance-measuring wave from all thieves at the same time.
* `while(!q.empty()){ ... }`: Runs the Multi-Source BFS. It pops cells from the queue, looks at their 4 neighbors (up, down, left, right), and if a neighbor hasn't been visited, records its distance as `dist[r][c] + 1`, marks it visited, and pushes it to the queue.
* `priority_queue<pair<int, pair<int,int>>> store; vector<vector<bool>> vis2(n, vector<bool>(n, false));`: Sets up a max-heap (`store`) to find the path with the highest safeness factor. The heap stores elements as `{safeness, {row, col}}`, keeping the cell with the highest safeness at the top. `vis2` tracks visited cells for this second phase.
* `store.push({dist[0][0], {0, 0}});`: Pushes our starting position (0, 0) into the heap along with its pre-calculated safeness value.
* `while(!store.empty()){ ... }`: Greedily explores the grid. It pops the cell with the highest current safeness factor.
* `if(vis2[r][c]) continue; vis2[r][c] = true;`: Skips cells we have already processed to avoid infinite loops, and marks the current cell as visited.
* `if(r == n - 1 && c == n - 1) return safeE;`: Because we are using a max-heap, the very first time we pop the destination cell (n minus 1, n minus 1), its safeness factor is guaranteed to be the maximum possible for any valid path. We immediately return it.
* `int newSafe = min(safeE, dist[nr][nc]); store.push({newSafe, {nr, nc}});`: For each valid neighbor, calculates the path's safeness up to that neighbor, which is the lower of our current path safeness (`safeE`) and the neighbor's individual thief distance (`dist[nr][nc]`). Pushes the neighbor into the heap.

---

## Dry Run

### Case 1: Simple 3x3 grid with thieves at corners
Input: `grid = [[1, 0, 0], [0, 0, 0], [0, 0, 1]]`

| Step / Action | Queue / Heap State | `dist` grid change | `vis` / `vis2` change |
| :--- | :--- | :--- | :--- |
| **Initial BFS Setup** | `q` has `(0,0)` and `(2,2)` | Both corners set to `0` | Both corners marked `vis = true` |
| **BFS Step 1** | Processes `(0,0)`, adds neighbors | `dist[0][1] = 1`, `dist[1][0] = 1` | Neighbors marked `vis = true` |
| **BFS Step 2** | Processes `(2,2)`, adds neighbors | `dist[1][2] = 1`, `dist[2][1] = 1` | Neighbors marked `vis = true` |
| **BFS Finalize** | BFS finishes filling all `dist` values | Center `dist[1][1] = 2` | Entire grid marked `vis = true` |
| **Dijkstra Start** | `store` pushes `{dist[0][0], {0,0}}` -> `{0, {0,0}}` | Unchanged | `vis2[0][0] = true` |
| **Dijkstra End** | Pops destination `(2,2)` with safeness `0` | Unchanged | Returns `0` |

### Case 2: 3x3 grid with one thief in the top right
Input: `grid = [[0, 0, 1], [0, 0, 0], [0, 0, 0]]`

| Step / Action | Queue / Heap State | `dist` grid change | `vis` / `vis2` change |
| :--- | :--- | :--- | :--- |
| **Initial BFS Setup** | `q` has `(0,2)` | `dist[0][2] = 0` | `vis[0][2] = true` |
| **BFS Wave** | BFS expands outward from `(0,2)` | `dist[0][0]` becomes `2`, `dist[2][2]` becomes `2` | Entire grid marked `vis = true` |
| **Dijkstra Start** | `store` pushes `{dist[0][0], {0,0}}` -> `{2, {0,0}}` | Unchanged | `vis2[0][0] = true` |
| **Dijkstra Expand** | Explores neighbors of `(0,0)`, pushes to `store` | Unchanged | Marks visited path cells |
| **Dijkstra Reaches End** | Pops `(2,2)` with max safeness `2` | Unchanged | Returns `2` |

---

## Time & Space Complexity

- **Time:** O(N * N * log(N * N)) — The Multi-Source BFS visits every cell once, taking O(N * N) time. The priority queue traversal visits every cell, and each insertion or deletion takes logarithmic time relative to the number of cells in the grid (N * N).
- **Space:** O(N * N) — We store distance tables, visited tracker matrices, and queue/heap structures, all of which scale linearly with the total number of cells in the N by N grid.

**Is this already the most optimal possible complexity, or can it be improved?**

Yes, this complexity is optimal for this problem. You must inspect every cell at least once to determine distances to thieves (O(N * N)), and exploring the grid to find the best path requires graph traversal. While you can solve this alternative-style using a combination of Binary Search on the answer combined with Breadth-First Search (checking if a path of safeness X exists), the theoretical time complexity remains bound by evaluating the grid cells, and heap-based traversal matches the optimal performance profile for this difficulty tier. No further improvement is necessary.

---

## Edge Cases Handled

- **Thieves at the Start or End:** If `(0, 0)` or `(n - 1, n - 1)` contains a thief, `dist[0][0]` is 0, correctly resulting in a safeness factor of 0 as handled by the grid constraints and minimum logic.
- **Minimum Grid Size (N = 1):** If the grid is size 1 by 1 containing a thief, the loop terminates immediately and returns 0; if empty, it handles the single cell gracefully.
- **Multiple Closest Thieves:** Multi-source BFS correctly assigns the shortest distance to any thief even if multiple thieves compete for the same empty cell.
