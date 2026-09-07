![Runtime](https://img.shields.io/badge/Runtime-43%20ms%20(beats%2056.63%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-136.9%20MB%20(beats%2026.04%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a map of cities (called **vertices**) and roads connecting them (called **edges**). This forms a **graph**. 

Sometimes, the roads are grouped into isolated clusters. In graph theory, each isolated cluster is called a **connected component** — meaning you can travel from any city in the cluster to any other city in that same cluster, but there are no roads leading out to cities in other clusters.

This problem asks us to find how many of these clusters are **complete**. A cluster is complete if *every single city* inside it has a direct road connecting it to *every other city* in that same cluster. In other words, everybody is directly connected to everybody else within that group. We need to count how many such fully connected clusters exist in the graph.

## Intuition

To solve this, we need to look at every isolated cluster one by one, count how many cities are in it, and count how many roads it contains.

How do we know if a cluster is complete just by looking at its size and road count? There is a neat mathematical rule for complete groups. If a cluster has N cities, every city can connect to N - 1 other cities. If you multiply the number of cities by how many connections each one has, you get N * (N - 1). But since each road connects two cities, we double-count every road. So the total number of unique roads in a complete cluster must equal N * (N - 1) / 2.

If we visit a cluster, count its cities (let us call this variable `nodes`), and count its roads (let us call this variable `edgeCount`), we can just check if `edgeCount` equals `nodes * (nodes - 1) / 2`. If it matches, that cluster is complete! We use a **Breadth-First Search** (a way to explore a graph level by level, like a spreading ripple) to hop from city to city and map out each entire cluster.

## Approach

- `vector<vector<int>> adj(n);`: Creates an **adjacency list** (a list of lists where each city stores a list of its direct neighbors) to represent the graph, sizing it to hold N empty lists.
- `for (auto &e : edges) { ... }`: Loops through every road in the input, adding the connection to both cities' lists since the roads are undirected (two-way).
- `vector<bool> vis(n, false);`: Creates a tracking list of size N initialized to false to remember which cities we have already visited so we do not process the same cluster twice.
- `int ans = 0;`: Initializes our answer counter to zero to keep track of how many complete components we find.
- `for (int i = 0; i < n; i++) {`: Loops through every single city from 0 to n - 1 to make sure we do not miss any isolated groups.
- `if (vis[i]) continue;`: Skips the current city if it has already been visited as part of a previous cluster exploration.
- `queue<int> q; q.push(i); vis[i] = true;`: Sets up a **queue** (a first-in, first-out line) to explore the current cluster, adding the starting city and marking it as visited.
- `int nodes = 0; int edgeCount = 0;`: Initializes local counters to track the number of cities and roads found inside the current cluster.
- `while (!q.empty()) {`: Loops as long as there are still connected cities in our queue to explore.
- `int u = q.front(); q.pop();`: Grabs the next city from the front of the queue and removes it.
- `nodes++; edgeCount += adj[u].size();`: Increments our city count by 1 and adds the number of roads connected to this city to our running road count.
- `for (int v : adj[u]) { if (!vis[v]) { vis[v] = true; q.push(v); } }`: Looks at all neighbors of the current city, and if we have not visited them yet, marks them as visited and pushes them into the queue to explore later.
- `edgeCount /= 2;`: Divides the total road count by 2 because each undirected road was counted twice (once from each of its two endpoints).
- `if (edgeCount == nodes * (nodes - 1) / 2) ans++;`: Checks if the actual road count matches the required formula for a complete cluster, and if so, increments our answer counter.
- `return ans;`: Returns the final count of complete components after checking all cities.

## Dry Run

### Case 1: Typical case (n = 6, edges = [[0,1],[0,2],[1,2],[3,4]])
Vertices: 0, 1, 2, 3, 4, 5. Edge graph has two separate clusters: {0,1,2} and {3,4}, plus isolated vertex 5.

| i | vis[i] | q contents | nodes | edgeCount | Formula check: nodes*(nodes-1)/2 | ans | Action |
|---|---|---|---|---|---|---|---|
| 0 | false -> true | [0] | 1 | 2 | 1 * 0 / 2 = 0 (fails) -> expands to queue | 0 | Starts BFS at 0, visits neighbors 1 and 2 |
| 1 | true (loop) | - | - | - | - | - | Skipped because already visited |
| 3 | false -> true | [3] | 1 | 1 | 1 * 0 / 2 = 0 (fails) -> expands to queue | 0 | Starts BFS at 3, visits neighbor 4 |
| 5 | false -> true | [5] | 1 | 0 | 1 * 0 / 2 = 0 (matches) | 1 | Processes isolated vertex 5, edgeCount 0 equals formula 0, increments ans |

*(Note: During the BFS for cluster {0,1,2}, nodes reaches 3 and edgeCount reaches 6, which divides to 3. The formula 3 * 2 / 2 is also 3, so that cluster increments ans as well. Final ans = 3.)*

### Case 2: Incomplete component (n = 4, edges = [[0,1],[1,2],[2,3]])
Vertices: 0, 1, 2, 3 forming a single straight line path (not fully connected).

| i | vis[i] | q contents | nodes | edgeCount | edgeCount vs Formula | ans | Action |
|---|---|---|---|---|---|---|---|
| 0 | false -> true | [0, 1, 2, 3] | 4 | 6 -> becomes 3 | 3 != 4 * 3 / 2 (6) | 0 | Runs full BFS for the whole line graph, tallies 4 nodes and 6 directed edge ends (3 actual edges) |

Since 3 does not equal 6, it is not a complete component, so ans remains 0.

## Time & Space Complexity

- **Time:** O(N + E) — where N is the number of vertices and E is the number of edges. We visit every vertex and every edge a constant number of times during graph construction and the breadth-first search.
- **Space:** O(N + E) — to store the adjacency list `adj`, the visited array `vis`, and the queue `q`.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

This code is **already optimal** in terms of time and space complexity. 

Why? Because any solution to this problem must at least look at every edge and vertex given in the input to know how they connect. Reading the input alone takes O(N + E) time and space. Since our algorithm runs in linear time relative to the input size, it is mathematically impossible to do faster than O(N + E). No further optimization is needed.

## Edge Cases Handled

- **Completely isolated single vertices:** Handled correctly because the loop checks individual nodes. A single node has 1 node and 0 edges, and 0 equals 1 * 0 / 2, so it correctly counts as a complete component of size 1.
- **Empty edge list (0 edges):** Handled correctly. Every vertex acts as its own separate complete component of size 1, and the code returns N.
- **Graph with no edges except one pair:** Handled correctly by the formula check, which will fail for components that are paths or stars rather than cliques.
