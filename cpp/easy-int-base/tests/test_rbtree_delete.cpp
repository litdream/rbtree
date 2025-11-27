#include "gtest/gtest.h"
#include "rbtree.hpp"

class RBTreeDeleteTest : public ::testing::Test {
protected:
    RBTree tree;
};

TEST_F(RBTreeDeleteTest, DeleteRedLeafNode) {
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3); // Red leaf

    tree.Remove(3);

    ASSERT_EQ(tree.Search(3), nullptr);
    // Further checks can be added to verify tree structure and properties
}


TEST_F(RBTreeDeleteTest, DeleteBlackNodeWithRedChild) {
    tree.Insert(10); // Black
    tree.Insert(5);  // Red -> Black
    tree.Insert(15); // Red -> Black
    tree.Insert(3);  // Red

    // Tree state should be:
    //      10(B)
    //     /   \
    //    5(B)  15(B)
    //   /
    //  3(R)

    tree.Remove(5);

    ASSERT_EQ(tree.Search(5), nullptr);
    Node* node3 = tree.Search(3);
    ASSERT_NE(node3, nullptr);
    ASSERT_EQ(node3->color, BLACK); // 3 should be promoted and become black
}

