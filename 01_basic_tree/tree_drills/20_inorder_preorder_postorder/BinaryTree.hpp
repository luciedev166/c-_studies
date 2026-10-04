#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: printPreorder, printInorder, printPostorder
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function(s) you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names root / size / elem / parent / left / right: the helpers use them.)
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

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: printPreorder, printInorder, printPostorder
    // ---------------------------------------------------------------

private:
    void preorderHelper(Node *p)
    {
        if (p == nullptr) return;
        cout << p->elem << " -> ";               // node, LEFT, right
        preorderHelper(p->left);
        preorderHelper(p->right);
    }

    void inorderHelper(Node *p)
    {
        if (p == nullptr) return;
        inorderHelper(p->left);                  // LEFT, node, right
        cout << p->elem << " -> ";
        inorderHelper(p->right);
    }

    void postorderHelper(Node *p)
    {
        if (p == nullptr) return;
        postorderHelper(p->left);                // LEFT, right, node
        postorderHelper(p->right);
        cout << p->elem << " -> ";
    }

public:
    void printPreorder()
    {
        preorderHelper(root);
        cout << "NULL" << endl;
    }

    void printInorder()
    {
        inorderHelper(root);
        cout << "NULL" << endl;
    }

    void printPostorder()
    {
        postorderHelper(root);
        cout << "NULL" << endl;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:

    Node *addRoot(int val)
    {
        if (root != nullptr) { cout << "Tree is not empty, can't add a root" << endl; return nullptr; }
        root = new Node(val, nullptr);
        size = 1;
        return root;
    }

    Node *addLeft(Node *p, int val)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return nullptr; }
        if (p->left != nullptr) { cout << "Left child already exists" << endl; return nullptr; }
        p->left = new Node(val, p);              // the new node remembers its parent
        size++;
        return p->left;
    }

    Node *addRight(Node *p, int val)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return nullptr; }
        if (p->right != nullptr) { cout << "Right child already exists" << endl; return nullptr; }
        p->right = new Node(val, p);
        size++;
        return p->right;
    }

    Node *left(Node *p)
    {
        if (p == nullptr) return nullptr;
        return p->left;                          // may be nullptr: that just means "no left child"
    }

    Node *right(Node *p)
    {
        if (p == nullptr) return nullptr;
        return p->right;
    }

    Node *sibling(Node *p)
    {
        if (p == nullptr || p->parent == nullptr) return nullptr;   // the root has no sibling
        if (p == p->parent->left) return p->parent->right;          // I'm the left one: sibling is the right one
        return p->parent->left;                                     // I'm the right one: sibling is the left one
    }

    int set(Node *p, int val)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return -1; }
        int old = p->elem;                       // remember the old element BEFORE overwriting it
        p->elem = val;
        return old;
    }

    int remove(Node *p)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return -1; }
        if (p->left != nullptr && p->right != nullptr)
        {
            cout << "Node has two children, can't remove" << endl;
            return -1;
        }
        Node *child = (p->left != nullptr) ? p->left : p->right;   // the ONE child, or nullptr if p is a leaf

        if (child != nullptr) child->parent = p->parent;           // the child moves up to p's place
        if (p->parent == nullptr) root = child;                    // EDGE CASE: p was the root, so the child becomes the root
        else if (p->parent->left == p) p->parent->left = child;    // p hung on its parent's LEFT: child takes the left slot
        else p->parent->right = child;                             // p hung on the RIGHT: child takes the right slot

        int val = p->elem;
        delete p;
        size--;
        return val;
    }

    bool attach(Node *p, BinaryTree &t1, BinaryTree &t2)
    {
        if (p == nullptr || p->left != nullptr || p->right != nullptr)   // p must be a LEAF (no children at all)
        {
            cout << "Position is not a leaf" << endl;
            return false;
        }
        if (t1.root != nullptr)                  // an empty T1 has nothing to attach
        {
            p->left = t1.root;                   // T1's root becomes p's left child...
            t1.root->parent = p;                 // ...and must point back up at p
            size += t1.size;                     // this tree grew by the size of T1
            t1.root = nullptr;                   // T1 gives its nodes away: reset it to an empty tree
            t1.size = 0;
        }
        if (t2.root != nullptr)                  // same for T2, on the RIGHT
        {
            p->right = t2.root;
            t2.root->parent = p;
            size += t2.size;
            t2.root = nullptr;
            t2.size = 0;
        }
        return true;
    }

    // ---- helpers the tests (and some of the functions above) rely on ----

    Node *getRoot() { return root; }

    bool isEmpty() { return root == nullptr; }

    int getSize() { return size; }

    bool isLeaf(Node *p) { return p != nullptr && p->left == nullptr && p->right == nullptr; }

private:
    void clearHelper(Node *p)
    {
        if (p == nullptr) return;
        clearHelper(p->left);
        clearHelper(p->right);
        delete p;
    }

    // The tree's shape as text. A leaf is just its number; otherwise  elem(left,right)
    // and a missing child is "-".   Example:  1(2(4,5),3(-,6))
    string shapeHelper(Node *p)
    {
        if (p == nullptr) return "-";
        if (p->left == nullptr && p->right == nullptr) return to_string(p->elem);
        return to_string(p->elem) + "(" + shapeHelper(p->left) + "," + shapeHelper(p->right) + ")";
    }

    string checkNode(Node *p, int &count)
    {
        count++;
        if (count > 10000) return "a node points back into the tree (infinite loop)";
        if (p->left != nullptr)
        {
            if (p->left->parent != p) return "node " + to_string(p->left->elem) + " is the left child of " + to_string(p->elem) + " but its parent pointer is wrong";
            string err = checkNode(p->left, count);
            if (err != "") return err;
        }
        if (p->right != nullptr)
        {
            if (p->right->parent != p) return "node " + to_string(p->right->elem) + " is the right child of " + to_string(p->elem) + " but its parent pointer is wrong";
            string err = checkNode(p->right, count);
            if (err != "") return err;
        }
        return "";
    }

public:
    void clear()
    {
        clearHelper(root);
        root = nullptr;
        size = 0;
    }

    // ---- TEST-ONLY helpers (not part of the usual assignment) ----

    string shape()
    {
        if (root == nullptr) return "(empty)";
        return shapeHelper(root);
    }

    // Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        if (root == nullptr)
        {
            if (size != 0) return "tree is empty but size says " + to_string(size);
            return "OK";
        }
        if (root->parent != nullptr) return "root's parent pointer is not nullptr";
        int count = 0;
        string err = checkNode(root, count);
        if (err != "") return err;
        if (count != size) return "size says " + to_string(size) + " but the tree really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
