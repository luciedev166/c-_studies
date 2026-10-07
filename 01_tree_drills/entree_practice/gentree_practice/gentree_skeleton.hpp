#include <iostream>
#include <string>
#include <stdexcept>
using namespace std; 

// ---------------------------------------------------------------
// THE STRUCT: fill in the constructor and destructor
// ---------------------------------------------------------------
struct node {
    int elem;
    node *parent;
    node **children;      // an ARRAY of node pointers (room for 10)
    int num_child;        // how many slots are in use
    int height;
    int depth;

    node(int e, node *p)
    {
        // TODO: set every field. Allocate the array, null out its slots,
        //       and work out depth from the parent (no parent = root).
        elem = e;
        parent = p;
        num_child = 0;
        height = 0;
        children = new node*[10];
        for(int i = 0; i < 10; i++)
        {
            children[i] = nullptr;
        }
        if(p == nullptr) depth = 0;
        else depth = p->depth+ 1;
    }

    ~node()
    {
        // TODO: free the children ARRAY (not the child nodes)
        delete [] children;
    }
};

// ---------------------------------------------------------------
// GenTree is GIVEN to you in the real file. Don't write it there.
// ---------------------------------------------------------------
class GenTree {
public:
    virtual node *addRoot(int) = 0;
    virtual node *addChild(node *, int) = 0;
    virtual void remove(node *) = 0;
    virtual ~GenTree() {}
};

// ---------------------------------------------------------------
// YOUR CLASS
// ---------------------------------------------------------------
class MyGenTree : public GenTree {
private:
    node *root;

    void updateHeight(node *n)
    {
        // TODO: n->height = 1 + biggest child height (a leaf is 0)
        int best = -1;
        for(int i = 0; i < n->num_child; i++)
        {
            if(n->children[i]->height > best) best = n->children[i]->height > best;
        }
        n->height = best + 1;
    }

    void updateHeightsUp(node *n)
    {
        // TODO: updateHeight on n and on every ancestor above it
        for(node *a != nullptr; a != nullptr; a=a->parent)
        updateHeight(a);
    }

    void clearHelper(node *p)
    {
        // TODO: delete p and everything under it (so the destructor can use it)
    }

public:
    MyGenTree()
    {
        root = nullptr;
    }

    ~MyGenTree()
    {
        clearHelper(root);
    }

    node *addRoot(int val)
    {
        // TODO: throw logic_error("Already has root") if a root exists
        return nullptr;
    }

    node *addChild(node *p, int val)
    {
        // TODO
        return nullptr;
    }

    void remove(node *n)
    {
        // TODO: only a node with no children can go.
        //       Otherwise print: <elem> has children
    }
};

capacity = 10;
children = (node**) malloc(cap * sizeof(node*));
for(int i = 0; i < 10; ) children[i] = nullptr;

//growing

