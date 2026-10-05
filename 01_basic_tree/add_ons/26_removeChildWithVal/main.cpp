/*
 * DRILL: removeChildWithVal
 * ------------------------------------------------------------------
 * removeChildWithVal(p, val): look through p's DIRECT children for the first one whose elem is val.
 *   If it is a leaf, remove it and return true. Otherwise return false and change nothing.
 *   It is "remove a node from a linked list by value" again: first, middle, last and only node all matter.
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
 *   returned  -> wrong true / false
 *   children  -> p's child list doesn't read the way it should afterwards
 *   size      -> getSize() is wrong (forgot size-- or deleted twice)
 *   integrity -> parent / firstChild / lastChild / size don't agree. Usual cause: lastChild (or firstChild)
 *                not updated when the removed child was the last / first / only one.
 *
 * RULES THESE TESTS ASSUME
 *   - Only the DIRECT children of p are searched (not grandchildren).
 *   - Only the FIRST child with that value is looked at. If it is a leaf it is removed and the function returns true.
 *   - If that first match has children of its own: return false and change nothing (like removeLeaf).
 *   - If no child has that value, or p is nullptr: return false and change nothing.
 *   - Removing the only child makes p a leaf (firstChild AND lastChild become nullptr).
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
    cout << "=== DRILL: removeChildWithVal ===" << endl;

    beginTest("remove the FIRST child by value (1 in 1 2 3, all leaves)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        t.addChild(r, 1);
        Node *b = t.addChild(r, 2);
        Node *c = t.addChild(r, 3);
        bool ok = t.removeChildWithVal(r, 1);
        expectBool("returned", ok, true);
        expectStr("children of root", kids(r), "2 3");
        expectBool("root.firstChild is 2", r->firstChild == b, true);
        expectBool("root.lastChild is 3", r->lastChild == c, true);
        expectTree(t, 3);
    }
    endTest();

    beginTest("remove a MIDDLE child by value (3 in 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = t.removeChildWithVal(s.n1, 3);
        expectBool("returned", ok, true);
        expectStr("children of 1", kids(s.n1), "2 4");
        expectBool("1.firstChild is 2", s.n1->firstChild == s.n2, true);
        expectBool("1.lastChild is 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove the LAST child by value (6 under 2: children 5 6)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = t.removeChildWithVal(s.n2, 6);
        expectBool("returned", ok, true);
        expectStr("children of 2", kids(s.n2), "5");
        expectBool("2.lastChild is 5", s.n2->lastChild == s.n5, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove the FIRST child by value when it IS a leaf (5 under 2)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = t.removeChildWithVal(s.n2, 5);
        expectBool("returned", ok, true);
        expectStr("children of 2", kids(s.n2), "6");
        expectBool("2.firstChild is 6", s.n2->firstChild == s.n6, true);
        expectBool("2.lastChild is 6", s.n2->lastChild == s.n6, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("remove an ONLY child by value (7 under 4): parent becomes a leaf");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = t.removeChildWithVal(s.n4, 7);
        expectBool("returned", ok, true);
        expectStr("children of 4", kids(s.n4), "(none)");
        expectBool("4.firstChild is nullptr", s.n4->firstChild == nullptr, true);
        expectBool("4.lastChild is nullptr", s.n4->lastChild == nullptr, true);
        expectBool("isLeaf(4)", t.isLeaf(s.n4), true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("value not found among the children: returns false, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(s.n1, 99); });
        expectBool("returned", ok, false);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectTree(t, 7);
    }
    endTest();

    beginTest("only DIRECT children count (5 is a grandchild of 1): returns false");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(s.n1, 5); });
        expectBool("returned", ok, false);
        expectStr("children of 2", kids(s.n2), "5 6");
        expectTree(t, 7);
    }
    endTest();

    beginTest("the matching child has children (2 under 1): returns false, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(s.n1, 2); });
        expectBool("returned", ok, false);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectStr("children of 2", kids(s.n2), "5 6");
        expectTree(t, 7);
    }
    endTest();

    beginTest("p has no children (leaf 3): returns false");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(s.n3, 1); });
        expectBool("returned", ok, false);
        expectTree(t, 7);
    }
    endTest();

    beginTest("p == nullptr returns false");
    {
        GeneralTree t;
        buildSample(t);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(nullptr, 1); });
        expectBool("returned", ok, false);
        expectTree(t, 7);
    }
    endTest();

    beginTest("DUPLICATES: only the FIRST child with that value goes (7 3 7 7 5 7 -> 3 7 7 5 7)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        int vals[] = { 7, 3, 7, 7, 5, 7 };
        for (int v : vals) t.addChild(r, v);
        bool ok = t.removeChildWithVal(r, 7);
        expectBool("returned", ok, true);
        expectStr("children of root", kids(r), "3 7 7 5 7");
        expectTree(t, 6);
    }
    endTest();

    beginTest("DUPLICATES where the first match has children: returns false (the later leaf 2 stays)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        Node *a = t.addChild(r, 2);
        t.addChild(r, 5);
        t.addChild(r, 2);
        t.addChild(a, 9);
        bool ok = true;
        capture([&] { ok = t.removeChildWithVal(r, 2); });
        expectBool("returned", ok, false);
        expectStr("children of root", kids(r), "2 5 2");
        expectTree(t, 5);
    }
    endTest();

    beginTest("tail still works: remove the last child by value, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeChildWithVal(s.n2, 6);
        t.addChild(s.n2, 9);
        expectStr("children of 2", kids(s.n2), "5 9");
        expectTree(t, 7);
    }
    endTest();

    beginTest("tail still works: remove the only child by value, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.removeChildWithVal(s.n4, 7);
        t.addChild(s.n4, 8);
        expectStr("children of 4", kids(s.n4), "8");
        expectTree(t, 7);
    }
    endTest();

    beginTest("remove every child of root 0 (children 1..5), one by one, in the order 3 1 5 2 4");
    {
        GeneralTree t;
        Node *r = buildWide(t, 5);
        int order[] = { 3, 1, 5, 2, 4 };
        string want[] = { "1 2 4 5", "2 4 5", "2 4", "4", "(none)" };
        int k = 0;
        for (int v : order)
        {
            expectBool("remove " + to_string(v), t.removeChildWithVal(r, v), true);
            expectStr("children after removing " + to_string(v), kids(r), want[k++]);
            expectTree(t, 6 - k);
        }
        expectBool("root.lastChild is nullptr", r->lastChild == nullptr, true);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
