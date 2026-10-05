/*
 * DRILL: addChildAtPosition
 * ------------------------------------------------------------------
 * addChildAtPosition(p, val, pos): make a new node holding val and insert it into p's child list so that
 *   it becomes the pos-th child (pos counts from 1). Returns the new node.
 *   It is "insert into a linked list at an index" again, with the same edge cases: empty list, front, middle, back.
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
 *   returned  -> wrong return value (e.g. you should have returned nullptr)
 *   children  -> the child list of p doesn't read the way it should (wrong order / missing node)
 *   size      -> getSize() is wrong (forgot size++, or you counted a node you didn't add)
 *   integrity -> parent / firstChild / lastChild / size don't agree. Usual cause: lastChild (or firstChild)
 *                not updated when you insert into an EMPTY list or at the FRONT / BACK.
 *
 * RULES THESE TESTS ASSUME
 *   - pos is 1-BASED: pos = 1 makes the new node the FIRST child; pos = (number of children + 1) puts it LAST.
 *   - Valid positions are 1 .. (number of children + 1). Anything else (0, negative, too big): return nullptr
 *     and change nothing (no node created, size unchanged).
 *   - If p is nullptr: return nullptr.
 *   - A leaf has 0 children, so pos = 1 is the only valid position and the new node is its first AND last child.
 *   - The new node's parent must be p. size goes up by 1.
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
    cout << "=== DRILL: addChildAtPosition ===" << endl;

    beginTest("insert at the FRONT (pos 1) of 2's children 5 6");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAtPosition(s.n2, 9, 1);
        expectBool("returned a node", n != nullptr, true);
        expectInt("returned elem", elemOf(n), 9);
        expectStr("children of 2", kids(s.n2), "9 5 6");
        expectBool("2.firstChild is the new node", s.n2->firstChild == n, true);
        expectBool("2.lastChild is still 6", s.n2->lastChild == s.n6, true);
        expectBool("new node's parent is 2", n != nullptr && n->parent == s.n2, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("insert in the MIDDLE (pos 2) of 1's children 2 3 4");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAtPosition(s.n1, 9, 2);
        expectInt("returned elem", elemOf(n), 9);
        expectStr("children of 1", kids(s.n1), "2 9 3 4");
        expectBool("1.firstChild is still 2", s.n1->firstChild == s.n2, true);
        expectBool("1.lastChild is still 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("insert in the MIDDLE (pos 3) of 1's children 2 3 4");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAtPosition(s.n1, 9, 3);
        expectStr("children of 1", kids(s.n1), "2 3 9 4");
        expectBool("1.lastChild is still 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("insert at the BACK (pos = children + 1) of 2's children 5 6");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAtPosition(s.n2, 9, 3);
        expectStr("children of 2", kids(s.n2), "5 6 9");
        expectBool("2.lastChild is the new node", s.n2->lastChild == n, true);
        expectBool("2.firstChild is still 5", s.n2->firstChild == s.n5, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("EMPTY child list: leaf 3, pos 1");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAtPosition(s.n3, 9, 1);
        expectStr("children of 3", kids(s.n3), "9");
        expectBool("3.firstChild is the new node", s.n3->firstChild == n, true);
        expectBool("3.lastChild is the new node", s.n3->lastChild == n, true);
        expectBool("isLeaf(3)", t.isLeaf(s.n3), false);
        expectTree(t, 8);
    }
    endTest();

    beginTest("ONE child (7 under 4): pos 1 puts it before, pos 2 puts it after");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAtPosition(s.n4, 8, 1);
        expectStr("children of 4 after pos 1", kids(s.n4), "8 7");
        expectBool("4.lastChild is 7", s.n4->lastChild == s.n7, true);
        t.addChildAtPosition(s.n4, 9, 3);
        expectStr("children of 4 after pos 3", kids(s.n4), "8 7 9");
        expectTree(t, 9);
    }
    endTest();

    beginTest("pos = 0 or negative: returns nullptr, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *a = s.n1, *b = s.n1;
        capture([&] { a = t.addChildAtPosition(s.n1, 9, 0); b = t.addChildAtPosition(s.n1, 9, -3); });
        expectBool("pos 0 returned nullptr", a == nullptr, true);
        expectBool("pos -3 returned nullptr", b == nullptr, true);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectTree(t, 7);
    }
    endTest();

    beginTest("pos too big (children + 2 and 100): returns nullptr, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *a = s.n1, *b = s.n3, *c = s.n1;
        capture([&] {
            a = t.addChildAtPosition(s.n1, 9, 5);       // 1 has 3 children: valid positions are 1..4
            b = t.addChildAtPosition(s.n3, 9, 2);       // 3 is a leaf: only pos 1 is valid
            c = t.addChildAtPosition(s.n1, 9, 100);
        });
        expectBool("pos 5 on node 1 returned nullptr", a == nullptr, true);
        expectBool("pos 2 on a leaf returned nullptr", b == nullptr, true);
        expectBool("pos 100 returned nullptr", c == nullptr, true);
        expectStr("children of 1", kids(s.n1), "2 3 4");
        expectStr("children of 3", kids(s.n3), "(none)");
        expectTree(t, 7);
    }
    endTest();

    beginTest("p == nullptr returns nullptr");
    {
        GeneralTree t;
        buildSample(t);
        Node *r = t.getRoot();
        capture([&] { r = t.addChildAtPosition(nullptr, 9, 1); });
        expectBool("returned nullptr", r == nullptr, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("tail still works: insert at the back, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAtPosition(s.n2, 9, 3);
        t.addChild(s.n2, 8);
        expectStr("children of 2", kids(s.n2), "5 6 9 8");
        expectTree(t, 9);
    }
    endTest();

    beginTest("tail still works: insert into an empty list, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAtPosition(s.n3, 9, 1);
        t.addChild(s.n3, 8);
        expectStr("children of 3", kids(s.n3), "9 8");
        expectTree(t, 9);
    }
    endTest();

    beginTest("insert at pos 1 five times: the list comes out reversed");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        for (int i = 1; i <= 5; i++) t.addChildAtPosition(r, i, 1);
        expectStr("children of root", kids(r), "5 4 3 2 1");
        expectTree(t, 6);
    }
    endTest();

    beginTest("insert at the back five times: same as addChild (1 2 3 4 5)");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        for (int i = 1; i <= 5; i++) t.addChildAtPosition(r, i, i);
        expectStr("children of root", kids(r), "1 2 3 4 5");
        expectBool("lastChild is 5", r->lastChild != nullptr && r->lastChild->elem == 5, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("mixed inserts into root 0 with children 1 2 3 4 5");
    {
        GeneralTree t;
        Node *r = buildWide(t, 5);
        t.addChildAtPosition(r, 90, 3);
        expectStr("after 90 at pos 3", kids(r), "1 2 90 3 4 5");
        t.addChildAtPosition(r, 91, 1);
        expectStr("after 91 at pos 1", kids(r), "91 1 2 90 3 4 5");
        t.addChildAtPosition(r, 92, 8);
        expectStr("after 92 at pos 8", kids(r), "91 1 2 90 3 4 5 92");
        t.addChildAtPosition(r, 93, 5);
        expectStr("after 93 at pos 5", kids(r), "91 1 2 90 93 3 4 5 92");
        expectTree(t, 10);
    }
    endTest();

    beginTest("the new node is a real child: it can get children of its own");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAtPosition(s.n1, 9, 2);
        t.addChild(n, 10);
        t.addChildAtPosition(n, 11, 1);
        expectStr("children of the new node", kids(n), "11 10");
        expectInt("depth of 11", t.depth(n->firstChild), 2);
        expectTree(t, 10);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
