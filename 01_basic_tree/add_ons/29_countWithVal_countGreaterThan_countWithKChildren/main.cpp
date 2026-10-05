/*
 * DRILL: countWithVal, countGreaterThan, countWithKChildren
 * ------------------------------------------------------------------
 * countWithVal(p, val)       : how many nodes in the subtree of p (p included) have elem == val.
 * countGreaterThan(p, val)    : how many nodes in the subtree of p (p included) have elem  > val  (strictly).
 * countWithKChildren(p, k)    : how many nodes in the subtree of p (p included) have EXACTLY k children.
 * Each one also has a no-argument-for-p version that counts the whole tree (same idea as countNodes in drill 09).
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
 *   returned  -> wrong count
 *   size / integrity -> the tree changed (these functions only LOOK, they must never modify anything)
 *
 * RULES THESE TESTS ASSUME
 *   - The subtree of p includes p itself.
 *   - countGreaterThan is STRICT: a node whose elem equals val is not counted.
 *   - countWithKChildren(p, 0) counts the leaves; it counts DIRECT children only (not descendants).
 *   - The versions without p, e.g. countWithVal(val), count the whole tree. An empty tree gives 0.
 *   - p == nullptr gives 0.
 *   - Use recursion over the subtree (the same pattern as countNodes). Do not rely on size.
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
    cout << "=== DRILL: countWithVal, countGreaterThan, countWithKChildren ===" << endl;

    // Tree with repeated values used below, written 5(5(5,2),3,5):
    //  root 5 with children 5, 3, 5; and the first 5 has the children 5 and 2.

    beginTest("countWithVal: sample tree (each value appears once)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("count of 3", t.countWithVal(3), 1);
        expectInt("count of 7", t.countWithVal(7), 1);
        expectInt("count of 99", t.countWithVal(99), 0);
        expectInt("count of 3 in the subtree of 2", t.countWithVal(s.n2, 3), 0);
        expectInt("count of 5 in the subtree of 2", t.countWithVal(s.n2, 5), 1);
        expectTree(t, 7);
    }
    endTest();

    beginTest("countWithVal: repeated values 5(5(5,2),3,5)");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        Node *a = t.addChild(r, 5);
        t.addChild(r, 3);
        t.addChild(r, 5);
        t.addChild(a, 5);
        t.addChild(a, 2);
        expectInt("whole tree: 5s", t.countWithVal(5), 4);
        expectInt("whole tree: 3s", t.countWithVal(3), 1);
        expectInt("whole tree: 2s", t.countWithVal(2), 1);
        expectInt("whole tree: 9s", t.countWithVal(9), 0);
        expectInt("subtree of the first child: 5s", t.countWithVal(a, 5), 2);
        expectInt("subtree of the root: 5s", t.countWithVal(r, 5), 4);
        expectInt("subtree of a leaf 5", t.countWithVal(a->firstChild, 5), 1);
        expectInt("subtree of a leaf 2 looking for 5", t.countWithVal(a->lastChild, 5), 0);
        expectTree(t, 6);
    }
    endTest();

    beginTest("countWithVal: p == nullptr and an empty tree give 0");
    {
        GeneralTree t;
        expectInt("empty tree", t.countWithVal(5), 0);
        buildSample(t);
        expectInt("p == nullptr", t.countWithVal(nullptr, 1), 0);
    }
    endTest();

    beginTest("countWithVal: every node has the same value");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *c = t.addChild(r, 1);
        t.addChild(c, 1);
        t.addChild(r, 1);
        expectInt("count of 1", t.countWithVal(1), 4);
        expectInt("count of 2", t.countWithVal(2), 0);
    }
    endTest();

    beginTest("countGreaterThan: sample tree (values 1..7)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("> 3", t.countGreaterThan(3), 4);
        expectInt("> 0", t.countGreaterThan(0), 7);
        expectInt("> 6", t.countGreaterThan(6), 1);
        expectInt("> 7 (strict: 7 itself is not counted)", t.countGreaterThan(7), 0);
        expectInt("> 100", t.countGreaterThan(100), 0);
        expectInt("> -5", t.countGreaterThan(-5), 7);
        expectInt("> 4 in the subtree of 2 (5 and 6)", t.countGreaterThan(s.n2, 4), 2);
        expectInt("> 3 in the subtree of 4 (4 and 7)", t.countGreaterThan(s.n4, 3), 2);
        expectTree(t, 7);
    }
    endTest();

    beginTest("countGreaterThan: second tree (10 20 30 40 50 60 70 80)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        expectInt("> 45", t.countGreaterThan(45), 4);
        expectInt("> 50 (strict)", t.countGreaterThan(50), 3);
        expectInt("> 10 in the subtree of 30", t.countGreaterThan(s.n30, 10), 5);
        expectInt("> 60 in the subtree of 20", t.countGreaterThan(s.n20, 60), 0);
        expectTree(t, 8);
    }
    endTest();

    beginTest("countGreaterThan: p == nullptr and an empty tree give 0");
    {
        GeneralTree t;
        expectInt("empty tree", t.countGreaterThan(0), 0);
        buildSample(t);
        expectInt("p == nullptr", t.countGreaterThan(nullptr, 0), 0);
    }
    endTest();

    beginTest("countGreaterThan: negative values");
    {
        GeneralTree t;
        Node *r = t.addRoot(-3);
        t.addChild(r, -10);
        t.addChild(r, 4);
        expectInt("> -5", t.countGreaterThan(-5), 2);
        expectInt("> -10", t.countGreaterThan(-10), 2);
        expectInt("> -3", t.countGreaterThan(-3), 1);
    }
    endTest();

    beginTest("countWithKChildren: sample tree (0 kids: 4 nodes, 1 kid: node 4, 2 kids: node 2, 3 kids: node 1)");
    {
        GeneralTree t;
        buildSample(t);
        expectInt("k = 0", t.countWithKChildren(0), 4);
        expectInt("k = 1", t.countWithKChildren(1), 1);
        expectInt("k = 2", t.countWithKChildren(2), 1);
        expectInt("k = 3", t.countWithKChildren(3), 1);
        expectInt("k = 4", t.countWithKChildren(4), 0);
        expectInt("k = -1", t.countWithKChildren(-1), 0);
        expectTree(t, 7);
    }
    endTest();

    beginTest("countWithKChildren on subtrees (sample tree)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("subtree of 2, k = 0", t.countWithKChildren(s.n2, 0), 2);
        expectInt("subtree of 2, k = 2", t.countWithKChildren(s.n2, 2), 1);
        expectInt("subtree of 2, k = 3", t.countWithKChildren(s.n2, 3), 0);
        expectInt("subtree of 4, k = 1", t.countWithKChildren(s.n4, 1), 1);
        expectInt("subtree of 3 (a leaf), k = 0", t.countWithKChildren(s.n3, 0), 1);
        expectInt("subtree of 3 (a leaf), k = 1", t.countWithKChildren(s.n3, 1), 0);
    }
    endTest();

    beginTest("countWithKChildren: second tree (k=1: 20 and 50, k=2: 10, k=3: 30)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        expectInt("k = 0", t.countWithKChildren(0), 4);
        expectInt("k = 1", t.countWithKChildren(1), 2);
        expectInt("k = 2", t.countWithKChildren(2), 1);
        expectInt("k = 3", t.countWithKChildren(3), 1);
        expectInt("subtree of 30, k = 1", t.countWithKChildren(s.n30, 1), 1);
    }
    endTest();

    beginTest("countWithKChildren counts DIRECT children (a chain of 5: 4 nodes have 1 child, 1 has 0)");
    {
        GeneralTree t;
        buildChain(t, 5);
        expectInt("k = 0", t.countWithKChildren(0), 1);
        expectInt("k = 1", t.countWithKChildren(1), 4);
        expectInt("k = 2", t.countWithKChildren(2), 0);
    }
    endTest();

    beginTest("countWithKChildren: p == nullptr, an empty tree, and a one-node tree");
    {
        GeneralTree t;
        expectInt("empty tree, k = 0", t.countWithKChildren(0), 0);
        expectInt("p == nullptr", t.countWithKChildren(nullptr, 0), 0);
        t.addRoot(9);
        expectInt("one node, k = 0", t.countWithKChildren(0), 1);
        expectInt("one node, k = 1", t.countWithKChildren(1), 0);
    }
    endTest();

    beginTest("the counts follow the tree as it changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChild(s.n3, 7);
        t.addChild(s.n3, 8);
        expectInt("count of 7", t.countWithVal(7), 2);
        expectInt("> 6", t.countGreaterThan(6), 3);
        expectInt("k = 2", t.countWithKChildren(2), 2);
        t.removeLeaf(s.n5);
        expectInt("k = 1 after removing 5", t.countWithKChildren(1), 2);
        expectTree(t, 8);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
