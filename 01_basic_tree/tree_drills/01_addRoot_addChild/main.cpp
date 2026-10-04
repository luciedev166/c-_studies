/*
 * DRILL: addRoot, addChild
 * ------------------------------------------------------------------
 * addRoot(val): create the root of an EMPTY tree.  addChild(p, val): add a new LAST child under node p.  Both return the new node.
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
 *   - addRoot on a tree that already has a root returns nullptr and changes nothing.
 *   - addChild(nullptr, val) returns nullptr and changes nothing.
 *   - A new child goes to the BACK of the parent's child list (like addBack).
 *   - What message you print on an error doesn't matter; the tests ignore it.
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
    cout << "=== DRILL: addRoot, addChild ===" << endl;

    beginTest("EMPTY tree: addRoot(10)");
    {
        GeneralTree t;
        Node *r = t.addRoot(10);
        expectBool("returned a node", r != nullptr, true);
        if (r != nullptr)
        {
            expectInt("root elem", r->elem, 10);
            expectBool("root has no parent", r->parent == nullptr, true);
        }
        expectBool("isEmpty()", t.isEmpty(), false);
        expectTree(t, 1);
    }
    endTest();

    beginTest("addRoot on a NON-empty tree fails");
    {
        GeneralTree t;
        Node *r = t.addRoot(10);
        Node *again = t.addRoot(20);
        expectNode("second addRoot returns", again, nullptr);
        expectBool("root is still the first one", t.getRoot() == r, true);
        expectTree(t, 1);
    }
    endTest();

    beginTest("addChild: first child of the root");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *c = t.addChild(r, 2);
        expectBool("returned a node", c != nullptr, true);
        if (c != nullptr)
        {
            expectInt("child elem", c->elem, 2);
            expectBool("child's parent is the root", c->parent == r, true);
        }
        expectBool("root.firstChild is the new node", r->firstChild == c, true);
        expectBool("root.lastChild is the new node", r->lastChild == c, true);
        expectStr("children of root", kids(r), "2");
        expectTree(t, 2);
    }
    endTest();

    beginTest("three addChilds: new child goes to the BACK");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        t.addChild(r, 2);
        t.addChild(r, 3);
        t.addChild(r, 4);
        expectStr("children of root", kids(r), "2 3 4");
        expectInt("firstChild elem", elemOf(r->firstChild), 2);
        expectInt("lastChild elem", elemOf(r->lastChild), 4);
        expectTree(t, 4);
    }
    endTest();

    beginTest("children are in insertion order, NOT sorted (9, 3, 6)");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        t.addChild(r, 9);
        t.addChild(r, 3);
        t.addChild(r, 6);
        expectStr("children of root", kids(r), "9 3 6");
        expectTree(t, 4);
    }
    endTest();

    beginTest("addChild under a grandchild (deeper level)");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *c = t.addChild(r, 2);
        Node *g = t.addChild(c, 3);
        expectBool("grandchild's parent is the child", g != nullptr && g->parent == c, true);
        expectStr("children of root", kids(r), "2");
        expectStr("children of node 2", kids(c), "3");
        expectTree(t, 3);
    }
    endTest();

    beginTest("build the sample tree (see helper below)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectStr("children of 2", kids(s.n2), "5 6");
        expectStr("children of 3", kids(s.n3), "(none)");
        expectStr("children of 4", kids(s.n4), "7");
        expectStr("children of 7", kids(s.n7), "(none)");
        expectBool("7's parent is 4", s.n7->parent == s.n4, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("the same value twice is allowed");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        t.addChild(r, 5);
        t.addChild(r, 5);
        expectStr("children of root", kids(r), "5 5");
        expectTree(t, 3);
    }
    endTest();

    beginTest("addChild(nullptr, 5) returns nullptr and changes nothing");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *c = t.addChild(nullptr, 5);
        expectNode("returned", c, nullptr);
        expectStr("children of root", kids(r), "(none)");
        expectTree(t, 1);
    }
    endTest();

    beginTest("100 children under one parent");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        string want;
        for (int i = 1; i <= 100; i++)
        {
            t.addChild(r, i);
            if (i > 1) want += " ";
            want += to_string(i);
        }
        expectStr("children of root", kids(r), want);
        expectInt("lastChild elem", elemOf(r->lastChild), 100);
        expectTree(t, 101);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
