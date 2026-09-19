// ============================================================================
//  PSET 19 - removeBetween(n1, n2), removeOutside(n1, n2)        (doubly linked list)
//
//  You only edit LinkedList.hpp. This file just runs the tests and prints
//  PASS / FAIL for every case.
//
//      g++ -std=c++17 -Wall -o test main.cpp && ./test
//
//  Stricter build (Linux / Mac / WSL) - also catches use-after-free and leaks,
//  which a plain build can miss (e.g. a prev pointer left pointing at a deleted node):
//      g++ -std=c++17 -Wall -g -fsanitize=address,undefined -o test main.cpp && ./test
//
//  What this file expects LinkedList to have:
//
//      void removeBetween(int n1, int n2)
//          remove everything STRICTLY between n1 and n2 (both stay)
//      void removeOutside(int n1, int n2)
//          keep only n1 .. n2 (inclusive); remove everything before n1 and after n2
//      n1 = first node holding n1; n2 = first node holding n2 from n1 onward.
//      If either isn't found, or n2 comes before n1: nothing changes.
//
//  Also used for setup only: add, clear, print, traverseBackward
// ============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include "LinkedList.hpp"
using namespace std;

// ---- the tools used below (they live at the bottom of this file) --------------
void test(string title, bool scored = true);             // start a new case  (add , false = shown only, not scored)
void setup(LinkedList& list, vector<int> values);        // empty the list, then fill it (via add)
void expectList(LinkedList& list, vector<int> want);     // list must be exactly this (forward + backward)
void showList(LinkedList& list);                         // just show the list, never fails
void note(string text);                                  // just print a remark
void startTests(string title);                           // print title, switch on the prev-pointer check
int  summary();                                          // print PASS/FAIL totals

int main() {
    LinkedList list;
    startTests("removeBetween(n1, n2), removeOutside(n1, n2)");

    test("EMPTY list: no crash (both)");
    setup(list, {});
    list.removeBetween(1, 5);
    list.removeOutside(1, 5);
    expectList(list, {});

    test("n2 not found: unchanged");
    setup(list, {1, 2, 3});
    list.removeBetween(1, 99);
    expectList(list, {1, 2, 3});

    test("n1 and n2 adjacent: nothing strictly between, no-op");
    setup(list, {1, 2, 3});
    list.removeBetween(1, 2);
    expectList(list, {1, 2, 3});

    test("removeOutside with n1 == n2: keeps just that one node");
    setup(list, {1, 2, 3});
    list.removeOutside(2, 2);
    expectList(list, {2});

    test("n1 comes AFTER n2 (NOT SCORED - your spec decides; it just mustn't crash)", false);
    setup(list, {1, 2, 3, 4, 5});
    list.removeBetween(4, 2);
    showList(list);
    note("common choices: no-op (unchanged), or swap the two and carry on");

    test("Normal case removeBetween(1, 5): removes 2, 3, 4");
    setup(list, {1, 2, 3, 4, 5});
    list.removeBetween(1, 5);
    expectList(list, {1, 5});

    test("Normal case removeOutside(2, 4): removes 1 and 5; head/tail must be right (append afterwards)");
    setup(list, {1, 2, 3, 4, 5});
    list.removeOutside(2, 4);
    expectList(list, {2, 3, 4});
    list.add(9);
    expectList(list, {2, 3, 4, 9});

    return summary();
}

// ============================================================================
//  TEST PLUMBING  -  nothing below this line needs reading.
//  It runs your print() into a buffer, compares the NUMBERS in it with what
//  is expected (so your arrows / NULL / brackets don't matter), and prints
//  PASS / FAIL. Each case prints its title BEFORE running, so if your code
//  crashes or hangs, the last line on screen is the case that did it.
// ============================================================================
static int    totalCases = 0, passedCases = 0;
static bool   caseOpen = false, caseOK = true, caseScored = true;
static bool   checkBackward = false;
static string caseTitle;
static vector<string> failedCases;

void endCase() {
    if (!caseOpen) return;
    if (!caseScored) {
        cout << "    -> (not scored - check by eye)" << endl;
    } else {
        totalCases++;
        if (caseOK) { passedCases++; cout << "    -> PASS" << endl; }
        else        { failedCases.push_back(caseTitle); cout << "    -> FAIL" << endl; }
    }
    caseOpen = false;
}

void fail(string why) {
    caseOK = false;
    cout << "    !! " << why << endl;
}

void test(string title, bool scored) {
    endCase();
    caseTitle = "[" + to_string(totalCases + 1) + "] " + title;
    caseOK = true;
    caseScored = scored;
    caseOpen = true;
    cout << endl << (scored ? caseTitle : "[-] " + title) << endl;
}

void note(string text) {
    cout << "    note : " << text << endl;
}

// ---- turning text / lists into each other ---------------------------------
vector<int> numbersIn(string text) {
    vector<int> found;
    size_t i = 0;
    while (i < text.size()) {
        if (isdigit((unsigned char)text[i])) {
            int value = 0;
            while (i < text.size() && isdigit((unsigned char)text[i])) {
                value = value * 10 + (text[i] - '0');
                i++;
            }
            found.push_back(value);
        } else {
            i++;
        }
    }
    return found;
}

string show(vector<int> v) {
    if (v.empty()) return "(empty)";
    string s;
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) s += " -> ";
        s += to_string(v[i]);
    }
    return s;
}

string oneLine(string s) {
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] == '\n' || s[i] == '\r') s[i] = ' ';
    size_t b = s.find_first_not_of(' ');
    size_t e = s.find_last_not_of(' ');
    if (b == string::npos) return "(nothing printed)";
    s = s.substr(b, e - b + 1);
    if (s.size() > 110) s = s.substr(0, 110) + " ...(cut off)";
    return s;
}

// ---- catching what print() / traverseBackward() write to cout -------------
// The buffer refuses to grow past 3000 characters, so a list whose links loop
// back on themselves gets REPORTED instead of printing forever.
struct TooMuchOutput {};

struct LimitedBuffer : public streambuf {
    string text;
    int_type overflow(int_type c) override {
        if (c == traits_type::eof()) return 0;
        text.push_back((char)c);
        if (text.size() > 3000) throw TooMuchOutput();
        return c;
    }
};

string capture(LinkedList& list, bool backward, string& problem) {
    LimitedBuffer buf;
    streambuf* realCout = cout.rdbuf(&buf);
    cout.clear();
    cout.exceptions(ios::badbit);
    try {
        if (backward) list.traverseBackward();
        else          list.print();
    } catch (TooMuchOutput&) {
        problem = "output never ended (a next/prev link loops back on itself?)";
    } catch (...) {
        problem = "it threw an exception";
    }
    cout.exceptions(ios::goodbit);
    cout.clear();
    cout.rdbuf(realCout);
    return buf.text;
}

// ---- the tools main() uses -------------------------------------------------
void startTests(string title) {
    cout << "TEST: " << title << endl;
    cout << "backward check: " << flush;
    // The backward (prev pointer) check is only switched on if traverseBackward()
    // already works: a throwaway list 1,2,3 has to print 3 -> 2 -> 1.
    LinkedList probe;
    probe.add(1);
    probe.add(2);
    probe.add(3);
    string problem;
    string out = capture(probe, true, problem);
    checkBackward = (problem == "" && numbersIn(out) == vector<int>{3, 2, 1});
    if (checkBackward) cout << "ON  (prev pointers get verified after every list check)" << endl;
    else               cout << "OFF (traverseBackward() doesn't print 3 -> 2 -> 1 yet)" << endl;
}

void setup(LinkedList& list, vector<int> values) {
    list.clear();
    for (size_t i = 0; i < values.size(); i++) list.add(values[i]);
    cout << "    start: " << show(values) << endl;
}

void expectList(LinkedList& list, vector<int> want) {
    string problem;
    cout << "    got  : " << flush;
    string fwd = capture(list, false, problem);
    cout << oneLine(fwd) << endl;
    cout << "    want : " << show(want) << endl;
    if (problem != "")                fail("print(): " + problem);
    else if (numbersIn(fwd) != want)  fail("forward list is wrong");

    if (checkBackward) {
        vector<int> reversed(want.rbegin(), want.rend());
        string backProblem;
        cout << "    back : " << flush;
        string bwd = capture(list, true, backProblem);
        cout << oneLine(bwd) << endl;
        if (backProblem != "")               fail("traverseBackward(): " + backProblem);
        else if (numbersIn(bwd) != reversed) fail("backward walk is wrong - a prev pointer (or the tail) is off; want " + show(reversed));
    }
}

void showList(LinkedList& list) {
    string problem;
    cout << "    got  : " << flush;
    string fwd = capture(list, false, problem);
    cout << oneLine(fwd) << endl;
    if (problem != "") fail("print(): " + problem);
}

void expectBackward(LinkedList& list, vector<int> want) {
    string problem;
    cout << "    back : " << flush;
    string bwd = capture(list, true, problem);
    cout << oneLine(bwd) << endl;
    cout << "    want : " << show(want) << endl;
    if (problem != "")                 fail("traverseBackward(): " + problem);
    else if (numbersIn(bwd) != want)   fail("backward walk is wrong");
}

void expectInt(int got, int want) {
    cout << "    returned: " << got << "    want: " << want << endl;
    if (got != want) fail("wrong return value");
}

void expectBool(bool got, bool want) {
    cout << "    returned: " << (got ? "true" : "false")
         << "    want: " << (want ? "true" : "false") << endl;
    if (got != want) fail("wrong return value");
}

void expectStillWorks(LinkedList& list) {
    cout << "    then : list.add(7)   (an emptied list must still accept inserts)" << endl;
    list.add(7);
    expectList(list, {7});
}

int summary() {
    endCase();
    cout << endl << "============================================================" << endl;
    if (passedCases == totalCases) {
        cout << "ALL PASSED  (" << passedCases << "/" << totalCases << ")" << endl;
    } else {
        cout << passedCases << "/" << totalCases << " passed. Failed:" << endl;
        for (size_t i = 0; i < failedCases.size(); i++) cout << "   " << failedCases[i] << endl;
    }
    return passedCases == totalCases ? 0 : 1;
}
