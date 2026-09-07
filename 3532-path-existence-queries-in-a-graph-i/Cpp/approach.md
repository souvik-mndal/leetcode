![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-224.1%20MB%20(beats%2049.52%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine you have a group of nodes (numbered from 0 up to n - 1). Each node has a number value assigned to it, and these numbers are given in a sorted list called nums. 

You also have a rule about connections: an undirected edge (a two-way path) exists between node i and node j if the difference between their values is small enough — specifically, if the absolute difference between nums[i] and nums[j] is at most maxDiff. 

You are given a list of queries. Each query gives you a starting node and an ending node. Your job is to look at the graph and figure out whether a path exists between them. You return a list of true or false answers for each query.

## Intuition

Even though the problem talks about graphs and path queries (which usually make you think of heavy graph algorithms like Union-Find or Breadth-First Search), there is a clever shortcut hidden in the setup. 

Notice that the input array nums is already sorted. If node A and node B have values close to each other (within maxDiff), they can talk to each other. More importantly, if node A connects to node B, and node B connects to node C, they all form a single connected group (a component). 

Because nums is sorted, any chain of connected nodes must be right next to each other in the sorted list. If the gap between adjacent numbers in the sorted array (nums[i] - nums[i - 1]) is less than or equal to maxDiff, they belong to the exact same connected group. If the gap is suddenly larger than maxDiff, the chain breaks, and all subsequent nodes belong to a brand-new group.

Instead of building a real graph and searching through it, we can just assign a group ID to every node by scanning the sorted array once. Two nodes can reach each other if and only if they share the same group ID. This turns a complex graph problem into a simple group-comparison problem.

## Approach

* `vector<int> comp(n);`
  Creates a vector to store the group identifier for each node from index 0 to n - 1.
* `comp[0] = 0;`
  Initializes the first node to belong to the first group, labeled group 0, since it has no previous neighbor.
* `for (int i = 1; i < n; i++) { ... }`
  Loops through the sorted array starting from the second element to assign group IDs to every node.
* `if (nums[i] - nums[i - 1] <= maxDiff) comp[i] = comp[i - 1];`
  Checks if the current node's value is close enough to the previous node's value. If it is, the current node joins the exact same group as the previous node.
* `else comp[i] = comp[i - 1] + 1;`
  Triggers if the gap is too large. This breaks the chain, so the current node starts a brand-new group by incrementing the group ID by 1.
* `vector<bool> ans;`
  Prepares a boolean vector to hold the final true or false answer for each query.
* `ans.reserve(queries.size());`
  Reserves memory space ahead of time to make pushing answers faster and more efficient.
* `for (auto &q : queries)`
  Loops through every query in the input list, where each query is a pair of nodes `[u_i, v_i]`.
* `ans.push_back(comp[q[0]] == comp[q[1]]);`
  Checks if the starting node `q[0]` and ending node `q[1]` share the same group ID. If their IDs match, a path exists, so true is added; otherwise, false.
* `return ans;`
  Returns the completed list of boolean answers for all queries.

## Dry Run

### Case 1: Typical case with multiple groups
**Input:** n = 4, nums = [2, 5, 6, 8], maxDiff = 2, queries = [[0,1],[0,2],[1,3],[2,3]]

| i / q | nums[i] | Action / Logic | comp array state | Query check | Result pushed |
|---|---|---|---|---|---|
| 0 | 2 | Set base group for node 0 | `[0, 0, 0, 0]` | - | - |
| 1 | 5 | Gap 5 - 2 = 3 > 2. New group. | `[0, 1, 0, 0]` | - | - |
| 2 | 6 | Gap 6 - 5 = 1 <= 2. Same group. | `[0, 1, 1, 0]` | - | - |
| 3 | 8 | Gap 8 - 6 = 2 <= 2. Same group. | `[0, 1, 1, 1]` | - | - |
| [0, 1] | - | Compare comp[0] (0) and comp[1] (1) | `[0, 1, 1, 1]` | 0 == 1 | false |
| [0, 2] | - | Compare comp[0] (0) and comp[2] (1) | `[0, 1, 1, 1]` | 0 == 1 | false |
| [1, 3] | - | Compare comp[1] (1) and comp[3] (1) | `[0, 1, 1, 1]` | 1 == 1 | true |
| [2, 3] | - | Compare comp[2] (1) and comp[3] (1) | `[0, 1, 1, 1]` | 1 == 1 | true |

### Case 2: Edge case where all nodes are connected
**Input:** n = 2, nums = [1, 3], maxDiff = 1, queries = [[0,0],[0,1]]

| i / q | nums[i] | Action / Logic | comp array state | Query check | Result pushed |
|---|---|---|---|---|---|
| 0 | 1 | Set base group for node 0 | `[0, 0]` | - | - |
| 1 | 3 | Gap 3 - 1 = 2 > 1. New group. | `[0, 1]` | - | - |
| [0, 0] | - | Compare comp[0] (0) and comp[0] (0) | `[0, 1]` | 0 == 0 | true |
| [0, 1] | - | Compare comp[0] (0) and comp[1] (1) | `[0, 1]` | 0 == 1 | false |

## Time & Space Complexity

**Time:** O(N + Q) — where N is the number of elements in nums and Q is the number of queries. We loop through the nums array once to assign groups, which takes O(N) time. Then we loop through each query and do a single array lookup, which takes O(1) time per query, totaling O(Q) time. 

**Space:** O(N) — because we allocate the comp vector of size N to store the group identifiers, and an answer vector of size Q to store the results.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this code is already fully optimal. We must read the input arrays of size N and process the queries of size Q, meaning any correct solution must take at least linear time relative to the input and query sizes. You cannot go faster than visiting the data linearly.

## Edge Cases Handled

* **Duplicate values in nums:** Handled correctly because if nums[i] - nums[i - 1] is 0, it is less than or equal to maxDiff, so duplicate elements smoothly merge into the same group.
* **maxDiff equals 0:** Handled properly; numbers must match exactly to be in the same group, otherwise every distinct number forms its own separate group.
* **Queries for the exact same node (e.g., [0, 0]):** Handled naturally because comparing a node's group to itself always yields true.
* **Large constraints (N up to 10^5):** Handled efficiently with a single linear pass and no nested loops or heavy data structures, easily running well within time limits.
