![Runtime](https://img.shields.io/badge/Runtime-72%20ms%20(beats%2098.50%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-251.95%20MB%20(beats%2098.42%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

You are given a network of `n` computers (nodes) numbered from `0` to `n - 1`. Connections between nodes are directed, one-way paths given in an array called `edges`, where each edge `[u, v, cost]` goes from node `u` to node `v` with a specific recovery cost.

Some nodes may be offline, indicated by a boolean array `online`. If a node is offline (`online[i] == false`), no path can pass through it. Nodes `0` and `n - 1` are always online.

A path from node `0` to node `n - 1` is **valid** if:
1. Every intermediate node on the path is online.
2. The total sum of all edge costs on the path is at most `k`.

For any valid path, its **score** is defined as the smallest edge cost along that path (the "bottleneck" edge).

Your goal is to find a valid path from `0` to `n - 1` that gives the largest possible path score, and return that score. If no valid path exists within budget `k`, return `-1`.

---

## Intuition

We want to find a path whose smallest edge is as large as possible, while keeping the overall path cost under `k`.

Instead of trying all paths, we can flip the question: **"Can we reach node n - 1 using only edges with cost at least target_weight, without spending more than k total cost?"**

If we fix a `target_weight`:
1. Ignore all edges with a weight smaller than `target_weight`.
2. Find the path from node `0` to node `n - 1` that has the **smallest total path cost**.
3. If that minimum total cost is at most `k`, then `target_weight` is achievable!

Because higher target weights make it harder to find a path, this decision is monotonic: if a target weight works, smaller target weights also work. This allows us to use **Binary Search** over the possible edge costs. 

During Dijkstra's algorithm, once we find a path to node `n - 1`, we also know the actual smallest edge on that specific path (`aMin`). We can use `aMin` to jump our binary search lower bound straight past `aMin`, speeding up the search!

---

## Approach

Here is step-by-step how the solution executes:

* `fill_n(adj, n, -1); eIdx=0;`  
  Resets the adjacency list head array `adj` to `-1` and edge index counter `eIdx` to `0` so state from previous runs is cleared.

* `if (online[u] && online[v]) { addEdge(u, v, w); ... }`  
  Filters out offline nodes during graph construction. An edge is only added to the graph if both endpoint nodes `u` and `v` are online. Simultaneously tracks global minimum edge weight `mnC` and maximum edge weight `mxC`.

* `int l=mnC, r=mxC, ans=-1;`  
  Initializes binary search bounds to the minimum and maximum edge weights present in the graph. `ans` holds the best valid score found so far.

* `while(l<=r)`  
  Runs the binary search loop while the search range is valid.

* `int mid=l+(r-l)/2;`  
  Calculates `mid`, the target minimum edge cost we are testing in the current step.

* `int aMin=Dijkstra(mid, n, k);`  
  Calls Dijkstra's algorithm to test if node `n - 1` is reachable from node `0` using only edges with weight at least `mid`, staying within total cost budget `k`.

* `fill_n(dist, n, LLONG_MAX); memset(pathMin, 0, n*sizeof(int));`  
  Inside `Dijkstra`, initializes shortest path distances `dist` to infinity and bottleneck trackers `pathMin` to 0.

* `pq.emplace(0, 0); dist[0]=0; pathMin[0]=INT_MAX;`  
  Pushes start node `0` with path cost `0` into the priority queue `pq`. Sets distance to `0` and initial minimum edge weight tracker to infinity.

* `if (d>dist[u]) continue;`  
  Skips outdated priority queue entries if a shorter path to node `u` has already been processed.

* `if (d>k) return -1;`  
  If the current smallest total cost popped from the priority queue exceeds budget `k`, no remaining valid path can fit within budget `k`, so returns `-1`.

* `if (u==n-1) return pathMin[u];`  
  Since Dijkstra processes nodes in order of increasing total cost, the first time node `n - 1` is popped, we have found the path with the smallest total cost. Returns the bottleneck score `pathMin[n - 1]`.

* `if (w<minW) continue;`  
  Ignores any edge during neighbor expansion whose weight `w` is strictly less than our target threshold `minW`.

* `if (d2<dist[v]) ... else if (d2==dist[v] && curMin>pathMin[v])`  
  Relaxes edges. Updates distance and minimum edge tracker for node `v` if a strictly shorter path is found, or if total distance is tied but yields a larger bottleneck weight `curMin`.

* `if (aMin!=-1){ ans=max(ans, aMin); l=aMin+1; } else r=mid-1;`  
  In `findMaxPathScore`, if a valid path was found, updates `ans` with the actual path score `aMin` and sets lower bound `l` to `aMin + 1`. Otherwise, reduces upper bound `r` to `mid - 1`.

---

## Dry Run

### Case 1: Standard valid path selection with total cost limit
Inputs: `edges = [[0,1,5],[1,3,10],[0,2,3],[2,3,4]]`, `online = [true,true,true,true]`, `k = 10`
Nodes 0, 1, 2, 3 are all online. `mnC = 3`, `mxC = 10`.

| Step | Variables state (`l`, `r`, `mid`) | Dijkstra Action & Result | `ans` state |
| :--- | :--- | :--- | :--- |
| 1 | `l=3, r=10, mid=6` | Only keeps edges >= 6: edge (1,3,10). Node 0 has no outgoing edges. Node 3 unreachable. Returns `-1`. | `ans = -1` |
| 2 | `l=3, r=5, mid=4` | Keeps edges >= 4: (0,1,5) & (1,3,10). Path 0->1->3 has total cost 15 > 10 (`k`). Exceeds budget. Returns `-1`. | `ans = -1` |
| 3 | `l=3, r=3, mid=3` | Keeps edges >= 3: all edges. Path 0->2->3 has cost 3+4=7 <= 10. Minimum edge on path is 3. Returns `3`. | `ans = 3` |
| 4 | `l=4, r=3` | Loop terminates since `l > r`. Returns `3`. | `ans = 3` |

---

### Case 2: Graph with an offline node
Inputs: `edges = [[0,1,7],[1,4,5],[0,2,6],[2,3,6],[3,4,2],[2,4,6]]`, `online = [true,true,true,false,true]`, `k = 12`
Node 3 is offline (`online[3] = false`). Edges involving node 3 are filtered out during graph building.
Graph edges added: `(0,1,7)`, `(1,4,5)`, `(0,2,6)`, `(2,4,6)`. `mnC = 5`, `mxC = 7`.

| Step | Variables state (`l`, `r`, `mid`) | Dijkstra Action & Result | `ans` state |
| :--- | :--- | :--- | :--- |
| 1 | `l=5, r=7, mid=6` | Keeps edges >= 6: (0,1,7), (0,2,6), (2,4,6). Path 0->2->4 cost = 6+6=12 <= 12. Bottleneck = min(6,6) = 6. Returns `6`. | `ans = 6` |
| 2 | `l=7, r=7, mid=7` | Keeps edges >= 7: (0,1,7). Node 4 is unreachable. Returns `-1`. | `ans = 6` |
| 3 | `l=7, r=6` | Loop terminates since `l > r`. Returns `6`. | `ans = 6` |

---

## Time & Space Complexity

### Current Solution Complexity

* **Time Complexity:** `O(log(W) * (M log N))` where `N` is the number of nodes, `M` is the number of edges, and `W` is the range of edge weights (`mxC - mnC`).
  * Binary search takes `O(log W)` iterations.
  * In each iteration, Dijkstra takes `O((N + M) log N)` time using a min-priority queue.
* **Space Complexity:** `O(N + M)`
  * Adjacency list array `E` stores up to `M` edges.
  * Distance and state arrays (`dist`, `pathMin`, `adj`) take `O(N)` memory.

---

### Can We Optimize This Complexity?

**Yes!** The problem states that the given graph is a **Directed Acyclic Graph (DAG)** (a directed graph with no cycles).

Dijkstra's algorithm takes `O(M log N)` time because it uses a priority queue to handle arbitrary graphs with potential cycles. But on a **DAG**, we can find shortest paths in linear time `O(N + M)` using **Topological Sort** or Dynamic Programming!

Even better:
1. We can compute the topological ordering of the nodes **once** at the beginning in `O(N + M)` time.
2. Filtering out weak edges (`w < minW`) does **not** break or change the topological ordering of the remaining nodes.
3. Therefore, during each binary search check, we can replace Dijkstra with a simple 1-pass DP over the precomputed topological order in `O(N + M)` time without any heap operations.

#### Optimized Logic Explanation
We first order nodes so that every directed edge goes from an earlier node to a later node in the list. To check a `minW`, we process nodes in this fixed topological order. For each node `u`, if it is reachable, we attempt to relax its outgoing edges `u -> v` (ignoring edges with `w < minW`). This achieves the exact same shortest path result as Dijkstra, but without `log N` priority queue overhead.

#### Key Code Modification
```cpp
// Precompute topological order once using Kahn's algorithm or DFS
vector<int> topoOrder;
// ... compute topoOrder in O(N + M) ...

// Fast O(N + M) checker replacing Dijkstra
static int checkDAG(int minW, int n, long long k, const vector<int>& topoOrder) {
    fill_n(dist, n, LLONG_MAX);
    dist[0] = 0;
    pathMin[0] = INT_MAX;

    for (int u : topoOrder) {
        if (dist[u] == LLONG_MAX) continue;
        for (int idx = adj[u]; idx != -1; idx = E[idx].nxt) {
            int v = E[idx].v, w = E[idx].w;
            if (w < minW) continue;

            ll d2 = dist[u] + w;
            int curMin = min(pathMin[u], w);

            if (d2 < dist[v] || (d2 == dist[v] && curMin > pathMin[v])) {
                dist[v] = d2;
                pathMin[v] = curMin;
            }
        }
    }
    return dist[n - 1] <= k ? pathMin[n - 1] : -1;
}
```

* **Improved Time Complexity:** `O((N + M) log W)` — eliminating the `log N` factor from the heap.
* **Theoretical Best Complexity:** `O((N + M) log W)` is optimal for binary searching the edge weight range on general DAGs.

---

## Edge Cases Handled

1. **Offline Intermediate Nodes:** Edges connected to offline nodes are filtered out during graph construction (`online[u] && online[v]`), ensuring offline nodes are never traversed.
2. **Unreachable Destination / No Valid Path:** If node `n - 1` cannot be reached under budget `k`, the algorithm correctly returns `-1`.
3. **Multiple Paths with Equal Total Distance:** Ties in total distance are broken using `curMin > pathMin[v]` so the path with the larger bottleneck edge weight is preferred.
4. **Large Edge Weights & Cost Overflow:** Total distance calculations use `long long` (`ll d2 = d + w`) to prevent integer overflow when adding weights up to `10^9` across paths.
5. **Multiple Calls / Reusability:** The global array `adj` is reset using `fill_n` and `eIdx = 0` at the start of `findMaxPathScore`, preventing memory leakage or corrupted graph state across multiple test cases.
