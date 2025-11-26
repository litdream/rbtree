#ifndef RBTREE_INT_HPP
#define RBTREE_INT_HPP

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;

    Node *left;
    Node *right;
    Node *parent;
};


class RBTree {
public:
    RBTree();
    ~RBTree();

    void insert(int key);
    void remove(int key);
    void printTree();
    void inOrderTraverse();

    // From here, it could be private, but for testing, making public.

    void rotateLeft(Node *&node);
    void rotateRight(Node *&node);
    void fixInsert(Node *&node);
    void fixDelete(Node *&node);

    Node *minValueNode(Node *&node);
    void transplant(Node *&root, Node *&u, Node *&v);
    void printHelper(Node *root, string indent, bool last);
    void deleteTree(Node *node);

private:
    Node *root;
};



#endif  // RBTREE_INT_HPP
