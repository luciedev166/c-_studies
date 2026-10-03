/*
 * DRILL: printFromHead
 * ------------------------------------------------------------------
 * printFromHead(): print the list front to back.
 *
 * HOW TO USE
 *   1. Open LinkedList.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function, then comment it out and write your own.
 *   2. Compile and run (AddressSanitizer also catches leaks and bad memory access):
 *        g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
 *      No ASan on your machine? Just drop "-fsanitize=address".
 *   3. Every test prints one line: PASS, or FAIL plus what you returned vs. what was expected.
 *      If the program CRASHES, the last "- ..." line on screen is the test that crashed it.
 *
 * WHAT A FAIL MEANS
 *   list      -> printFromHead() output didn't match what the list should look like
 *   size      -> getSize() is wrong (forgot size++ / size--)
 *   integrity -> head/tail/size don't agree. Usual cause: tail (or head) not updated
 *                in an edge case (empty list, one node, first/last position).
 *                integrity() is a test-only helper below the DO NOT CHANGE line.
 *
 * RULES THESE TESTS ASSUME
 *   - Format: each value followed by " -> ", then "NULL", then a newline.
 *   -    10 -> 20 -> 30 -> NULL     (empty list: just  NULL )
 *   - Printing must not change the list.
 */

#include <iostream>
#include <sstream>
#include <string>
#include <initializer_list>
#include "LinkedList.hpp"
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

// The list as text via printFromHead, e.g. "10 -> 20 -> NULL"
inline string show(SinglyLinkedList &l)
{
    string s = capture([&] { l.printFromHead(); });
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// The three things that should be true after (almost) every operation.
inline void expectList(SinglyLinkedList &l, const string &wantText, int wantSize)
{
    expectStr("list", show(l), wantText);
    expectInt("size", l.getSize(), wantSize);
    expectStr("integrity", l.integrity(), "OK");
}

// Build a list from values. Uses addBack from the helper section (below the DO NOT CHANGE line).
inline void fill(SinglyLinkedList &l, initializer_list<int> v)
{
    for (int x : v) l.addBack(x);
}


// ------------------------------------------------------------------
//  The tests
// ------------------------------------------------------------------
int main()
{
    cout << "=== DRILL: printFromHead ===" << endl;

    beginTest("EMPTY list prints just NULL");
    {
        SinglyLinkedList l;
        fill(l, {});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 0);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("ONE node");
    {
        SinglyLinkedList l;
        fill(l, {7});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "7 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 1);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("TWO nodes");
    {
        SinglyLinkedList l;
        fill(l, {1, 2});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "1 -> 2 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 2);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("several nodes");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "10 -> 20 -> 30 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 3);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("zero and negative values");
    {
        SinglyLinkedList l;
        fill(l, {-5, 0, 5});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "-5 -> 0 -> 5 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 3);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("printing twice gives the same output");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 3});
        string first  = capture([&] { l.printFromHead(); });
        string second = capture([&] { l.printFromHead(); });
        expectStr("first print", first, "1 -> 2 -> 3 -> NULL\n");
        expectStr("second print", second, "1 -> 2 -> 3 -> NULL\n");
        expectInt("size", l.getSize(), 3);
    }
    endTest();

    beginTest("output follows the list as it changes");
    {
        SinglyLinkedList l;
        fill(l, {2, 3});
        string before = capture([&] { l.printFromHead(); });
        l.addFront(1);
        string after = capture([&] { l.printFromHead(); });
        expectStr("before addFront", before, "2 -> 3 -> NULL\n");
        expectStr("after addFront", after, "1 -> 2 -> 3 -> NULL\n");
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
