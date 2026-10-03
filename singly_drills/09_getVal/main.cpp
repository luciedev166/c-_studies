/*
 * DRILL: getVal
 * ------------------------------------------------------------------
 * bool getVal(val): return true if val is somewhere in the list (the list is NOT changed).
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
 *   - Returns true if val is anywhere in the list, otherwise false. Empty list: false.
 *   - Never removes or changes anything.
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
    cout << "=== DRILL: getVal ===" << endl;

    beginTest("value at the HEAD (10 in 10, 20, 30)");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        bool r = l.getVal(10);
        expectBool("returned", r, true);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("value in the MIDDLE (20)");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        bool r = l.getVal(20);
        expectBool("returned", r, true);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("value at the TAIL (30)");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        bool r = l.getVal(30);
        expectBool("returned", r, true);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("value NOT in the list (99)");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        bool r = l.getVal(99);
        expectBool("returned", r, false);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("negative value NOT in the list (-5)");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        bool r = l.getVal(-5);
        expectBool("returned", r, false);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("EMPTY list (must not crash)");
    {
        SinglyLinkedList l;
        fill(l, {});
        bool r = l.getVal(5);
        expectBool("returned", r, false);
        expectList(l, "NULL", 0);
    }
    endTest();

    beginTest("ONE node, matching");
    {
        SinglyLinkedList l;
        fill(l, {7});
        bool r = l.getVal(7);
        expectBool("returned", r, true);
        expectList(l, "7 -> NULL", 1);
    }
    endTest();

    beginTest("ONE node, not matching");
    {
        SinglyLinkedList l;
        fill(l, {7});
        bool r = l.getVal(8);
        expectBool("returned", r, false);
        expectList(l, "7 -> NULL", 1);
    }
    endTest();

    beginTest("duplicates: 5 in 5, 5, 7");
    {
        SinglyLinkedList l;
        fill(l, {5, 5, 7});
        bool r = l.getVal(5);
        expectBool("returned", r, true);
        expectList(l, "5 -> 5 -> 7 -> NULL", 3);
    }
    endTest();

    beginTest("stored value 0 is found (0 is not 'null')");
    {
        SinglyLinkedList l;
        fill(l, {0, 1});
        bool r = l.getVal(0);
        expectBool("returned", r, true);
        expectList(l, "0 -> 1 -> NULL", 2);
    }
    endTest();

    beginTest("2 is not in 0, 1");
    {
        SinglyLinkedList l;
        fill(l, {0, 1});
        bool r = l.getVal(2);
        expectBool("returned", r, false);
        expectList(l, "0 -> 1 -> NULL", 2);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
