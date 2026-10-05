/*
 * DRILL: addRoot, addLeft, addRight
 * ------------------------------------------------------------------
 * addRoot(val): make the root of an EMPTY tree.  addLeft(p, val) / addRight(p, val): give p a new left / right child. Each returns the new node.
 *
 * HOW TO USE
 *   1. Open BinaryTree.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function(s), then comment them out and write your own.
 *   2. Compile and run (AddressSanitizer also catches leaks and bad memory access):
 *        g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
 *      No ASan on your machine? Just drop "-fsanitize=address".
 *   3. Every test prints one line: PASS, or FAIL plus what you returned vs. what was expected.
 *      If the program CRASHES, the last "- ..." line on screen is the test that crashed it.
 * 
 * WHAT A FAIL MEANS
 *   shape     -> the tree doesn't look the way it should. Shapes are written elem(left,right):
 *                "-" means no child, a leaf is just its number, e.g. 1(2(4,5),3(-,6)).
 *   printed   -> what your function printed doesn't match the expected text
 *   size      -> getSize() is wrong (forgot size++ / size--, or didn't add the attached sizes)
 *   integrity -> parent / left / right / root / size don't agree. Usual cause: a child's PARENT pointer
 *                (or root) not updated. integrity() is a test-only helper below the DO NOT CHANGE line.
 *
 * RULES THESE TESTS ASSUME
 *   - addRoot on a non-empty tree returns nullptr.
 *   - addLeft(p, val) returns nullptr if p already has a left child (same for addRight / right).
 *   - addLeft / addRight with p == nullptr return nullptr.
 *   - On an error, return what the rules say and change nothing. What message you print is ignored by the tests.
 *   - shape() text: elem(left,right); "-" = no child; a leaf is just its number. Sample tree = 1(2(4,5),3(-,6)).
 */

#include <iostream>
#include <sstream>
#include <string>
#include "BinaryTree.hpp"
using namespace std;

// ------------------------------------------------------------------
//  Tiny test harness (don't edit)
// ------------------------------------------------------------------
static int testsRun = 0, testsPassed = 0;
static bool curOk = true;
static string curFails;

// Run f() and return everything it printed, keeping it off the screen.
template <typename F>
string capture(F f)
{
    ostringstream buf;
    streambuf *old = cout.rdbuf(buf.rdbuf());
    f();
    cout.rdbuf(old);
    return buf.str();
}

inline string esc(const string &s)
{
    string o;
    for (char c : s) { if (c == '\n') o += "\\n"; else o += c; }
    return o.size() > 110 ? o.substr(0, 110) + "..." : o;
}

inline void beginTest(const string &label)
{
    cout << "- " << label << " ... " << flush;
    curOk = true;
    curFails = "";
    testsRun++;
}

inline void expectStr(const string &what, const string &got, const string &want)
{
    if (got == want) return;
    curOk = false;
    curFails += "      " + what + ": got      [" + esc(got) + "]\n"
              + "      " + string(what.size() + 2, ' ') + "expected [" + esc(want) + "]\n";
}

inline void expectInt(const string &what, int got, int want) { expectStr(what, to_string(got), to_string(want)); }
inline void expectBool(const string &what, bool got, bool want) { expectStr(what, got ? "true" : "false", want ? "true" : "false"); }

inline void endTest()
{
    if (curOk) { cout << "PASS" << endl; testsPassed++; }
    else       { cout << "FAIL" << endl << curFails; }
}

// ------------------------------------------------------------------
//  Tree helpers for the tests (don't edit)
// ------------------------------------------------------------------
inline int elemOf(Node *p) { return p == nullptr ? -999 : p->elem; }

inline string describe(Node *p)
{
    if (p == nullptr) return "nullptr";
    string s = "node " + to_string(p->elem);
    if (p->parent == nullptr) s += " (the root)";
    else s += " (parent " + to_string(p->parent->elem) + ")";
    return s;
}

inline void expectNode(const string &what, Node *got, Node *want)
{
    if (got == want) return;
    curOk = false;
    curFails += "      " + what + ": got      [" + describe(got) + "]\n"
              + "      " + string(what.size() + 2, ' ') + "expected [" + describe(want) + "]\n";
}

// shape + size + integrity. shape() writes the tree as text:  elem(left,right), "-" = no child,
// a leaf is just its number.  Example: 1(2(4,5),3(-,6))
inline void expectBin(BinaryTree &t, const string &wantShape, int wantSize)
{
    expectStr("shape", t.shape(), wantShape);
    expectInt("size", t.getSize(), wantSize);
    expectStr("integrity", t.integrity(), "OK");
}

/*
 Sample tree used in most drills:
         1
        / \
       2   3
      / \   \
     4   5   6
   shape:  1(2(4,5),3(-,6))
*/
struct Bin { Node *n1, *n2, *n3, *n4, *n5, *n6; };
inline Bin buildBin(BinaryTree &t)
{
    Bin b;
    b.n1 = t.addRoot(1);
    b.n2 = t.addLeft(b.n1, 2);
    b.n3 = t.addRight(b.n1, 3);
    b.n4 = t.addLeft(b.n2, 4);
    b.n5 = t.addRight(b.n2, 5);
    b.n6 = t.addRight(b.n3, 6);
    return b;
}

// Checks all three traversals on one tree.
inline void expectTraversals(BinaryTree &t, const string &pre, const string &in, const string &post)
{
    expectStr("preorder", capture([&] { t.printPreorder(); }), pre);
    expectStr("inorder", capture([&] { t.printInorder(); }), in);
    expectStr("postorder", capture([&] { t.printPostorder(); }), post);
}


// ------------------------------------------------------------------
//  The tests
// ------------------------------------------------------------------
int main()
{
    cout << "=== DRILL: addRoot, addLeft, addRight ===" << endl;

    beginTest("EMPTY tree: addRoot(7)");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        expectBool("returned a node", r != nullptr, true);
        expectInt("elem", elemOf(r), 7);
        expectBool("root has no parent", r != nullptr && r->parent == nullptr, true);
        expectBin(t, "7", 1);
    }
    endTest();

    beginTest("addRoot on a NON-empty tree fails");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        Node *again = t.addRoot(8);
        expectNode("second addRoot returns", again, nullptr);
        expectBool("root is still the first one", t.getRoot() == r, true);
        expectBin(t, "7", 1);
    }
    endTest();

    beginTest("addLeft on the root");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        Node *l = t.addLeft(r, 8);
        expectBool("returned a node", l != nullptr, true);
        expectBool("l.parent is the root", l != nullptr && l->parent == r, true);
        expectBool("root.left is the new node", r->left == l, true);
        expectBin(t, "7(8,-)", 2);
    }
    endTest();

    beginTest("addRight on the root");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        Node *rt = t.addRight(r, 9);
        expectBool("rt.parent is the root", rt != nullptr && rt->parent == r, true);
        expectBool("root.right is the new node", r->right == rt, true);
        expectBin(t, "7(-,9)", 2);
    }
    endTest();

    beginTest("both children");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        t.addLeft(r, 8);
        t.addRight(r, 9);
        expectBin(t, "7(8,9)", 3);
    }
    endTest();

    beginTest("addLeft when a left child already exists: nullptr, nothing changes");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        Node *first = t.addLeft(r, 8);
        Node *second = nullptr;
        capture([&] { second = t.addLeft(r, 99); });
        expectNode("second addLeft returns", second, nullptr);
        expectBool("left child is still the first one", r->left == first, true);
        expectBin(t, "7(8,-)", 2);
    }
    endTest();

    beginTest("addRight when a right child already exists: nullptr, nothing changes");
    {
        BinaryTree t;
        Node *r = t.addRoot(7);
        Node *first = t.addRight(r, 9);
        Node *second = nullptr;
        capture([&] { second = t.addRight(r, 99); });
        expectNode("second addRight returns", second, nullptr);
        expectBool("right child is still the first one", r->right == first, true);
        expectBin(t, "7(-,9)", 2);
    }
    endTest();

    beginTest("build the sample tree");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectBool("4.parent is 2", b.n4->parent == b.n2, true);
        expectBool("6.parent is 3", b.n6->parent == b.n3, true);
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    beginTest("addLeft / addRight with p == nullptr");
    {
        BinaryTree t;
        t.addRoot(1);
        Node *a = t.addLeft(nullptr, 5);
        Node *b = t.addRight(nullptr, 5);
        expectNode("addLeft(nullptr)", a, nullptr);
        expectNode("addRight(nullptr)", b, nullptr);
        expectBin(t, "1", 1);
    }
    endTest();

    beginTest("chain of lefts: 1 -> 2 -> 3");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        t.addLeft(b, 3);
        expectBin(t, "1(2(3,-),-)", 3);
    }
    endTest();

    beginTest("chain of rights: 1 -> 2 -> 3");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addRight(a, 2);
        t.addRight(b, 3);
        expectBin(t, "1(-,2(-,3))", 3);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
