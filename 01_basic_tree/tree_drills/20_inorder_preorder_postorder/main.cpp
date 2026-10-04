/*
 * DRILL: printPreorder, printInorder, printPostorder
 * ------------------------------------------------------------------
 * The three depth-first traversals of a binary tree. They differ only in WHEN the node is printed: before (pre), between (in) or after (post) its left and right subtrees.
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
 *   - Preorder: node, left, right.   Inorder: left, node, right.   Postorder: left, right, node.
 *   - Format is the same as your list drills: "1 -> 2 -> 3 -> NULL" (arrow with spaces, NULL at the end, newline).
 *   - An EMPTY tree prints just "NULL".
 *   - Printing must not change the tree.
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
    cout << "=== DRILL: printPreorder, printInorder, printPostorder ===" << endl;

    beginTest("EMPTY tree");
    {
        BinaryTree t;

        expectTraversals(t, "NULL\n", "NULL\n", "NULL\n");
        expectBin(t, "(empty)", 0);
    }
    endTest();

    beginTest("ONE node");
    {
        BinaryTree t;
        t.addRoot(5);
        expectTraversals(t, "5 -> NULL\n", "5 -> NULL\n", "5 -> NULL\n");
        expectBin(t, "5", 1);
    }
    endTest();

    beginTest("sample tree 1(2(4,5),3(-,6))");
    {
        BinaryTree t;
        buildBin(t);
        expectTraversals(t, "1 -> 2 -> 4 -> 5 -> 3 -> 6 -> NULL\n", "4 -> 2 -> 5 -> 1 -> 3 -> 6 -> NULL\n", "4 -> 5 -> 2 -> 6 -> 3 -> 1 -> NULL\n");
        expectBin(t, "1(2(4,5),3(-,6))", 6);
    }
    endTest();

    beginTest("left chain 1 -> 2 -> 3 (always a left child)");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addLeft(a, 2);
        t.addLeft(b, 3);
        expectTraversals(t, "1 -> 2 -> 3 -> NULL\n", "3 -> 2 -> 1 -> NULL\n", "3 -> 2 -> 1 -> NULL\n");
        expectBin(t, "1(2(3,-),-)", 3);
    }
    endTest();

    beginTest("right chain 1 -> 2 -> 3 (always a right child)");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addRight(a, 2);
        t.addRight(b, 3);
        expectTraversals(t, "1 -> 2 -> 3 -> NULL\n", "1 -> 2 -> 3 -> NULL\n", "3 -> 2 -> 1 -> NULL\n");
        expectBin(t, "1(-,2(-,3))", 3);
    }
    endTest();

    beginTest("balanced 4(2(1,3),6(5,7))");
    {
        BinaryTree t;
        Node *a = t.addRoot(4);
        Node *b = t.addLeft(a, 2);
        Node *c = t.addRight(a, 6);
        t.addLeft(b, 1);
        t.addRight(b, 3);
        t.addLeft(c, 5);
        t.addRight(c, 7);
        expectTraversals(t, "4 -> 2 -> 1 -> 3 -> 6 -> 5 -> 7 -> NULL\n", "1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> NULL\n", "1 -> 3 -> 2 -> 5 -> 7 -> 6 -> 4 -> NULL\n");
        expectBin(t, "4(2(1,3),6(5,7))", 7);
    }
    endTest();

    beginTest("zig-zag 1(-,2(3,-))");
    {
        BinaryTree t;
        Node *a = t.addRoot(1);
        Node *b = t.addRight(a, 2);
        t.addLeft(b, 3);
        expectTraversals(t, "1 -> 2 -> 3 -> NULL\n", "1 -> 3 -> 2 -> NULL\n", "3 -> 2 -> 1 -> NULL\n");
        expectBin(t, "1(-,2(3,-))", 3);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
