/*
 * DRILL: left, right, sibling
 * ------------------------------------------------------------------
 * left(p) / right(p): the left / right child of p (or nullptr).  sibling(p): the other child of p's parent (or nullptr).
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
 *   - left / right / sibling of nullptr return nullptr.
 *   - The root has no sibling.
 *   - A node whose parent has only that one child has no sibling (nullptr).
 *   - These only LOOK at the tree: nothing changes.
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
    cout << "=== DRILL: left, right, sibling ===" << endl;

    beginTest("left(p) on the sample tree");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("left(1)", t.left(b.n1), b.n2);
        expectNode("left(2)", t.left(b.n2), b.n4);
        expectNode("left(3)", t.left(b.n3), nullptr);
        expectNode("left(6)", t.left(b.n6), nullptr);
    }
    endTest();

    beginTest("right(p) on the sample tree");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("right(1)", t.right(b.n1), b.n3);
        expectNode("right(2)", t.right(b.n2), b.n5);
        expectNode("right(3)", t.right(b.n3), b.n6);
        expectNode("right(4)", t.right(b.n4), nullptr);
    }
    endTest();

    beginTest("left / right of nullptr");
    {
        BinaryTree t;
        buildBin(t);
        expectNode("left(nullptr)", t.left(nullptr), nullptr);
        expectNode("right(nullptr)", t.right(nullptr), nullptr);
    }
    endTest();

    beginTest("sibling of a LEFT child");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("sibling(2)", t.sibling(b.n2), b.n3);
        expectNode("sibling(4)", t.sibling(b.n4), b.n5);
    }
    endTest();

    beginTest("sibling of a RIGHT child");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("sibling(3)", t.sibling(b.n3), b.n2);
        expectNode("sibling(5)", t.sibling(b.n5), b.n4);
    }
    endTest();

    beginTest("an ONLY child has no sibling (6 under 3)");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("sibling(6)", t.sibling(b.n6), nullptr);
    }
    endTest();

    beginTest("an only LEFT child has no sibling either");
    {
        BinaryTree t;
        Node *r = t.addRoot(1);
        Node *l = t.addLeft(r, 2);
        expectNode("sibling(left child)", t.sibling(l), nullptr);
    }
    endTest();

    beginTest("the root has no sibling");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        expectNode("sibling(1)", t.sibling(b.n1), nullptr);
    }
    endTest();

    beginTest("sibling(nullptr)");
    {
        BinaryTree t;
        buildBin(t);
        expectNode("sibling(nullptr)", t.sibling(nullptr), nullptr);
    }
    endTest();

    beginTest("single-node tree: no children, no sibling");
    {
        BinaryTree t;
        Node *r = t.addRoot(1);
        expectNode("left(root)", t.left(r), nullptr);
        expectNode("right(root)", t.right(r), nullptr);
        expectNode("sibling(root)", t.sibling(r), nullptr);
    }
    endTest();

    beginTest("nothing changed after all those calls");
    {
        BinaryTree t;
        Bin b = buildBin(t);
        t.left(b.n1);
        t.right(b.n2);
        t.sibling(b.n4);
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
