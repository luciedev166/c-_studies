#ifndef GENERALTREE_HPP
#define GENERALTREE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: printPathFromRoot
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function(s) you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, NodeQueue, members, constructor and destructor are given scaffolding.
//   Keep the names root / size / elem / parent / firstChild / lastChild / nextSibling: the helpers use them.)
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

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: printPathFromRoot
    // ---------------------------------------------------------------

private:
    void pathHelper(Node *p)
    {
        if (p == nullptr) return;
        pathHelper(p->parent);                                 // go UP first...
        cout << p->elem << " -> ";                             // ...so the printing happens on the way back DOWN
    }

public:
    bool printPathFromRoot(int val)
    {
        Node *target = search(val);                            // reuse search from the helper section
        if (target == nullptr) { cout << "Value not found" << endl; return false; }
        pathHelper(target);
        cout << "NULL" << endl;
        return true;
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

    Node *addChild(Node *p, int val)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return nullptr; }
        Node *n = new Node(val, p);              // the new node remembers its parent
        if (p->firstChild == nullptr)            // EDGE CASE: p had no children yet, so n is
            p->firstChild = p->lastChild = n;    //   both the first AND the last child
        else
        {
            p->lastChild->nextSibling = n;       // exactly addBack on a list that has a tail
            p->lastChild = n;
        }
        size++;
        return n;
    }

    bool isRoot(Node *p)
    {
        return p != nullptr && p->parent == nullptr;      // only the root has no parent
    }

    bool isLeaf(Node *p)
    {
        return p != nullptr && p->firstChild == nullptr;  // no children at all
    }

    bool isInternal(Node *p)
    {
        return p != nullptr && p->firstChild != nullptr;  // at least one child
    }

    bool isEmpty()
    {
        return root == nullptr;
    }

    int getSize()
    {
        return size;
    }

private:
    void preorderHelper(Node *p)
    {
        if (p == nullptr) return;
        cout << p->elem << " -> ";                             // PRE = visit the node FIRST...
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            preorderHelper(c);                                 // ...then every child, left to right
    }

public:
    void printPreorder()
    {
        preorderHelper(root);
        cout << "NULL" << endl;
    }

private:
    void postorderHelper(Node *p)
    {
        if (p == nullptr) return;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            postorderHelper(c);                                // POST = every child first...
        cout << p->elem << " -> ";                             // ...and the node LAST
    }

public:
    void printPostorder()
    {
        postorderHelper(root);
        cout << "NULL" << endl;
    }

    void printLevelOrder()
    {
        NodeQueue q;                                           // the queue holds nodes WAITING to be visited
        if (root != nullptr) q.enqueue(root);
        while (!q.isEmpty())
        {
            Node *cur = q.dequeue();                           // take the node that has waited longest
            cout << cur->elem << " -> ";
            for (Node *c = cur->firstChild; c != nullptr; c = c->nextSibling)
                q.enqueue(c);                                  // its children go to the BACK of the line
        }
        cout << "NULL" << endl;
    }

private:
    void indentedHelper(Node *p, int depth)
    {
        if (p == nullptr) return;
        for (int i = 0; i < depth * 2; i++) cout << ' ';       // 2 spaces per level
        cout << p->elem << endl;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            indentedHelper(c, depth + 1);                      // children are one level deeper
    }

public:
    void printIndented()
    {
        indentedHelper(root, 0);                               // empty tree: prints nothing at all
    }

    int height(Node *p)
    {
        if (p == nullptr) return -1;                           // so that a leaf comes out as -1 + 1 = 0
        int tallest = -1;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
        {
            int h = height(c);
            if (h > tallest) tallest = h;
        }
        return tallest + 1;
    }

    int height()
    {
        return height(root);                                   // empty tree: -1
    }

    int depth(Node *p)
    {
        if (p == nullptr) return -1;
        int d = 0;
        while (p->parent != nullptr)                           // climb to the root, counting the steps
        {
            d++;
            p = p->parent;
        }
        return d;
    }

    int countNodes(Node *p)
    {
        if (p == nullptr) return 0;
        int total = 1;                                         // this node...
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            total += countNodes(c);                            // ...plus everything under each child
        return total;
    }

    int countNodes()
    {
        return countNodes(root);                               // (count by recursion, don't just return size)
    }

    int countLeaves(Node *p)
    {
        if (p == nullptr) return 0;
        if (p->firstChild == nullptr) return 1;                // a leaf counts as one
        int total = 0;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            total += countLeaves(c);
        return total;
    }

    int countLeaves()
    {
        return countLeaves(root);
    }

private:
    Node *searchHelper(Node *p, int val)
    {
        if (p == nullptr) return nullptr;
        if (p->elem == val) return p;                          // checked in PREORDER: node first, then children
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
        {
            Node *found = searchHelper(c, val);
            if (found != nullptr) return found;                // stop at the first match
        }
        return nullptr;
    }

public:
    Node *search(int val)
    {
        return searchHelper(root, val);
    }

private:
    int sumHelper(Node *p)
    {
        if (p == nullptr) return 0;
        int total = p->elem;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
            total += sumHelper(c);
        return total;
    }

    int maxHelper(Node *p)                                     // p is never nullptr here
    {
        int best = p->elem;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
        {
            int m = maxHelper(c);
            if (m > best) best = m;
        }
        return best;
    }

public:
    int sumAll()
    {
        return sumHelper(root);                                // empty tree: 0
    }

    int maxVal()
    {
        if (root == nullptr) { cout << "Tree is empty" << endl; return -1; }
        return maxHelper(root);
    }

    int removeLeaf(Node *p)
    {
        if (p == nullptr) { cout << "Invalid position" << endl; return -1; }
        if (p->firstChild != nullptr) { cout << "Not a leaf, can't remove" << endl; return -1; }

        if (p->parent == nullptr)                              // EDGE CASE: p is the root (and a leaf = the ONLY node)
        {
            root = nullptr;
        }
        else
        {
            Node *par = p->parent;
            if (par->firstChild == p)                          // p is the FIRST child (like removeFront)
            {
                par->firstChild = p->nextSibling;
                if (par->firstChild == nullptr)                // EDGE CASE: it was the ONLY child, so the
                    par->lastChild = nullptr;                  //   parent's list is empty now: fix the tail too
            }
            else                                               // p is later in the list: find the node before it
            {
                Node *prev = par->firstChild;
                while (prev->nextSibling != p) prev = prev->nextSibling;
                prev->nextSibling = p->nextSibling;            // skip over p
                if (par->lastChild == p) par->lastChild = prev;  // EDGE CASE: p was the LAST child (like removeBack)
            }
        }
        int val = p->elem;
        delete p;
        size--;
        return val;
    }

private:
    void clearHelper(Node *p)
    {
        if (p == nullptr) return;
        Node *c = p->firstChild;
        while (c != nullptr)
        {
            Node *nxt = c->nextSibling;                        // save it BEFORE c is deleted
            clearHelper(c);
            c = nxt;
        }
        delete p;                                              // POSTORDER: children first, the node last
    }

public:
    void clear()
    {
        clearHelper(root);
        root = nullptr;                                        // don't leave root dangling
        size = 0;
    }

    // ---- TEST-ONLY helpers (not part of the usual assignment) ----

    Node *getRoot() { return root; }

private:
    // Walks the whole tree and checks every link. Returns "" if fine, else a sentence.
    string checkNode(Node *p, int &count)
    {
        count++;
        if (count > 10000) return "a node points back into the tree (infinite loop)";
        Node *last = nullptr;
        for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
        {
            if (c->parent != p)
                return "node " + to_string(c->elem) + " is in node " + to_string(p->elem) + "'s child list but its parent pointer is wrong";
            string err = checkNode(c, count);
            if (err != "") return err;
            last = c;
        }
        if (p->firstChild == nullptr && p->lastChild != nullptr)
            return "node " + to_string(p->elem) + " has no children but lastChild is not nullptr (lastChild left dangling)";
        if (p->lastChild != last)
            return "node " + to_string(p->elem) + ": lastChild is not the last node in its child list";
        return "";
    }

public:
    // Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        if (root == nullptr)
        {
            if (size != 0) return "tree is empty but size says " + to_string(size);
            return "OK";
        }
        if (root->parent != nullptr) return "root's parent pointer is not nullptr";
        if (root->nextSibling != nullptr) return "root has a nextSibling";
        int count = 0;
        string err = checkNode(root, count);
        if (err != "") return err;
        if (count != size) return "size says " + to_string(size) + " but the tree really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
