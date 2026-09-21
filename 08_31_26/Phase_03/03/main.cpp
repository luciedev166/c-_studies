/*
 * ============================================================
 *  PHASE 3 — PROBLEM 3 (hard)
 *  ALLOCATION MATH + FIXED ARRAY -> DYNAMIC MEMORY
 *  SPEC — build Record and RecordBook from scratch.
 * ============================================================
 *
 *   struct Record — multiple fields on purpose, so a wrong
 *   malloc(capacity) instead of malloc(capacity * sizeof(Record))
 *   corrupts data almost immediately instead of "mostly working":
 *              int    id;
 *              double value;
 *
 *   class RecordBook
 *     private:
 *       static const int FIXED_CAP = 3;
 *       Record fixed[FIXED_CAP];  // STACK/member storage. NOT heap.
 *                                  // Cannot be passed to realloc().
 *       Record* heap;             // nullptr until we outgrow 'fixed'
 *       bool    usingHeap;        // starts false
 *       int     capacity;         // starts at FIXED_CAP
 *       int     size;             // starts at 0
 *
 *     public:
 *       RecordBook();
 *       ~RecordBook(); // free(heap) ONLY if usingHeap is true.
 *                       // 'fixed' must never be freed -- it isn't heap.
 *
 *       void add(Record r);
 *         // If size < capacity: just store into whichever storage
 *         // (fixed or heap) is currently active. No allocation.
 *         //
 *         // If size == capacity, grow FIRST, THEN store:
 *         //   - If !usingHeap (still on 'fixed'):
 *         //       malloc a NEW heap block of (capacity * 2) records
 *         //       -- correct math is capacity * sizeof(Record).
 *         //       Manually copy all 'size' existing records out of
 *         //       'fixed' into the new heap block (memcpy or a loop --
 *         //       'fixed' cannot be handed to realloc()).
 *         //       Set usingHeap = true, capacity *= 2.
 *         //   - If usingHeap: realloc the existing heap block to
 *         //       (capacity * 2) records -- again, capacity * sizeof(Record).
 *         //       capacity *= 2.
 *
 *       int    getSize() const;
 *       int    getCapacity() const;
 *       bool   isUsingHeap() const;
 *
 *       Record getMin() const;     // by .value. Throws std::out_of_range if empty.
 *       Record getMax() const;     // by .value. Throws std::out_of_range if empty.
 *       double getAverage() const; // Throws std::out_of_range if empty.
 *
 * Header should be named RecordBook.hpp.
 * ============================================================
 */

#include "RecordBook.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << boolalpha;

    // ================= SECTION A: starts on the fixed array =================
    RecordBook book;
    cout << "size: " << book.getSize() << endl;         // Expected: size: 0
    cout << "capacity: " << book.getCapacity() << endl; // Expected: capacity: 3
    cout << "usingHeap: " << book.isUsingHeap() << endl; // Expected: usingHeap: false

    // ================= SECTION B: filling the fixed array (no allocation yet) =================
    book.add({1, 10.0});
    book.add({2, 20.0});
    book.add({3, 30.0}); // size == capacity(3) now, but add() itself doesn't grow yet
    cout << "size: " << book.getSize() << endl;         // Expected: size: 3
    cout << "capacity: " << book.getCapacity() << endl; // Expected: capacity: 3
    cout << "usingHeap: " << book.isUsingHeap() << endl; // Expected: usingHeap: false

    // ================= SECTION C: the fixed -> heap transition =================
    // size(3) == capacity(3) -> this add() must grow FIRST:
    //   malloc a heap block, copy all 3 existing records out of 'fixed'
    //   by hand, THEN store the 4th record.
    book.add({4, 40.0});
    cout << "size: " << book.getSize() << endl;         // Expected: size: 4
    cout << "capacity: " << book.getCapacity() << endl; // Expected: capacity: 6
    cout << "usingHeap: " << book.isUsingHeap() << endl; // Expected: usingHeap: true
    // If the copy out of 'fixed' was wrong (or the malloc math used
    // capacity instead of capacity * sizeof(Record)), these stats will
    // be wrong/garbage even though nothing crashed:
    cout << "min id: " << book.getMin().id << endl;   // Expected: min id: 1
    cout << "max id: " << book.getMax().id << endl;   // Expected: max id: 4
    cout << "average: " << book.getAverage() << endl; // Expected: average: 25

    // ================= SECTION D: growth again, now via realloc =================
    // size(4) < capacity(6): plain adds, no allocation.
    book.add({5, 50.0});
    book.add({6, 60.0}); // size == capacity(6) now
    cout << "size: " << book.getSize() << endl;          // Expected: size: 6
    cout << "usingHeap: " << book.isUsingHeap() << endl;  // Expected: usingHeap: true

    // size(6) == capacity(6), and we're already on the heap -> this
    // add() must realloc (not malloc-and-copy-by-hand) to grow.
    book.add({7, 70.0});
    cout << "size: " << book.getSize() << endl;         // Expected: size: 7
    cout << "capacity: " << book.getCapacity() << endl; // Expected: capacity: 12
    // All earlier records must have survived the realloc intact:
    cout << "min id: " << book.getMin().id << endl;   // Expected: min id: 1
    cout << "max id: " << book.getMax().id << endl;   // Expected: max id: 7
    cout << "average: " << book.getAverage() << endl; // Expected: average: 40

    // ================= SECTION E: single-element edge case =================
    RecordBook single;
    single.add({99, 5.5});
    cout << "single min == max? "
         << (single.getMin().id == single.getMax().id) << endl; // Expected: true
    cout << "single average: " << single.getAverage() << endl;   // Expected: single average: 5.5

    // ================= SECTION F: empty edge case =================
    RecordBook empty;
    try {
        empty.getAverage();
        cout << "no exception thrown (unexpected)" << endl;
    } catch (const std::out_of_range& e) {
        cout << "caught out_of_range on empty getAverage()" << endl;
        // Expected: caught out_of_range on empty getAverage()
    }

    return 0;
}