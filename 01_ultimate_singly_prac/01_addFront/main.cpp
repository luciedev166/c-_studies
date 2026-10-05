/*
 * DRILL: addFront
 * ------------------------------------------------------------------
 * addFront(val): put a new node at the FRONT of the list.
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
 *   - On an empty list the new node is both head and tail.
 *   - size goes up by 1.
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
    cout << "=== DRILL: addFront ===" << endl;

    beginTest("EMPTY list: addFront(5)");
    {
        SinglyLinkedList l;
        l.addFront(5);
        expectList(l, "5 -> NULL", 1);
        expectBool("isEmpty()", l.isEmpty(), false);
    }
    endTest();

    beginTest("ONE-node list: addFront(5) in front of 10");
    {
        SinglyLinkedList l;
        fill(l, {10});
        l.addFront(5);
        expectList(l, "5 -> 10 -> NULL", 2);
    }
    endTest();

    beginTest("several nodes: addFront(5) in front of 10, 20, 30");
    {
        SinglyLinkedList l;
        fill(l, {10, 20, 30});
        l.addFront(5);
        expectList(l, "5 -> 10 -> 20 -> 30 -> NULL", 4);
    }
    endTest();

    beginTest("three addFronts in a row reverse the order");
    {
        SinglyLinkedList l;
        l.addFront(1);
        l.addFront(2);
        l.addFront(3);
        expectList(l, "3 -> 2 -> 1 -> NULL", 3);
    }
    endTest();

    beginTest("tail still works: addFront on EMPTY list, then addBack");
    {
        SinglyLinkedList l;
        l.addFront(5);
        l.addBack(6);
        expectList(l, "5 -> 6 -> NULL", 2);
    }
    endTest();

    beginTest("zero and negative values");
    {
        SinglyLinkedList l;
        l.addFront(0);
        l.addFront(-7);
        expectList(l, "-7 -> 0 -> NULL", 2);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
