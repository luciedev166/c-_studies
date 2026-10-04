/*
 * DRILL: removeLeaf
 * ------------------------------------------------------------------
 * removeLeaf(p): remove node p if it is a leaf. It is "remove from a linked list" again: p lives in its PARENT's child list.
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
 *   - removeLeaf(p) removes p ONLY if it is a leaf, and returns its elem.
 *   - If p is nullptr, or p has children: return -1 and change nothing.
 *   - Removing the root of a one-node tree empties the tree (size 0, root nullptr).
 *   - The parent's list of children must stay correct: first child, middle child, last child and only child all matter.
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
    cout << "=== DRILL: removeLeaf ===" << endl;

    beginTest("remove the FIRST child (5 under 2: children 5 6)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = t.removeLeaf(s.n5);
        expectInt("returned", r, 5);
        expectStr("children of 2", kids(s.n2), "6");
        expectBool("2.firstChild is 6", s.n2->firstChild == s.n6, true);
        expectBool("2.lastChild is 6", s.n2->lastChild == s.n6, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove the LAST child (6 under 2: children 5 6)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = t.removeLeaf(s.n6);
        expectInt("returned", r, 6);
        expectStr("children of 2", kids(s.n2), "5");
        expectBool("2.lastChild is 5", s.n2->lastChild == s.n5, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove a MIDDLE child (3 in 2 3 4, but 3 is a leaf)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = t.removeLeaf(s.n3);
        expectInt("returned", r, 3);
        expectStr("children of 1", kids(s.n1), "2 4");
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove an ONLY child (7 under 4): parent becomes a leaf");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = t.removeLeaf(s.n7);
        expectInt("returned", r, 7);
        expectStr("children of 4", kids(s.n4), "(none)");
        expectBool("4.firstChild is nullptr", s.n4->firstChild == nullptr, true);
        expectBool("4.lastChild is nullptr", s.n4->lastChild == nullptr, true);
        expectBool("isLeaf(4)", t.isLeaf(s.n4), true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("tail still works: remove the last child, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeLeaf(s.n6);
        t.addChild(s.n2, 9);
        expectStr("children of 2", kids(s.n2), "5 9");
        expectTree(t, 7);
    }
    endTest();

    beginTest("tail still works: remove the only child, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeLeaf(s.n7);
        t.addChild(s.n4, 8);
        expectStr("children of 4", kids(s.n4), "8");
        expectTree(t, 7);
    }
    endTest();

    beginTest("NOT a leaf (2 has children): returns -1, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = -5;
        capture([&] { r = t.removeLeaf(s.n2); });
        expectInt("returned", r, -1);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectStr("children of 2", kids(s.n2), "5 6");
        expectTree(t, 7);
    }
    endTest();

    beginTest("the root has children: returns -1, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int r = -5;
        capture([&] { r = t.removeLeaf(s.n1); });
        expectInt("returned", r, -1);
        expectBool("root is still 1", t.getRoot() == s.n1, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("removeLeaf(nullptr) returns -1");
    {
        GeneralTree t;
        buildSample(t);
        int r = -5;
        capture([&] { r = t.removeLeaf(nullptr); });
        expectInt("returned", r, -1);
        expectTree(t, 7);
    }
    endTest();

    beginTest("ONE-node tree: removing the root empties the tree");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        int v = t.removeLeaf(r);
        expectInt("returned", v, 5);
        expectBool("isEmpty()", t.isEmpty(), true);
        expectBool("getRoot() is nullptr", t.getRoot() == nullptr, true);
        expectTree(t, 0);
    }
    endTest();

    beginTest("...and the tree is usable again afterwards");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        t.removeLeaf(r);
        Node *n = t.addRoot(8);
        t.addChild(n, 9);
        expectStr("children of new root", kids(n), "9");
        expectTree(t, 2);
    }
    endTest();

    beginTest("peel the whole sample tree leaf by leaf: 5 6 2 3 7 4 1");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("remove 5", t.removeLeaf(s.n5), 5);
        expectTree(t, 6);
        expectInt("remove 6", t.removeLeaf(s.n6), 6);
        expectTree(t, 5);
        expectInt("remove 2 (a leaf now)", t.removeLeaf(s.n2), 2);
        expectTree(t, 4);
        expectInt("remove 3", t.removeLeaf(s.n3), 3);
        expectTree(t, 3);
        expectInt("remove 7", t.removeLeaf(s.n7), 7);
        expectTree(t, 2);
        expectInt("remove 4", t.removeLeaf(s.n4), 4);
        expectTree(t, 1);
        expectInt("remove 1 (the root)", t.removeLeaf(s.n1), 1);
        expectTree(t, 0);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
