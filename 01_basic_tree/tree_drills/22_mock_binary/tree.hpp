#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  MOCK PRACTICAL: binary tree
//
//  main.cpp is LOCKED. You only write functions in this file, in the
//  marked area at the bottom of the class. Don't rename anything above it.
// =====================================================================

struct Node {
    int elem;
    Node *parent;
    Node *left;
    Node *right;
    Node(int val, Node *par) : elem(val), parent(par), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node *root;
    int size;

public:
    BinaryTree() : root(nullptr), size(0) {}

    ~BinaryTree()
    {
        clear();
    }


    // =================================================================
    //  WRITE YOUR FUNCTIONS HERE. You may add private helper functions too.
    //
    //  RULES (main.cpp does all the "not found" / error printing, so in the
    //  error cases your functions print NOTHING):
    //
    //   addRoot(val)        make the root. If the tree already has a root: return nullptr.
    //   addLeft(p, val)     new LEFT child of p; return it. If p already has a left child: nullptr.
    //   addRight(p, val)    same on the right.
    //   search(val)         first node holding val in PREORDER, or nullptr.
    //   sibling(p)          the other child of p's parent, or nullptr (root / no sibling).
    //   set(p, val)         replace p's element, return the OLD element.
    //   remove(p)           p has at most ONE child: that child takes p's place. Return p's element.
    //                       If p has two children: return -1 and change nothing.
    //   attach(p, t1, t2)   p must be a leaf: t1's tree becomes p's left subtree, t2's the right subtree,
    //                       then t1 and t2 are empty trees. Return true. If p is not a leaf: return false.
    //   printPreorder() / printInorder() / printPostorder()
    //                       "1 -> 2 -> 4 -> NULL"   (empty tree: "NULL")
    //   height()            edges on the longest path. Empty tree: -1
    //   getSize()           number of nodes
    //   clear()             delete every node, leave an empty tree
    // =================================================================

    Node *addRoot(int val)
    {
        // TODO
        return nullptr;
    }

    Node *addLeft(Node *p, int val)
    {
        // TODO
        return nullptr;
    }

    Node *addRight(Node *p, int val)
    {
        // TODO
        return nullptr;
    }

    Node *search(int val)
    {
        // TODO
        return nullptr;
    }

    Node *sibling(Node *p)
    {
        // TODO
        return nullptr;
    }

    int set(Node *p, int val)
    {
        // TODO
        return -1;
    }

    int remove(Node *p)
    {
        // TODO
        return -1;
    }

    bool attach(Node *p, BinaryTree &t1, BinaryTree &t2)
    {
        // TODO
        return false;
    }

    void printPreorder()
    {
        // TODO
    }

    void printInorder()
    {
        // TODO
    }

    void printPostorder()
    {
        // TODO
    }

    int height()
    {
        // TODO
        return 0;
    }

    int getSize()
    {
        // TODO
        return 0;
    }

    void clear()
    {
        // TODO
    }
};

#endif
