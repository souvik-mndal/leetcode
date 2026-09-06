![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-77.1%20MB%20(beats%2075.80%25)-green?style=for-the-badge)

---

## Problem Explained

The problem asks us to add two numbers that are given as **linked lists** and return the sum as a new linked list. 

A **linked list** is a chain of connected nodes where each node holds a value and points to the next node in line. In this problem:
* Each node holds a single digit (0 through 9).
* The digits are stored in **reverse order**. This means the ones place comes first (at the head of the list), followed by the tens place, hundreds place, and so on.

For example, the number **342** is stored as `2 -> 4 -> 3`.
If we want to add **342** and **465** (`5 -> 6 -> 4`), we perform standard addition:
`342 + 465 = 807`.

The resulting list must also be in reverse order, so **807** becomes `7 -> 0 -> 8`.

---

## Intuition

Think about how you add numbers by hand on paper:
1. You line up the numbers from right to left (starting at the ones place).
2. You add the two digits in that column.
3. If the sum is 10 or greater, you keep the ones digit for that column and **carry over** the tens digit to the next column on the left.
4. You repeat this process column by column.

Because the linked lists store digits in reverse order, the head of each list is already the ones place! This convenient setup means we can walk through both lists from left to right, adding corresponding digits and tracking any **carry** along the way.

---

## Approach

Here is step-by-step how the code executes this logic:

* `ListNode* dummy=new ListNode();` — Creates a blank "dummy" node. This serves as a fixed starting anchor so we can easily attach new nodes to our answer list without needing special code for the very first digit.
* `ListNode* temp=dummy;` — Creates a tracking pointer named `temp` that points to `dummy`. We will use `temp` to keep track of the end of our growing result list.
* `int carry=0;` — Initializes a integer variable named `carry` to 0. This holds any extra value carried over when two digits sum to 10 or more.
* `while(l1!=NULL || l2!=NULL || carry)` — Runs a loop that continues as long as there is at least one digit left to process in `l1`, at least one digit left in `l2`, or a remaining `carry` value that needs its own node.
* `int sum=0;` — Resets a variable named `sum` to 0 at the beginning of each loop iteration to calculate the total for the current position.
* `if(l1!=NULL){ sum+=l1->val; l1=l1->next; }` — Checks if `l1` has a node available. If it does, adds its value to `sum` and moves `l1` forward to the next node.
* `if(l2!=NULL){ sum+=l2->val; l2=l2->next; }` — Checks if `l2` has a node available. If it does, adds its value to `sum` and moves `l2` forward to the next node.
* `sum+=carry;` — Adds any carried-over value from the previous column's addition to `sum`.
* `carry=sum/10;` — Calculates the new carry value for the next position using integer division. For example, if `sum` is 13, `13 / 10` sets `carry` to 1.
* `ListNode* newnode=new ListNode(sum%10);` — Creates a new list node containing the single-digit remainder of `sum`. For example, `13 % 10` leaves 3 for the current node.
* `temp->next=newnode;` — Connects `newnode` to the end of our answer list.
* `temp=temp->next;` — Advances `temp` forward so it points to `newnode`, ready for the next iteration.
* `return dummy->next;` — Returns the node right after `dummy`. This skips the temporary anchor node and gives us the true head of our final answer list.

---

## Dry Run

### Case 1: Standard Addition (`l1 = [2, 4, 3]`, `l2 = [5, 6, 4]`)

This represents adding `342 + 465 = 807`.

| Step | `l1->val` | `l2->val` | `carry` (in) | `sum` | `carry` (out) | Node Created | Action / State |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Start** | 2 | 5 | 0 | - | - | - | `dummy` created, `temp` points to `dummy` |
| **1** | 2 | 5 | 0 | 7 | 0 | 7 | `sum = 2 + 5 + 0 = 7`. Attach 7 to list. Advance `l1`, `l2`, `temp`. |
| **2** | 4 | 6 | 0 | 10 | 1 | 0 | `sum = 4 + 6 + 0 = 10`. Attach 0 to list. Carry becomes 1. Advance pointers. |
| **3** | 3 | 4 | 1 | 8 | 0 | 8 | `sum = 3 + 4 + 1 = 8`. Attach 8 to list. Carry becomes 0. Advance pointers. |
| **End** | NULL | NULL | 0 | - | - | - | Loop terminates (`l1`, `l2` NULL and `carry` is 0). Return `dummy->next` (`7 -> 0 -> 8`). |

---

### Case 2: Unequal Lengths with Final Carry (`l1 = [9, 9]`, `l2 = [1]`)

This represents adding `99 + 1 = 100`.

| Step | `l1->val` | `l2->val` | `carry` (in) | `sum` | `carry` (out) | Node Created | Action / State |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Start** | 9 | 1 | 0 | - | - | - | `dummy` created, `temp` points to `dummy` |
| **1** | 9 | 1 | 0 | 10 | 1 | 0 | `sum = 9 + 1 + 0 = 10`. Attach 0. Carry becomes 1. Advance `l1`, `l2`, `temp`. |
| **2** | 9 | NULL | 1 | 10 | 1 | 0 | `sum = 9 + 1 = 10`. Attach 0. Carry becomes 1. Advance `l1` (now NULL), `temp`. |
| **3** | NULL | NULL | 1 | 1 | 0 | 1 | `l1` and `l2` are NULL, but `carry` is 1. `sum = 0 + 0 + 1 = 1`. Attach 1. Carry becomes 0. |
| **End** | NULL | NULL | 0 | - | - | - | Loop terminates. Return `dummy->next` (`0 -> 0 -> 1`). |

---

## Time & Space Complexity

* **Time Complexity:** **O(max(N, M))**, where **N** is the number of nodes in `l1` and **M** is the number of nodes in `l2`. The `while` loop runs once for every digit in the longer list, plus at most one extra loop iteration if there is a leftover carry.
* **Space Complexity:** **O(max(N, M))**. We construct a brand new linked list to store the output. The length of this new list is at most `max(N, M) + 1` nodes.

### Is this optimal?

**Yes, this is already optimal.**
* **Time:** You must inspect every single digit of both lists at least once to compute an accurate sum, making **O(max(N, M))** the theoretical minimum time possible.
* **Space:** Returning a brand-new linked list containing the result requires **O(max(N, M))** space for the output nodes. If you are strictly allowed to modify one of the input lists in-place rather than allocating a new list, you could achieve **O(1)** auxiliary extra space, but creating a fresh result list is standard practice and keeps the original data intact.

---

## Edge Cases Handled

* **Lists of different lengths:** The conditions `if(l1!=NULL)` and `if(l2!=NULL)` ensure that when the shorter list runs out of nodes, the code safely treats missing digits as 0 and continues processing the longer list.
* **Extra carry at the very end:** The loop condition `|| carry` guarantees that if adding the final digits creates a carry (like `99 + 1 = 100`), an extra node is created at the end for that leftover carry.
* **Single-element lists with zeros:** Works correctly for inputs like `l1 = [0]` and `l2 = [0]`. The sum is 0, carry stays 0, and a single output node `[0]` is returned.
* **Chained carries across multiple digits:** Works seamlessly for inputs with multiple consecutive carries (e.g., `9999 + 1 = 10000`).
