/*
 * DRILL: printLeaves, removeAllLeaves
 * ------------------------------------------------------------------
 * printLeaves()      : print every leaf of the tree, left to right, in the usual format:  3 -> 5 -> NULL
 * removeAllLeaves()   : remove every node that is a leaf RIGHT NOW. Returns how many were removed.
 *                       Only ONE layer comes off: a parent that BECOMES a leaf because its children were removed stays.
 *                       (Call it again to peel the next layer.)
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
 *   printed   -> what printLeaves printed doesn't match the expected text
 *   returned  -> removeAllLeaves returned the wrong count
 *   children  -> a child list doesn't read the way it should afterwards
 *   size      -> getSize() is wrong
 *   integrity -> parent / firstChild / lastChild / root / size don't agree. Usual cause: lastChild not updated
 *                after removing the last child (or all children), or root not set to nullptr.
 *
 * RULES THESE TESTS ASSUME
 *   - printLeaves prints the leaves in the order a preorder walk meets them, each followed by " -> ", then "NULL" and a newline.
 *     Empty tree: just "NULL". A one-node tree: that node IS a leaf, so  5 -> NULL.
 *   - removeAllLeaves removes the nodes that are leaves at the moment it is called. A parent that becomes a leaf
 *     only because its children were just removed is NOT removed in the same call.
 *   - It returns the number of nodes removed (0 for an empty tree).
 *   - If the whole tree is just the root, the root is a leaf: it is removed (tree empty, size 0, root nullptr).
 *   - Child lists must stay correct (first, last, only child) and size must be updated.
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
    cout << "=== DRILL: printLeaves, removeAllLeaves ===" << endl;

    beginTest("printLeaves: sample tree");
    {
        GeneralTree t;
        buildSample(t);
        string out = capture([&] { t.printLeaves(); });
        expectStr("printed", out, "5 -> 6 -> 3 -> 7 -> NULL\n");
        expectTree(t, 7);
    }
    endTest();

    beginTest("printLeaves: second tree (50 is NOT a leaf, 80 is)");
    {
        GeneralTree t;
        buildSecond(t);
        string out = capture([&] { t.printLeaves(); });
        expectStr("printed", out, "40 -> 80 -> 60 -> 70 -> NULL\n");
    }
    endTest();

    beginTest("printLeaves: empty tree prints just NULL");
    {
        GeneralTree t;
        string out = capture([&] { t.printLeaves(); });
        expectStr("printed", out, "NULL\n");
    }
    endTest();

    beginTest("printLeaves: a one-node tree (the root is a leaf)");
    {
        GeneralTree t;
        t.addRoot(5);
        string out = capture([&] { t.printLeaves(); });
        expectStr("printed", out, "5 -> NULL\n");
    }
    endTest();

    beginTest("printLeaves: a chain has exactly one leaf; a wide tree has n");
    {
        GeneralTree t;
        buildChain(t, 6);
        expectStr("chain of 6", capture([&] { t.printLeaves(); }), "6 -> NULL\n");
        GeneralTree u;
        buildWide(u, 4);
        expectStr("root with 1..4", capture([&] { u.printLeaves(); }), "1 -> 2 -> 3 -> 4 -> NULL\n");
    }
    endTest();

    beginTest("printLeaves does not change the tree");
    {
        GeneralTree t;
        buildSample(t);
        capture([&] { t.printLeaves(); });
        expectTree(t, 7);
        expectInt("leaves still", t.countLeaves(), 4);
    }
    endTest();

    beginTest("removeAllLeaves on the sample tree: 5 6 3 7 go (returns 4), 1 2 4 stay");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int removed = t.removeAllLeaves();
        expectInt("returned", removed, 4);
        expectStr("children of 1", kids(s.n1), "2 4");
        expectStr("children of 2", kids(s.n2), "(none)");
        expectStr("children of 4", kids(s.n4), "(none)");
        expectBool("2.lastChild is nullptr", s.n2->lastChild == nullptr, true);
        expectBool("4.lastChild is nullptr", s.n4->lastChild == nullptr, true);
        expectBool("1.lastChild is 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("only ONE layer comes off: 2 and 4 became leaves but are still there");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeAllLeaves();
        expectBool("2 is a leaf now", t.isLeaf(s.n2), true);
        expectBool("search(2) finds it", t.search(2) == s.n2, true);
        expectBool("search(4) finds it", t.search(4) == s.n4, true);
        expectBool("search(5) is gone", t.search(5) == nullptr, true);
    }
    endTest();

    beginTest("peel the sample tree layer by layer: 4, then 2, then 1, then 0");
    {
        GeneralTree t;
        buildSample(t);
        expectInt("1st call", t.removeAllLeaves(), 4);
        expectTree(t, 3);
        expectInt("2nd call", t.removeAllLeaves(), 2);
        expectTree(t, 1);
        expectInt("3rd call (the root)", t.removeAllLeaves(), 1);
        expectBool("isEmpty()", t.isEmpty(), true);
        expectBool("getRoot() is nullptr", t.getRoot() == nullptr, true);
        expectTree(t, 0);
        expectInt("4th call (empty tree)", t.removeAllLeaves(), 0);
        expectTree(t, 0);
    }
    endTest();

    beginTest("removeAllLeaves on the second tree: 40 80 60 70 go (returns 4)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        int removed = t.removeAllLeaves();
        expectInt("returned", removed, 4);
        expectStr("children of 10", kids(s.n10), "20 30");
        expectStr("children of 20", kids(s.n20), "(none)");
        expectStr("children of 30", kids(s.n30), "50");
        expectStr("children of 50", kids(s.n50), "(none)");
        expectBool("30.lastChild is 50", s.n30->lastChild == s.n50, true);
        expectStr("printLeaves now", capture([&] { t.printLeaves(); }), "20 -> 50 -> NULL\n");
        expectTree(t, 4);
    }
    endTest();

    beginTest("a wide tree: all 6 children go, the root stays and becomes a leaf");
    {
        GeneralTree t;
        Node *r = buildWide(t, 6);
        int removed = t.removeAllLeaves();
        expectInt("returned", removed, 6);
        expectBool("root.firstChild is nullptr", r->firstChild == nullptr, true);
        expectBool("root.lastChild is nullptr", r->lastChild == nullptr, true);
        expectBool("root is still the root", t.getRoot() == r, true);
        expectTree(t, 1);
    }
    endTest();

    beginTest("a chain of 5: only the deepest node goes, each call");
    {
        GeneralTree t;
        buildChain(t, 5);
        expectInt("1st call", t.removeAllLeaves(), 1);
        expectTree(t, 4);
        expectInt("2nd call", t.removeAllLeaves(), 1);
        expectTree(t, 3);
        expectStr("printLeaves", capture([&] { t.printLeaves(); }), "3 -> NULL\n");
    }
    endTest();

    beginTest("a ONE-node tree: the root is a leaf, so it is removed");
    {
        GeneralTree t;
        t.addRoot(5);
        expectInt("returned", t.removeAllLeaves(), 1);
        expectBool("isEmpty()", t.isEmpty(), true);
        expectTree(t, 0);
    }
    endTest();

    beginTest("...and the tree is usable again afterwards");
    {
        GeneralTree t;
        t.addRoot(5);
        t.removeAllLeaves();
        Node *n = t.addRoot(8);
        t.addChild(n, 9);
        expectStr("children of new root", kids(n), "9");
        expectTree(t, 2);
    }
    endTest();

    beginTest("mixed children: leaf, internal, leaf, internal, leaf (tail and head both change)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        t.addChild(r, 1);                    // leaf  (the FIRST child)
        Node *b = t.addChild(r, 2);          // internal
        t.addChild(r, 3);                    // leaf
        Node *d = t.addChild(r, 4);          // internal
        t.addChild(r, 5);                    // leaf  (the LAST child)
        t.addChild(b, 20);
        t.addChild(d, 40);
        t.addChild(d, 41);
        int removed = t.removeAllLeaves();
        expectInt("returned", removed, 6);
        expectStr("children of root", kids(r), "2 4");
        expectBool("root.firstChild is 2", r->firstChild == b, true);
        expectBool("root.lastChild is 4", r->lastChild == d, true);
        expectStr("children of 2", kids(b), "(none)");
        expectStr("children of 4", kids(d), "(none)");
        expectTree(t, 3);
    }
    endTest();

    beginTest("tail still works afterwards: addChild on a node that lost all its children");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeAllLeaves();
        t.addChild(s.n2, 50);
        t.addChild(s.n2, 51);
        expectStr("children of 2", kids(s.n2), "50 51");
        expectTree(t, 5);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
