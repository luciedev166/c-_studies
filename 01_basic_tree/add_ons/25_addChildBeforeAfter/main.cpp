/*
 * DRILL: addChildBefore, addChildAfter
 * ------------------------------------------------------------------
 * addChildBefore(ref, val): insert a new node holding val as the sibling RIGHT BEFORE ref. Returns the new node.
 * addChildAfter(ref, val)    : insert a new node holding val as the sibling RIGHT AFTER ref.  Returns the new node.
 *   ref is a node that is already in the tree; the new node ends up in the same child list as ref
 *   (same parent). Before = same edge cases as "insert at the front"; After = same edge cases as "insert at the back".
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
 *   children  -> the child list doesn't read the way it should (wrong order / missing node)
 *   size      -> getSize() is wrong
 *   integrity -> parent / firstChild / lastChild / size don't agree. Usual cause: BEFORE the first child
 *                (parent's firstChild must move) or AFTER the last child (parent's lastChild must move).
 *
 * RULES THESE TESTS ASSUME
 *   - The new node becomes a sibling of ref, so its parent is ref's parent. size goes up by 1.
 *   - The root has no siblings (and no parent whose list could hold a new node): addChildBefore(root, ...)
 *     and addChildAfter(root, ...) return nullptr and change nothing. Same for ref == nullptr.
 *   - If ref is the first child, addChildBefore makes the new node the parent's firstChild.
 *   - If ref is the last child, addChildAfter makes the new node the parent's lastChild.
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
    cout << "=== DRILL: addChildBefore, addChildAfter ===" << endl;

    beginTest("addChildAfter a MIDDLE child (after 3 in 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAfter(s.n3, 9);
        expectInt("returned elem", elemOf(n), 9);
        expectStr("children of 1", kids(s.n1), "2 3 9 4");
        expectBool("new node's parent is 1", n != nullptr && n->parent == s.n1, true);
        expectBool("1.lastChild is still 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildAfter the FIRST child (after 2 in 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAfter(s.n2, 9);
        expectStr("children of 1", kids(s.n1), "2 9 3 4");
        expectBool("1.firstChild is still 2", s.n1->firstChild == s.n2, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildAfter the LAST child (after 4 in 2 3 4): the tail moves");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAfter(s.n4, 9);
        expectStr("children of 1", kids(s.n1), "2 3 4 9");
        expectBool("1.lastChild is the new node", s.n1->lastChild == n, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildAfter an ONLY child (after 7 under 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildAfter(s.n7, 9);
        expectStr("children of 4", kids(s.n4), "7 9");
        expectBool("4.firstChild is still 7", s.n4->firstChild == s.n7, true);
        expectBool("4.lastChild is the new node", s.n4->lastChild == n, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildBefore a MIDDLE child (before 3 in 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildBefore(s.n3, 9);
        expectInt("returned elem", elemOf(n), 9);
        expectStr("children of 1", kids(s.n1), "2 9 3 4");
        expectBool("new node's parent is 1", n != nullptr && n->parent == s.n1, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildBefore the FIRST child (before 2 in 2 3 4): firstChild moves");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildBefore(s.n2, 9);
        expectStr("children of 1", kids(s.n1), "9 2 3 4");
        expectBool("1.firstChild is the new node", s.n1->firstChild == n, true);
        expectBool("1.lastChild is still 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildBefore the LAST child (before 4 in 2 3 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildBefore(s.n4, 9);
        expectStr("children of 1", kids(s.n1), "2 3 9 4");
        expectBool("1.lastChild is still 4", s.n1->lastChild == s.n4, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("addChildBefore an ONLY child (before 7 under 4)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *n = t.addChildBefore(s.n7, 9);
        expectStr("children of 4", kids(s.n4), "9 7");
        expectBool("4.firstChild is the new node", s.n4->firstChild == n, true);
        expectBool("4.lastChild is still 7", s.n4->lastChild == s.n7, true);
        expectTree(t, 8);
    }
    endTest();

    beginTest("the ROOT has no siblings: both return nullptr, nothing changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *a = s.n1, *b = s.n1;
        capture([&] { a = t.addChildBefore(s.n1, 9); b = t.addChildAfter(s.n1, 9); });
        expectBool("addChildBefore(root) returned nullptr", a == nullptr, true);
        expectBool("addChildAfter(root) returned nullptr", b == nullptr, true);
        expectBool("root is still 1", t.getRoot() == s.n1, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("a one-node tree: the lone root still has no siblings");
    {
        GeneralTree t;
        Node *r = t.addRoot(5);
        Node *a = r, *b = r;
        capture([&] { a = t.addChildBefore(r, 9); b = t.addChildAfter(r, 9); });
        expectBool("before returned nullptr", a == nullptr, true);
        expectBool("after returned nullptr", b == nullptr, true);
        expectTree(t, 1);
    }
    endTest();

    beginTest("ref == nullptr: both return nullptr");
    {
        GeneralTree t;
        buildSample(t);
        Node *a = t.getRoot(), *b = t.getRoot();
        capture([&] { a = t.addChildBefore(nullptr, 9); b = t.addChildAfter(nullptr, 9); });
        expectBool("addChildBefore returned nullptr", a == nullptr, true);
        expectBool("addChildAfter returned nullptr", b == nullptr, true);
        expectTree(t, 7);
    }
    endTest();

    beginTest("tail still works: addChildAfter the last child, then addChild");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAfter(s.n4, 9);
        t.addChild(s.n1, 8);
        expectStr("children of 1", kids(s.n1), "2 3 4 9 8");
        expectTree(t, 9);
    }
    endTest();

    beginTest("head still works: addChildBefore the first child, then addChildBefore it again");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        Node *a = t.addChildBefore(s.n2, 9);
        t.addChildBefore(a, 8);
        expectStr("children of 1", kids(s.n1), "8 9 2 3 4");
        expectTree(t, 9);
    }
    endTest();

    beginTest("chain it: build 1 2 3 4 5 under a root using only before / after");
    {
        GeneralTree t;
        Node *r = t.addRoot(0);
        Node *three = t.addChild(r, 3);
        Node *two = t.addChildBefore(three, 2);
        t.addChildBefore(two, 1);
        Node *four = t.addChildAfter(three, 4);
        t.addChildAfter(four, 5);
        expectStr("children of root", kids(r), "1 2 3 4 5");
        expectBool("lastChild is 5", r->lastChild != nullptr && r->lastChild->elem == 5, true);
        expectTree(t, 6);
    }
    endTest();

    beginTest("works deep in the tree (after 5 and before 6, both under node 2)");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChildAfter(s.n5, 55);
        t.addChildBefore(s.n6, 56);
        expectStr("children of 2", kids(s.n2), "5 55 56 6");
        expectInt("depth of 55", t.depth(s.n2->firstChild->nextSibling), 2);
        expectTree(t, 9);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
