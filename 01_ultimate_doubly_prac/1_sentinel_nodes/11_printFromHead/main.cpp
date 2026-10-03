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
 *   list                      -> printFromHead() output didn't match what the list should look like
 *   size                      -> getSize() is wrong (forgot size++ / size--)
 *   integrity                 -> next pointers, prev pointers and size don't agree. Usual cause: forgot to set a prev
 *                (or a next), forgot size++ / size--, or left a neighbor pointing at a deleted node.
 *                                integrity() is a test-only helper below the DO NOT CHANGE line.
 *   list (backward via prev)  -> reading the list backwards gives the wrong thing: a prev pointer is wrong
 *
 * RULES THESE TESTS ASSUME
 *   - Format: 10 -> 20 -> 30 -> NULL  (arrow with spaces, NULL at the end, newline).
 *   - Empty list prints just NULL.
 *   - Printing must not change the list.
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
inline string show(DoublyLinkedList &l)
{
    string s = capture([&] { l.printFromHead(); });
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// "10 -> 20 -> 30 -> NULL"  becomes  "30 -> 20 -> 10 -> NULL"
inline string reverseText(const string &t)
{
    const string d = " -> ";
    vector<string> parts;
    string s = t;
    size_t p;
    while ((p = s.find(d)) != string::npos) { parts.push_back(s.substr(0, p)); s.erase(0, p + d.size()); }
    string out;
    for (int i = (int)parts.size() - 1; i >= 0; i--) out += parts[i] + d;
    return out + "NULL";
}

// The things that should be true after (almost) every operation:
//   list      = forward text (printFromHead) is right
//   size      = getSize() is right
//   integrity = next / prev pointers and size agree with each other
//   backward  = reading through the prev pointers gives the same values reversed
inline void expectList(DoublyLinkedList &l, const string &wantText, int wantSize)
{
    expectStr("list", show(l), wantText);
    expectInt("size", l.getSize(), wantSize);
    string integ = l.integrity();
    expectStr("integrity", integ, "OK");
    if (integ == "OK") expectStr("list (backward via prev)", l.backwardText(), reverseText(wantText));
}

// Build a list from values. Uses addTail from the helper section (below the DO NOT CHANGE line).
inline void fill(DoublyLinkedList &l, initializer_list<int> v)
{
    for (int x : v) l.addTail(x);
}


// ------------------------------------------------------------------
//  The tests
// ------------------------------------------------------------------
int main()
{
    cout << "=== DRILL: printFromHead ===" << endl;

    beginTest("EMPTY list prints just NULL");
    {
        DoublyLinkedList l;
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 0);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("ONE node");
    {
        DoublyLinkedList l;
        fill(l, {7});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "7 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 1);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("TWO nodes");
    {
        DoublyLinkedList l;
        fill(l, {1, 2});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "1 -> 2 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 2);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("several nodes (10, 20, 30)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "10 -> 20 -> 30 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 3);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("zero and negative values");
    {
        DoublyLinkedList l;
        fill(l, {-5, 0, 5});
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, "-5 -> 0 -> 5 -> NULL\n");
        expectInt("size (printing must not change the list)", l.getSize(), 3);
        expectStr("integrity", l.integrity(), "OK");
    }
    endTest();

    beginTest("printing twice gives the same output");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        string first  = capture([&] { l.printFromHead(); });
        string second = capture([&] { l.printFromHead(); });
        expectStr("first print", first, "1 -> 2 -> 3 -> NULL\n");
        expectStr("second print", second, "1 -> 2 -> 3 -> NULL\n");
    }
    endTest();

    beginTest("BIG list (500 nodes) prints 1 up to 500");
    {
        DoublyLinkedList l;
        string want;
        for (int i = 1; i <= 500; i++) l.addTail(i);
        for (int i = 1; i <= 500; i++) want += to_string(i) + " -> ";
        want += "NULL\n";
        string out = capture([&] { l.printFromHead(); });
        expectStr("printed", out, want);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
