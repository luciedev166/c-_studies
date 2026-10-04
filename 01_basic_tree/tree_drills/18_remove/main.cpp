/*
 * DRILL: remove
 * ------------------------------------------------------------------
 * remove(p): delete node p and let its ONE child (if any) take its place. Returns the element that was stored at p.
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
 *   - If p has TWO children (or p is nullptr): return -1 and change nothing.
 *   - p may be a leaf, or have exactly one child (a whole subtree hangs under that child).
 *   - The child takes p's slot: if p was a LEFT child the child becomes a left child, if p was a RIGHT child it becomes a right child.
 *   - If p is the root, its child becomes the new root (and has no parent). If p was the only node, the tree is empty.
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
    cout << "=== DRILL: remove ===" << endl;

    beginTest("remove a LEFT leaf (4)");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        int r = t.remove(b.n4);
        expectInt("returned", r, 4);
        expectBin(t, "1(2(-,5),3(-,6))", 5);
    }
    endTest();

    beginTest("remove a RIGHT leaf (5)");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        int r = t.remove(b.n5);
        expectInt("returned", r, 5);
        expectBin(t, "1(2(4,-),3(-,6))", 5);
    }
    endTest();

    beginTest("node with only a RIGHT child (3): 6 takes its place");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        int r = t.remove(b.n3);
        expectInt("returned", r, 3);
        expectBool("1.right is now 6", b.n1->right == b.n6, true);
        expectBin(t, "1(2(4,5),6)", 5);
    }
    endTest();

    beginTest("node with only a LEFT child: 1(2(3,-),-) remove 2");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        t.addLeft(b, 3);
        int r = t.remove(b);
        expectInt("returned", r, 2);
        expectBin(t, "1(3,-)", 2);
    }
    endTest();

    beginTest("the child brings its WHOLE subtree: 1(2(3(4,5),-),-) remove 2");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        Node *c = t.addLeft(b, 3);
        t.addLeft(c, 4);
        t.addRight(c, 5);
        int r = t.remove(b);
        expectInt("returned", r, 2);
        expectBin(t, "1(3(4,5),-)", 4);
    }
    endTest();

    beginTest("a LEFT child with only a RIGHT child stays on the LEFT: 1(2(-,3),-) remove 2");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        t.addRight(b, 3);
        t.remove(b);
        expectBin(t, "1(3,-)", 2);
    }
    endTest();

    beginTest("a RIGHT child with only a LEFT child stays on the RIGHT: 1(-,2(3,-)) remove 2");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addRight(a, 2);
        t.addLeft(b, 3);
        t.remove(b);
        expectBin(t, "1(-,3)", 2);
    }
    endTest();

    beginTest("node with TWO children (2): returns -1, nothing changes");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        int r = 0;
        capture([&] { r = t.remove(b.n2); });
        expectInt("returned", r, -1);
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    beginTest("root with TWO children: returns -1, nothing changes");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        int r = 0;
        capture([&] { r = t.remove(b.n1); });
        expectInt("returned", r, -1);
        expectBool("root is still 1", t.getRoot() == b.n1, true);
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    beginTest("root with ONE child: the child becomes the root");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        int r = t.remove(a);
        expectInt("returned", r, 1);
        expectBool("root is now 2", t.getRoot() == b, true);
        expectBin(t, "2", 1);
        t.addLeft(t.getRoot(), 5);
        expectBin(t, "2(5,-)", 2);
    }
    endTest();

    beginTest("ONE-node tree: removing the root empties the tree");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        int r = t.remove(a);
        expectInt("returned", r, 1);
        expectBool("isEmpty()", t.isEmpty(), true);
        expectBin(t, "(empty)", 0);
        t.addRoot(9);
        expectBin(t, "9", 1);
    }
    endTest();

    beginTest("remove(nullptr) returns -1");
    {
        BinaryTree t;
        buildBin(t);
        int r = 0;
        capture([&] { r = t.remove(nullptr); });
        expectInt("returned", r, -1);
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    beginTest("peel it down: remove 4, 5, 2 (a leaf now), 6, 3, 1");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectInt("remove 4", t.remove(b.n4), 4);
        expectBin(t, "1(2(-,5),3(-,6))", 5);
        expectInt("remove 5", t.remove(b.n5), 5);
        expectBin(t, "1(2,3(-,6))", 4);
        expectInt("remove 2", t.remove(b.n2), 2);
        expectBin(t, "1(-,3(-,6))", 3);
        expectInt("remove 6", t.remove(b.n6), 6);
        expectBin(t, "1(-,3)", 2);
        expectInt("remove 3", t.remove(b.n3), 3);
        expectBin(t, "1", 1);
        expectInt("remove 1", t.remove(b.n1), 1);
        expectBin(t, "(empty)", 0);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
