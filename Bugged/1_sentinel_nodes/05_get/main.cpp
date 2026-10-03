/*
 * DRILL: get
 * ------------------------------------------------------------------
 * int get(pos): return the value at index pos (0-based). The list is NOT changed.
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
 *   - pos < 0 or pos >= size (or empty list): return -1.
 *   - Bonus idea (not tested, can't be): walk from whichever end is closer to pos.
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
    cout << "=== DRILL: get ===" << endl;

    beginTest("pos 0 (head) of 10..50");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(0); });
        expectInt("returned", r, 10);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("pos 1");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(1); });
        expectInt("returned", r, 20);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("pos 2 (middle)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(2); });
        expectInt("returned", r, 30);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("pos 3 (second to last)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(3); });
        expectInt("returned", r, 40);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("pos 4 (last index)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(4); });
        expectInt("returned", r, 50);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("INVALID pos == size (5): returns -1, list unchanged");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(5); });
        expectInt("returned", r, -1);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("INVALID negative pos (-1): returns -1");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(-1); });
        expectInt("returned", r, -1);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("INVALID pos way out of range (100)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40, 50});
        int r = -99;
        capture([&] { r = l.get(100); });
        expectInt("returned", r, -1);
        expectList(l, "10 -> 20 -> 30 -> 40 -> 50 -> NULL", 5);
    }
    endTest();

    beginTest("ONE node, pos 0");
    {
        DoublyLinkedList l;
        fill(l, {7});
        int r = -99;
        capture([&] { r = l.get(0); });
        expectInt("returned", r, 7);
        expectList(l, "7 -> NULL", 1);
    }
    endTest();

    beginTest("ONE node, pos 1 (invalid)");
    {
        DoublyLinkedList l;
        fill(l, {7});
        int r = -99;
        capture([&] { r = l.get(1); });
        expectInt("returned", r, -1);
        expectList(l, "7 -> NULL", 1);
    }
    endTest();

    beginTest("EMPTY list, pos 0 (must not crash)");
    {
        DoublyLinkedList l;
        int r = -99;
        capture([&] { r = l.get(0); });
        expectInt("returned", r, -1);
        expectList(l, "NULL", 0);
    }
    endTest();

    beginTest("stored value 0 comes back as 0");
    {
        DoublyLinkedList l;
        fill(l, {0, 1});
        int r = -99;
        capture([&] { r = l.get(0); });
        expectInt("returned", r, 0);
        expectList(l, "0 -> 1 -> NULL", 2);
    }
    endTest();

    beginTest("every position of 1..10 (front half AND back half)");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        for (int i = 0; i < 10; i++)
        {
            int r = -99;
            capture([&] { r = l.get(i); });
            expectInt("get(" + to_string(i) + ")", r, i + 1);
        }
        expectList(l, "1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> NULL", 10);
    }
    endTest();

    beginTest("get() never changes the list: 5 gets in a row");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        capture([&] { l.get(0); l.get(1); l.get(2); l.get(3); l.get(-1); });
        expectList(l, "1 -> 2 -> 3 -> NULL", 3);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
