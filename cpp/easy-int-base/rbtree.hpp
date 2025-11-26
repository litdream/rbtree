#ifndef RBTREE_INT_HPP
#define RBTREE_INT_HPP

#include <iostream>
#include <string>

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;

    Node *left;
    Node *right;
    Node *parent;

    Node(int data, Color color = RED) : data(data), color(color), left(nullptr), right(nullptr), parent(nullptr) {}
};


#ifdef UNIT_TEST_BUILD
#include "gtest/gtest.h"
#endif

class RBTree {
public:
    RBTree();
    ~RBTree();

    Node *Insert(int key);
    // Node *Remove(int key);
    Node *Search(int key) const;
    // void  PrintTree() const;
    // void  InOrderTraverse() const;

private:

    // These are internal RBTree ops.  It could be private when deploying.
    // Easy way to test is, making these function public: scope for testing only.
    //  - Annoying to change back/forth.
    //  - Instead!   Use GoogleTest macro:  FRIEND_TEST( TestSuite, TestName )
    //

#ifdef UNIT_TEST_BUILD
FRIEND_TEST(RBTreeBasicTest, ConstructorAndDestructor);
FRIEND_TEST(RBTreeInsertTest, InsertIntoEmptyTree);
FRIEND_TEST(RBTreeInsertTest, DuplicateInsertion);
FRIEND_TEST(RBTreeInsertTest, MultipleInsertionsSimpleStructure);
#endif
    //
    // NOTE:  Make sure to define UNIT_TEST_BUILD variable in compiler.
    //   - CMake provides like this:
    //
    //   # Define the macro specifically for the test build
    //   target_compile_definitions(my_tests PUBLIC UNIT_TEST_BUILD)
    //

    void rotateLeft(Node *&node);
    void rotateRight(Node *&node);
    void fixInsert(Node *&node);
    // void fixDelete(Node *&node);

    // Node *minValueNode(Node *&node);
    // void transplant(Node *&u, Node *&v);
    // void printHelper(Node *root, std::string indent, bool last) const;
    // void deleteTree(Node *node);

private:
    Node *root;
};



#endif  // RBTREE_INT_HPP
