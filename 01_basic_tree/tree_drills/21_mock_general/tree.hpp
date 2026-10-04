#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  MOCK PRACTICAL: general tree (every node has a linked list of children)
//
//  main.cpp is LOCKED. You only write functions in this file, in the
//  marked area at the bottom of the class. Don't rename anything above it.
// =====================================================================

struct Node {
    int elem;
    Node *parent;
    Node *firstChild;    // head of THIS node's list of children
    Node *lastChild;     // tail of THIS node's list of children
    Node *nextSibling;   // next node in the PARENT's list of children
    Node(int val, Node *par) : elem(val), parent(par), firstChild(nullptr), lastChild(nullptr), nextSibling(nullptr) {}
};

// A plain linked-list queue of Node pointers (same idea as the Part 0 drills).
// Given scaffolding: use it for level-order traversal. Methods: enqueue, dequeue, isEmpty.
struct QNode {
    Node *data;
    QNode *next;
    QNode(Node *d) : data(d), next(nullptr) {}
};

class NodeQueue {
private:
    QNode *head;   // front of the line
    QNode *tail;   // back of the line
public:
    NodeQueue() : head(nullptr), tail(nullptr) {}
    ~NodeQueue()
    {
        while (head != nullptr) { QNode *nxt = head->next; delete head; head = nxt; }
    }
    void enqueue(Node *p)
    {
        QNode *n = new QNode(p);
        if (head == nullptr) head = tail = n;
        else { tail->next = n; tail = n; }
    }
    Node *dequeue()            // returns nullptr if the queue is empty
    {
        if (head == nullptr) return nullptr;
        QNode *old = head;
        Node *p = old->data;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete old;
        return p;
    }
    bool isEmpty() { return head == nullptr; }
};

class GeneralTree {
private:
    Node *root;
    int size;

public:
    GeneralTree() : root(nullptr), size(0) {}

    ~GeneralTree()
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
    //   addChild(p, val)    add val as the LAST child of p; return the new node.
    //   search(val)         first node holding val in PREORDER, or nullptr.
    //   printPreorder()     "1 -> 2 -> 5 -> NULL"   (empty tree: "NULL")
    //   printPostorder()    same format
    //   printLevelOrder()   same format (use NodeQueue above)
    //   printIndented()     one node per line, 2 spaces per level of depth (empty tree: nothing)
    //   height()            edges on the longest root-to-leaf path. Empty tree: -1
    //   depth(p)            edges from the root to p. The root is 0.
    //   countNodes()        number of nodes.   countLeaves()   number of leaves
    //   sumAll()            sum of all elements (empty tree: 0)
    //   maxVal()            biggest element (empty tree: -1)
    //   printPathFromRoot(val)   prints "1 -> 2 -> 6 -> NULL" and returns true;
    //                            if val is not in the tree: return false and print nothing.
    //   removeLeaf(p)       remove p if it is a leaf and return its elem;
    //                       if p has children: return -1 and change nothing.
    //   clear()             delete every node, leave an empty tree.
    //   getSize()           number of nodes.
    // =================================================================

    Node *addRoot(int val)
    {
        // TODO
        return nullptr;
    }

    Node *addChild(Node *p, int val)
    {
        // TODO
        return nullptr;
    }

    Node *search(int val)
    {
        // TODO
        return nullptr;
    }

    void printPreorder()
    {
        // TODO
    }

    void printPostorder()
    {
        // TODO
    }

    void printLevelOrder()
    {
        // TODO
    }

    void printIndented()
    {
        // TODO
    }

    int height()
    {
        // TODO
        return 0;
    }

    int depth(Node *p)
    {
        // TODO
        return 0;
    }

    int countNodes()
    {
        // TODO
        return 0;
    }

    int countLeaves()
    {
        // TODO
        return 0;
    }

    int sumAll()
    {
        // TODO
        return 0;
    }

    int maxVal()
    {
        // TODO
        return 0;
    }

    bool printPathFromRoot(int val)
    {
        // TODO
        return false;
    }

    int removeLeaf(Node *p)
    {
        // TODO
        return -1;
    }

    void clear()
    {
        // TODO
    }

    int getSize()
    {
        // TODO
        return 0;
    }
};

#endif
