## Visualizing `transplant()` in `Remove` Scenarios

This document provides a visual breakdown of how the `transplant()` helper function is used within the main `Remove` function for the two primary Binary Search Tree (BST) deletion cases.

---

### Case 1: Deleting a Node with One Child

This is the most straightforward use of `transplant`. We want to remove a node (`Z`) and have its single child (`X`) take its place.

**Initial State:**

Node `Z` is the node to be deleted. It has one child, `X`. `P` is the parent of `Z`.

```
      P
      |
      Z
     /
    X
   / \
  A   B
```

**Action:**

A single call to `transplant(Z, X)` is made.

*   `transplant` changes `P`'s child pointer (which was pointing to `Z`) to now point to `X`.
*   `transplant` changes `X`'s parent pointer to point to `P`.

**Final State:**

Node `Z` has been effectively bypassed and can now be safely deallocated. `X` has been promoted, taking `Z`'s place, and the subtree structure below `X` remains intact.

```
      P
      |
      X
     / \
    A   B
```

---


### Case 2: Deleting a Node with Two Children

This case is more complex and often involves two `transplant` calls. Here, we want to delete node `Z`, which has two children. We find its in-order successor, `Y` (the smallest node in `Z`'s right subtree). `Y` will replace `Z`.

**Initial State:**

`Z` is the node to be deleted. `Y` is `Z`'s successor. `Y` can have at most one child (a right child, `C`).

```
           P
           |
           Z
          / \
         L   R
            /
           ...
            |
            Q (Parent of Y)
           /
          Y
           \
            C
```

**Step 1: Move `Y`'s child up.**

First, we need to handle `Y`'s own child, `C`. We replace `Y` with `C` in `Y`'s original position.

**Action:** `transplant(Y, C)`

**Intermediate State:**

`Y` is now detached from its original position. Its child `C` has taken its place. The connection between `Z` and `R` is still intact for now.

```
           P
           |
           Z
          / \
         L   R
            /
           ...
            |
            Q
           /
          C
```

**Step 2: Replace `Z` with the successor `Y`.**

Now that `Y` is free, we can use it to replace `Z`.

**Action:** `transplant(Z, Y)`

**Final State (after transplant, before pointer fixup):**

`Y` now occupies `Z`'s original position in the tree, from the perspective of `Z`'s parent, `P`.

```
           P
           |
           Y
          / \
         ?   ?
```

**Final Cleanup (done by the `Remove` function):**

After the `transplant` calls, the main `Remove` function is responsible for the final pointer assignments to fully integrate `Y`:
1.  `Y`'s left child is set to `Z`'s original left child (`L`).
2.  `Y`'s right child is set to `Z`'s original right child (`R`).
3.  The parent pointers of `L` and `R` are updated to point to `Y`.
4.  The color of `Y` is set to the original color of `Z`.
5.  Node `Z` is deallocated.

This orchestration of `transplant` calls allows the `Remove` function to handle the complex structural changes of deletion in a more modular and readable way.