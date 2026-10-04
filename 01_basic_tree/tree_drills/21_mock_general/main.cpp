// main.cpp  -  LOCKED. You can't change this file. Write your functions in tree.hpp only.
//
// It reads commands from the input, one at a time, until the input ends:
//
//   root V          add the root V               child P V      add V as the last child of the node holding P
//   pre             print preorder               post           print postorder
//   level           print level order            indent         print indented
//   height          print the tree's height      size           print the size
//   depth V         depth of the node holding V  path V         path from the root to V
//   leaves          count leaves                 nodes          count nodes
//   sum             sum of all elements          max            biggest element
//   removeleaf V    remove the node holding V (only if it is a leaf)
//   clear           delete everything

#include <iostream>
#include <string>
#include "tree.hpp"
using namespace std;

int main()
{
    GeneralTree tree;
    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "root")
        {
            int v; cin >> v;
            if (tree.addRoot(v) == nullptr) cout << "Root not added" << endl;
            else cout << "Root added: " << v << endl;
        }
        else if (cmd == "child")
        {
            int par, v; cin >> par >> v;
            Node *p = tree.search(par);
            if (p == nullptr) cout << "Parent " << par << " not found" << endl;
            else { tree.addChild(p, v); cout << "Added " << v << " under " << par << endl; }
        }
        else if (cmd == "pre")    tree.printPreorder();
        else if (cmd == "post")   tree.printPostorder();
        else if (cmd == "level")  tree.printLevelOrder();
        else if (cmd == "indent") tree.printIndented();
        else if (cmd == "height") cout << "Height: " << tree.height() << endl;
        else if (cmd == "size")   cout << "Size: " << tree.getSize() << endl;
        else if (cmd == "leaves") cout << "Leaves: " << tree.countLeaves() << endl;
        else if (cmd == "nodes")  cout << "Nodes: " << tree.countNodes() << endl;
        else if (cmd == "sum")    cout << "Sum: " << tree.sumAll() << endl;
        else if (cmd == "max")    cout << "Max: " << tree.maxVal() << endl;
        else if (cmd == "depth")
        {
            int v; cin >> v;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else cout << "Depth of " << v << ": " << tree.depth(p) << endl;
        }
        else if (cmd == "path")
        {
            int v; cin >> v;
            if (!tree.printPathFromRoot(v)) cout << v << " not found" << endl;
        }
        else if (cmd == "removeleaf")
        {
            int v; cin >> v;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else
            {
                int r = tree.removeLeaf(p);
                if (r == -1) cout << v << " is not a leaf" << endl;
                else cout << "Removed " << r << endl;
            }
        }
        else if (cmd == "clear")
        {
            tree.clear();
            cout << "Cleared" << endl;
        }
        else cout << "Unknown command: " << cmd << endl;
    }
    return 0;
}
