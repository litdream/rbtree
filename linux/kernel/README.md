# Userspace Red-Black Tree Library

This is a userspace adaptation of the Linux kernel's red-black tree implementation, converted from the latest kernel sources for use in userspace C programs.

## Features

- Complete red-black tree implementation from Linux kernel
- Augmented rbtree support for interval trees and custom augmentation
- Cached rbtrees with O(1) access to leftmost node
- Helper functions for common operations (rb_add, rb_find, etc.)
- Zero dependencies on kernel headers
- Clean, standalone implementation

## Files

- `rbtree_types.h` - Core data structure definitions
- `rbtree.h` - Main API header with function declarations and inline helpers
- `rbtree_augmented.h` - Augmented rbtree API for advanced use cases
- `rbtree.c` - Implementation of core rbtree algorithms
- `rbtree-tst.c` - Example test program
- `Makefile` - Build configuration

## Building

```bash
make              # Build library and test program
make clean        # Clean build artifacts
```

This produces:
- `librbtree.a` - Static library
- `rbtree-tst` - Test program

## Usage

### Basic Example

```c
#include "rbtree.h"
#include <stddef.h>

struct mynode {
    struct rb_node node;
    int key;
    /* your data fields */
};

struct rb_root mytree = RB_ROOT;

/* Search function */
struct mynode *my_search(struct rb_root *root, int key)
{
    struct rb_node *node = root->rb_node;

    while (node) {
        struct mynode *data = container_of(node, struct mynode, node);

        if (key < data->key)
            node = node->rb_left;
        else if (key > data->key)
            node = node->rb_right;
        else
            return data;
    }
    return NULL;
}

/* Insert function */
int my_insert(struct rb_root *root, struct mynode *data)
{
    struct rb_node **new = &(root->rb_node), *parent = NULL;

    /* Figure out where to put new node */
    while (*new) {
        struct mynode *this = container_of(*new, struct mynode, node);

        parent = *new;
        if (data->key < this->key)
            new = &((*new)->rb_left);
        else if (data->key > this->key)
            new = &((*new)->rb_right);
        else
            return 0;  /* duplicate */
    }

    /* Add new node and rebalance tree */
    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);

    return 1;
}

/* Delete function */
void my_delete(struct rb_root *root, int key)
{
    struct mynode *data = my_search(root, key);
    if (data) {
        rb_erase(&data->node, root);
        free(data);
    }
}

/* Iterate over all nodes in order */
void my_iterate(struct rb_root *root)
{
    struct rb_node *node;
    for (node = rb_first(root); node; node = rb_next(node)) {
        struct mynode *data = rb_entry(node, struct mynode, node);
        printf("key = %d\n", data->key);
    }
}
```

## API Overview

### Core Functions

- `rb_insert_color()` - Rebalance tree after insertion
- `rb_erase()` - Remove node and rebalance tree
- `rb_link_node()` - Link a new node into position (before rb_insert_color)
- `rb_replace_node()` - Replace a node without rebalancing

### Traversal Functions

- `rb_first()` - Get first (leftmost) node
- `rb_last()` - Get last (rightmost) node
- `rb_next()` - Get next node in order
- `rb_prev()` - Get previous node in order
- `rb_first_postorder()` - Get first node in postorder traversal
- `rb_next_postorder()` - Get next node in postorder traversal

### Helper Functions (inline)

- `rb_add()` - Insert using comparison callback
- `rb_find()` - Search using comparison callback
- `rb_find_add()` - Find or add using comparison callback
- `rb_find_first()` - Find leftmost matching node
- `rb_next_match()` - Find next matching node

### Cached Tree Functions

- `rb_insert_color_cached()` - Insert with leftmost caching
- `rb_erase_cached()` - Erase with leftmost caching
- `rb_first_cached()` - O(1) access to leftmost node

### Macros

- `RB_ROOT` - Initialize empty tree
- `RB_ROOT_CACHED` - Initialize empty cached tree
- `rb_entry(ptr, type, member)` - Get container struct from rb_node
- `rb_entry_safe()` - Safe version that handles NULL
- `container_of(ptr, type, member)` - Generic container_of macro

## Design Philosophy

The Linux kernel rbtree API intentionally does not use callbacks for insert/search to maximize performance. Users must implement their own insert and search functions that know about their specific data structures. This avoids function call overhead and allows the compiler to optimize better.

## Red-Black Tree Properties

1. A node is either red or black
2. The root is black
3. All leaves (NULL) are black
4. Both children of every red node are black
5. Every path from root to leaves contains the same number of black nodes

These properties guarantee O(log n) insertion, deletion, and search operations.

## Changes from Linux Kernel

- Removed RCU (Read-Copy-Update) functions (not needed for userspace)
- Removed kernel header dependencies
- Added userspace replacements for kernel macros (WRITE_ONCE, READ_ONCE, etc.)
- Removed EXPORT_SYMBOL macros
- Standalone header files with no external dependencies

## License

GPL-2.0-or-later (inherited from Linux kernel)

## Credits

- Original authors: Andrea Arcangeli, David Woodhouse, Michel Lespinasse
- Adapted for userspace from Linux kernel sources
