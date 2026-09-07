![Runtime](https://img.shields.io/badge/Runtime-16%20ms%20(beats%2088.64%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-140.7%20MB%20(beats%2090.06%25)-brightgreen?style=for-the-badge)

---

## Problem Explained

Imagine you are looking at a map of cities. Some pairs of cities are connected by roads, and each road has a distance attached to it. The entire network of cities might be split into completely separate groups, meaning you cannot necessarily drive from every city to every other city.

The problem defines the score of a path as the minimum road distance you encounter while traveling along it. Even if you loop around, visit the same city multiple times, or cross the same road over and over, you only care about the single shortest road used in that journey. 

We are asked to find the minimum possible score of a path that starts at city 1 and ends at city n. 

For example, if you have cities 1, 2, 3, and 4, and roads connect 1 to 2 (distance 9), 2 to 3 (distance 6), 2 to 4 (distance 5), and 1 to 4 (distance 7), the path from 1 to 4 through city 2 uses roads of distance 9 and 5. The minimum road on that path is 5. No other path gives a smaller minimum road, so the answer is 5.

## Intuition

At first glance, you might think you need to search for a specific path from city 1 to city n, maybe using **Depth-First Search** (DFS) or **Breadth-First Search** (BFS) to try every route. But notice a very freeing detail in the problem description: *you can visit cities 1 and n multiple times, and you can reuse roads.*

This means if a city is anywhere in the same connected component (group of interconnected cities) as city 1, you can easily travel to it, explore around it, and come back to city 1 without any penalty. 

Therefore, any city that can reach city 1 is part of our playground. Any road connected to *any* city in this playground can be crossed as part of our journey. To find the minimum score, we just need to look at every single road in the entire connected component of city 1, and find the smallest distance among them.

To group the cities efficiently, we use a **Union-Find** data structure (also called **Disjoint Set Union**). It lets us quickly merge cities that share a road into the same group and check if two cities belong to the same group. Once all roads are processed, we loop through every road one last time and pick the minimum distance of any road whose endpoints both belong to city 1's group.

## Approach

- `vector<int> root(n + 1); iota(root.begin(), root.end(), 0);`: Creates a parent tracking array where every city starts out as its own separate group (the root of city `i` is `i`).
- `auto find = [&](this auto& self, int i) -> int { return root[i] == i ? i : root[i] = self(root[i]); };`: Defines a recursive helper function to find the top representative (root) of a city's group, using path compression (`root[i] = self(root[i])`) to flatten the tree for speed.
- `for (auto& r : roads) root[find(r[0])] = find(r[1]);`: Loops through every road in the input, finding the group roots of both endpoints `r[0]` and `r[1]`, and merging their groups together.
- `int res = 10001;`: Initializes our minimum score result to a number larger than any possible road distance (since the maximum distance constraint is 10,000).
- `for (auto& r : roads)`: Loops through all the roads a second time to inspect their distances.
- `if (find(r[0]) == find(1)) res = min(res, r[2]);`: Checks if the current road's city belongs to the same connected group as city 1. If it does, we update our minimum score `res` using the road's distance `r[2]`.
- `return res;`: Returns the final minimum score found across all roads connected to city 1's component.

## Dry Run

### Case 1: Example 1 from the problem description
Inputs: `n = 4`, `roads = [[1,2,9],[2,3,6],[2,4,5],[1,4,7]]`

| Road Checked (`r`) | Action Taken | State of `root` Array (indices 1 to 4) |
| :--- | :--- | :--- |
| Initialization | Create roots for 4 cities | `[0, 1, 2, 3, 4]` |
| `[1, 2, 9]` | Merge city 1 and city 2 | `[0, 1, 1, 3, 4]` |
| `[2, 3, 6]` | Merge city 2 and city 3 | `[0, 1, 1, 1, 4]` |
| `[2, 4, 5]` | Merge city 2 and city 4 | `[0, 1, 1, 1, 1]` |
| `[1, 4, 7]` | Both already in group 1, no-op | `[0, 1, 1, 1, 1]` |
| Second pass: `[1, 2, 9]` | Both in group 1; `res` becomes `min(10001, 9) = 9` | `res = 9` |
| Second pass: `[2, 3, 6]` | Both in group 1; `res` becomes `min(9, 6) = 6` | `res = 6` |
| Second pass: `[2, 4, 5]` | Both in group 1; `res` becomes `min(6, 5) = 5` | `res = 5` |
| Second pass: `[1, 4, 7]` | Both in group 1; `res` becomes `min(5, 7) = 5` | `res = 5` |

Final output: `5`

### Case 2: Linear road chain edge case
Inputs: `n = 3`, `roads = [[1,2,10],[2,3,20]]`

| Road Checked (`r`) | Action Taken | State of `root` Array (indices 1 to 3) |
| :--- | :--- | :--- |
| Initialization | Create roots for 3 cities | `[0, 1, 2, 3]` |
| `[1, 2, 10]` | Merge city 1 and city 2 | `[0, 1, 1, 3]` |
| `[2, 3, 20]` | Merge city 2 and city 3 | `[0, 1, 1, 1]` |
| Second pass: `[1, 2, 10]` | Both in group 1; `res` becomes `min(10001, 10) = 10` | `res = 10` |
| Second pass: `[2, 3, 20]` | Both in group 1; `res` becomes `min(10, 20) = 10` | `res = 10` |

Final output: `10`

## Time & Space Complexity

- **Time:** O(V + E * alpha(V)), where V is the number of cities (n) and E is the number of roads. The Union-Find operations take nearly constant time thanks to path compression, denoted by the inverse Ackermann function `alpha(V)`. We loop through the roads twice, making the runtime extremely fast and easily passing within limits.
- **Space:** O(n), because we store the `root` parent array of size `n + 1` to track city groups.

**Is this already the most optimal possible complexity for this problem, or can it be improved?**

Yes, this is already optimal. Any solution to this problem must inspect all relevant roads and group connected cities together, which takes linear-ish time with respect to the input size. Union-Find provides nearly constant-time updates and lookups, meaning we cannot asymptotically improve upon O(E * alpha(V)) time or O(n) space.

## Edge Cases Handled

- **Disconnected components:** The problem states the graph is not necessarily connected. The Union-Find approach naturally isolates cities that cannot reach city 1, preventing their roads from incorrectly lowering our score during the second pass.
- **Multiple paths and cycles:** The constraints allow revisiting roads and cities. Union-Find handles cycles gracefully because merging already-connected nodes does nothing harmful.
- **Large distance values:** The initialization of `res = 10001` correctly sits above the maximum possible road distance constraint of 10,000, ensuring the first valid road comparison always overwrites it correctly.
