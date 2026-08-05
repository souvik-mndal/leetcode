![Runtime](https://img.shields.io/badge/Runtime-574%20ms%20(beats%2011.73%25)-red?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-460.82%20MB%20(beats%205.17%25)-red?style=for-the-badge)

---

## Problem Explained

You are managing a project with `n` methods (numbered `0` to `n - 1`). Some methods call other methods. These call relationships are given in an array called `invocations`, where `[a, b]` means method `a` calls method `b`.

Method `k` has a bug. Method `k` and any method called by `k` (directly or indirectly through a chain of calls) are flagged as **suspicious**.

We want to remove all suspicious methods from the project. However, there is a catch: **you can only remove the suspicious group if NO healthy (non-suspicious) method calls ANY suspicious method**. If even one healthy method calls a suspicious method, it is unsafe to remove them, so you must keep all methods intact.

Your job is to return a list of all remaining methods after attempting the removal.

**Example:**
* `n = 4, k = 1, invocations = [[1,2],[0,1],[3,2]]`
* Starting from `k = 1`, method `1` calls `2`. So suspicious methods are `{1, 2}`.
* Method `0` (healthy) calls `1` (suspicious), and method `3` (healthy) calls `2` (suspicious).
* Because healthy methods depend on suspicious ones, we cannot remove anything.
* Output: `[0, 1, 2, 3]`

---

## Intuition

Think of method calls as a graph where an arrow points from the caller to the called method (`a -> b`).

First, start at node `k` and follow all outgoing arrows. Every node you reach is part of the suspicious group.

Next, you need to check if any node *outside* this group points *into* this group. 

In the provided solution, the author checks this by launching a search from every non-suspicious node to see if any path lands on a suspicious node. If it does, we flag that an outside connection exists, meaning we cannot delete the suspicious nodes.

---

## Approach

Here is how the provided C++ code executes step-by-step:

* `unordered_map<int, vector<int>> graph;`: Declares a hash map to hold the directed graph where each key is a caller method and its value is a list of methods it calls.
* `mark.assign(n, 0);`: Resizes the `mark` array to size `n` and fills it with `0`. A value of `0` means unvisited, `1` means suspicious, and `2` means visited during non-suspicious checks.
* `for(auto &e : edges) graph[e[0]].push_back(e[1]);`: Builds the adjacency graph by inserting a directed edge from `e[0]` to `e[1]` for every invocation.
* `bfs(1, graph, k);`: Runs a Breadth-First Search starting from method `k` using color code `1`. This visits method `k` and every method reachable from `k`, marking all of them with `1` (suspicious).
* `for(int i = 0; i < n; i++)`: Iterates through every method from `0` to `n - 1`.
* `if(i == k || mark[i] == 1) continue;`: Skips method `i` if it is method `k` or already marked as suspicious (`mark[i] == 1`).
* `bfs(2, graph, i);`: Runs a BFS starting from healthy method `i` using color code `2` to explore what healthy methods can reach.
* `if(mark[nxt] == 1 && color == 2)`: Inside the BFS, if an outgoing neighbor `nxt` is already marked `1` (suspicious) while exploring from a healthy method (`color == 2`), sets `outsideConnection = true` and exits the BFS immediately.
* `if(mark[nxt] != color)`: Marks unvisited nodes with the current search color (`mark[nxt] = color`) and adds them to the `q` queue for further exploration.
* `if(!outsideConnection && mark[i] == 1) continue;`: In the final collection loop, if no outside connection was detected and method `i` is suspicious (`mark[i] == 1`), skips method `i` (removing it).
* `res.push_back(i);`: Adds method `i` to the `res` array if it is remaining.

---

## Dry Run

### Case 1: Removal Blocked by Outside Call
Input: `n = 4, k = 1, edges = [[1,2],[0,1],[3,2]]`

Graph representation:
* `0 -> 1`
* `1 -> 2`
* `3 -> 2`

| Step | Current Action | `mark` state | `outsideConnection` | Description |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `bfs(1, graph, 1)` | `[0, 1, 1, 0]` | `false` | Visits node `1`, then node `2`. Marks both as `1` (suspicious). |
| 2 | Check `i = 0` | `[0, 1, 1, 0]` | `false` | Node `0` is `0` (healthy). Call `bfs(2, graph, 0)`. |
| 3 | Inside `bfs(2)` for `0` | `[2, 1, 1, 0]` | `false` | Pop `node = 0`. Inspect neighbor `nxt = 1`. |
| 4 | Neighbor check | `[2, 1, 1, 0]` | `true` | `mark[1] == 1` and `color == 2`. Sets `outsideConnection = true` and returns! |
| 5 | Skip remaining BFS | `[2, 1, 1, 0]` | `true` | Loop finishes checking other `i`. `outsideConnection` stays `true`. |
| 6 | Collect results | `[2, 1, 1, 0]` | `true` | Since `outsideConnection` is `true`, no marked nodes are deleted. |

**Final Output:** `[0, 1, 2, 3]`

---

### Case 2: Successful Removal
Input: `n = 5, k = 0, edges = [[1,2],[0,2],[0,1],[3,4]]`

Graph representation:
* `0 -> 1`, `0 -> 2`
* `1 -> 2`
* `3 -> 4`

| Step | Current Action | `mark` state | `outsideConnection` | Description |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `bfs(1, graph, 0)` | `[1, 1, 1, 0, 0]` | `false` | Visits `0`, then `1` and `2`. All marked `1` (suspicious). |
| 2 | Check `i = 0, 1, 2` | `[1, 1, 1, 0, 0]` | `false` | All marked `1`. Loop skips them. |
| 3 | Check `i = 3` | `[1, 1, 1, 0, 0]` | `false` | Node `3` is `0`. Call `bfs(2, graph, 3)`. |
| 4 | Inside `bfs(2)` for `3` | `[1, 1, 1, 2, 2]` | `false` | Marks `3` and `4` as `2`. Sees no suspicious nodes (`mark == 1`). |
| 5 | Check `i = 4` | `[1, 1, 1, 2, 2]` | `false` | Node `4` is marked `2` (not `1`), skipped. |
| 6 | Collect results | `[1, 1, 1, 2, 2]` | `false` | `outsideConnection` is `false`. Nodes `0, 1, 2` (marked `1`) are omitted. |

**Final Output:** `[3, 4]`

---

## Time & Space Complexity

### Current Solution Analysis
* **Time Complexity:** **O(N * (N + E))** in the worst case. 
  * Building the graph using `unordered_map` adds hash overhead.
  * Running BFS from `k` takes **O(N + E)**.
  * Running BFS starting from *every* non-suspicious node can visit nodes and edges repeatedly, causing redundant traversals.
* **Space Complexity:** **O(N + E)** to store the graph in `unordered_map`, the `mark` array of size `N`, and queue storage during BFS.

---

### Can this be improved?

**Yes!** The current code is doing way too much work by running a BFS search from *every* healthy node.

**Optimization Idea:**
1. Run **one** traversal (BFS or DFS) starting from `k` to find all suspicious nodes.
2. Instead of running BFS from every healthy node, simply **loop over the original edge list once**.
3. For every edge `[u, v]`, if `u` is **not suspicious** and `v` **is suspicious**, then a healthy method calls a suspicious method! We immediately know we cannot remove anything.
4. Use a `vector<vector<int>>` instead of `unordered_map<int, vector<int>>` for faster graph operations.

#### Optimized C++ Snippet

```cpp
vector<int> remainingMethods(int n, int k, vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
    }

    // Step 1: Find all suspicious methods starting from k
    vector<bool> is_suspicious(n, false);
    queue<int> q;
    q.push(k);
    is_suspicious[k] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (!is_suspicious[v]) {
                is_suspicious[v] = true;
                q.push(v);
            }
        }
    }

    // Step 2: Check if any healthy method calls a suspicious method
    bool blocked = false;
    for (const auto& e : edges) {
        int u = e[0], v = e[1];
        if (!is_suspicious[u] && is_suspicious[v]) {
            blocked = true;
            break; // Outside connection found!
        }
    }

    // Step 3: Collect results
    vector<int> res;
    for (int i = 0; i < n; i++) {
        if (blocked || !is_suspicious[i]) {
            res.push_back(i);
        }
    }
    return res;
}
```

#### Line-by-line connection for key changes:
* `vector<vector<int>> graph(n);`: Direct vector array indexing is much faster and uses less memory than `unordered_map`.
* `if (!is_suspicious[u] && is_suspicious[v])`: Replaces all the extra BFS calls with a single simple pass over the edge list.

#### Improved Complexity
* **Time Complexity:** **O(N + E)** — exactly one graph traversal to mark suspicious nodes, plus one linear scan over the edge list.
* **Space Complexity:** **O(N + E)** — graph storage plus an `O(N)` boolean array for visited tracking.

#### Is this optimal?
**Yes.** We must examine every edge at least once in the worst case to know if an outside call exists, making **O(N + E)** the absolute theoretical best performance possible.

---

## Edge Cases Handled

* **No invocation calls (`edges` is empty):** Method `k` is suspicious and calls nothing. No non-suspicious method calls `k`. Method `k` is safely removed, returning all other methods.
* **`k` calls everything:** If `k` can reach all `n` methods, all methods become suspicious. None are invoked from outside (since no outside methods exist). Returns an empty array `[]`.
* **Isolated components:** Healthy methods in completely disconnected parts of the graph never call suspicious methods, so they don't block the removal.
* **Self-loops and cycles:** Methods inside the suspicious group calling each other (e.g., `1 -> 2` and `2 -> 1`) are correctly handled and do not trigger false outside connections.
