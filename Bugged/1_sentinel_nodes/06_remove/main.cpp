/*
 * DRILL: remove
 * ------------------------------------------------------------------
 * int remove(num): remove the FIRST node whose value equals num (removing by VALUE, not position).
 *
 * HOW TO USE
 *   1. Open LinkedList.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function, then comment it out and write your own.
 *   (This drill's practice area also holds the primitive removeNode. Rewrite it too if you want;
 *    only your remove calls it, so a bug there can't break the helpers.)
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
 *   - Returns num if a node was removed, -1 if num isn't in the list (or list is empty).
 *   - Only the first match goes; later duplicates stay.
 *   - Tests never store -1, so "-1" always means "not found".
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
    cout << "=== DRILL: remove ===" << endl;

    beginTest("value at the HEAD: remove(10) from 10, 20, 30");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        int r = -99;
        capture([&] { r = l.remove(10); });
        expectInt("returned", r, 10);
        expectList(l, "20 -> 30 -> NULL", 2);
    }
    endTest();

    beginTest("value in the MIDDLE: remove(20)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        int r = -99;
        capture([&] { r = l.remove(20); });
        expectInt("returned", r, 20);
        expectList(l, "10 -> 30 -> NULL", 2);
    }
    endTest();

    beginTest("value at the TAIL: remove(30)");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        int r = -99;
        capture([&] { r = l.remove(30); });
        expectInt("returned", r, 30);
        expectList(l, "10 -> 20 -> NULL", 2);
    }
    endTest();

    beginTest("value NOT in the list: returns -1, list unchanged");
    {
        DoublyLinkedList l;
        fill(l, {10, 20, 30});
        int r = -99;
        capture([&] { r = l.remove(99); });
        expectInt("returned", r, -1);
        expectList(l, "10 -> 20 -> 30 -> NULL", 3);
    }
    endTest();

    beginTest("only the FIRST match goes: remove(5) from 5, 1, 5, 2, 5");
    {
        DoublyLinkedList l;
        fill(l, {5, 1, 5, 2, 5});
        int r = -99;
        capture([&] { r = l.remove(5); });
        expectInt("returned", r, 5);
        expectList(l, "1 -> 5 -> 2 -> 5 -> NULL", 4);
    }
    endTest();

    beginTest("first match is in the middle: remove(2) from 1, 2, 3, 2");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3, 2});
        int r = -99;
        capture([&] { r = l.remove(2); });
        expectInt("returned", r, 2);
        expectList(l, "1 -> 3 -> 2 -> NULL", 3);
    }
    endTest();

    beginTest("ONE node, it matches (list becomes empty)");
    {
        DoublyLinkedList l;
        fill(l, {7});
        int r = -99;
        capture([&] { r = l.remove(7); });
        expectInt("returned", r, 7);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("ONE node, no match");
    {
        DoublyLinkedList l;
        fill(l, {7});
        int r = -99;
        capture([&] { r = l.remove(8); });
        expectInt("returned", r, -1);
        expectList(l, "7 -> NULL", 1);
    }
    endTest();

    beginTest("EMPTY list (must not crash): returns -1");
    {
        DoublyLinkedList l;
        int r = -99;
        capture([&] { r = l.remove(5); });
        expectInt("returned", r, -1);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    beginTest("same value twice: remove(4) x3 on 4, 4, 9");
    {
        DoublyLinkedList l;
        fill(l, {4, 4, 9});
        int r1 = -99;
        capture([&] { r1 = l.remove(4); });
        expectInt("returned", r1, 4);
        int r2 = -99;
        capture([&] { r2 = l.remove(4); });
        expectInt("returned", r2, 4);
        int r3 = -99;
        capture([&] { r3 = l.remove(4); });
        expectInt("returned", r3, -1);
        expectList(l, "9 -> NULL", 1);
    }
    endTest();

    beginTest("value 0 is a real value");
    {
        DoublyLinkedList l;
        fill(l, {1, 0, 2});
        int r = -99;
        capture([&] { r = l.remove(0); });
        expectInt("returned", r, 0);
        expectList(l, "1 -> 2 -> NULL", 2);
    }
    endTest();

    beginTest("tail moved correctly: remove(3) from 1, 2, 3, then addTail(9)");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        int r = -99;
        capture([&] { r = l.remove(3); });
        expectInt("returned", r, 3);
        l.addTail(9);
        expectList(l, "1 -> 2 -> 9 -> NULL", 3);
    }
    endTest();

    beginTest("head moved correctly: remove(1) from 1, 2, 3, then addHead(0)");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        int r = -99;
        capture([&] { r = l.remove(1); });
        expectInt("returned", r, 1);
        l.addHead(0);
        expectList(l, "0 -> 2 -> 3 -> NULL", 3);
    }
    endTest();

    beginTest("remove every node one by one (2, then 1, then 3)");
    {
        DoublyLinkedList l;
        fill(l, {1, 2, 3});
        int r1 = -99;
        capture([&] { r1 = l.remove(2); });
        expectInt("returned", r1, 2);
        int r2 = -99;
        capture([&] { r2 = l.remove(1); });
        expectInt("returned", r2, 1);
        int r3 = -99;
        capture([&] { r3 = l.remove(3); });
        expectInt("returned", r3, 3);
        expectList(l, "NULL", 0);
        expectBool("isEmpty()", l.isEmpty(), true);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
