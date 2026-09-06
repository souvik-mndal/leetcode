![Runtime](https://img.shields.io/badge/Runtime-72%20ms%20(beats%2098.72%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-251.9%20MB%20(beats%2098.12%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

We are given a network of computer nodes numbered from `0` to `n - 1`. We are also given:
* A list of connected `edges`, where each edge has `[u, v, w]` representing a connection between node `u` and node `v` with a weight (or travel cost) of `w`.
* A boolean array `online`, where `online[i]` tells us if node `i` is working (`true`) or offline (`false`).
* A total path distance limit `k`.

We want to travel from the start node (`0`) to the target node (`n - 1`).

Here are the rules:
1. We can only travel through nodes that are online. An edge can only be used if both of its endpoint nodes are online.
2. The total length (sum of edge weights) of the path cannot exceed `k`.
3. Every path has a **path score**, defined as the weight of the single smallest edge along that path (the bottleneck edge).

Our goal is to find a valid path from node `0` to node `n - 1` that costs at most `k` in total distance, while making its **path score** (the minimum edge weight on that path) as large as possible. Return this maximum possible path score. If no valid path exists, return `-1`.

---

## Intuition

Think of edge weights as bridge weight limits. We want to drive a heavy truck from node `0` to node `n - 1`. The truck's weight cannot exceed the smallest bridge limit along our path. We want to find the heaviest truck that can make the journey within a maximum total distance of `k`.

1. **Filter Out Inactive Nodes**: If a node is offline, we cannot step on it. So we completely ignore any edge touching an offline node right from the start.
2. **Guess and Check (Binary Search)**: Instead of trying every path, we can guess a target bridge limit `mid`. If we only keep edges with weight at least `mid`, can we still reach node `n - 1` within distance `k`?
3. **Shortest Path (Dijkstra)**: To check if a guessed target `mid` is possible, we run Dijkstra's shortest path algorithm using only edges with weight `>= mid`. If the shortest total distance to node `n - 1` is `<= k`, then our guess is possible!
4. **Fast Jumps**: When Dijkstra finds a valid path, it doesn't just tell us "yes" — it also tells us the actual smallest edge on that path (`aMin`). We can immediately jump our binary search lower bound to `aMin + 1`, skipping unnecessary intermediate guesses.

---

## Approach

Here is how the logic flows step-by-step through the code:

* `const int n=online.size();` — Finds the total number of nodes `n` from the size of the `online` list.
* `fill_n(adj, n, -1); eIdx=0;` — Resets the adjacency array `adj` (head pointers for linked lists) to `-1` and sets the edge counter `eIdx` to 0.
* `if (online[u] && online[v]) { ... addEdge(u, v, w); }` — Loops through every edge. If both endpoint nodes `u` and `v` are online, it adds the edge to the graph using `addEdge` and updates the overall minimum edge weight `mnC` and maximum edge weight `mxC`.
* `int l=mnC, r=mxC, ans=-1;` — Sets up the binary search range `[l, r]` between the smallest and largest edge weights present in the valid graph. Sets default answer `ans = -1`.
* `while(l<=r)` — Starts the binary search loop to pinpoint the highest valid path score.
* `int mid=l+(r-l)/2;` — Picks the middle edge weight threshold to test.
* `int aMin=Dijkstra(mid, n, k);` — Calls `Dijkstra` to check if a path exists using only edges with weight `>= mid` without exceeding total distance `k`.
* `fill_n(dist, n, LLONG_MAX); memset(pathMin, 0, n*sizeof(int));` — Inside `Dijkstra`, sets all distances in `dist` to infinity and clears `pathMin` array (which tracks the bottleneck edge weight along the path to each node).
* `pq.emplace(0, 0); dist[0]=0; pathMin[0]=INT_MAX;` — Initializes node 0 in the min-priority queue with cumulative distance 0 and sets its bottleneck score to infinity (`INT_MAX`).
* `if (d>dist[u]) continue; if (d>k) return -1; if (u==n-1) return pathMin[u];` — Inside the Dijkstra loop: ignores outdated queue entries; aborts early if distance exceeds `k`; returns the path bottleneck `pathMin[u]` as soon as destination `n - 1` is reached.
* `if (w<minW) continue;` — Skips any neighboring edge whose weight `w` is strictly smaller than the binary search threshold `minW`.
* `ll d2=d+w; int curMin=min(pathMin[u], w);` — Calculates the potential new distance `d2` to neighbor `v` and the new bottleneck value `curMin`.
* `if (d2<dist[v]){ dist[v]=d2; pathMin[v]=curMin; pq.emplace(d2, v); }` — Relaxes the distance to node `v` if `d2` is strictly shorter than the previously recorded `dist[v]`.
* `else if (d2==dist[v] && curMin>pathMin[v]) { pathMin[v]=curMin; pq.emplace(d2, v); }` — If `d2` matches the best distance so far, but `curMin` offers a better bottleneck score, updates `pathMin[v]` and re-enqueues `v`.
* `if (aMin!=-1){ ans=max(ans, aMin); l=aMin+1; } else r=mid-1;` — In the binary search loop: if Dijkstra succeeded, updates `ans` to `aMin` and moves the lower bound `l` to `aMin + 1`. If it failed, reduces upper bound `r` to `mid - 1`.

---

## Dry Run

### Case 1: Standard graph with all nodes online

**Input:**
* `edges = [[0,1,5],[1,3,10],[0,2,3],[2,3,4]]`
* `online = [true, true, true, true]`
* `k = 10`

`n = 4`. All nodes online. Valid edges: (0,1,w=5), (1,3,w=10), (0,2,w=3), (2,3,w=4).
Binary search bounds: `l = 3`, `r = 10`.

| Step | `l` | `r` | `mid` | Dijkstra Action / Path Taken | Distance | Bottleneck (`aMin`) | New `ans` | Next Bounds |
|---|---|---|---|---|---|---|---|---|
| 1 | 3 | 10 | 6 | Traverses edges >= 6. Edge (0,1,5) skipped. Edge (0,2,3) skipped. No path found. | N/A | -1 | -1 | `r = 5` |
| 2 | 3 | 5 | 4 | Traverses edges >= 4. Traverses 0 -> 1 (w=5). Next edge 1 -> 3 (w=10) makes dist 15 > 10. No valid path <= 10. | 15 (> 10) | -1 | -1 | `r = 3` |
| 3 | 3 | 3 | 3 | Traverses edges >= 3. Traverses 0 -> 2 (w=3), then 2 -> 3 (w=4). Total dist = 7 <= 10. Destination 3 reached! | 7 | 3 | 3 | `l = 4` |

Loop ends because `l = 4` is greater than `r = 3`. Final Output: `3`.

---

### Case 2: Graph with an offline node

**Input:**
* `edges = [[0,1,7],[1,4,5],[0,2,6],[2,3,6],[3,4,2],[2,4,6]]`
* `online = [true, true, true, false, true]`
* `k = 12`

`n = 5`. Node 3 is offline (`online[3] = false`). Edges connected to node 3 are filtered out.
Valid edges kept: (0,1,w=7), (1,4,w=5), (0,2,w=6), (2,4,w=6).
Binary search bounds: `l = 5`, `r = 7`.

| Step | `l` | `r` | `mid` | Dijkstra Action / Path Taken | Distance | Bottleneck (`aMin`) | New `ans` | Next Bounds |
|---|---|---|---|---|---|---|---|---|
| 1 | 5 | 7 | 6 | Traverses edges >= 6. Path 0 -> 2 (w=6) -> 4 (w=6). Total dist = 12 <= 12. Destination 4 reached! | 12 | 6 | 6 | `l = 7` |
| 2 | 7 | 7 | 7 | Traverses edges >= 7. Path 0 -> 1 (w=7). Edge 1 -> 4 (w=5) skipped (5 < 7). Queue becomes empty. | N/A | -1 | 6 | `r = 6` |

Loop ends because `l = 7` is greater than `r = 6`. Final Output: `6`.

---

## Time & Space Complexity

* **Time Complexity:** `O(E * log V * log(W_max - W_min))`
  * Filtering edges and constructing the adjacency list takes `O(E)` time, where `E` is the number of edges.
  * The binary search performs `O(log(W_max - W_min))` iterations, where `W_max` and `W_min` are the maximum and minimum edge weights in the graph.
  * Each binary search iteration runs Dijkstra's algorithm using a min-priority queue, taking `O(E * log V)` time, where `V` is the number of nodes (`online.size()`).
  * Combined total time is `O(E * log V * log(W_max - W_min))`.

* **Space Complexity:** `O(V + E)`
  * Graph storage (`adj` heads array and static `E` edge array) uses `O(V + E)` space.
  * Auxiliary arrays `dist` and `pathMin` take `O(V)` space.
  * Min-priority queue `pq` stores at most `O(E)` entries in the worst case.
  * Combined total space is `O(V + E)`.

### Is this optimal?
**Yes, this solution is optimal.**
Finding the maximum bottleneck path under a total distance constraint on arbitrary weighted graphs is a known variation of the constrained shortest path problem. Combining binary search over edge weights with Dijkstra's algorithm achieves optimal logarithmic scaling over edge weights while guaranteeing minimal path distances. The static array-based forward star graph representation used in this code eliminates dynamic memory allocation overhead, yielding optimal runtime and space usage.

---

## Edge Cases Handled

* **Offline Start or Target Nodes:** If node `0` or node `n - 1` is marked `false` in `online`, no edges connected to them are added to the graph. Dijkstra immediately returns `-1`.
* **Disconnected Destination / Exceeding Budget `k`:** If destination `n - 1` cannot be reached or all paths cost strictly more than `k`, `if (d > k) return -1;` safe-guards and returns `-1`.
* **Equal Distance Paths with Different Scores:** When two paths to node `v` have the exact same distance (`d2 == dist[v]`), the tie-breaker `curMin > pathMin[v]` prefers the path with the larger bottleneck score.
* **Large Distance Values:** Total path distances can exceed standard 32-bit integer limits. The `dist` array uses 64-bit integers (`long long`) to prevent integer overflow during addition `d + w`.
