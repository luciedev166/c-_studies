/*
 * DRILL: addHead
 * ------------------------------------------------------------------
 * addHead(val): put a new node at the FRONT of the list.
 *
 * HOW TO USE
 *   1. Open LinkedList.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function, then comment it out and write your own.
 *   (This drill's practice area also holds the primitive addBetween. Rewrite it too if you want;
 *    only your addHead calls it, so a bug there can't break the helpers.)
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
 *   - On an empty list the new node is both first and last.
 *   - size goes up by 1; the old first node must point BACK at the new node (prev).
 *   - Reading the list backwards (through prev) must give the same values reversed.
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
    cout << "=== DRILL: addHead ===" << endl;

    beginTest("EMPTY list: addHead(5)");
    {
        DoublyLinkedList l;
        l.addHead(5);
        expectList(l, "5 -> NULL", 1);
        expectBool("isEmpty()", l.isEmpty(), false);
    }
    endTest();

    beginTest("ONE-node list: addHead(5) in front of 10");
    {
        DoublyLinkedList l;
        fill(l, {10});
        l.addHead(5);
        expectList(l, "5 -> 10 -> NULL", 2);
    }
    endTest();

    beginTest("several nodes: addHead(5) in front of 10, 20, 30");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        l.addHead(5);
        expectList(l, "5 -> 10 -> 20 -> 30 -> NULL", 4);
    }
    endTest();

    beginTest("three addHeads in a row reverse the order");
    {
        DoublyLinkedList l;
        l.addHead(1);
        l.addHead(2);
        l.addHead(3);
        expectList(l, "3 -> 2 -> 1 -> NULL", 3);
    }
    endTest();

    beginTest("last node still right: addHead on EMPTY list, then addTail");
    {
        DoublyLinkedList l;
        l.addHead(5);
        l.addTail(6);
        expectList(l, "5 -> 6 -> NULL", 2);
    }
    endTest();

    beginTest("prev pointers: addHead twice, removeTail must give the FIRST one added");
    {
        DoublyLinkedList l;
        l.addHead(1);
        l.addHead(2);
        int r = -99;
        capture([&] { r = l.removeTail(); });
        expectInt("returned", r, 1);
        expectList(l, "2 -> NULL", 1);
    }
    endTest();

    beginTest("zero and negative values");
    {
        DoublyLinkedList l;
        l.addHead(0);
        l.addHead(-7);
        expectList(l, "-7 -> 0 -> NULL", 2);
    }
    endTest();

    beginTest("100 addHeads in a row");
    {
        DoublyLinkedList l;
        string want;
        for (int i = 1; i <= 100; i++) l.addHead(i);
        for (int i = 100; i >= 1; i--) want += to_string(i) + " -> ";
        want += "NULL";
        expectList(l, want, 100);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
