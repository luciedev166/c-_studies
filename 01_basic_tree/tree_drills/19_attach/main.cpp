/*
 * DRILL: attach
 * ------------------------------------------------------------------
 * attach(p, T1, T2): p must be a LEAF. T1's whole tree becomes p's left subtree, T2's becomes p's right subtree, then T1 and T2 are reset to EMPTY trees. (See the hand-drawn picture in your notes.)
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
 *   - Returns true on success. Returns false (and changes nothing, T1 and T2 included) if p is nullptr or p is NOT a leaf.
 *   - A node with only one child is NOT a leaf.
 *   - T1 or T2 may be an empty tree: then that side is simply not attached.
 *   - After a successful attach: T1 and T2 are empty (root nullptr, size 0) and this tree's size grew by size(T1) + size(T2).
 *   - The attached roots must point back up to p (their parent pointer!).
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
    cout << "=== DRILL: attach ===" << endl;

    beginTest("attach two trees to a leaf of a bigger tree");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        Node *l = t.addLeft(r, 2);
        t.addRight(r, 3);
        Node *a = t1.addRoot(10);
        t1.addLeft(a, 11);
        t2.addRoot(20);
        bool ok = t.attach(l, t1, t2);
        expectBool("returned", ok, true);
        expectBin(t, "1(2(10(11,-),20),3)", 6);
    }
    endTest();

    beginTest("T1 and T2 are EMPTY trees afterwards");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t1.addRoot(10);
        t2.addRoot(20);
        t.attach(r, t1, t2);
        expectBin(t, "1(10,20)", 3);
        expectBin(t1, "(empty)", 0);
        expectBin(t2, "(empty)", 0);
        expectBool("t1.isEmpty()", t1.isEmpty(), true);
        expectBool("t2.isEmpty()", t2.isEmpty(), true);
    }
    endTest();

    beginTest("attach to a ONE-node tree (the root is a leaf)");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        Node *a = t1.addRoot(2);
        t1.addLeft(a, 4);
        Node *b = t2.addRoot(3);
        t2.addRight(b, 5);
        t.attach(r, t1, t2);
        expectBin(t, "1(2(4,-),3(-,5))", 5);
    }
    endTest();

    beginTest("T1 is EMPTY: only T2 is attached (on the right)");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t2.addRoot(3);
        bool ok = t.attach(r, t1, t2);
        expectBool("returned", ok, true);
        expectBin(t, "1(-,3)", 2);
        expectBin(t2, "(empty)", 0);
    }
    endTest();

    beginTest("T2 is EMPTY: only T1 is attached (on the left)");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t1.addRoot(2);
        bool ok = t.attach(r, t1, t2);
        expectBool("returned", ok, true);
        expectBin(t, "1(2,-)", 2);
        expectBin(t1, "(empty)", 0);
    }
    endTest();

    beginTest("BOTH empty: still true, tree unchanged");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        bool ok = t.attach(r, t1, t2);
        expectBool("returned", ok, true);
        expectBin(t, "1", 1);
    }
    endTest();

    beginTest("p is NOT a leaf (the root has children): false, nothing changes, T1/T2 keep their nodes");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t.addLeft(r, 2);
        t.addRight(r, 3);
        t1.addRoot(10);
        t2.addRoot(20);
        bool ok = true;
        capture([&] { ok = t.attach(r, t1, t2); });
        expectBool("returned", ok, false);
        expectBin(t, "1(2,3)", 3);
        expectBin(t1, "10", 1);
        expectBin(t2, "20", 1);
    }
    endTest();

    beginTest("a node with only ONE child is not a leaf either");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t.addLeft(r, 2);
        t1.addRoot(10);
        bool ok = true;
        capture([&] { ok = t.attach(r, t1, t2); });
        expectBool("returned", ok, false);
        expectBin(t, "1(2,-)", 2);
        expectBin(t1, "10", 1);
    }
    endTest();

    beginTest("attach(nullptr, ...) returns false");
    {
        BinaryTree t, t1, t2;
        t.addRoot(1);
        t1.addRoot(10);
        bool ok = true;
        capture([&] { ok = t.attach(nullptr, t1, t2); });
        expectBool("returned", ok, false);
        expectBin(t, "1", 1);
        expectBin(t1, "10", 1);
    }
    endTest();

    beginTest("sizes add up: 6 + 3 + 2 = 11");
    {
        BinaryTree t, t1, t2;
        Bin b = buildBin(t);
        Node *a = t1.addRoot(10);
        t1.addLeft(a, 11);
        t1.addRight(a, 12);
        Node *c = t2.addRoot(20);
        t2.addRight(c, 21);
        t.attach(b.n4, t1, t2);
        expectInt("size", t.getSize(), 11);
        expectBin(t, "1(2(4(10(11,12),20(-,21)),5),3(-,6))", 11);
    }
    endTest();

    beginTest("the attached roots point back up at p (parent pointers)");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        Node *a = t1.addRoot(2);
        Node *b = t2.addRoot(3);
        t.attach(r, t1, t2);
        expectBool("2.parent is 1", a->parent == r, true);
        expectBool("3.parent is 1", b->parent == r, true);
        expectBool("1.left is 2", r->left == a, true);
        expectBool("1.right is 3", r->right == b, true);
        expectStr("integrity", t.integrity(), "OK");
    }
    endTest();

    beginTest("T1 and T2 can be reused after being emptied");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        t1.addRoot(2);
        t2.addRoot(3);
        t.attach(r, t1, t2);
        Node *x = t1.addRoot(77);
        t1.addLeft(x, 78);
        expectBin(t1, "77(78,-)", 2);
        expectBin(t, "1(2,3)", 3);
    }
    endTest();

    beginTest("keep growing the tree after attach (add a child under an attached node)");
    {
        BinaryTree t, t1, t2;
        Node *r = t.addRoot(1);
        Node *a = t1.addRoot(2);
        t2.addRoot(3);
        t.attach(r, t1, t2);
        t.addLeft(a, 9);
        expectBin(t, "1(2(9,-),3)", 4);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
