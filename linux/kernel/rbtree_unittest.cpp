/*
 * rbtree_unittest.cpp - GoogleTest unit tests for rbtree implementation
 * Adapted from Linux kernel rbtree for userspace testing
 */

#include <gtest/gtest.h>
#include <cstring>
#include <cstdlib>

// Use C++ header
#include "rbtree.hpp"

// Test node structure
struct test_node {
    struct rb_node rb;
    int key;
};

// Helper function to insert a node
static int test_insert(struct rb_root *root, struct test_node *data)
{
    struct rb_node **new_node = &(root->rb_node);
    struct rb_node *parent = NULL;

    // Figure out where to put new node
    while (*new_node) {
        struct test_node *this_node = container_of(*new_node, struct test_node, rb);
        int result = data->key - this_node->key;

        parent = *new_node;
        if (result < 0)
            new_node = &((*new_node)->rb_left);
        else if (result > 0)
            new_node = &((*new_node)->rb_right);
        else
            return 0; // Duplicate
    }

    // Add new node and rebalance tree
    rb_link_node(&data->rb, parent, new_node);
    rb_insert_color(&data->rb, root);

    return 1;
}

// Helper function to search for a node
static struct test_node *test_search(struct rb_root *root, int key)
{
    struct rb_node *node = root->rb_node;

    while (node) {
        struct test_node *data = container_of(node, struct test_node, rb);

        if (key < data->key)
            node = node->rb_left;
        else if (key > data->key)
            node = node->rb_right;
        else
            return data;
    }
    return NULL;
}

// Helper function to count nodes in tree
static int count_nodes(struct rb_root *root)
{
    int count = 0;
    struct rb_node *node;

    for (node = rb_first(root); node; node = rb_next(node))
        count++;

    return count;
}

// Test fixture for rbtree tests
class RBTreeTest : public ::testing::Test {
protected:
    struct rb_root tree;

    void SetUp() override {
        tree = RB_ROOT;
    }

    void TearDown() override {
        // For stack-allocated nodes, just reset the tree
        // No need to explicitly erase since nodes will be cleaned up by stack unwinding
        tree = RB_ROOT;
    }
};

// Basic insertion test
TEST_F(RBTreeTest, InsertSingleNode) {
    struct test_node node;
    node.key = 10;

    EXPECT_EQ(test_insert(&tree, &node), 1);
    EXPECT_FALSE(RB_EMPTY_ROOT(&tree));
}

// Test multiple insertions
TEST_F(RBTreeTest, InsertMultipleNodes) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        EXPECT_EQ(test_insert(&tree, &nodes[i]), 1);
    }

    EXPECT_EQ(count_nodes(&tree), NUM_NODES);
}

// Test duplicate insertion
TEST_F(RBTreeTest, InsertDuplicate) {
    struct test_node node1, node2;
    node1.key = 10;
    node2.key = 10;

    EXPECT_EQ(test_insert(&tree, &node1), 1);
    EXPECT_EQ(test_insert(&tree, &node2), 0); // Should fail
}

// Test search functionality
TEST_F(RBTreeTest, SearchExistingNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i * 2; // Even numbers
        test_insert(&tree, &nodes[i]);
    }

    // Search for existing nodes
    for (int i = 0; i < NUM_NODES; i++) {
        struct test_node *found = test_search(&tree, i * 2);
        ASSERT_NE(found, nullptr);
        EXPECT_EQ(found->key, i * 2);
    }
}

// Test search for non-existing node
TEST_F(RBTreeTest, SearchNonExistingNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i * 2; // Even numbers
        test_insert(&tree, &nodes[i]);
    }

    // Search for odd numbers (which don't exist)
    for (int i = 0; i < NUM_NODES; i++) {
        struct test_node *found = test_search(&tree, i * 2 + 1);
        EXPECT_EQ(found, nullptr);
    }
}

// Test deletion
TEST_F(RBTreeTest, DeleteNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    // Delete node with key 5
    struct test_node *to_delete = test_search(&tree, 5);
    ASSERT_NE(to_delete, nullptr);
    rb_erase(&to_delete->rb, &tree);

    // Verify it's gone
    EXPECT_EQ(test_search(&tree, 5), nullptr);
    EXPECT_EQ(count_nodes(&tree), NUM_NODES - 1);
}

// Test deletion of multiple nodes
TEST_F(RBTreeTest, DeleteMultipleNodes) {
    const int NUM_NODES = 32;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    // Delete nodes 20, 10, and 15 (like the original test)
    int to_delete[] = {20, 10, 15};
    for (int key : to_delete) {
        struct test_node *node = test_search(&tree, key);
        ASSERT_NE(node, nullptr);
        rb_erase(&node->rb, &tree);
    }

    // Verify they're gone
    for (int key : to_delete) {
        EXPECT_EQ(test_search(&tree, key), nullptr);
    }

    EXPECT_EQ(count_nodes(&tree), NUM_NODES - 3);
}

// Test rb_first
TEST_F(RBTreeTest, GetFirstNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    // Insert in reverse order
    for (int i = NUM_NODES - 1; i >= 0; i--) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    struct rb_node *first = rb_first(&tree);
    ASSERT_NE(first, nullptr);

    struct test_node *first_node = container_of(first, struct test_node, rb);
    EXPECT_EQ(first_node->key, 0);
}

// Test rb_last
TEST_F(RBTreeTest, GetLastNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    struct rb_node *last = rb_last(&tree);
    ASSERT_NE(last, nullptr);

    struct test_node *last_node = container_of(last, struct test_node, rb);
    EXPECT_EQ(last_node->key, NUM_NODES - 1);
}

// Test iteration with rb_next
TEST_F(RBTreeTest, IterateForward) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    // Insert in random order
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = keys[i];
        test_insert(&tree, &nodes[i]);
    }

    // Iterate and verify sorted order
    int expected_key = 0;
    struct rb_node *node;
    for (node = rb_first(&tree); node; node = rb_next(node)) {
        struct test_node *data = container_of(node, struct test_node, rb);
        EXPECT_EQ(data->key, expected_key);
        expected_key++;
    }

    EXPECT_EQ(expected_key, NUM_NODES);
}

// Test iteration with rb_prev
TEST_F(RBTreeTest, IterateBackward) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    // Iterate backwards and verify reverse sorted order
    int expected_key = NUM_NODES - 1;
    struct rb_node *node;
    for (node = rb_last(&tree); node; node = rb_prev(node)) {
        struct test_node *data = container_of(node, struct test_node, rb);
        EXPECT_EQ(data->key, expected_key);
        expected_key--;
    }

    EXPECT_EQ(expected_key, -1);
}

// Test empty tree operations
TEST_F(RBTreeTest, EmptyTreeOperations) {
    EXPECT_TRUE(RB_EMPTY_ROOT(&tree));
    EXPECT_EQ(rb_first(&tree), nullptr);
    EXPECT_EQ(rb_last(&tree), nullptr);
    EXPECT_EQ(count_nodes(&tree), 0);
}

// Test large number of insertions
TEST_F(RBTreeTest, LargeNumberOfInsertions) {
    const int NUM_NODES = 1000;
    struct test_node *nodes = new struct test_node[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        EXPECT_EQ(test_insert(&tree, &nodes[i]), 1);
    }

    EXPECT_EQ(count_nodes(&tree), NUM_NODES);

    // Verify all nodes can be found
    for (int i = 0; i < NUM_NODES; i++) {
        struct test_node *found = test_search(&tree, i);
        ASSERT_NE(found, nullptr);
        EXPECT_EQ(found->key, i);
    }

    delete[] nodes;
}

// Test rb_replace_node
TEST_F(RBTreeTest, ReplaceNode) {
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        test_insert(&tree, &nodes[i]);
    }

    // Find node with key 5 and replace it
    struct test_node *old_node = test_search(&tree, 5);
    ASSERT_NE(old_node, nullptr);

    struct test_node *new_node = new struct test_node;
    new_node->key = 5;

    rb_replace_node(&old_node->rb, &new_node->rb, &tree);

    // Verify the new node is in the tree
    struct test_node *found = test_search(&tree, 5);
    EXPECT_EQ(found, new_node);
    EXPECT_NE(found, old_node);

    delete new_node;
}

// Test with cached root
TEST_F(RBTreeTest, CachedRootOperations) {
    struct rb_root_cached cached_tree = RB_ROOT_CACHED;
    const int NUM_NODES = 10;
    struct test_node nodes[NUM_NODES];

    // Insert nodes using cached root
    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].key = i;
        struct rb_node **new_node = &(cached_tree.rb_root.rb_node);
        struct rb_node *parent = NULL;
        bool leftmost = true;

        while (*new_node) {
            struct test_node *this_node = container_of(*new_node, struct test_node, rb);
            parent = *new_node;

            if (nodes[i].key < this_node->key) {
                new_node = &((*new_node)->rb_left);
            } else {
                new_node = &((*new_node)->rb_right);
                leftmost = false;
            }
        }

        rb_link_node(&nodes[i].rb, parent, new_node);
        rb_insert_color_cached(&nodes[i].rb, &cached_tree, leftmost);
    }

    // Verify leftmost is node with key 0
    ASSERT_NE(cached_tree.rb_leftmost, nullptr);
    struct test_node *leftmost_node = container_of(cached_tree.rb_leftmost,
                                                    struct test_node, rb);
    EXPECT_EQ(leftmost_node->key, 0);

    // Verify rb_first_cached is O(1)
    EXPECT_EQ(rb_first_cached(&cached_tree), cached_tree.rb_leftmost);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
