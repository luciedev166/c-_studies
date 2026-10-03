/*
 * DRILL: removeHead
 * ------------------------------------------------------------------
 * int removeHead(): remove the FIRST node and return its value.
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
 *   integrity                 -> head/tail/size/prev/next don't agree. Usual cause: head, tail or a prev pointer
 *                not updated in an edge case (empty list, one node, first/last position).
 *                                integrity() is a test-only helper below the DO NOT CHANGE line.
 *   list (backward via prev)  -> reading the list backwards gives the wrong thing: a prev pointer is wrong
 *
 * RULES THESE TESTS ASSUME
 *   - Empty list: return -1 and don't crash (the message you print doesn't matter).
 *   - One node: the list becomes empty and still works afterwards.
 *   - The new first node must not keep a prev pointer into the deleted node.
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
    cout << "=== DRILL: removeHead ===" << endl;

    beginTest("several nodes: removeHead() of 10, 20, 30, 40");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30, 40});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 10);
        expectList(l, "20 -> 30 -> 40 -> NULL", 3);
    }
    endTest();

    beginTest("TWO nodes");
    {
        DoublyLinkedList l;
        fill(l, {10, 20});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 10);
        expectList(l, "20 -> NULL", 1);
    }
    endTest();

    beginTest("ONE node (list becomes empty)");
    {
        DoublyLinkedList l;
        fill(l, {10});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 10);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("EMPTY list (must not crash): returns -1");
    {
        DoublyLinkedList l;
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, -1);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("remove everything one by one, then one more removeHead");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        int r1 = -99;
        capture([&] { r1 = l.removeHead(); });
        expectInt("returned", r1, 1);
        int r2 = -99;
        capture([&] { r2 = l.removeHead(); });
        expectInt("returned", r2, 2);
        int r3 = -99;
        capture([&] { r3 = l.removeHead(); });
        expectInt("returned", r3, 3);
        expectList(l, "NULL", 0);
        int r4 = -99;
        capture([&] { r4 = l.removeHead(); });
        expectInt("returned", r4, -1);
        expectList(l, "NULL", 0);
    }
    endTest();

    beginTest("new head has no prev: removeHead then removeTail on 10, 20");
    {
        DoublyLinkedList l;
        fill(l, {10, 20});
        int r1 = -99;
        capture([&] { r1 = l.removeHead(); });
        expectInt("returned", r1, 10);
        int r2 = -99;
        capture([&] { r2 = l.removeTail(); });
        expectInt("returned", r2, 20);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("list works again after being emptied");
    {
        DoublyLinkedList l;
        fill(l, {4});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 4);
        l.addTail(1);
        l.addHead(0);
        expectList(l, "0 -> 1 -> NULL", 2);
    }
    endTest();

    beginTest("only ONE node goes when values repeat: 5, 5, 5");
    {
        DoublyLinkedList l;
        fill(l, {5, 5, 5});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 5);
        expectList(l, "5 -> 5 -> NULL", 2);
    }
    endTest();

    beginTest("zero is a real value (not 'empty')");
    {
        DoublyLinkedList l;
        fill(l, {0, -3});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 0);
        expectList(l, "-3 -> NULL", 1);
    }
    endTest();

    beginTest("tail still right: removeHead on 10, 20, then addTail(30)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20});
        int r = -99;
        capture([&] { r = l.removeHead(); });
        expectInt("returned", r, 10);
        l.addTail(30);
        expectList(l, "20 -> 30 -> NULL", 2);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
