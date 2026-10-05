/*
 * DRILL: getNumChildren, getTotalChildren, getNthChild
 * ------------------------------------------------------------------
 * getNumChildren(p)    : how many DIRECT children p has.
 * getTotalChildren(p)  : how many nodes are BELOW p (children, grandchildren, ...). p itself is not counted.
 * getNthChild(p, n)    : the n-th child of p, counting from 1 (1 = firstChild). Walk the sibling list n-1 steps.
 *
 * HOW TO USE
 *   1. Open GeneralTree.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function(s), then comment them out and write your own.
 *   2. Compile and run (AddressSanitizer also catches leaks and bad memory access):
 *        g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
 *      No ASan on your machine? Just drop "-fsanitize=address".
 *   3. Every test prints one line: PASS, or FAIL plus what you returned vs. what was expected.
 *      If the program CRASHES, the last "- ..." line on screen is the test that crashed it.
 *
 * WHAT A FAIL MEANS
 *   returned  -> your function returned the wrong number / the wrong node
 *   size      -> the tree changed (these functions only LOOK, they must never modify anything)
 *   integrity -> same: you changed a pointer you should only have read
 *
 * RULES THESE TESTS ASSUME
 *   - getNumChildren(nullptr) and getTotalChildren(nullptr) return -1.
 *   - A leaf has 0 children and 0 total children.
 *   - getTotalChildren counts every descendant, so for the root it is (size - 1).
 *   - getNthChild is 1-BASED: n = 1 is the first child, n = (number of children) is the last child.
 *   - getNthChild returns nullptr if p is nullptr, n < 1, or n is bigger than the number of children.
 *   - None of these functions change the tree. What you print on an error doesn't matter.
 */

#include <iostream>
#include <sstream>
#include <string>
#include "GeneralTree.hpp"
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

// The children of p as text, e.g. "2 3 4" ("(none)" if p has no children).
inline string kids(Node *p)
{
    string s;
    for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
    {
        if (!s.empty()) s += " ";
        s += to_string(c->elem);
    }
    return s.empty() ? "(none)" : s;
}

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

// size + integrity: should hold after (almost) every operation.
inline void expectTree(GeneralTree &t, int wantSize)
{
    expectInt("size", t.getSize(), wantSize);
    expectStr("integrity", t.integrity(), "OK");
}

/*
 Sample tree used in most drills:
            1
         /  |  \
        2   3   4
       / \      |
      5   6     7
*/
struct Sample { Node *n1, *n2, *n3, *n4, *n5, *n6, *n7; };
inline Sample buildSample(GeneralTree &t)
{
    Sample s;
    s.n1 = t.addRoot(1);
    s.n2 = t.addChild(s.n1, 2);
    s.n3 = t.addChild(s.n1, 3);
    s.n4 = t.addChild(s.n1, 4);
    s.n5 = t.addChild(s.n2, 5);
    s.n6 = t.addChild(s.n2, 6);
    s.n7 = t.addChild(s.n4, 7);
    return s;
}

/*
 Second tree:
              10
             /  \
           20    30
           |    / | \
          40  50  60  70
               |
              80
*/
struct Second { Node *n10, *n20, *n30, *n40, *n50, *n60, *n70, *n80; };
inline Second buildSecond(GeneralTree &t)
{
    Second s;
    s.n10 = t.addRoot(10);
    s.n20 = t.addChild(s.n10, 20);
    s.n30 = t.addChild(s.n10, 30);
    s.n40 = t.addChild(s.n20, 40);
    s.n50 = t.addChild(s.n30, 50);
    s.n60 = t.addChild(s.n30, 60);
    s.n70 = t.addChild(s.n30, 70);
    s.n80 = t.addChild(s.n50, 80);
    return s;
}

// 1 -> 2 -> 3 -> ... -> n  (each node the only child of the one before). Returns the deepest node.
inline Node *buildChain(GeneralTree &t, int n)
{
    Node *cur = t.addRoot(1);
    for (int i = 2; i <= n; i++) cur = t.addChild(cur, i);
    return cur;
}

// root 0 with n children 1..n. Returns the root.
inline Node *buildWide(GeneralTree &t, int n)
{
    Node *r = t.addRoot(0);
    for (int i = 1; i <= n; i++) t.addChild(r, i);
    return r;
}




// ------------------------------------------------------------------
//  The tests
// ------------------------------------------------------------------
int main()
{
    cout << "=== DRILL: getNumChildren, getTotalChildren, getNthChild ===" << endl;

    beginTest("getNumChildren on the sample tree (1:3  2:2  3:0  4:1  5:0)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("children of 1", t.getNumChildren(s.n1), 3);
        expectInt("children of 2", t.getNumChildren(s.n2), 2);
        expectInt("children of 3 (leaf)", t.getNumChildren(s.n3), 0);
        expectInt("children of 4", t.getNumChildren(s.n4), 1);
        expectInt("children of 5 (leaf)", t.getNumChildren(s.n5), 0);
        expectTree(t, 7);
    }
    endTest();

    beginTest("getNumChildren counts DIRECT children only (second tree: 10:2  30:3  50:1)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        expectInt("children of 10", t.getNumChildren(s.n10), 2);
        expectInt("children of 30", t.getNumChildren(s.n30), 3);
        expectInt("children of 50", t.getNumChildren(s.n50), 1);
        expectInt("children of 80 (leaf)", t.getNumChildren(s.n80), 0);
        expectTree(t, 8);
    }
    endTest();

    beginTest("getNumChildren(nullptr) returns -1");
    {
        GeneralTree t;
        buildSample(t);
        int r = -5;
        capture([&] { r = t.getNumChildren(nullptr); });
        expectInt("returned", r, -1);
        expectTree(t, 7);
    }
    endTest();

    beginTest("getNumChildren: root with 50 children, and a one-node tree");
    {
        GeneralTree t;
        Node *r = buildWide(t, 50);
        expectInt("children of root", t.getNumChildren(r), 50);
        GeneralTree u;
        Node *only = u.addRoot(9);
        expectInt("children of a lone root", u.getNumChildren(only), 0);
    }
    endTest();

    beginTest("getNumChildren follows the tree as it changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChild(s.n3, 30);
        t.addChild(s.n3, 31);
        expectInt("children of 3 after adding two", t.getNumChildren(s.n3), 2);
        t.removeLeaf(s.n5);
        expectInt("children of 2 after removing 5", t.getNumChildren(s.n2), 1);
        t.removeLeaf(s.n7);
        expectInt("children of 4 after removing 7", t.getNumChildren(s.n4), 0);
        expectTree(t, 7);
    }
    endTest();

    beginTest("getTotalChildren on the sample tree (1:6  2:2  3:0  4:1  7:0)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("total below 1", t.getTotalChildren(s.n1), 6);
        expectInt("total below 2", t.getTotalChildren(s.n2), 2);
        expectInt("total below 3 (leaf)", t.getTotalChildren(s.n3), 0);
        expectInt("total below 4", t.getTotalChildren(s.n4), 1);
        expectInt("total below 7 (leaf)", t.getTotalChildren(s.n7), 0);
        expectTree(t, 7);
    }
    endTest();

    beginTest("getTotalChildren on the second tree (10:7  20:1  30:4  50:1)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        expectInt("total below 10", t.getTotalChildren(s.n10), 7);
        expectInt("total below 20", t.getTotalChildren(s.n20), 1);
        expectInt("total below 30", t.getTotalChildren(s.n30), 4);
        expectInt("total below 50", t.getTotalChildren(s.n50), 1);
        expectTree(t, 8);
    }
    endTest();

    beginTest("getTotalChildren(nullptr) returns -1");
    {
        GeneralTree t;
        buildSample(t);
        int r = -5;
        capture([&] { r = t.getTotalChildren(nullptr); });
        expectInt("returned", r, -1);
    }
    endTest();

    beginTest("getTotalChildren: a chain of 100 nodes = 99, and always size - 1 for the root");
    {
        GeneralTree t;
        buildChain(t, 100);
        expectInt("total below the root", t.getTotalChildren(t.getRoot()), 99);
        GeneralTree u;
        Sample s = buildSample(u);
        expectInt("root total == size - 1", u.getTotalChildren(s.n1), u.getSize() - 1);
    }
    endTest();

    beginTest("getNthChild on node 1 (children 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectNode("1st child", t.getNthChild(s.n1, 1), s.n2);
        expectNode("2nd child", t.getNthChild(s.n1, 2), s.n3);
        expectNode("3rd child", t.getNthChild(s.n1, 3), s.n4);
        expectTree(t, 7);
    }
    endTest();

    beginTest("getNthChild on node 2 (children 5 6) and node 4 (child 7)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectNode("1st child of 2", t.getNthChild(s.n2, 1), s.n5);
        expectNode("2nd child of 2", t.getNthChild(s.n2, 2), s.n6);
        expectNode("1st child of 4", t.getNthChild(s.n4, 1), s.n7);
    }
    endTest();

    beginTest("getNthChild out of range returns nullptr (n = 0, -1, count+1, 100)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectNode("n = 0", t.getNthChild(s.n1, 0), nullptr);
        expectNode("n = -1", t.getNthChild(s.n1, -1), nullptr);
        expectNode("n = 4 (only 3 children)", t.getNthChild(s.n1, 4), nullptr);
        expectNode("n = 100", t.getNthChild(s.n1, 100), nullptr);
    }
    endTest();

    beginTest("getNthChild on a leaf, and with p == nullptr");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectNode("1st child of leaf 3", t.getNthChild(s.n3, 1), nullptr);
        expectNode("1st child of nullptr", t.getNthChild(nullptr, 1), nullptr);
    }
    endTest();

    beginTest("getNthChild: first and last of 50 children");
    {
        GeneralTree t;
        Node *r = buildWide(t, 50);
        expectInt("1st child elem", elemOf(t.getNthChild(r, 1)), 1);
        expectInt("25th child elem", elemOf(t.getNthChild(r, 25)), 25);
        expectInt("50th child elem", elemOf(t.getNthChild(r, 50)), 50);
        expectBool("50th child is lastChild", t.getNthChild(r, 50) == r->lastChild, true);
        expectNode("51st child", t.getNthChild(r, 51), nullptr);
    }
    endTest();

    beginTest("getNthChild and getNumChildren agree on every node");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        Node *all[] = { s.n10, s.n20, s.n30, s.n40, s.n50, s.n60, s.n70, s.n80 };
        for (Node *p : all)
        {
            int k = t.getNumChildren(p);
            expectNode("child number k+1 of " + to_string(p->elem), t.getNthChild(p, k + 1), nullptr);
            if (k > 0) expectNode("child number k of " + to_string(p->elem), t.getNthChild(p, k), p->lastChild);
        }
        expectTree(t, 8);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
