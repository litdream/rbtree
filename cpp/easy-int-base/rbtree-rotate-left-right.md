### Red-Black Tree Rotations: `rotateLeft()` and `rotateRight()`

Rotations are fundamental operations in self-balancing binary search trees like Red-Black Trees. They restructure the tree by changing pointer relationships between nodes, maintaining the binary search tree property, and are crucial for balancing the tree after insertions and deletions.

---

#### `rotateLeft(Node *x)`

A left rotation is performed on a node `x`. It makes `x`'s right child, `y`, the new root of the subtree, with `x` becoming `y`'s left child. This operation effectively moves `x` down and to the left, and `y` up and to the left.

**Visual Representation:**

```
       x                      y
      / \                    / \ 
     a   y     ======>      x   c
        / \                / \ 
       b   c              a   b
```

*   `x`: The node around which the rotation is performed.
*   `y`: `x`'s right child, which will become the new root of the subtree.
*   `a`: `x`'s left child (unchanged relative to `x`).
*   `b`: `y`'s left child, which becomes `x`'s new right child.
*   `c`: `y`'s right child (unchanged relative to `y`).

**Implementation:**

```cpp
/**
 * @brief Performs a left rotation on the given node.
 * 
 *       x                      y
 *      / \                    / \ 
 *     a   y     ======>      x   c
 *        / \                / \ 
 *       b   c              a   b
 *
 * @param x The node to rotate.
 */
void RBTree::rotateLeft(Node *x) {
    // y becomes the new root of the subtree.
    Node *y = x->right;

    // Turn y's left subtree (b) into x's right subtree.
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }

    // Link y's parent to x's parent.
    y->parent = x->parent;
    if (x->parent == nullptr) {
        // x was the root, so y is now the root.
        root = y;
    } else if (x == x->parent->left) {
        // x was a left child.
        x->parent->left = y;
    } else {
        // x was a right child.
        x->parent->right = y;
    }

    // Put x on y's left.
    y->left = x;
    x->parent = y;
}
```

---

#### `rotateRight(Node *y)`

A right rotation is the symmetrical operation to a left rotation, performed on a node `y`. It makes `y`'s left child, `x`, the new root of the subtree, with `y` becoming `x`'s right child. This operation effectively moves `y` down and to the right, and `x` up and to the right.

**Visual Representation:**

```
         y                    x
        / \                  / \ 
       x   c   ======>      a   y
      / \                      / \ 
     a   b                    b   c
```

*   `y`: The node around which the rotation is performed.
*   `x`: `y`'s left child, which will become the new root of the subtree.
*   `a`: `x`'s left child (unchanged relative to `x`).
*   `b`: `x`'s right child, which becomes `y`'s new left child.
*   `c`: `y`'s right child (unchanged relative to `y`).

**Implementation:**

```cpp
/**
 * @brief Performs a right rotation on the given node.
 *
 *         y                    x
 *        / \                  / \ 
 *       x   c   ======>      a   y
 *      / \                      / \ 
 *     a   b                    b   c
 *
 * @param y The node to rotate.
 */
void RBTree::rotateRight(Node *y) {
    // x becomes the new root of the subtree.
    Node *x = y->left;

    // Turn x's right subtree (b) into y's left subtree.
    y->left = x->right;
    if (x->right != nullptr) {
        x->right->parent = y;
    }

    // Link x's parent to y's parent.
    x->parent = y->parent;
    if (y->parent == nullptr) {
        // y was the root, so x is now the root.
        root = x;
    } else if (y == y->parent->right) {
        // y was a right child.
        y->parent->right = x;
    } else {
        // y was a left child.
        y->parent->left = x;
    }

    // Put y on x's right.
    x->right = y;
    y->parent = x;
}
```
