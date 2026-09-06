![Runtime](https://img.shields.io/badge/Runtime-574%20ms%20(beats%2010.24%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-460.8%20MB%20(beats%205.01%25)-red?style=for-the-badge)

---

## Problem Explained

Imagine you are maintaining a software project with several functions (called **methods**), numbered from `0` to `n - 1`. Some methods call other methods. These calls are given as a list of directed pairs called `invocations`. For example, `[a, b]` means method `a` calls method `b`.

Method `k` has a bug in it. Because method `k` is buggy, any method that `k` calls (and any method *those* methods call, and so on) is also considered **suspicious**.

Your goal is to delete all suspicious methods from the project. However, there is one strict rule:
* You can **only** delete the suspicious methods if **no normal (clean) method** calls any suspicious method. 
* If even one clean method calls a suspicious method, you **cannot** delete anything. In that case, you must keep all methods and return every method from `0` to `n - 1`.

If it is safe to delete them, you return a list of only the remaining clean methods.

---

## Intuition

Think of the methods as spots (nodes) and calls as one-way roads (edges) connecting them.

First, start at the broken method `k` and follow every road leading away from it. Every spot you can reach belongs to the "suspicious" group. We can paint these nodes with a special flag (or "color 1").

Second, we need to check if any clean node can reach a suspicious node. Instead of searching from clean nodes, the core logic asks: *"Is there any clean method that attempts to invoke a suspicious method?"* 

The given code tests this by running a second search starting from clean nodes (painting them "color 2"). If a color 2 search ever hits a node that was already marked as color 1, we know an outside clean method depends on a suspicious method. That sets a flag `outsideConnection = true`, meaning we must cancel the deletion and keep everything.

---

## Approach

Here is how the code executes step-by-step:

* `unordered_map<int, vector<int>> graph;` creates a adjacency list (a hash map mapping each method to a list of methods it calls directly).
* `mark.assign(n, 0);` initializes a tracker array `mark` of size `n` filled with `0`s. A value of `0` means unvisited, `1` means suspicious, and `2` means visited by a clean method search.
* `for(auto &e : edges) graph[e[0]].push_back(e[1]);` loops through the input `edges` and populates the `graph` so each method maps to its outgoing calls.
* `bfs(1, graph, k);` starts a Breadth-First Search (BFS) beginning at method `k` using `color = 1`. This marks method `k` and every method reachable from `k` as suspicious (`mark[node] = 1`).
* `for(int i = 0; i < n; i++) { if(i == k || mark[i] == 1) continue; bfs(2, graph, i); }` iterates through all nodes `0` to `n - 1`. If node `i` is not suspicious, it runs `bfs(2, graph, i)` to explore what clean methods can reach.
* `if(mark[nxt] == 1 && color == 2) { outsideConnection = true; return; }` inside the `bfs` function, if a color `2` search reaches a neighbor `nxt` that has `mark[nxt] == 1`, it means an outside clean node calls a suspicious node. It sets `outsideConnection` to `true` and stops early.
* `for(int i = 0; i < n; i++) { if(!outsideConnection && mark[i] == 1) continue; res.push_back(i); }` builds the final output list. If `outsideConnection` is `false`, suspicious nodes (`mark[i] == 1`) are skipped. If `outsideConnection` is `true`, no nodes are skipped and all `0` to `n - 1` nodes are saved into `res`.

---

## Dry Run

### Case 1: Outside connection exists (Cannot remove)
Inputs: `n = 4`, `k = 1`, `edges = [[1, 2], [0, 1], [3, 2]]`

Graph connections: `1 -> 2`, `0 -> 1`, `3 -> 2`.

1. Run `bfs(1, graph, k=1)` to mark suspicious nodes:

| Queue `q` | Current `node` | Neighbor `nxt` | Action / `mark` state update |
| :--- | :--- | :--- | :--- |
| `[1]` | - | - | Start BFS at `k=1`. Set `mark[1] = 1`. |
| `[]` | `1` | `2` | Pop `1`. Neighbor `2` is unvisited. Set `mark[2] = 1`, push `2`. |
| `[2]` | `2` | None | Pop `2`. No outgoing connections. Queue becomes empty. |

Suspicious nodes identified: `mark[1] = 1`, `mark[2] = 1`.

2. Loop `i` from `0` to `3` to run `bfs(2, graph, i)` on non-suspicious nodes:

| Loop `i` | Queue `q` | Current `node` | Neighbor `nxt` | Action / Flag Update |
| :--- | :--- | :--- | :--- | :--- |
| `i = 0` | `[0]` | - | `mark[0]` is `0`. Start BFS. Set `mark[0] = 2`. |
| `i = 0` | `[]` | `0` | `1` | Pop `0`. Neighbor `1` has `mark[1] == 1` and `color == 2`! |
| `i = 0` | `[]` | `0` | `1` | Set `outsideConnection = true` and `return`. |

3. Build result: Since `outsideConnection` is `true`, we cannot remove anything. `res` gets `[0, 1, 2, 3]`.

---

### Case 2: Clean removal (No outside connection)
Inputs: `n = 5`, `k = 0`, `edges = [[1, 2], [0, 2], [0, 1], [3, 4]]`

Graph connections: `1 -> 2`, `0 -> 2`, `0 -> 1`, `3 -> 4`.

1. Run `bfs(1, graph, k=0)` to mark suspicious nodes:

| Queue `q` | Current `node` | Neighbor `nxt` | Action / `mark` state update |
| :--- | :--- | :--- | :--- |
| `[0]` | - | - | Start BFS at `k=0`. Set `mark[0] = 1`. |
| `[]` | `0` | `2` | Pop `0`. Set `mark[2] = 1`, push `2`. |
| `[2]` | `0` | `1` | Still processing `0`. Set `mark[1] = 1`, push `1`. |
| `[2, 1]` | `2` | None | Pop `2`. No outgoing connections. |
| `[1]` | `1` | `2` | Pop `1`. Neighbor `2` already has `mark[2] == 1`. Skip. |

Suspicious nodes identified: `mark[0] = 1`, `mark[1] = 1`, `mark[2] = 1`.

2. Loop `i` from `0` to `4` to run `bfs(2, graph, i)` on non-suspicious nodes:

| Loop `i` | Queue `q` | Current `node` | Neighbor `nxt` | Action / Flag Update |
| :--- | :--- | :--- | :--- | :--- |
| `i = 0` | - | - | - | `mark[0] == 1` (suspicious), skip. |
| `i = 1` | - | - | - | `mark[1] == 1` (suspicious), skip. |
| `i = 2` | - | - | - | `mark[2] == 1` (suspicious), skip. |
| `i = 3` | `[3]` | - | - | `mark[3]` is `0`. Start BFS. Set `mark[3] = 2`. |
| `i = 3` | `[]` | `3` | `4` | Pop `3`. Set `mark[4] = 2`, push `4`. |
| `i = 3` | `[4]` | `4` | None | Pop `4`. No outgoing connections. |
| `i = 4` | - | - | - | `mark[4] == 2` (already processed), skip. |

3. Build result: `outsideConnection` is `false`. We remove suspicious nodes (`0`, `1`, `2`). `res` gets `[3, 4]`.

---

## Time & Space Complexity

### Current Solution Performance
* **Time Complexity:** `O(N + E)` where `N` is the number of methods (`n`) and `E` is the number of invocations (`edges.length`). The BFS visits each node and edge at most a couple of times. However, using `unordered_map<int, vector<int>>` adds extra hashing overhead on every lookup.
* **Space Complexity:** `O(N + E)` to store the graph in an `unordered_map`, track visited states in `mark`, and process nodes in the BFS `queue`.

### Can it be improved?
**Yes!** The algorithm currently performs poorly in benchmarks (beating ~10% runtime) for two reasons:
1. `unordered_map` is slow due to hash collisions and dynamic allocation. A simple dynamic array (`vector<vector<int>>`) of fixed size `n` is significantly faster.
2. The second pass runs full BFS searches (`color = 2`) across clean components. This is unnecessary! Once we have marked all suspicious methods with `mark[i] == 1`, we only need to inspect the input `edges` array once. If any edge `[u, v]` has `mark[u] != 1` (caller is clean) and `mark[v] == 1` (callee is suspicious), an outside invocation exists!

### Improved Logic & Code

Instead of a second BFS, we:
1. Use `vector<vector<int>>` for fast graph indexing.
2. Run one BFS from `k` to set `mark[i] = true` for suspicious nodes.
3. Check all original edges once in a simple loop.

```cpp
class Solution {
public:
    vector<int> remainingMethods(int n, int k, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (const auto& e : edges) {
            graph[e[0]].push_back(e[1]);
        }

        vector<bool> isSuspicious(n, false);
        queue<int> q;
        q.push(k);
        isSuspicious[k] = true;

        // BFS 1: Find all suspicious nodes
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : graph[u]) {
                if (!isSuspicious[v]) {
                    isSuspicious[v] = true;
                    q.push(v);
                }
            }
        }

        // Check if any clean node calls a suspicious node
        for (const auto& e : edges) {
            if (!isSuspicious[e[0]] && isSuspicious[e[1]]) {
                // Outside connection found! Return all nodes.
                vector<int> all(n);
                for (int i = 0; i < n; ++i) all[i] = i;
                return all;
            }
        }

        // No outside connection: Return only clean nodes
        vector<int> res;
        for (int i = 0; i < n; ++i) {
            if (!isSuspicious[i]) {
                res.push_back(i);
            }
        }
        return res;
    }
};
```

### Optimal Complexity Comparison
* **Optimized Time Complexity:** `O(N + E)` — One BFS pass through reachable nodes, plus one linear pass over the `edges` array.
* **Optimized Space Complexity:** `O(N + E)` — Uses compact vectors for graph adjacency lists, queue, and boolean tracking array.
* **Is this theoretically optimal?** Yes. Any solution must read all input edges at least once, so `O(N + E)` is the absolute theoretical lower bound.

---

## Edge Cases Handled

* **No calls out of `k` (`edges` is empty or `k` calls nothing):** Only method `k` is marked suspicious. If no clean node calls `k`, only `k` is removed.
* **All methods are suspicious:** If method `k` can reach every other method directly or indirectly, every node gets marked suspicious. There are no clean nodes left to trigger an outside invocation, so the code safely removes all methods and returns an empty array `[]`.
* **Cyclic dependencies:** If suspicious methods invoke each other in a loop (e.g., `A -> B -> A`), the visited array (`mark` or `isSuspicious`) prevents infinite loops during BFS.
* **Disconnected graphs:** Methods in completely isolated components are naturally treated as clean and remain untouched unless invoked by/invoking suspicious nodes.
