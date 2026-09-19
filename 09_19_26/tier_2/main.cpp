/*
 * TEST DRIVER for DoublyLinkedList — TIER 2 (Very Common Lab/Exam Staples)
 * ---------------------------------------------------------------------------
 * Paste your implemented DoublyLinkedList class above this file's contents
 * (or #include it if you split it into a separate header), then run.
 * Check each printed result against the "expected" comment beside it.
 *
 *   g++ -std=c++17 -Wall -fsanitize=address -o test main_test.cpp
 *   ./test
 *
 * ASSUMES Tier 1 functions already work (insertAtTail, traverseForward,
 * isEmpty, getSize, clear, etc.) since Tier 2 funcs build on top of them.
 *
 * NOTE ON AMBIGUOUS SIGNATURES — confirm these against your actual spec
 * before trusting the "expected" values below:
 *   - swapNodes here is tested as swapByValue(v1, v2): swaps the DATA of
 *     the two matching nodes (the "easy" version). If your spec requires
 *     swapping the actual node LINKS instead, the list structure changes
 *     but the printed values end up identical either way — this test
 *     only checks final printed order, not which approach you used.
 *   - concatenate(other) is assumed to APPEND other's nodes onto the end
 *     of this list, and leave `other` EMPTY afterward (ownership transfer).
 *     If your version instead copies nodes and leaves `other` unchanged,
 *     adjust the expected output for `other` accordingly.
 *   - splitList() is assumed to keep the FIRST half in the original list
 *     and RETURN a new DoublyLinkedList containing the second half.
 *     If your signature differs (e.g. takes an out-parameter), adapt the
 *     call but the expected split point is the same.
 *   - rotateByK(k) is assumed to rotate LEFT (elements shift toward head,
 *     wrapping around to the tail). Confirm direction with your prof.
 *   - findMiddle() on an EVEN-length list is assumed to return the
 *     UPPER middle (i.e. index size/2). Confirm which convention your
 *     prof wants.
 */

#include <iostream>
#include "LinkedList.hpp"
using namespace std;

// ==================== PASTE YOUR DoublyLinkedList CLASS HERE ====================
// (must already include Tier 1 functions, plus Tier 2 functions below)





// ==================================================================================

int main() {
    DoublyLinkedList list;

    cout << "=== SETUP: insertAtTail 10,20,30,40,50 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.insertAtTail(40);
    list.insertAtTail(50);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> 50 -> NULL" << endl << endl;

    cout << "=== getFront() / getBack() ===" << endl;
    cout << list.getFront() << "  (expected: 10)" << endl;
    cout << list.getBack() << "  (expected: 50)" << endl << endl;

    cout << "=== getNodeAt(2) ===" << endl;
    Node* n = list.getNodeAt(2);
    cout << (n != nullptr ? n->data : -1) << "  (expected: 30)" << endl << endl;

    cout << "=== countOccurrences / updateValue setup: insertAtTail(30) again ===" << endl;
    list.insertAtTail(30);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> 50 -> 30 -> NULL" << endl << endl;

    cout << "=== countOccurrences(30) ===" << endl;
    cout << list.countOccurrences(30) << "  (expected: 2)" << endl << endl;

    cout << "=== updateValue(40, 99) ===" << endl;
    list.updateValue(40, 99);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> 99 -> 50 -> 30 -> NULL" << endl << endl;

    cout << "=== removeAll(30) [removes EVERY match] ===" << endl;
    list.removeAll(30);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 99 -> 50 -> NULL" << endl << endl;

    cout << "=== rebuild sorted base for insertSorted: clear + 10,20,40,50 ===" << endl;
    list.clear();
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(40);
    list.insertAtTail(50);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 40 -> 50 -> NULL" << endl << endl;

    cout << "=== insertSorted(30) [middle insert] ===" << endl;
    list.insertSorted(30);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> 50 -> NULL" << endl << endl;

    cout << "=== insertSorted(5) [new head] ===" << endl;
    list.insertSorted(5);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 40 -> 50 -> NULL" << endl << endl;

    cout << "=== insertSorted(100) [new tail] ===" << endl;
    list.insertSorted(100);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 40 -> 50 -> 100 -> NULL" << endl << endl;

    cout << "=== findMiddle() on 7-node list (odd -> unambiguous) ===" << endl;
    cout << list.findMiddle() << "  (expected: 30)" << endl << endl;

    cout << "=== rotateByK(2) [left-rotate by 2] ===" << endl;
    list.rotateByK(2);
    list.traverseForward();
    cout << "expected: 20 -> 30 -> 40 -> 50 -> 100 -> 5 -> 10 -> NULL" << endl << endl;

    cout << "=== rotateByK(0) [no-op] ===" << endl;
    list.rotateByK(0);
    list.traverseForward();
    cout << "expected: 20 -> 30 -> 40 -> 50 -> 100 -> 5 -> 10 -> NULL  (unchanged)" << endl << endl;

    cout << "=== rotateByK(size) [full rotation, no-op] ===" << endl;
    list.rotateByK(list.getSize());
    list.traverseForward();
    cout << "expected: 20 -> 30 -> 40 -> 50 -> 100 -> 5 -> 10 -> NULL  (unchanged)" << endl << endl;

    cout << "=== swapByValue(20, 10) [swap two nodes' values] ===" << endl;
    list.swapByValue(20, 10);
    list.traverseForward();
    cout << "expected: 10 -> 30 -> 40 -> 50 -> 100 -> 5 -> 20 -> NULL" << endl << endl;

    cout << "=== toArray() ===" << endl;
    int* arr = list.toArray();
    cout << "[ ";
    for (int i = 0; i < list.getSize(); i++) cout << arr[i] << " ";
    cout << "]  (expected: [ 10 30 40 50 100 5 20 ])" << endl;
    delete[] arr; // if toArray() heap-allocates; skip if your version returns a vector
    cout << endl;

    cout << "=== fromArray({1,2,3,4,5}) into a NEW list ===" << endl;
    int seed[] = {1, 2, 3, 4, 5};
    DoublyLinkedList list2;
    list2.fromArray(seed, 5);
    list2.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> 4 -> 5 -> NULL" << endl << endl;

    cout << "=== concatenate: list.concatenate(list2) ===" << endl;
    cout << "list BEFORE: "; list.traverseForward();
    cout << "list2 BEFORE: "; list2.traverseForward();
    list.concatenate(list2);
    cout << "list AFTER: "; list.traverseForward();
    cout << "expected: 10 -> 30 -> 40 -> 50 -> 100 -> 5 -> 20 -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL" << endl;
    cout << "list2 AFTER: "; list2.traverseForward();
    cout << "expected:  -> NULL  (empty, if your concatenate transfers ownership)" << endl << endl;

    cout << "=== splitList() on a fresh 6-node list ===" << endl;
    DoublyLinkedList list3;
    for (int i = 1; i <= 6; i++) list3.insertAtTail(i);
    cout << "list3 BEFORE: "; list3.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL" << endl;
    DoublyLinkedList secondHalf = list3.splitList();
    cout << "list3 AFTER (first half): "; list3.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> NULL" << endl;
    cout << "secondHalf: "; secondHalf.traverseForward();
    cout << "expected: 4 -> 5 -> 6 -> NULL" << endl << endl;

    cout << "=== EDGE CASES: Tier 2 functions on EMPTY list (should not crash) ===" << endl;
    DoublyLinkedList empty;
    cout << "getFront()/getBack() on empty (define sentinel/error behavior): " << endl;
    // e.g. empty.getFront(); -- confirm this either throws, returns a sentinel, or is guarded
    cout << "getNodeAt(0): " << (empty.getNodeAt(0) == nullptr ? "nullptr (expected)" : "NOT nullptr (bug!)") << endl;
    cout << "countOccurrences(5): " << empty.countOccurrences(5) << "  (expected: 0)" << endl;
    empty.updateValue(5, 10); // expected: no-op, no crash
    empty.removeAll(5);       // expected: no-op, no crash
    empty.insertSorted(1);    // expected: becomes single-node list [1]
    empty.traverseForward();
    cout << "expected after insertSorted(1) on empty: 1 -> NULL" << endl;
    empty.clear();
    // findMiddle() and rotateByK() on empty -- confirm defined sentinel/no-op behavior
    empty.rotateByK(3); // expected: no-op, no crash
    empty.traverseForward();
    cout << "expected: still empty ->  -> NULL" << endl << endl;

    cout << "=== EDGE CASES: single-node list ===" << endl;
    DoublyLinkedList single;
    single.insertAtTail(42);
    cout << "findMiddle(): " << single.findMiddle() << "  (expected: 42)" << endl;
    single.rotateByK(5); // any k on single node = no-op
    single.traverseForward();
    cout << "expected: 42 -> NULL  (rotate no-op on single node)" << endl;
    single.swapByValue(42, 42); // swap with itself, should not corrupt the list
    single.traverseForward();
    cout << "expected: 42 -> NULL  (self-swap should not break anything)" << endl << endl;

    cout << "=== EDGE CASES: removeAll / countOccurrences with no match ===" << endl;
    list3.removeAll(999); // list3 currently [1,2,3]
    list3.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> NULL  (unchanged, value not found)" << endl;
    cout << "countOccurrences(999): " << list3.countOccurrences(999) << "  (expected: 0)" << endl;

    return 0;
}