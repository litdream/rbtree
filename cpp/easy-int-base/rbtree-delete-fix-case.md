### Red-Black Tree `fixDelete()` Cases Explained

The `fixDelete` method is called to restore the Red-Black Tree properties when a `BLACK` node is removed. Removing a black node disrupts **Property 5 (the black-height property)**, as paths that went through the removed node now have one fewer black node.

#### What is `node(2B)`? (Conceptual "Doubly-Black")

In the context of the `fixDelete` algorithm, `node(2B)` refers to a node that is **conceptually "doubly-black."** This means it carries an "extra black" property in addition to its actual color (which would be `BLACK`).

**Why it arises**:
When a `BLACK` node is removed from the Red-Black Tree, it violates the black-height property (Property 5). To track this imbalance, the node that replaces the removed black node (our `node` parameter in `fixDelete`) is marked as `2B` if it's already `BLACK`.

**Not a Real Color**:
It's vital to remember that `2B` is **not a real color** that can be assigned to a node. The `Color` enum only has `RED` and `BLACK`. `Doubly-black` is an algorithmic abstraction used *during the fixDelete process* to indicate that this particular node needs to contribute an additional black count to the paths passing through it.

**The Goal of `fixDelete`**:
The purpose of the `fixDelete` algorithm is to resolve this "doubly-black" property. It does this by performing rotations and recolorings to:
1.  **Push the `2B` property up the tree** towards the root.
2.  **Absorb the "extra black"** by recoloring a `RED` node to `BLACK`, or by restructuring the tree.
3.  If the `2B` property reaches the `root`, it can simply be removed, as the root's color doesn't affect black-height comparisons between different paths.

So, `node(2B)` is a temporary marker to indicate an imbalance that `fixDelete` is actively trying to resolve.

#### The Core Problem & The "Extra Black" Concept

To fix this, we introduce the concept of an **"extra black"**. The node (`node`) that replaces the deleted black node is considered to hold this "extra black". If `node` was `RED`, it simply absorbs the extra black and becomes `BLACK` (problem solved). If `node` was `BLACK`, it becomes "doubly-black".

The goal of the `fixDelete` loop is to move this "extra black" up the tree until:
1. It reaches a red node, which can be turned black.
2. It reaches the root, where it can simply be removed.
3. The tree can be restructured through rotations and recoloring to eliminate it.

The strategy for fixing the violation depends on the color of `node`'s **sibling** (`S`). The following explanation assumes `node` is a **left child**. The cases for a right child are symmetrical.

---

### Case 1: The Sibling (`S`) is `RED`

**Situation:**
*   `node` is "doubly-black".
*   Its sibling `S` is `RED`.
*   The parent `P` must be `BLACK` (otherwise, a red-red violation would have existed before deletion).

```
      P(B)
     /    \
 node(2B)  S(R)   <-- Sibling is RED
         /   \
       SL(B) SR(B)
```

**Logic:**
This case is about transforming the structure to one where the sibling is `BLACK`. We can't directly address the "extra black" on `node` while its sibling is `RED`. We perform a rotation and recoloring to demote the sibling and promote the parent.

**Action:**
1.  Set `S` to `BLACK`.
2.  Set `P` to `RED`.
3.  Perform a `rotateLeft` on `P`.

**Result:**
`node`'s new sibling is now `SL` (one of the old sibling's children), which is guaranteed to be `BLACK`. The "extra black" on `node` remains, but the problem has been converted into one of the "black sibling" cases (2, 3, or 4), which can be solved.

```
        S(B)
       /    \
      P(R)   SR(B)
     /  \
 node(2B) SL(B)  <-- New sibling is BLACK
```

---

### Case 2: Sibling (`S`) is `BLACK`, and Both its Children are `BLACK`

**Situation:**
*   `node` is "doubly-black".
*   `S` is `BLACK`.
*   `S`'s children (`SL`, `SR`) are both `BLACK` (or `nullptr`).

```
      P(B or R)
     /    \
 node(2B)  S(B)
         /   \
       SL(B) SR(B)
```

**Logic:**
There are no red nodes in the sibling's subtree to help us. We cannot "borrow" blackness. Instead, we balance the subtree by removing one black node from the sibling's path. This is done by coloring the sibling `RED`.

**Action:**
1.  Set `S` to `RED`.

**Result:**
The black-height of `node`'s path is now equal to the black-height of the sibling's path within the subtree rooted at `P`. However, we have effectively transferred the "extra black" problem from `node` up to its parent `P`. The `fixDelete` loop continues with `P` as the new `node`.

```
      P(B+1B or R+1B -> B)
     /    \
 node(B)   S(R)
         /   \
       SL(B) SR(B)
```

---

### Case 3: Sibling (`S`) is `BLACK`, Near Child (`SL`) is `RED`, Far Child (`SR`) is `BLACK` (Triangle Case)

**Situation:**
*   `node` is "doubly-black".
*   `S` is `BLACK`.
*   `S`'s left child (`SL`) is `RED`.
*   `S`'s right child (`SR`) is `BLACK`.

```
      P(B or R)
     /    \
 node(2B)  S(B)
         /   \
       SL(R) SR(B)
```

**Logic:**
This is an awkward "triangle" or "kink" configuration. A direct rotation on `P` won't solve the issue. We must first transform this into the "line" configuration of Case 4.

**Action:**
1.  Set `SL` to `BLACK`.
2.  Set `S` to `RED`.
3.  Perform a `rotateRight` on `S`.

**Result:**
The "extra black" is still on `node`, but the tree structure is now in the state of Case 4, with `node`'s new sibling (`SL`) being `BLACK` and having a `RED` far child (`S`).

```
      P(B or R)
     /    \
 node(2B)  SL(B)
             \
              S(R)
               \
                SR(B)
```

---

### Case 4: Sibling (`S`) is `BLACK`, and Far Child (`SR`) is `RED` (Line Case)

**Situation:**
*   `node` is "doubly-black".
*   `S` is `BLACK`.
*   `S`'s far child (`SR`) is `RED`.

```
      P(B or R)
     /    \
 node(2B)  S(B)
             \
              SR(R)
```

**Logic:**
This is the terminal case. We have a "spare" `RED` node (`SR`) that we can use to restructure and recolor the subtree to solve the "extra black" problem permanently.

**Action:**
1.  Set `S`'s color to `P`'s color.
2.  Set `P` to `BLACK`.
3.  Set `SR` to `BLACK`.
4.  Perform a `rotateLeft` on `P`.

**Result:**
The "extra black" on `node` is absorbed into the new structure. The path through `node` has gained a black node (`S` or `P`), restoring the black-height property for the entire tree. The `fixDelete` loop terminates because the problem is solved.

```
      S(P's old color)
     /      \
    P(B)     SR(B)
   /
node(B)
```