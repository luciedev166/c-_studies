// TO DO
/*
 * ============================================================
 *  PHASE 1 — PROBLEM 1 (easy)
 *  SPEC — build DynamicArray from scratch. Nothing below is provided.
 * ============================================================
 *
 * Pure mechanics, no structs, no domain story. Get malloc/calloc/
 * realloc/free bookkeeping into your hands before anything else
 * gets layered on top.
 *
 *   class DynamicArray
 *     private:
 *       int* data;      // heap array — NOT a fixed array, NOT a vector
 *       int  capacity;  // how many ints `data` can currently hold
 *       int  size;      // how many ints are actually in use
 *
 *     public:
 *       DynamicArray(int initialCapacity);
 *         // calloc's `data` to initialCapacity ints (zero-initialized).
 *         // capacity = initialCapacity, size = 0.
 *
 *       ~DynamicArray();
 *         // frees data.
 *
 *       bool add(int value);
 *         // If size == capacity: GROW FIRST — realloc data to
 *         // (capacity + GROWTH) ints where GROWTH = 3, update capacity.
 *         // THEN place value at data[size], size++.
 *         // Always returns true. Still check realloc's return isn't
 *         // NULL before committing to it — a common malloc-family bug
 *         // is overwriting your only pointer with a failed realloc's
 *         // NULL, orphaning (leaking) the original block.
 *
 *       void display() const;
 *         // prints all `size` elements space-separated, then a newline.
 *         // prints just a newline if empty.
 *
 *       int getSize() const;
 *       int getCapacity() const;
 *
 *       void clear();
 *         // LOGICAL clear only: size = 0. Do NOT free/realloc data,
 *         // do NOT touch capacity. Next add() should reuse existing
 *         // memory, not allocate fresh.
 *
 *       double getAverage() const;  // must NOT truncate to int. Assume non-empty.
 *       int    getMax() const;      // assume non-empty
 *       int    getMin() const;      // assume non-empty
 *
 * Header should be named DynamicArray.hpp.
 * ============================================================
 */

#include "DynamicArray.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << boolalpha;

    // ================= SECTION A: growth mechanics =================
    DynamicArray nums(2); // starts with room for 2

    cout << "add(10): " << nums.add(10) << endl; // Expected: add(10): true
    nums.add(20);
    nums.display();                                      // Expected: 10 20
    cout << "capacity: " << nums.getCapacity() << endl;   // Expected: capacity: 2
    cout << "size: " << nums.getSize() << endl;           // Expected: size: 2

    // size == capacity right now — next add() must grow BEFORE inserting
    nums.add(30);
    nums.display();                                      // Expected: 10 20 30
    cout << "capacity: " << nums.getCapacity() << endl;   // Expected: capacity: 5
    // ^ if you got 4 here, you grew by 2x or +1 instead of the fixed +3 rule.

    nums.add(40);
    nums.add(50);
    nums.display();                                      // Expected: 10 20 30 40 50
    cout << "capacity: " << nums.getCapacity() << endl;   // Expected: capacity: 5
    // size == capacity again (5 == 5) but we haven't called add() yet —
    // capacity should NOT change just from filling up.

    nums.add(60);
    nums.add(61);
    nums.display();                                      // Expected: 10 20 30 40 50 60 61
    cout << "capacity: " << nums.getCapacity() << endl;   // Expected: capacity: 8
    cout << "size: " << nums.getSize() << endl;           // Expected: size: 7

    // ================= SECTION B: stats =================
    cout << "average: " << nums.getAverage() << endl; // Expected: average: 38.7143
    // 10+20+30+40+50+60+61 = 271, 271/7 = 38.714285...
    // if you got 38 exactly, you divided two ints before casting to double.
    cout << "max: " << nums.getMax() << endl; // Expected: max: 61
    cout << "min: " << nums.getMin() << endl; // Expected: min: 10

    // ================= SECTION C: clear does NOT release memory =================
    nums.clear();
    cout << "size after clear: " << nums.getSize() << endl;         // Expected: size after clear: 0
    cout << "capacity after clear: " << nums.getCapacity() << endl; // Expected: capacity after clear: 8
    // ^ capacity staying at 8 proves you didn't free/realloc inside clear().
    nums.display(); // Expected: (blank line)

    nums.add(5);
    nums.display();                                      // Expected: 5
    cout << "capacity: " << nums.getCapacity() << endl;   // Expected: capacity: 8
    // still 8 — reused existing memory instead of allocating fresh.

    // ================= SECTION D: integrity across multiple resizes =================
    // Bulk insert — if your realloc/copy logic is wrong, earlier values
    // (starting with that lone 5) get corrupted or silently dropped.
    for (int i = 1; i <= 10; i++) {
        nums.add(i * 100);
    }
    nums.display();
    // Expected: 5 100 200 300 400 500 600 700 800 900 1000
    cout << "size: " << nums.getSize() << endl;         // Expected: size: 11
    cout << "capacity: " << nums.getCapacity() << endl; // Expected: capacity: 11
    // size 1 -> capacity 8 absorbs adds up through size 8, the 8th add in
    // this loop triggers one more grow (8+3=11), the last two fit without
    // growing again.

    return 0;
}