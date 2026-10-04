/*
 * DRILL: front, isEmpty, getSize
 * ------------------------------------------------------------------
 * front(): peek at the front item WITHOUT removing it.  isEmpty(): is the queue empty?  getSize(): how many items?
 *
 * HOW TO USE
 *   1. Open Queue.hpp. Everything ABOVE the "DO NOT CHANGE" line is yours:
 *      read the solved function(s), then comment them out and write your own.
 *   2. Compile and run (AddressSanitizer also catches leaks and bad memory access):
 *        g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
 *      No ASan on your machine? Just drop "-fsanitize=address".
 *   3. Every test prints one line: PASS, or FAIL plus what you returned vs. what was expected.
 *      If the program CRASHES, the last "- ..." line on screen is the test that crashed it.
 *
 * WHAT A FAIL MEANS
 *   queue     -> printQueue() output didn't match what the queue should look like (front -> ... -> back)
 *   size      -> getSize() is wrong (forgot size++ / size--)
 *   integrity -> head / tail / size don't agree. Usual cause: tail (or head) not updated in an edge
 *                case: empty queue, one node. integrity() is a test-only helper below the DO NOT CHANGE line.
 *
 * RULES THESE TESTS ASSUME
 *   - front() on an empty queue returns -1 (the message you print is ignored).
 *   - front() only LOOKS: it must not remove anything or change the size.
 */

#include <iostream>
#include <sstream>
#include <string>
#include "Queue.hpp"
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

// The queue as text via printQueue, e.g. "10 -> 20 -> NULL"
inline string show(Queue &q)
{
    string s = capture([&] { q.printQueue(); });
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// The three things that should be true after (almost) every operation.
inline void expectQueue(Queue &q, const string &wantText, int wantSize)
{
    expectStr("queue", show(q), wantText);
    expectInt("size", q.getSize(), wantSize);
    expectStr("integrity", q.integrity(), "OK");
}


// ------------------------------------------------------------------
//  The tests
// ------------------------------------------------------------------
int main()
{
    cout << "=== DRILL: front, isEmpty, getSize ===" << endl;

    beginTest("EMPTY queue");
    {
        Queue q;
        int v = 0;
        capture([&] { v = q.front(); });
        expectInt("front()", v, -1);
        expectBool("isEmpty()", q.isEmpty(), true);
        expectInt("getSize()", q.getSize(), 0);
    }
    endTest();

    beginTest("ONE item");
    {
        Queue q;
        q.enqueue(5);
        expectInt("front()", q.front(), 5);
        expectBool("isEmpty()", q.isEmpty(), false);
        expectInt("getSize()", q.getSize(), 1);
    }
    endTest();

    beginTest("front is the OLDEST item, not the newest");
    {
        Queue q;
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        expectInt("front()", q.front(), 10);
        expectInt("getSize()", q.getSize(), 3);
    }
    endTest();

    beginTest("front() does not remove anything (call it 3 times)");
    {
        Queue q;
        q.enqueue(10);
        q.enqueue(20);
        q.front();
        q.front();
        expectInt("front() 3rd time", q.front(), 10);
        expectQueue(q, "10 -> 20 -> NULL", 2);
    }
    endTest();

    beginTest("front changes after dequeue");
    {
        Queue q;
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        q.dequeue();
        expectInt("front()", q.front(), 20);
        q.dequeue();
        expectInt("front()", q.front(), 30);
    }
    endTest();

    beginTest("isEmpty goes false -> true -> false");
    {
        Queue q;
        expectBool("isEmpty() at start", q.isEmpty(), true);
        q.enqueue(1);
        expectBool("isEmpty() after enqueue", q.isEmpty(), false);
        q.dequeue();
        expectBool("isEmpty() after dequeue", q.isEmpty(), true);
        q.enqueue(2);
        expectBool("isEmpty() after enqueue again", q.isEmpty(), false);
    }
    endTest();

    beginTest("getSize follows every enqueue and dequeue");
    {
        Queue q;
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        expectInt("after 3 enqueues", q.getSize(), 3);
        q.dequeue();
        expectInt("after 1 dequeue", q.getSize(), 2);
        q.dequeue();
        q.dequeue();
        expectInt("after 3 dequeues", q.getSize(), 0);
    }
    endTest();

    beginTest("front after the queue was emptied and refilled");
    {
        Queue q;
        q.enqueue(1);
        q.dequeue();
        int v = 0;
        capture([&] { v = q.front(); });
        expectInt("front() of the emptied queue", v, -1);
        q.enqueue(8);
        expectInt("front() after enqueue(8)", q.front(), 8);
    }
    endTest();

    beginTest("negative front value");
    {
        Queue q;
        q.enqueue(-4);
        q.enqueue(6);
        expectInt("front()", q.front(), -4);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
