/*
 * DRILL: search
 * ------------------------------------------------------------------
 * search(val): return the node holding val, or nullptr. If several nodes hold val, return the FIRST one in preorder.
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
 *   - Returns a Node* (the actual node in the tree), or nullptr if val is not there.
 *   - Duplicates: the first match in PREORDER (node, then children left to right) wins.
 *   - Searching must not change the tree.
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
    cout << "=== DRILL: search ===" << endl;

    beginTest("EMPTY tree: nullptr");
    {
        GeneralTree t;
        expectNode("search(5)", t.search(5), nullptr);
    }
    endTest();

    beginTest("ONE node: found / not found");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        expectNode("search(5)", t.search(5), r);
        expectNode("search(6)", t.search(6), nullptr);
    }
    endTest();

    beginTest("sample tree: root, middle and deep nodes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectNode("search(1)", t.search(1), s.n1);
        expectNode("search(2)", t.search(2), s.n2);
        expectNode("search(5)", t.search(5), s.n5);
        expectNode("search(7)", t.search(7), s.n7);
        expectNode("search(4)", t.search(4), s.n4);
    }
    endTest();

    beginTest("sample tree: values that are not there");
    {
        GeneralTree t;
        buildSample(t);
        expectNode("search(99)", t.search(99), nullptr);
        expectNode("search(0)", t.search(0), nullptr);
        expectNode("search(-1)", t.search(-1), nullptr);
    }
    endTest();

    beginTest("DUPLICATES: first in preorder wins");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChild(s.n3, 6);        // a second 6, but LATER in preorder than the one under 2
        expectNode("search(6)", t.search(6), s.n6);
    }
    endTest();

    beginTest("DUPLICATES: shallower copy is NOT automatically first (preorder, not level order)");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *a = t.addChild(r, 2);
        Node *deep = t.addChild(a, 9);     // 9 under 2: comes BEFORE 3 in preorder
        t.addChild(r, 9);                  // 9 directly under the root, but later in preorder
        expectNode("search(9)", t.search(9), deep);
    }
    endTest();

    beginTest("a node and its OWN descendant hold the same value: the outer one wins (preorder)");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *outer = t.addChild(r, 7);
        t.addChild(outer, 7);
        expectNode("search(7)", t.search(7), outer);
    }
    endTest();

    beginTest("negative values");
    {
        GeneralTree t;
        Node *r = t.addRoot(-3);
        Node *c = t.addChild(r, -8);
        expectNode("search(-8)", t.search(-8), c);
        expectNode("search(-3)", t.search(-3), r);
    }
    endTest();

    beginTest("deep: 500th node of a chain");
    {
        GeneralTree t;
        Node *deepest = buildChain(t, 500);
        expectNode("search(500)", t.search(500), deepest);
    }
    endTest();

    beginTest("search does not change the tree");
    {
        GeneralTree t;
        buildSample(t);
        t.search(7);
        t.search(99);
        expectTree(t, 7);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
