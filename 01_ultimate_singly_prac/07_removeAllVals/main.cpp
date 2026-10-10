/*
 * DRILL: removeAllVals
 * ------------------------------------------------------------------
 * int removeAllVals(val): remove EVERY node equal to val, return how many were removed.
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
 *   - Returns the number of nodes removed (0 if val isn't there or the list is empty).
 *   - Matches can sit at the head, in the middle, at the tail, or be consecutive; all must go.
 *   - head / tail / size must be right afterwards (including when everything was removed).
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
    cout << "=== DRILL: removeAllVals ===" << endl;

    beginTest("value NOT in the list");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 3});
        int r = -99;
        capture([&] { r = l.removeAllVals(9); });
        expectInt("removed count", r, 0);
        expectList(l, "1 -> 2 -> 3 -> NULL", 3);
    }
    endTest();

    beginTest("one match in the middle");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 3});
        int r = -99;
        capture([&] { r = l.removeAllVals(2); });
        expectInt("removed count", r, 1);
        expectList(l, "1 -> 3 -> NULL", 2);
    }
    endTest();

    beginTest("one match at the head");
    {
        SinglyLinkedList l;
        fill(l, {5, 1, 2});
        int r = -99;
        capture([&] { r = l.removeAllVals(5); });
        expectInt("removed count", r, 1);
        expectList(l, "1 -> 2 -> NULL", 2);
    }
    endTest();

    beginTest("one match at the tail");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 5});
        int r = -99;
        capture([&] { r = l.removeAllVals(5); });
        expectInt("removed count", r, 1);
        expectList(l, "1 -> 2 -> NULL", 2);
    }
    endTest();

    beginTest("matches at head, middle AND tail");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 1, 3, 1});
        int r = -99;
        capture([&] { r = l.removeAllVals(1); });
        expectInt("removed count", r, 3);
        expectList(l, "2 -> 3 -> NULL", 2);
    }
    endTest();

    beginTest("consecutive matches at the FRONT");
    {
        SinglyLinkedList l;
        fill(l, {7, 7, 7, 1});
        int r = -99;
        capture([&] { r = l.removeAllVals(7); });
        expectInt("removed count", r, 3);
        expectList(l, "1 -> NULL", 1);
    }
    endTest();

    beginTest("consecutive matches in the MIDDLE");
    {
        SinglyLinkedList l;
        fill(l, {1, 5, 5, 2});
        int r = -99;
        capture([&] { r = l.removeAllVals(5); });
        expectInt("removed count", r, 2);
        expectList(l, "1 -> 2 -> NULL", 2);
    }
    endTest();

    beginTest("consecutive matches at the BACK");
    {
        SinglyLinkedList l;
        fill(l, {5, 6, 6, 6});
        int r = -99;
        capture([&] { r = l.removeAllVals(6); });
        expectInt("removed count", r, 3);
        expectList(l, "5 -> NULL", 1);
    }
    endTest();

    beginTest("EVERY node matches (list empties)");
    {
        SinglyLinkedList l;
        fill(l, {7, 7, 7});
        int r = -99;
        capture([&] { r = l.removeAllVals(7); });
        expectInt("removed count", r, 3);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("ONE node, it matches");
    {
        SinglyLinkedList l;
        fill(l, {5});
        int r = -99;
        capture([&] { r = l.removeAllVals(5); });
        expectInt("removed count", r, 1);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("ONE node, no match");
    {
        SinglyLinkedList l;
        fill(l, {5});
        int r = -99;
        capture([&] { r = l.removeAllVals(6); });
        expectInt("removed count", r, 0);
        expectList(l, "5 -> NULL", 1);
    }
    endTest();

    beginTest("EMPTY list (must not crash)");
    {
        SinglyLinkedList l;
        fill(l, {});
        int r = -99;
        capture([&] { r = l.removeAllVals(5); });
        expectInt("removed count", r, 0);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("only the given value goes, order of the rest is kept");
    {
        SinglyLinkedList l;
        fill(l, {3, 1, 3, 2, 3, 1});
        int r = -99;
        capture([&] { r = l.removeAllVals(3); });
        expectInt("removed count", r, 3);
        expectList(l, "1 -> 2 -> 1 -> NULL", 3);
    }
    endTest();

    beginTest("tail moved correctly: remove matches at the back, then addBack(9)");
    {
        SinglyLinkedList l;
        fill(l, {2, 3, 3});
        capture([&] { l.removeAllVals(3); });
        l.addBack(9);
        expectList(l, "2 -> 9 -> NULL", 2);
    }
    endTest();

    beginTest("list works again after everything was removed");
    {
        SinglyLinkedList l;
        fill(l, {4, 4});
        capture([&] { l.removeAllVals(4); });
        l.addBack(1);
        l.addBack(2);
        expectList(l, "1 -> 2 -> NULL", 2);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
