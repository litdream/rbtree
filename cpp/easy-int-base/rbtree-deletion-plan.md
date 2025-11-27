## RB-Tree Deletion: BST Deletion + fixDelete

RB-Tree deletion is fundamentally **Binary Search Tree (BST) deletion followed by a "fixup" phase**, analogous to how insertion uses `fixInsert`.

### 1. Standard BST Deletion

First, a standard BST deletion is performed. This involves three main cases for the node to be deleted (`z`):

1.  **`z` has no children (is a leaf):** The node is simply removed.
2.  **`z` has one child:** `z` is replaced by its only child.
3.  **`z` has two children:**
    *   Find `z`'s in-order successor (`y`), which is the smallest node in its right subtree.
    *   Copy the *data* from `y` into `z`.
    *   The problem transforms into deleting `y` from its original position. `y` will have at most one child (its right child), so its deletion falls into case 1 or 2.

The node *physically removed* from the tree structure will always have 0 or 1 child.

### 2. The RB-Tree Complication: Color

The color of the physically removed node determines if a fixup is needed:

*   **If the removed node was RED:** No RB-Tree properties are violated. The black-height of all paths remains the same, and no red-red violation is created. Deletion is complete.

*   **If the removed node was BLACK:** This violates the "black-height" property (Property 5), as some paths now have one fewer black node. The `fixDelete` function must resolve this.

### 3. The `fixDelete` Logic

The `fixDelete` function restores the black-height property by treating the node (`x`) that took the place of the removed black node as having an "extra black" color. This "extra black" is pushed up the tree until it's resolved. The logic consists of several cases based on the color of `x`'s **sibling**:

*   **Case 1: Sibling is RED.** Perform a rotation and recoloring. This transforms the situation into one of the "sibling is BLACK" cases.
*   **Case 2: Sibling is BLACK, and both its children are BLACK.** Recolor the sibling to RED. This pushes the "extra black" problem up to the parent, and the loop continues.
*   **Case 3: Sibling is BLACK, its "near" child is RED, and its "far" child is BLACK.** A rotation and recoloring transform this into Case 4.
*   **Case 4: Sibling is BLACK, and its "far" child is RED.** This is the terminal case. A final rotation and recoloring resolve the "extra black" imbalance, and the `fixDelete` loop terminates.

The principle is: perform the simple BST operation, then apply targeted rotations and recolorings to restore RB-Tree properties.
