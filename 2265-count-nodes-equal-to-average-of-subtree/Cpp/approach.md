![Runtime](https://img.shields.io/badge/Runtime-7%20ms%20(beats%2054.96%25)-yellow?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-15.71%20MB%20(beats%2064.72%25)-green?style=for-the-badge)

---

## Problem Explained

The task is to look at every node in a binary tree and check a special rule. 

For any given node, its **subtree** includes that node itself plus all the nodes below it connected to it. We need to calculate two numbers for that subtree:
1. The **sum** of all node values in the subtree.
2. The total **count** of nodes in the subtree.

Next, we calculate the average by dividing the total sum by the node count (rounding down to the nearest whole number). If this average equals the node's own value, we count that node as a match. Finally, we return the total number of matching nodes in the entire tree.

For example, if a node has value `5`, and its subtree contains values `5` and `6`:
* Sum = `5 + 6 = 11`
* Node count = `2`
* Average = `11 / 2 = 5` (rounded down)

Since the average `5` matches the node's value `5`, this node counts toward our answer.

---

## Intuition

To calculate the average of a subtree, a node needs to know two things: the sum of values and the total number of nodes in its subtrees. 

A parent node cannot compute this until its children have calculated their own sums and counts first. This means we must process the tree from the bottom up. 

In tree terms, this bottom-up order is called a **post-order depth-first search (DFS)**. We visit the left child, then the right child, and finally gather their results at the current parent node. Each node returns a pair of numbers back up to its parent: `(total sum of subtree, total count of nodes in subtree)`.

---

## Approach

Here is how the code works step-by-step:

* `using int2=pair<int, int>;` — Creates a short alias named `int2` for a pair of integers. The first number stores the sum, and the second stores the node count.
* `int count=0;` — Defines a global counter variable to keep track of how many nodes satisfy the average condition.
* `if (node==NULL) return {0, 0};` — Sets the base case for recursion. If we hit an empty branch, it contributes `0` to the sum and `0` to the node count.
* `auto [sumL, iL]=postOrder(node->left);` — Recursively processes the left subtree. It unpacks the returned pair into `sumL` (left sum) and `iL` (left node count).
* `auto [sumR, iR]=postOrder(node->right);` — Recursively processes the right subtree into `sumR` (right sum) and `iR` (right node count).
* `int sum=node->val, i=1;` — Initializes local variables for the current node. `sum` starts with the node's own value, and `i` starts at `1` to count the current node itself.
* `sum+=(sumL+sumR);` — Adds the sums from both left and right subtrees to `sum`.
* `i+=(iL+iR);` — Adds the node counts from both left and right subtrees to `i`.
* `if (sum/i==node->val) count++;` — Calculates the integer average `sum / i` (which automatically rounds down in C++). If it equals `node->val`, we increment our answer `count`.
* `return {sum, i};` — Sends the total `sum` and node count `i` of this node's subtree back up to its parent node.
* `averageOfSubtree(TreeNode* root)` — Starts the recursive traversal from `root` and returns the final value of `count`.

---

## Dry Run

### Case 1: Standard Tree (`root = [4,8,5,0,1,null,6]`)

Tree Structure:
```text
        4
       / \
      8   5
     / \   \
    0   1   6
```

| Step / Node | `node->val` | Left Return `{sumL, iL}` | Right Return `{sumR, iR}` | Calculated `sum` | Calculated `i` | Average (`sum / i`) | Match? | `count` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Leaf 1 | 0 | `{0, 0}` | `{0, 0}` | 0 | 1 | 0 / 1 = 0 | Yes (0 == 0) | 1 |
| Leaf 2 | 1 | `{0, 0}` | `{0, 0}` | 1 | 1 | 1 / 1 = 1 | Yes (1 == 1) | 2 |
| Node 8 | 8 | `{0, 1}` | `{1, 1}` | 8 + 0 + 1 = 9 | 1 + 1 + 1 = 3 | 9 / 3 = 3 | No (3 != 8) | 2 |
| Leaf 3 | 6 | `{0, 0}` | `{0, 0}` | 6 | 1 | 6 / 1 = 6 | Yes (6 == 6) | 3 |
| Node 5 | 5 | `{0, 0}` | `{6, 1}` | 5 + 0 + 6 = 11 | 1 + 0 + 1 = 2 | 11 / 2 = 5 | Yes (5 == 5) | 4 |
| Root 4 | 4 | `{9, 3}` | `{11, 2}` | 4 + 9 + 11 = 24 | 1 + 3 + 2 = 6 | 24 / 6 = 4 | Yes (4 == 4) | 5 |

Final `count` returned: **5**

---

### Case 2: Single Node Tree (`root = [1]`)

| Step / Node | `node->val` | Left Return `{sumL, iL}` | Right Return `{sumR, iR}` | Calculated `sum` | Calculated `i` | Average (`sum / i`) | Match? | `count` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Root 1 | 1 | `{0, 0}` | `{0, 0}` | 1 | 1 | 1 / 1 = 1 | Yes (1 == 1) | 1 |

Final `count` returned: **1**

---

## Time & Space Complexity

* **Time Complexity:** **O(N)**, where N is the total number of nodes in the binary tree. We perform a single post-order traversal that visits every node exactly once. Doing basic arithmetic operations at each node takes constant time O(1).
* **Space Complexity:** **O(H)**, where H is the height of the tree. This space is used by the call stack during recursion.
  * In the best case (a balanced tree), the height is **O(log N)**.
  * In the worst case (a completely skewed tree like a linked list), the height is **O(N)**.

### Is this optimal?
**Yes, this is optimal.** We must inspect every node at least once to know its value and include it in average computations, so any algorithm must take at least **O(N)** time. The space complexity **O(H)** is also minimal because recursion naturally requires stack memory proportional to the tree's depth.

---

## Edge Cases Handled

* **Single Node Tree:** Works automatically. The left and right subtrees return `{0, 0}`, so the sum is `node->val` and count is `1`. The average `val / 1` always equals `val`.
* **Leaf Nodes:** Handled smoothly without special conditional checks because `null` children safely return `{0, 0}`.
* **Nodes with Zero Values (`val = 0`):** Handled cleanly. For instance, `0 / 1` evaluates to `0`, which correctly matches `val = 0`.
* **Odd Sum Divisions:** Integer division in C++ automatically truncates toward zero (rounding down positive numbers), matching the problem requirement without needing extra floating-point logic or floor functions.
