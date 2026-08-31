/*
 * ============================================================
 *  PHASE 2 — PROBLEM 2 (medium)
 *  SPEC — build Student and Roster from scratch.
 * ============================================================
 *
 *   struct Student — holds:
 *              string name
 *              int    age
 *              char   gender     // stored AFTER validation, see addStudent
 *            (brace-init like {"Ana", 16, 'f'}.)
 *
 *   class Roster
 *     private:
 *       Student* students;   // heap array, malloc/calloc/realloc only
 *       int      capacity;
 *       int      size;
 *       bool     locked;     // starts false
 *
 *     public:
 *       Roster(int initialCapacity); // locked = false
 *       ~Roster();
 *
 *       bool addStudent(Student s);
 *         // If locked, change nothing and return false immediately.
 *         // Otherwise: validate s.gender — only lowercase 'm' or 'f'
 *         // are valid. Anything else (including uppercase 'M'/'F',
 *         // digits, whatever) gets FORCED to 'm' before storing.
 *         // Grow by GROWTH = 2 first if size == capacity. Append,
 *         // return true.
 *
 *       void finalizeRoster();  // sets locked = true. No way back.
 *       bool isLocked() const;
 *
 *       int    getSize() const;
 *       int    getCapacity() const;
 *       int    countMinors() const;      // age < 18
 *       double getAverageAge() const;    // must NOT truncate. Assume non-empty.
 *
 *       void display() const;
 *         // one line per student: "{name} ({gender}, {age})"
 *
 *       string describeStudent(int index) const;
 *         // assume valid index. EXACT sentence format:
 *         // "{name} is a {male|female} student and is a {minor|major}."
 *         // (minor if age < 18, else major)
 *
 * Header should be named Roster.hpp (or split Student.hpp — your call).
 * ============================================================
 */

#include "Roster.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << boolalpha;

    Roster room(2);

    // ================= SECTION A: gender validation/defaulting =================
    room.addStudent({"Ana", 16, 'f'});
    room.addStudent({"Ben", 20, 'X'}); // invalid char -> must be forced to 'm'
    room.display();
    // Expected:
    // Ana (f, 16)
    // Ben (m, 20)
    cout << "capacity: " << room.getCapacity() << endl; // Expected: capacity: 2

    // size == capacity -> next add must grow first.
    room.addStudent({"Cara", 15, 'f'});
    room.display();
    // Expected:
    // Ana (f, 16)
    // Ben (m, 20)
    // Cara (f, 15)
    cout << "capacity: " << room.getCapacity() << endl; // Expected: capacity: 4

    room.addStudent({"Dex", 22, 'm'}); // size(3) < capacity(4), no grow
    cout << "size: " << room.getSize() << endl;         // Expected: size: 4
    cout << "capacity: " << room.getCapacity() << endl; // Expected: capacity: 4

    // ================= SECTION B: descriptive sentences =================
    cout << room.describeStudent(0) << endl; // Expected: Ana is a female student and is a minor.
    cout << room.describeStudent(1) << endl; // Expected: Ben is a male student and is a major.
    // ^ Ben's gender was force-defaulted to 'm' back in Section A — that
    //   defaulting has to persist into every later read, not just display().
    cout << room.describeStudent(3) << endl; // Expected: Dex is a male student and is a major.

    // ================= SECTION C: aggregate stats =================
    cout << "countMinors: " << room.countMinors() << endl; // Expected: countMinors: 2
    // Ana(16) and Cara(15) are minors; Ben(20) and Dex(22) are not.
    cout << "averageAge: " << room.getAverageAge() << endl; // Expected: averageAge: 18.25
    // (16+20+15+22)/4 = 73/4 = 18.25

    // ================= SECTION D: locking is permanent =================
    room.finalizeRoster();
    cout << "isLocked: " << room.isLocked() << endl; // Expected: isLocked: true

    bool addedAfterLock = room.addStudent({"Eli", 14, 'm'});
    cout << "added after lock: " << addedAfterLock << endl; // Expected: added after lock: false
    cout << "size: " << room.getSize() << endl;               // Expected: size: 4 (unchanged — Eli never got in)
    room.display();
    // Expected (unchanged, no Eli):
    // Ana (f, 16)
    // Ben (m, 20)
    // Cara (f, 15)
    // Dex (m, 22)

    return 0;
}