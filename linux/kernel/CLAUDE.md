# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This repository contains a red-black tree (rbtree) implementation adapted from the Linux kernel for userspace C programs. It's based on an original port from https://github.com/forhappy/rbtree and includes custom test code.

The codebase includes:
- Linux kernel rbtree implementation (lib/rbtree.c and include/linux/rbtree*.h)
- Userspace test program
- Work in progress C++ implementation

## Building and Testing

### Build Commands

From `linux/mine/` directory:
```bash
make              # Build the test program
make clean        # Clean build artifacts
```

The Makefile uses:
- Compiler flags: `-g -O0 -Wall`
- Produces executable: `rbtree-tst`

### Running Tests

From `linux/mine/` directory:
```bash
./rbtree-tst      # Run the rbtree test program
```

The test program:
- Inserts 32 nodes (numbered 0-31)
- Searches all nodes in order
- Deletes nodes 20, 10, and 15
- Searches remaining nodes

## Code Architecture

### Directory Structure

- `linux/kernel/` - New rbtree implementation files (currently being developed)
  - `rbtree.c` - Core rbtree implementation
  - `rbtree.h` - Main API header
  - `rbtree_types.h` - Type definitions
  - `rbtree_augmented.h` - Augmented rbtree support

- `linux/mine/` - Working userspace implementation
  - `rbtree-tst.c` - Test program demonstrating rbtree usage
  - `rbtree.c`, `rbtree.h` - Working implementation (currently linked from parent dirs)
  - `Makefile` - Build configuration
  - `rbtree.md` - Notes about the implementation (from 2014 blog post)

- `cpp/` - C++ implementation (work in progress)
  - `rbtree.hpp` - Header-only C++ version

### Core Concepts

**Red-Black Tree Properties:**
1. A node is either red or black
2. The root is black
3. All leaves (NULL) are black
4. Both children of every red node are black
5. Every simple path from root to leaves contains the same number of black nodes

These properties guarantee O(log n) operations.

**API Pattern:**

Users must implement their own insert and search functions rather than using callbacks. This is a deliberate design choice for performance in C.

Example pattern (from `rbtree-tst.c`):
1. Define a struct containing `struct rb_node` and your data
2. Implement search using `container_of()` to get from `rb_node` to your struct
3. Implement insert by finding position, calling `rb_link_node()`, then `rb_insert_color()`
4. Use `rb_erase()` to delete nodes

**Key Functions:**
- `rb_insert_color()` - Rebalance tree after insertion
- `rb_erase()` - Delete and rebalance
- `rb_link_node()` - Link a new node into position
- `rb_first()`, `rb_last()` - Get first/last node
- `rb_next()`, `rb_prev()` - Iterate through tree
- `rb_entry()` - Macro to get container struct from rb_node

**Augmented Trees:**

The implementation supports augmented rbtrees where each node maintains additional data about its subtree (like max value, interval endpoints, etc.). See `rbtree_augmented.h` for the API.

**Cached Trees:**

`rb_root_cached` maintains a pointer to the leftmost node for O(1) access to minimum element. Related functions use `_cached` suffix.

## Header Dependencies

The kernel headers have dependencies that need to be satisfied for userspace:
- `<linux/container_of.h>` - Provides `container_of` macro
- `<linux/stddef.h>` - Standard definitions
- `<linux/rcupdate.h>` - RCU synchronization primitives
- `<linux/compiler.h>` - Compiler attributes
- `<linux/export.h>` - Export symbol macros

When adapting for pure userspace, these typically need to be replaced or stubbed out.

## Notes

- The `linux/kernel/` directory contains files staged in git but represents ongoing development
- The working implementation is in `linux/mine/`
- Original source: Linux kernel lib/rbtree.c and include/linux/rbtree*.h
- Reference blog post: http://litdream.blogspot.com/2014/12/quick-example-of-red-black-tree.html
