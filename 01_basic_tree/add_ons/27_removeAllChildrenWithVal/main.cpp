/*
 * DRILL: removeAllChildrenWithVal
 * ------------------------------------------------------------------
 * removeAllChildrenWithVal(p, val): go through ALL of p's direct children and remove every one whose elem is val
 *   (as long as it is a leaf). Returns how many nodes were removed.
 *   One pass over the list with a "previous" pointer: the trick is not to lose the next node when you delete one.
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
 *   returned  -> wrong count (or you didn't return -1 for nullptr)
 *   children  -> p's child list doesn't read the way it should afterwards
 *   size      -> getSize() is wrong
 *   integrity -> parent / firstChild / lastChild / size don't agree. Usual cause: lastChild not updated after
 *                removing the last child (or ALL children). A crash usually means you used a node after deleting it
 *                (save c->nextSibling BEFORE you delete c).
 *
 * RULES THESE TESTS ASSUME
 *   - Only the DIRECT children of p are looked at (not grandchildren).
 *   - Every child with elem == val is removed IF it is a leaf. A matching child that has children of its own is
 *     skipped: it stays in the list and is not counted.
 *   - Return the number of nodes removed (0 if nothing matched). If p is nullptr: return -1 and change nothing.
 *   - If every child is removed, p becomes a leaf: firstChild AND lastChild are nullptr.
 *   - Neighbouring matches (7 7 7) and matches at the front / back of the list all have to work.
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
    cout << "=== DRILL: removeAllChildrenWithVal ===" << endl;

    // a root 0 with children  7 3 7 7 5 7  (kept in the variables below)
    beginTest("remove all 7s from 7 3 7 7 5 7 (front, a run of two, back)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 7, 3, 7, 7, 5, 7 };
        Node *n[6];
        for (int i = 0; i < 6; i++) n[i] = t.addChild(r, vals[i]);
        int removed = t.removeAllChildrenWithVal(r, 7);
        expectInt("returned", removed, 4);
        expectStr("children of root", kids(r), "3 5");
        expectBool("firstChild is 3", r->firstChild == n[1], true);
        expectBool("lastChild is 5", r->lastChild == n[4], true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("remove all 7s from 1 7 7 2 (a run in the middle)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 1, 7, 7, 2 };
        for (int v : vals) t.addChild(r, v);
        int removed = t.removeAllChildrenWithVal(r, 7);
        expectInt("returned", removed, 2);
        expectStr("children of root", kids(r), "1 2");
        expectBool("lastChild is 2", r->lastChild != nullptr && r->lastChild->elem == 2, true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("remove all 7s from 1 2 7 7 (the run is at the BACK): the tail moves");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 1, 2, 7, 7 };
        Node *n[4];
        for (int i = 0; i < 4; i++) n[i] = t.addChild(r, vals[i]);
        int removed = t.removeAllChildrenWithVal(r, 7);
        expectInt("returned", removed, 2);
        expectStr("children of root", kids(r), "1 2");
        expectBool("lastChild is 2", r->lastChild == n[1], true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("remove all 7s from 7 7 1 2 (the run is at the FRONT): the head moves");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 7, 7, 1, 2 };
        Node *n[4];
        for (int i = 0; i < 4; i++) n[i] = t.addChild(r, vals[i]);
        int removed = t.removeAllChildrenWithVal(r, 7);
        expectInt("returned", removed, 2);
        expectStr("children of root", kids(r), "1 2");
        expectBool("firstChild is 1", r->firstChild == n[2], true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("EVERY child matches (5 5 5 5): the parent becomes a leaf");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        for (int i = 0; i < 4; i++) t.addChild(r, 5);
        int removed = t.removeAllChildrenWithVal(r, 5);
        expectInt("returned", removed, 4);
        expectStr("children of root", kids(r), "(none)");
        expectBool("firstChild is nullptr", r->firstChild == nullptr, true);
        expectBool("lastChild is nullptr", r->lastChild == nullptr, true);
        expectBool("isLeaf(root)", t.isLeaf(r), true);
        expectTree(t, 1);
    }
    endTest();

    beginTest("alternating 1 9 1 9 1 9 1: remove the 9s, then the 1s");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 1, 9, 1, 9, 1, 9, 1 };
        for (int v : vals) t.addChild(r, v);
        expectInt("removed 9s", t.removeAllChildrenWithVal(r, 9), 3);
        expectStr("children of root", kids(r), "1 1 1 1");
        expectTree(t, 5);
        expectInt("removed 1s", t.removeAllChildrenWithVal(r, 1), 4);
        expectStr("children of root", kids(r), "(none)");
        expectTree(t, 1);
    }
    endTest();

    beginTest("ONE matching child (the only child, 7 under 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int removed = t.removeAllChildrenWithVal(s.n4, 7);
        expectInt("returned", removed, 1);
        expectBool("4.firstChild is nullptr", s.n4->firstChild == nullptr, true);
        expectBool("4.lastChild is nullptr", s.n4->lastChild == nullptr, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("no child matches: returns 0, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int removed = t.removeAllChildrenWithVal(s.n1, 99);
        expectInt("returned", removed, 0);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectBool("1.lastChild is 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("only DIRECT children count: 5 is a grandchild of 1, so nothing is removed");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        int removed = t.removeAllChildrenWithVal(s.n1, 5);
        expectInt("returned", removed, 0);
        expectStr("children of 2", kids(s.n2), "5 6");
        expectTree(t, 7);
    }
    endTest();

    beginTest("a matching child that is NOT a leaf is skipped (children 4 4 4, the middle one has a child)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        Node *a = t.addChild(r, 4);
        Node *b = t.addChild(r, 4);
        Node *c = t.addChild(r, 4);
        t.addChild(b, 8);
        int removed = t.removeAllChildrenWithVal(r, 4);
        expectInt("returned", removed, 2);
        expectStr("children of root", kids(r), "4");
        expectBool("the survivor is the middle one", r->firstChild == b, true);
        expectBool("firstChild == lastChild", r->firstChild == r->lastChild, true);
        expectStr("children of the survivor", kids(b), "8");
        expectTree(t, 3);
        (void)a; (void)c;
    }
    endTest();

    beginTest("p is a leaf: returns 0");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        expectInt("returned", t.removeAllChildrenWithVal(s.n3, 1), 0);
        expectTree(t, 7);
    }
    endTest();

    beginTest("p == nullptr returns -1");
    {
        GeneralTree t;
        buildSample(t);
        int r = -5;
        capture([&] { r = t.removeAllChildrenWithVal(nullptr, 1); });
        expectInt("returned", r, -1);
        expectTree(t, 7);
    }
    endTest();

    beginTest("tail still works: remove the back run, then addChild");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 1, 7, 7 };
        for (int v : vals) t.addChild(r, v);
        t.removeAllChildrenWithVal(r, 7);
        t.addChild(r, 2);
        expectStr("children of root", kids(r), "1 2");
        expectTree(t, 3);
    }
    endTest();

    beginTest("tail still works: remove EVERY child, then addChild");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        for (int i = 0; i < 3; i++) t.addChild(r, 6);
        t.removeAllChildrenWithVal(r, 6);
        t.addChild(r, 1);
        t.addChild(r, 2);
        expectStr("children of root", kids(r), "1 2");
        expectTree(t, 3);
    }
    endTest();

    beginTest("works on a deeper node and leaves the rest of the tree alone (second tree, 30 has 50 60 70)");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        t.addChild(s.n30, 60);
        t.addChild(s.n30, 60);
        int removed = t.removeAllChildrenWithVal(s.n30, 60);
        expectInt("returned", removed, 3);
        expectStr("children of 30", kids(s.n30), "50 70");
        expectStr("children of 10", kids(s.n10), "20 30");
        expectStr("children of 50", kids(s.n50), "80");
        expectTree(t, 7);
    }
    endTest();

    beginTest("a lot of children: root 0 with 1..200, remove the value 100");
    {
        GeneralTree t;
        Node *r = buildWide(t, 200);
        expectInt("returned", t.removeAllChildrenWithVal(r, 100), 1);
        expectInt("size", t.getSize(), 200);
        expectTree(t, 200);
        (void)r;
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
