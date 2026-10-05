/*
 * DRILL: printByLevel, printAtDepth
 * ------------------------------------------------------------------
 * printByLevel()    : print the tree level by level, EACH LEVEL ON ITS OWN LINE, values separated by one space.
 *                        Level order again (drill 05), but now you also have to know where each level ENDS.
 * printAtDepth(k)    : print only the nodes at depth k (the root has depth 0), left to right, on one line.
 *                        Returns how many nodes it printed.
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
 *   printed   -> what your function printed doesn't match the expected text (look for a trailing space, a missing
 *                newline between levels, or a wrong order)
 *   returned  -> printAtDepth returned the wrong count
 *   size / integrity -> the tree changed (these functions only LOOK, they must never modify anything)
 *
 * RULES THESE TESTS ASSUME
 *   - printByLevel: one line per level. Values on a line are separated by a single space, with NO trailing space.
 *     Every line (including the last) ends with a newline. Example for the sample tree:
 *         1
 *         2 3 4
 *         5 6 7
 *     An empty tree prints nothing at all.
 *   - Within a level the nodes come out left to right (children in the order they were added, parents left to right).
 *   - printAtDepth(k): the same format for ONE line, e.g. printAtDepth(2) on the sample tree prints "5 6 7" + newline.
 *     The root is depth 0. If k is negative, or there are no nodes at depth k (or the tree is empty):
 *     print NOTHING (not even a newline) and return 0.
 *   - printAtDepth returns the number of nodes it printed.
 *   - You may use NodeQueue (given, same as drill 05) for printByLevel.
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
    cout << "=== DRILL: printByLevel, printAtDepth ===" << endl;

    beginTest("printByLevel: sample tree");
    {
        GeneralTree t;
        buildSample(t);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "1\n2 3 4\n5 6 7\n");
        expectTree(t, 7);
    }
    endTest();

    beginTest("printByLevel: second tree (levels of size 1, 2, 4, 1)");
    {
        GeneralTree t;
        buildSecond(t);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "10\n20 30\n40 50 60 70\n80\n");
    }
    endTest();

    beginTest("printByLevel: empty tree prints nothing");
    {
        GeneralTree t;
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "");
    }
    endTest();

    beginTest("printByLevel: a one-node tree");
    {
        GeneralTree t;
        t.addRoot(5);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "5\n");
    }
    endTest();

    beginTest("printByLevel: a chain of 4 (one value per line)");
    {
        GeneralTree t;
        buildChain(t, 4);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "1\n2\n3\n4\n");
    }
    endTest();

    beginTest("printByLevel: a wide tree (root, then everyone else on one line)");
    {
        GeneralTree t;
        buildWide(t, 5);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "0\n1 2 3 4 5\n");
    }
    endTest();

    beginTest("printByLevel: uneven tree, a level made of children of DIFFERENT parents, nodes with 0 / 1 / 3 children");
    {
        GeneralTree t;
        Node *r = t.addRoot(1);
        Node *a = t.addChild(r, 2);
        Node *b = t.addChild(r, 3);
        t.addChild(a, 4);
        t.addChild(b, 5);
        t.addChild(b, 6);
        t.addChild(b, 7);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "1\n2 3\n4 5 6 7\n");
    }
    endTest();

    beginTest("printByLevel: no trailing spaces and the last line ends with a newline");
    {
        GeneralTree t;
        buildSample(t);
        string out = capture([&] { t.printByLevel(); });
        expectBool("no ' \\n' anywhere", out.find(" \n") == string::npos, true);
        expectBool("ends with a newline", !out.empty() && out[out.size() - 1] == '\n', true);
        int lines = 0;
        for (char ch : out) if (ch == '\n') lines++;
        expectInt("number of lines", lines, 3);
    }
    endTest();

    beginTest("printByLevel: levels follow the tree as it changes");
    {
        GeneralTree t;
        Sample s = buildSample(t);
        t.addChild(s.n7, 8);
        t.removeLeaf(s.n3);
        string out = capture([&] { t.printByLevel(); });
        expectStr("printed", out, "1\n2 4\n5 6 7\n8\n");
        expectTree(t, 7);
    }
    endTest();

    beginTest("printAtDepth: every depth of the sample tree");
    {
        GeneralTree t;
        buildSample(t);
        int n0 = 0, n1 = 0, n2 = 0;
        expectStr("depth 0", capture([&] { n0 = t.printAtDepth(0); }), "1\n");
        expectStr("depth 1", capture([&] { n1 = t.printAtDepth(1); }), "2 3 4\n");
        expectStr("depth 2", capture([&] { n2 = t.printAtDepth(2); }), "5 6 7\n");
        expectInt("count at depth 0", n0, 1);
        expectInt("count at depth 1", n1, 3);
        expectInt("count at depth 2", n2, 3);
        expectTree(t, 7);
    }
    endTest();

    beginTest("printAtDepth: second tree, depth 2 (children of two different parents) and depth 3");
    {
        GeneralTree t;
        buildSecond(t);
        int a = 0, b = 0;
        expectStr("depth 2", capture([&] { a = t.printAtDepth(2); }), "40 50 60 70\n");
        expectStr("depth 3", capture([&] { b = t.printAtDepth(3); }), "80\n");
        expectInt("count at depth 2", a, 4);
        expectInt("count at depth 3", b, 1);
    }
    endTest();

    beginTest("printAtDepth: deeper than the tree prints nothing and returns 0");
    {
        GeneralTree t;
        buildSample(t);
        int n = -5;
        string out = capture([&] { n = t.printAtDepth(3); });
        expectStr("printed", out, "");
        expectInt("returned", n, 0);
        out = capture([&] { n = t.printAtDepth(50); });
        expectStr("printed (k = 50)", out, "");
        expectInt("returned (k = 50)", n, 0);
    }
    endTest();

    beginTest("printAtDepth: negative k prints nothing and returns 0");
    {
        GeneralTree t;
        buildSample(t);
        int n = -5;
        string out = capture([&] { n = t.printAtDepth(-1); });
        expectStr("printed", out, "");
        expectInt("returned", n, 0);
    }
    endTest();

    beginTest("printAtDepth: empty tree prints nothing and returns 0");
    {
        GeneralTree t;
        int n = -5;
        string out = capture([&] { n = t.printAtDepth(0); });
        expectStr("printed", out, "");
        expectInt("returned", n, 0);
    }
    endTest();

    beginTest("printAtDepth: a chain of 6 (one node at every depth) and a wide tree");
    {
        GeneralTree t;
        buildChain(t, 6);
        int n = 0;
        expectStr("chain, depth 4", capture([&] { n = t.printAtDepth(4); }), "5\n");
        expectInt("count", n, 1);
        GeneralTree u;
        buildWide(u, 4);
        expectStr("wide, depth 1", capture([&] { n = u.printAtDepth(1); }), "1 2 3 4\n");
        expectInt("count", n, 4);
    }
    endTest();

    beginTest("printAtDepth agrees with printByLevel and with depth()");
    {
        GeneralTree t;
        Second s = buildSecond(t);
        string levels = capture([&] { t.printByLevel(); });
        string joined;
        for (int k = 0; k <= 3; k++) joined += capture([&] { t.printAtDepth(k); });
        expectStr("depth lines joined == printByLevel", joined, levels);
        expectInt("depth of 80 is 3", t.depth(s.n80), 3);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
