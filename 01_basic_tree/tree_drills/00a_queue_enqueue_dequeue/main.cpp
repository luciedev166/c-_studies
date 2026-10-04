/*
 * DRILL: enqueue, dequeue
 * ------------------------------------------------------------------
 * enqueue(val): join the BACK of the line.  dequeue(): the FRONT leaves; return its value (FIFO = first in, first out).
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
 *   - dequeue on an empty queue returns -1 and changes nothing (the message you print is ignored).
 *   - enqueue adds at the BACK (tail). dequeue removes from the FRONT (head).
 *   - Print format: front first, e.g. "10 -> 20 -> 30 -> NULL". An empty queue prints just "NULL".
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
    cout << "=== DRILL: enqueue, dequeue ===" << endl;

    beginTest("EMPTY queue: enqueue(5)");
    {
        Queue q;
        q.enqueue(5);
        expectQueue(q, "5 -> NULL", 1);
    }
    endTest();

    beginTest("ONE item: enqueue(20) goes BEHIND 10");
    {
        Queue q;
        q.enqueue(10);
        q.enqueue(20);
        expectQueue(q, "10 -> 20 -> NULL", 2);
    }
    endTest();

    beginTest("several items stay in the order they arrived");
    {
        Queue q;
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        q.enqueue(4);
        expectQueue(q, "1 -> 2 -> 3 -> 4 -> NULL", 4);
    }
    endTest();

    beginTest("dequeue on an EMPTY queue returns -1");
    {
        Queue q;
        int v = 0;
        capture([&] { v = q.dequeue(); });
        expectInt("returned", v, -1);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    beginTest("dequeue returns the FRONT item (10), not the back");
    {
        Queue q;
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        int v = q.dequeue();
        expectInt("returned", v, 10);
        expectQueue(q, "20 -> 30 -> NULL", 2);
    }
    endTest();

    beginTest("FIFO: enqueue 1..5, dequeue comes out 1, 2, 3, 4, 5");
    {
        Queue q;
        for (int i = 1; i <= 5; i++) q.enqueue(i);
        for (int i = 1; i <= 5; i++) expectInt("dequeue #" + to_string(i), q.dequeue(), i);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    beginTest("ONE item: dequeue empties the queue (tail must reset)");
    {
        Queue q;
        q.enqueue(7);
        int v = q.dequeue();
        expectInt("returned", v, 7);
        expectBool("isEmpty()", q.isEmpty(), true);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    beginTest("tail still works: empty the queue, then enqueue again");
    {
        Queue q;
        q.enqueue(1);
        q.dequeue();
        q.enqueue(9);
        q.enqueue(10);
        expectQueue(q, "9 -> 10 -> NULL", 2);
    }
    endTest();

    beginTest("interleaved: enq 1, enq 2, deq, enq 3, deq, deq");
    {
        Queue q;
        q.enqueue(1);
        q.enqueue(2);
        expectInt("deq", q.dequeue(), 1);
        q.enqueue(3);
        expectQueue(q, "2 -> 3 -> NULL", 2);
        expectInt("deq", q.dequeue(), 2);
        expectInt("deq", q.dequeue(), 3);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    beginTest("dequeue on empty twice in a row is safe");
    {
        Queue q;
        capture([&] { q.dequeue(); q.dequeue(); });
        q.enqueue(4);
        expectQueue(q, "4 -> NULL", 1);
    }
    endTest();

    beginTest("zero and negative values");
    {
        Queue q;
        q.enqueue(0);
        q.enqueue(-7);
        expectInt("deq", q.dequeue(), 0);
        expectInt("deq", q.dequeue(), -7);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    beginTest("BIG: 500 in, 500 out, in the same order");
    {
        Queue q;
        for (int i = 1; i <= 500; i++) q.enqueue(i);
        expectInt("size after 500 enqueues", q.getSize(), 500);
        bool inOrder = true;
        for (int i = 1; i <= 500; i++) if (q.dequeue() != i) inOrder = false;
        expectBool("all 500 came out in order", inOrder, true);
        expectQueue(q, "NULL", 0);
    }
    endTest();

    cout << endl << testsPassed << "/" << testsRun << " tests passed" << endl;
    return testsPassed == testsRun ? 0 : 1;
}
