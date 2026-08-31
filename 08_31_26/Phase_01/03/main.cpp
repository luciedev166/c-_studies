// TO DO /*
//  * ============================================================
//  *  PHASE 1 — PROBLEM 3 (medium)
//  *  SPEC — build Task and TaskList from scratch.
//  * ============================================================
//  *
//  *   struct Task — holds:
//  *              int    id
//  *              string label
//  *            (brace-init like {1, "Wash dishes"}.)
//  *
//  *   class TaskList
//  *     private:
//  *       Task* tasks;     // heap array, malloc/calloc/realloc only
//  *       int   capacity;
//  *       int   size;
//  *
//  *     public:
//  *       TaskList(int initialCapacity);
//  *       ~TaskList();
//  *
//  *       bool add(Task t);
//  *         // Always appends at the end (no sorting). Grow by GROWTH = 2
//  *         // first if size == capacity. Always returns true.
//  *
//  *       bool removeById(int id);
//  *         // Find the task with this id. If not found, return false and
//  *         // leave the list completely unchanged.
//  *         // If found at index i: SHIFT LEFT everything after it by one
//  *         // (tasks[i] = tasks[i+1], tasks[i+1] = tasks[i+2], ...) to
//  *         // close the gap, then size--. This is the mirror direction of
//  *         // an insert-shift — don't just overwrite tasks[i] and leave a
//  *         // stale duplicate sitting at the old last index.
//  *         // Return true.
//  *
//  *       int  indexOf(int id) const;  // -1 if not found
//  *       int  getSize() const;
//  *       int  getCapacity() const;
//  *       bool isEmpty() const;
//  *
//  *       void display() const;
//  *         // one line per task: "ID {id}: {label}"
//  *
//  *       void clear();
//  *         // logical clear only (size = 0), capacity untouched —
//  *         // same rule as Phase 1 Problem 1.
//  *
//  * Header should be named TaskList.hpp (or split Task.hpp — your call).
//  * ============================================================
//  */

#include "TaskList.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << boolalpha;

    // ================= SECTION A: fill exactly to capacity, then grow =================
    TaskList list(3);

    list.add({1, "Wash dishes"});
    list.add({2, "Buy groceries"});
    list.add({3, "Walk dog"});
    list.display();
    // Expected:
    // ID 1: Wash dishes
    // ID 2: Buy groceries
    // ID 3: Walk dog
    cout << "capacity: " << list.getCapacity() << endl; // Expected: capacity: 3
    cout << "size: " << list.getSize() << endl;         // Expected: size: 3

    list.add({4, "Pay bills"}); // size == capacity -> must grow first
    list.display();
    // Expected: adds "ID 4: Pay bills" as a 4th line
    cout << "capacity: " << list.getCapacity() << endl; // Expected: capacity: 5

    // ================= SECTION B: remove from the MIDDLE =================
    // Removing id 2 (index 1) must shift id 3 and id 4 left by one —
    // no gap, no stale duplicate left at the old end.
    bool removed = list.removeById(2);
    cout << "removed 2: " << removed << endl; // Expected: removed 2: true
    list.display();
    // Expected:
    // ID 1: Wash dishes
    // ID 3: Walk dog
    // ID 4: Pay bills
    cout << "size: " << list.getSize() << endl;         // Expected: size: 3
    cout << "capacity: " << list.getCapacity() << endl; // Expected: capacity: 5
    // ^ removal never shrinks capacity — same "logical" philosophy as clear().

    // removing something that isn't there — list must stay untouched.
    bool removedGhost = list.removeById(99);
    cout << "removed 99: " << removedGhost << endl; // Expected: removed 99: false
    list.display();
    // Expected (unchanged):
    // ID 1: Wash dishes
    // ID 3: Walk dog
    // ID 4: Pay bills

    // indices should reflect the CURRENT positions after the shift, not
    // the original insertion order.
    cout << "indexOf(4): " << list.indexOf(4) << endl; // Expected: indexOf(4): 2
    cout << "indexOf(2): " << list.indexOf(2) << endl; // Expected: indexOf(2): -1 (removed earlier)

    // ================= SECTION C: remove from the FRONT =================
    list.add({5, "Feed cat"}); // size(3) < capacity(5), no grow
    list.display();
    // Expected:
    // ID 1: Wash dishes
    // ID 3: Walk dog
    // ID 4: Pay bills
    // ID 5: Feed cat

    list.removeById(1); // index 0 — everything shifts left by one
    list.display();
    // Expected:
    // ID 3: Walk dog
    // ID 4: Pay bills
    // ID 5: Feed cat
    cout << "size: " << list.getSize() << endl; // Expected: size: 3

    // ================= SECTION D: clear + reuse =================
    list.clear();
    cout << "isEmpty after clear: " << list.isEmpty() << endl;      // Expected: true
    cout << "capacity after clear: " << list.getCapacity() << endl; // Expected: capacity after clear: 5
    list.display(); // Expected: (blank line)

    list.add({10, "New task"});
    list.display();                                      // Expected: ID 10: New task
    cout << "capacity: " << list.getCapacity() << endl;   // Expected: capacity: 5
    // still 5 — reused existing memory, didn't allocate fresh.

    return 0;
}