/*
 * DRILL: countNodes, countLeaves
 * ------------------------------------------------------------------
 * countNodes(p) / countLeaves(p): how many nodes / leaves are in the subtree under p (p included). The no-argument versions do the whole tree.
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
 *   printed   -> what your function printed doesn't match the expected text
 *   size      -> getSize() is wrong (forgot size++ / size--)
 *   integrity -> parent / firstChild / lastChild / root / size don't agree. Usual cause: lastChild
 *                (or firstChild / root) not updated in an edge case (first child, only child, last child).
 *                integrity() is a test-only helper below the DO NOT CHANGE line.
 *
 * RULES THESE TESTS ASSUME
 *   - countNodes(nullptr) and countLeaves(nullptr) are 0.
 *   - A single node is 1 node and 1 leaf.
 *   - Count by recursion: the tests also ask about SUBTREES, so returning size would not be enough.
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
    cout << "=== DRILL: countNodes, countLeaves ===" << endl;

    beginTest("EMPTY tree: 0 nodes, 0 leaves");
    {
        GeneralTree t;
        expectInt("countNodes()", t.countNodes(), 0);
        expectInt("countLeaves()", t.countLeaves(), 0);
    }
    endTest();

    beginTest("nullptr: 0 and 0");
    {
        GeneralTree t;
        buildSample(t);
        expectInt("countNodes(nullptr)", t.countNodes(nullptr), 0);
        expectInt("countLeaves(nullptr)", t.countLeaves(nullptr), 0);
    }
    endTest();

    beginTest("ONE node: 1 node, 1 leaf");
    {
        GeneralTree t;
        t.addRoot(5);
        expectInt("countNodes()", t.countNodes(), 1);
        expectInt("countLeaves()", t.countLeaves(), 1);
    }
    endTest();

    beginTest("sample tree: 7 nodes, 4 leaves");
    {
        GeneralTree t;
        buildSample(t);
        expectInt("countNodes()", t.countNodes(), 7);
        expectInt("countLeaves()", t.countLeaves(), 4);
    }
    endTest();

    beginTest("sample tree: SUBTREES");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("countNodes(2)", t.countNodes(s.n2), 3);
        expectInt("countLeaves(2)", t.countLeaves(s.n2), 2);
        expectInt("countNodes(4)", t.countNodes(s.n4), 2);
        expectInt("countLeaves(4)", t.countLeaves(s.n4), 1);
        expectInt("countNodes(3)", t.countNodes(s.n3), 1);
        expectInt("countLeaves(3)", t.countLeaves(s.n3), 1);
    }
    endTest();

    beginTest("chain of 5: 5 nodes, ONE leaf");
    {
        GeneralTree t;
        buildChain(t, 5);
        expectInt("countNodes()", t.countNodes(), 5);
        expectInt("countLeaves()", t.countLeaves(), 1);
    }
    endTest();

    beginTest("wide (root + 4 children): 5 nodes, 4 leaves");
    {
        GeneralTree t;
        buildWide(t, 4);
        expectInt("countNodes()", t.countNodes(), 5);
        expectInt("countLeaves()", t.countLeaves(), 4);
    }
    endTest();

    beginTest("second tree: 8 nodes, 4 leaves");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        expectInt("countNodes()", t.countNodes(), 8);
        expectInt("countLeaves()", t.countLeaves(), 4);
        expectInt("countLeaves(30)", t.countLeaves(s.n30), 3);
    }
    endTest();

    beginTest("a leaf that gets a child stops being a leaf (leaf count stays 4, nodes go to 8)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChild(s.n3, 30);
        expectInt("countNodes()", t.countNodes(), 8);
        expectInt("countLeaves()", t.countLeaves(), 4);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
