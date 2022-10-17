#ifndef __RBTREE_HPP
#define __RBTREE_HPP


#include <rbtree.h>

#define MAX_IDLEN  100

template <typename T>
struct RBNode
{
    struct rb_node node;
    char id[MAX_IDLEN + 1];
    T *data;
};

template <typename T>
class RBTree
{
public:
    RBTree() : tree{ RB_ROOT }
    {}
    
    
private:
    struct rb_root  tree;
};


#endif  /* __RBTREE_HPP */
