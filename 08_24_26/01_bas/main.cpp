#include "MyArray.hpp"

// This simulates what a hidden CodeChum main() usually does: create ONE
// object, then call methods on it over and over, checking that state
// (the array + size) carries over correctly between calls.
//
// Run this, compare your terminal output to the "Expected" comments.
// If they match, your method is correct.

int main() {
    MyArray list;

    // --- Test insert(pos, n) ---
    list.insert(0, 1);           // empty array -> insert 1 at index 0
    list.insert(1, 2);           // insert 2 at index 1 (i.e. the end)
    list.insert(2, 3);           // insert 3 at index 2 (i.e. the end)
    list.display();              // Expected: 1 2 3

    // // --- Test addFront() ---
    list.addFront(0);
    list.display();              // Expected: 0 1 2 3

    // // --- Test addBack() ---
    list.addBack(9);
    list.display();              // Expected: 0 1 2 3 9

    // // --- Test insert() into the MIDDLE, and removeOccurrences() ---
    list.insert(5, 2);           // pos = size, so this is really an append
    list.display();              // Expected: 0 1 2 3 9 2
    list.removeOccurrences(2);
    list.display();              // Expected: 0 1 3 9

    // // --- Test removeAt() ---
    int removed = list.removeAt(1);
    cout << "Removed: " << removed << endl;   // Expected: Removed: 1
    list.display();              // Expected: 0 3 9

    // --- Test clear() ---
    list.clear();
    list.display();              // Expected: (blank line)
    cout << "Size after clear: " << list.getSize() << endl; // Expected: 0

    // // --- Prove the array still works AFTER clear (state not broken) ---
    list.insert(0, 99);
    list.addFront(1);
    list.display();              // Expected: 1 99

    // // ================= SNEAKY / EDGE CASES =================
    // // These mimic the "gotcha" test cases a strict checker throws at
    // // you -- boundaries, empty states, and duplicates that are easy
    // // to code wrong even if your basic logic already works.

    list.clear();

    // // Sneaky 1: addFront on a COMPLETELY EMPTY array (no elements to
    // // shift -- make sure your loop doesn't try to touch arr[-1] etc.)
    list.addFront(5);
    list.display();              // Expected: 5
    cout << "Size: " << list.getSize() << endl; // Expected: Size: 1

    // // Sneaky 2: removeAt on the LAST index -- classic off-by-one bug
    // // magnet, your shift loop might wrongly try to read arr[size]
    list.insert(1, 10);
    list.insert(2, 20);
    list.display();              // Expected: 5 10 20
    int lastRemoved = list.removeAt(2);
    cout << "Removed: " << lastRemoved << endl; // Expected: Removed: 20
    list.display();              // Expected: 5 10

    // // Sneaky 3: removeAt on index 0 (the very front)
    int frontRemoved = list.removeAt(0);
    cout << "Removed: " << frontRemoved << endl; // Expected: Removed: 5
    list.display();              // Expected: 10

    // // Sneaky 4: removeOccurrences of a value that ISN'T in the array
    // // (must silently do nothing -- not crash, not shrink the array)
    list.removeOccurrences(999);
    list.display();              // Expected: 10
    cout << "Size: " << list.getSize() << endl; // Expected: Size: 1

    // // Sneaky 5: removeOccurrences that wipes out EVERY element
    list.insert(1, 10);
    list.insert(2, 10);
    list.display();              // Expected: 10 10 10
    list.removeOccurrences(10);
    list.display();              // Expected: (blank line)
    cout << "Size: " << list.getSize() << endl; // Expected: Size: 0

    // // Sneaky 6: duplicates scattered at NON-adjacent positions --
    // // removeOccurrences must catch all of them, not just neighbors
    list.insert(0, 7);
    list.insert(1, 3);
    list.insert(2, 7);
    list.insert(3, 3);
    list.insert(4, 7);
    list.display();              // Expected: 7 3 7 3 7
    list.removeOccurrences(7);
    list.display();              // Expected: 3 3

    // // Sneaky 7: insert(pos, n) at the FRONT immediately followed by
    // // insert(pos, n) at the BACK (pos = getSize(), not a hardcoded number)
    list.insert(0, 1);
    list.insert(list.getSize(), 99);
    list.display();              // Expected: 1 3 3 99

    // // Sneaky 8: rapid clear -> reinsert -- nothing from before clear()
    // // should "leak" into the array afterward
    list.clear();
    list.insert(0, 42);
    list.display();              // Expected: 42
    cout << "Size: " << list.getSize() << endl; // Expected: Size: 1

    // // ================= NEW METHOD VARIETY =================
    // // Different category of methods: these read/inspect/transform
    // // instead of just adding or removing. Fresh object so old state
    // // doesn't carry over.

    cout << boolalpha; // makes bool print as true/false instead of 1/0
    MyArray list2;

    // // isEmpty() on a brand new array
    cout << "isEmpty: " << list2.isEmpty() << endl;   // Expected: isEmpty: true

    list2.insert(0, 4);
    list2.insert(1, 8);
    list2.insert(2, 2);
    list2.insert(3, 8);
    list2.display();              // Expected: 4 8 2 8

    // // search() -- returns index of FIRST match, -1 if not found
    cout << "search(8): " << list2.search(8) << endl;   // Expected: search(8): 1
    cout << "search(99): " << list2.search(99) << endl; // Expected: search(99): -1

    // // contains() -- true/false version of search
    cout << "contains(2): " << list2.contains(2) << endl;   // Expected: contains(2): true
    cout << "contains(50): " << list2.contains(50) << endl; // Expected: contains(50): false

    // // count() -- counts ALL matches, not just the first
    cout << "count(8): " << list2.count(8) << endl;   // Expected: count(8): 2

    // // sum()
    cout << "sum(): " << list2.sum() << endl;   // Expected: sum(): 22

    // // maxValue() / minValue()
    cout << "maxValue(): " << list2.maxValue() << endl;   // Expected: maxValue(): 8
    cout << "minValue(): " << list2.minValue() << endl;   // Expected: minValue(): 2

    // // reverse() -- flips in place
    list2.reverse();
    list2.display();              // Expected: 8 2 8 4

    // // replace() -- overwrite one slot, no shifting, size unchanged
    int oldVal = list2.replace(1, 100);
    cout << "replace old value: " << oldVal << endl;   // Expected: replace old value: 2
    list2.display();              // Expected: 8 100 8 4
    cout << "Size after replace: " << list2.getSize() << endl; // Expected: Size after replace: 4

    // // isEmpty() after clearing
    list2.clear();
    cout << "isEmpty after clear: " << list2.isEmpty() << endl; // Expected: isEmpty after clear: true

    // // TRAP CHECK: maxValue()/minValue()/sum() with ALL NEGATIVE values
    // // (this breaks implementations that start "best so far" at 0)
    list2.insert(0, -5);
    list2.insert(1, -1);
    list2.insert(2, -10);
    list2.display();              // Expected: -5 -1 -10
    cout << "maxValue(): " << list2.maxValue() << endl;   // Expected: maxValue(): -1
    cout << "minValue(): " << list2.minValue() << endl;   // Expected: minValue(): -10
    cout << "sum(): " << list2.sum() << endl;              // Expected: sum(): -16

    return 0;
}