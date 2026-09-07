![Runtime](https://img.shields.io/badge/Runtime-307%20ms%20(beats%2054.75%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-395.5%20MB%20(beats%2026.12%25)-orange?style=for-the-badge)

---

## Problem Explained

Imagine you have a group of nodes, and each node has a numerical value assigned to it. You are given an integer called maxDiff. An undirected edge (a two-way path) exists between two nodes if the absolute difference between their numerical values is less than or equal to maxDiff. 

This forms a graph where nodes with close values are connected to each other. You are also given a list of queries. Each query asks for the minimum number of steps to travel from a starting node to a target node. If the two nodes cannot reach each other through any chain of valid edges, you must return -1.

For example, suppose we have nodes with values [5, 3, 1, 9, 10] and a maxDiff of 2. 
- Node 1 (value 3) and Node 2 (value 1) can connect because their values differ by 2, which is not greater than maxDiff.
- Node 0 (value 5) and Node 1 (value 3) can connect because their values differ by 2.
- Node 3 (value 9) and Node 4 (value 10) connect because their values differ by 1.
If a query asks for the shortest path between Node 0 and Node 2, we can go from Node 0 to Node 1, and then from Node 1 to Node 2. That takes 2 steps, so the answer is 2. If two nodes are completely disconnected, we return -1.

## Intuition

The main challenge is that the nodes are given in a random order, and checking every possible path using standard graph traversal would be too slow because the number of nodes and queries can be up to 10^5.

The key insight is to **sort the nodes by their values**. When values are sorted from smallest to largest, any valid chain of steps becomes a sequence of adjacent jumps across the sorted list. Because maxDiff limits how far values can be, we can use a **Two Pointers** approach (moving two markers to find ranges) to figure out how far a node can reach in a single step.

However, a single step might not get us all the way to our destination. To find the minimum number of steps quickly across many queries, we use a technique called **Binary Lifting** (also used in sparse tables). Instead of jumping one node at a time, we precompute giant power-of-two leaps (jumps of 1 step, 2 steps, 4 steps, 8 steps, and so on). This lets us answer each query in logarithmic time rather than walking through every intermediate node.

## Approach

- `vector<pair<int, int>> newNums(n);` and the loop: This pairs each number with its original index so we don't lose track of where nodes came from, and stores them in a new list.
- `sort(newNums.begin(), newNums.end());`: Sorts all the nodes by their numerical values in ascending order, which is crucial for making adjacency sequential.
- `vector<int> getI(n);` and the loop: Creates a lookup table that maps each original node index to its new position in the sorted array.
- `vector<vector<int>> st(n, vector<int>(18));`: Initializes a table for binary lifting, sized for N rows and 18 columns because 2^18 is greater than 10^5, which covers all possible powers-of-two jump distances.
- `int r = 0;` and the outer loop with `while`: Uses a sliding window (two pointers, i and r) to find the furthest possible node r that can be reached from node i in a single step, ensuring the difference between values stays within maxDiff.
- `st[i][0] = r;`: Stores the single-step reachability limit for each node i in the first column of our sparse table.
- The nested loops for `st[i][j] = st[st[i][j - 1]][j - 1];`: Fills out the rest of the sparse table by combining smaller jumps. Jumping 2^j steps is the same as making two consecutive jumps of size 2^(j-1).
- `vector<int> ans(queries.size(), -1);`: Prepares the answer array, defaulting all query results to -1.
- `int a = getI[queries[i][0]];` and the following lines: Translates the query's start and end nodes into their sorted positions, handles cases where they are already the same node (returning 0), and ensures we always travel from left to right.
- The loop with `for (int j = 17; j >= 0; j--)`: Greedily builds up the distance for a query by making the largest possible power-of-two jumps without overshooting the destination b.
- `ans[i] = (st[curr][0] >= b) ? steps + 1 : -1;`: Takes one final step if needed to reach or pass the destination b, and records either the total step count or -1 if unreachable.

## Dry Run

### Case 1: Typical case (n = 5, nums = [5, 3, 1, 9, 10], maxDiff = 2, queries = [[0, 1], [0, 2]])
*Note: Only tracing the first two queries for brevity.*

| Step / Query | a (Sorted Start) | b (Sorted End) | curr | steps | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Query 0: [0, 1] | 2 | 1 | 2 | 0 | a > b, so we swap a and b. Now a = 1, b = 2. Distance is 1 step. |
| Query 1: [0, 2] | 2 | 0 | 2 | 0 | Swapped so a = 0, b = 2. We jump using the sparse table from index 0, taking 2 steps to reach index 2. |

### Case 2: Edge case with no path (n = 3, nums = [3, 6, 1], maxDiff = 1, queries = [[0, 1]])

| Step / Query | a (Sorted Start) | b (Sorted End) | curr | steps | Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Query 0: [0, 1] | 1 | 2 | 1 | 0 | Nodes are sorted values 1, 3, 6. With maxDiff = 1, no adjacent sorted elements can bridge the gap. st[curr][0] cannot reach b, so result is -1. |

## Time & Space Complexity

**Time:** O(N log N + Q log N), where N is the number of nodes and Q is the number of queries. Sorting the nodes takes N log N time. Building the sparse table takes N * 18 operations, which is O(N). Each of the Q queries uses binary lifting, taking 18 steps (log N) to compute.
**Space:** O(N log N) to store the sparse table of size N * 18, plus O(N) space for auxiliary arrays like sorting and index mapping.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already optimal for a comparison-based sorting approach with multiple range queries. Because we must sort the nodes to establish sequential connectivity and use logarithmic jumps to answer arbitrary queries quickly, the O(N log N + Q log N) complexity matches the theoretical best approach for offline/online range-jump path queries on sorted linear structures. No further asymptotic improvement is possible without changing the fundamental constraints of the problem.

## Edge Cases Handled

- **Duplicate values:** The code handles duplicate node values correctly because sorting groups equal values together, and the sliding window correctly allows zero-difference steps.
- **Start and end node are the same:** Handled immediately by checking `if (a == b) { ans[i] = 0; }`, returning 0 steps.
- **Nodes that cannot reach each other:** Handled by the final check `st[curr][0] >= b`, returning -1 if even a maximum single step cannot cross the remaining gap.
- **Unordered query inputs:** The code handles cases where the start node index is larger than the end node index by using `if (a > b) swap(a, b);`.
