// main.cpp  -  LOCKED. You can't change this file. Write your functions in tree.hpp only.
//
// It reads commands from the input, one at a time, until the input ends:
//
//   root V          add the root V
//   left P V        add V as the LEFT child of the node holding P
//   right P V       add V as the RIGHT child of the node holding P
//   sibling V       print the sibling of the node holding V
//   set P V         replace the element of the node holding P with V
//   remove V        remove the node holding V (it must not have two children)
//   attach P        attach two small trees under the node holding P (it must be a leaf)
//   pre / in / post print a traversal
//   height / size   print the height / the size

#include <iostream>
#include <string>
#include "tree.hpp"
using namespace std;

int main()
{
    BinaryTree tree;
    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "root")
        {
            int v; cin >> v;
            if (tree.addRoot(v) == nullptr) cout << "Root not added" << endl;
            else cout << "Root added: " << v << endl;
        }
        else if (cmd == "left" || cmd == "right")
        {
            int par, v; cin >> par >> v;
            Node *p = tree.search(par);
            if (p == nullptr) cout << "Parent " << par << " not found" << endl;
            else
            {
                Node *n = (cmd == "left") ? tree.addLeft(p, v) : tree.addRight(p, v);
                if (n == nullptr) cout << par << " already has a " << cmd << " child" << endl;
                else cout << "Added " << v << " as " << cmd << " child of " << par << endl;
            }
        }
        else if (cmd == "sibling")
        {
            int v; cin >> v;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else
            {
                Node *s = tree.sibling(p);
                if (s == nullptr) cout << v << " has no sibling" << endl;
                else cout << "Sibling of " << v << ": " << s->elem << endl;
            }
        }
        else if (cmd == "set")
        {
            int v, nv; cin >> v >> nv;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else
            {
                int old = tree.set(p, nv);
                cout << "Replaced " << old << " with " << nv << endl;
            }
        }
        else if (cmd == "remove")
        {
            int v; cin >> v;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else
            {
                int r = tree.remove(p);
                if (r == -1) cout << v << " has two children" << endl;
                else cout << "Removed " << r << endl;
            }
        }
        else if (cmd == "attach")
        {
            int v; cin >> v;
            Node *p = tree.search(v);
            if (p == nullptr) cout << v << " not found" << endl;
            else
            {
                BinaryTree t1, t2;                       // T1 =  100(101,-)     T2 =  200(-,201)
                Node *a = t1.addRoot(100);
                t1.addLeft(a, 101);
                Node *b = t2.addRoot(200);
                t2.addRight(b, 201);
                if (tree.attach(p, t1, t2))
                    cout << "Attached under " << v << " (T1 size " << t1.getSize() << ", T2 size " << t2.getSize() << ")" << endl;
                else
                    cout << v << " is not a leaf" << endl;
            }
        }
        else if (cmd == "pre")    tree.printPreorder();
        else if (cmd == "in")     tree.printInorder();
        else if (cmd == "post")   tree.printPostorder();
        else if (cmd == "height") cout << "Height: " << tree.height() << endl;
        else if (cmd == "size")   cout << "Size: " << tree.getSize() << endl;
        else cout << "Unknown command: " << cmd << endl;
    }
    return 0;
}
