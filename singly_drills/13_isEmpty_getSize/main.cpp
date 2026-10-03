/*
 * DRILL: isEmpty + getSize
 * ------------------------------------------------------------------
 * bool isEmpty() and int getSize().
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
 *   - isEmpty(): true only when the list has no nodes. getSize(): number of nodes.
 *   - Both must stay correct through adds, removals and clear().
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
    cout << "=== DRILL: isEmpty + getSize ===" << endl;

    beginTest("fresh list: isEmpty() true, getSize() 0");
    {
        SinglyLinkedList l;
        expectBool("isEmpty()", l.isEmpty(), true);
        expectInt("getSize()", l.getSize(), 0);
    }
    endTest();

    beginTest("after one addBack: false, 1");
    {
        SinglyLinkedList l;
        l.addBack(5);
        expectBool("isEmpty()", l.isEmpty(), false);
        expectInt("getSize()", l.getSize(), 1);
    }
    endTest();

    beginTest("size grows with every add (addBack x3, addFront x1)");
    {
        SinglyLinkedList l;
        l.addBack(1);
        expectInt("after 1 add", l.getSize(), 1);
        l.addBack(2);
        expectInt("after 2 adds", l.getSize(), 2);
        l.addBack(3);
        expectInt("after 3 adds", l.getSize(), 3);
        l.addFront(0);
        expectInt("after 4 adds", l.getSize(), 4);
        expectBool("isEmpty()", l.isEmpty(), false);
    }
    endTest();

    beginTest("size shrinks with every removal, isEmpty() flips at the end");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 3});
        capture([&] { l.removeFront(); });
        expectInt("after removeFront", l.getSize(), 2);
        capture([&] { l.removeBack(); });
        expectInt("after removeBack", l.getSize(), 1);
        expectBool("isEmpty() with 1 node left", l.isEmpty(), false);
        capture([&] { l.removeFront(); });
        expectInt("after last removal", l.getSize(), 0);
        expectBool("isEmpty() when empty", l.isEmpty(), true);
    }
    endTest();

    beginTest("empty again after clear()");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 3});
        l.clear();
        expectBool("isEmpty()", l.isEmpty(), true);
        expectInt("getSize()", l.getSize(), 0);
    }
    endTest();

    beginTest("a removal that fails does not change the size");
    {
        SinglyLinkedList l;
        capture([&] { l.removeFront(); });
        expectInt("empty list after failed removeFront", l.getSize(), 0);
        expectBool("isEmpty()", l.isEmpty(), true);
        fill(l, {1, 2});
        capture([&] { l.removeNreturnN(5); });
        expectInt("after invalid removeNreturnN(5)", l.getSize(), 2);
    }
    endTest();

    beginTest("insertAtN adds 1 to the size; removeAllVals subtracts the count");
    {
        SinglyLinkedList l;
        fill(l, {1, 2, 2, 3});
        expectInt("start", l.getSize(), 4);
        l.insertAtN(9, 1);
        expectInt("after insertAtN", l.getSize(), 5);
        capture([&] { l.removeAllVals(2); });
        expectInt("after removeAllVals(2)", l.getSize(), 3);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
