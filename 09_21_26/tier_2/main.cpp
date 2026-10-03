/*
 * TEST DRIVER for DoublyLinkedList — TIER 2 (Very Common Lab/Exam Staples)
 * -------------------------------------------------------------------
 * This drives the 13 Tier 2 functions in LinkedList.hpp:
 *   getNodeAt, getFront/getBack, insertSorted, removeDuplicatesSorted,
 *   removeDuplicatesUnsorted, removeAll, findMiddle, swapNodes,
 *   swapByValue, concatenate, splitList, toArray/fromArray,
 *   countOccurrences, updateValue, rotateByK
 *
 * Until you implement them, each stub prints "not implemented yet" and
 * returns a placeholder (-1 / false / nullptr / empty). That's expected.
 * As you implement each one in LinkedList.hpp, rerun and check its
 * block against the "expected" comment.
 *
 *   g++ -std=c++17 -Wall -o test main_test_tier2.cpp
 *   ./test
 *
 * NOTE: this assumes getNodeAt/swapNodes work with Node* the same way
 * search() does (same signatures already declared in the header).
 */

#include <iostream>
#include <vector>
#include "LinkedList.hpp"
using namespace std;

// small helpers just for printing — not part of what you're practicing
void printNode(Node* n) {
    if (n) cout << n->elem;
    else cout << "nullptr";
}

void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

int main() {
    DoublyLinkedList list;

    // ============================================================
    // getFront() / getBack() / getNodeAt(pos) / findMiddle()
    // ============================================================
    cout << "=== setup: insertAtTail 10,20,30,40,50 ===" << endl;
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

    cout << "=== getNodeAt(pos) ===" << endl;
    cout << "getNodeAt(0): "; printNode(list.getNodeAt(0)); cout << "  (expected: 10)" << endl;
    cout << "getNodeAt(2): "; printNode(list.getNodeAt(2)); cout << "  (expected: 30)" << endl;
    cout << "getNodeAt(99): "; printNode(list.getNodeAt(99)); cout << "  (expected: nullptr)" << endl << endl;

    cout << "=== findMiddle() — odd length (size 5) ===" << endl;
    cout << list.findMiddle() << "  (expected: 30)" << endl << endl;

    cout << "=== findMiddle() — even length (size 6) ===" << endl;
    list.insertAtTail(60);
    list.traverseForward();
    cout << "list is now: 10 -> 20 -> 30 -> 40 -> 50 -> 60 -> NULL" << endl;
    cout << list.findMiddle() << "  (expected: 40, the SECOND of the two middle values)" << endl << endl;

    list.clear();

    // ============================================================
    // insertSorted(val)
    // ============================================================
    cout << "=== insertSorted: 30, 10, 50, 20, 50, 5 (into empty list) ===" << endl;
    list.insertSorted(30);
    list.insertSorted(10);
    list.insertSorted(50);
    list.insertSorted(20);
    list.insertSorted(50);
    list.insertSorted(5);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 50 -> 50 -> NULL" << endl << endl;

    // ============================================================
    // removeDuplicatesSorted() — reuses the sorted list above
    // ============================================================
    cout << "=== removeDuplicatesSorted() on 5,10,20,30,50,50 ===" << endl;
    list.removeDuplicatesSorted();
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 50 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 5)" << endl << endl;

    list.clear();

    // ============================================================
    // removeDuplicatesUnsorted()
    // ============================================================
    cout << "=== removeDuplicatesUnsorted() on 10,30,10,20,30,10,40 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(30);
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.insertAtTail(10);
    list.insertAtTail(40);
    list.removeDuplicatesUnsorted();
    list.traverseForward();
    cout << "expected: 10 -> 30 -> 20 -> 40 -> NULL  (keep first occurrence, in order)" << endl << endl;

    list.clear();

    // ============================================================
    // removeAll(val)
    // ============================================================
    cout << "=== removeAll(10) on 10,20,10,30,10,10,40 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(10);
    list.insertAtTail(30);
    list.insertAtTail(10);
    list.insertAtTail(10);
    list.insertAtTail(40);
    int removedCount = list.removeAll(10);
    list.traverseForward();
    cout << "expected: 20 -> 30 -> 40 -> NULL" << endl;
    cout << "removed count: " << removedCount << "  (expected: 4)" << endl << endl;

    cout << "=== removeAll(999) — value not present ===" << endl;
    int removedNone = list.removeAll(999);
    list.traverseForward();
    cout << "expected: 20 -> 30 -> 40 -> NULL  (unchanged)" << endl;
    cout << "removed count: " << removedNone << "  (expected: 0)" << endl << endl;

    list.clear();

    // ============================================================
    // swapNodes(Node* a, Node* b) — structural swap
    // ============================================================
    cout << "=== swapNodes: interior swap on 10,20,30,40,50 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.insertAtTail(40);
    list.insertAtTail(50);
    Node* n20 = list.search(20);
    Node* n40 = list.search(40);
    list.swapNodes(n20, n40);
    list.traverseForward();
    cout << "expected: 10 -> 40 -> 30 -> 20 -> 50 -> NULL" << endl << endl;

    cout << "=== swapNodes: boundary swap (head node with tail node) ===" << endl;
    Node* nHeadVal = list.search(10);
    Node* nTailVal = list.search(50);
    list.swapNodes(nHeadVal, nTailVal);
    list.traverseForward();
    cout << "expected: 50 -> 40 -> 30 -> 20 -> 10 -> NULL" << endl << endl;

    list.clear();

    // ============================================================
    // swapByValue(val1, val2) — just swaps .elem, no relinking
    // ============================================================
    cout << "=== swapByValue(2, 4) on 1,2,3,4,5 ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(4);
    list.insertAtTail(5);
    list.swapByValue(2, 4);
    list.traverseForward();
    cout << "expected: 1 -> 4 -> 3 -> 2 -> 5 -> NULL" << endl << endl;

    cout << "=== swapByValue(99, 1) — 99 not present, should no-op ===" << endl;
    list.swapByValue(99, 1);
    list.traverseForward();
    cout << "expected: 1 -> 4 -> 3 -> 2 -> 5 -> NULL  (unchanged)" << endl << endl;

    list.clear();

    // ============================================================
    // concatenate(other)
    // ============================================================
    cout << "=== concatenate: A(1,2,3) + B(4,5,6) ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    DoublyLinkedList listB;
    listB.insertAtTail(4);
    listB.insertAtTail(5);
    listB.insertAtTail(6);
    list.concatenate(listB);
    list.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL" << endl;
    cout << "listB.isEmpty(): " << (listB.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "listB.getSize(): " << listB.getSize() << "  (expected: 0)" << endl << endl;

    cout << "=== concatenate: non-empty + empty ===" << endl;
    DoublyLinkedList listC; // empty
    list.concatenate(listC);
    list.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL  (unchanged)" << endl << endl;

    cout << "=== concatenate: empty + non-empty ===" << endl;
    DoublyLinkedList listD; // empty
    DoublyLinkedList listE;
    listE.insertAtTail(7);
    listE.insertAtTail(8);
    listD.concatenate(listE);
    listD.traverseForward();
    cout << "expected: 7 -> 8 -> NULL" << endl;
    cout << "listE.isEmpty(): " << (listE.isEmpty() ? "true" : "false") << "  (expected: true)" << endl << endl;

    list.clear();

    // ============================================================
    // splitList(secondHalf)
    // ============================================================
    cout << "=== splitList: odd length (1,2,3,4,5) ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(4);
    list.insertAtTail(5);
    DoublyLinkedList secondHalf;
    list.splitList(secondHalf);
    list.traverseForward();
    cout << "expected (first half):  1 -> 2 -> 3 -> NULL" << endl;
    secondHalf.traverseForward();
    cout << "expected (second half): 4 -> 5 -> NULL" << endl << endl;

    list.clear();

    cout << "=== splitList: even length (1,2,3,4,5,6) ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(4);
    list.insertAtTail(5);
    list.insertAtTail(6);
    DoublyLinkedList secondHalf2;
    list.splitList(secondHalf2);
    list.traverseForward();
    cout << "expected (first half):  1 -> 2 -> 3 -> NULL" << endl;
    secondHalf2.traverseForward();
    cout << "expected (second half): 4 -> 5 -> 6 -> NULL" << endl << endl;

    list.clear();

    // ============================================================
    // toArray() / fromArray()
    // ============================================================
    cout << "=== toArray() on 10,20,30 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    vector<int> arr = list.toArray();
    cout << "toArray(): "; printVec(arr); cout << "  (expected: [10, 20, 30])" << endl << endl;

    cout << "=== fromArray({100,200,300}) into an empty list ===" << endl;
    DoublyLinkedList list2;
    vector<int> src = {100, 200, 300};
    list2.fromArray(src);
    list2.traverseForward();
    cout << "expected: 100 -> 200 -> 300 -> NULL" << endl << endl;

    list.clear();

    // ============================================================
    // countOccurrences(val)
    // ============================================================
    cout << "=== countOccurrences on 5,3,5,5,2,5 ===" << endl;
    list.insertAtTail(5);
    list.insertAtTail(3);
    list.insertAtTail(5);
    list.insertAtTail(5);
    list.insertAtTail(2);
    list.insertAtTail(5);
    cout << "countOccurrences(5): " << list.countOccurrences(5) << "  (expected: 4)" << endl;
    cout << "countOccurrences(99): " << list.countOccurrences(99) << "  (expected: 0)" << endl << endl;

    list.clear();

    // ============================================================
    // updateValue(oldVal, newVal)
    // ============================================================
    cout << "=== updateValue(2, 99) on 1,2,3,2,5 ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(2);
    list.insertAtTail(5);
    bool updated = list.updateValue(2, 99);
    list.traverseForward();
    cout << "expected: 1 -> 99 -> 3 -> 2 -> 5 -> NULL  (only the FIRST 2 changes)" << endl;
    cout << "updateValue returned: " << (updated ? "true" : "false") << "  (expected: true)" << endl << endl;

    cout << "=== updateValue(1000, 1) — oldVal not present ===" << endl;
    bool updatedMissing = list.updateValue(1000, 1);
    list.traverseForward();
    cout << "expected: 1 -> 99 -> 3 -> 2 -> 5 -> NULL  (unchanged)" << endl;
    cout << "updateValue returned: " << (updatedMissing ? "true" : "false") << "  (expected: false)" << endl << endl;

    list.clear();

    // ============================================================
    // rotateByK(k)
    // ============================================================
    cout << "=== rotateByK(2) on 1,2,3,4,5 ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(4);
    list.insertAtTail(5);
    list.rotateByK(2);
    list.traverseForward();
    cout << "expected: 3 -> 4 -> 5 -> 1 -> 2 -> NULL" << endl << endl;

    cout << "=== rotateByK(0) — no-op ===" << endl;
    list.rotateByK(0);
    list.traverseForward();
    cout << "expected: 3 -> 4 -> 5 -> 1 -> 2 -> NULL  (unchanged)" << endl << endl;

    list.clear();
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(4);
    list.insertAtTail(5);

    cout << "=== rotateByK(5) — k == size, should be a no-op ===" << endl;
    list.rotateByK(5);
    list.traverseForward();
    cout << "expected: 1 -> 2 -> 3 -> 4 -> 5 -> NULL  (unchanged)" << endl << endl;

    cout << "=== rotateByK(7) — k > size, same as rotateByK(2) via 7 % 5 ===" << endl;
    list.rotateByK(7);
    list.traverseForward();
    cout << "expected: 3 -> 4 -> 5 -> 1 -> 2 -> NULL" << endl << endl;

    cout << "=== rotateByK on an EMPTY list (should not crash) ===" << endl;
    list.clear();
    list.rotateByK(3);
    list.traverseForward();
    cout << "expected:  -> NULL  (still empty, no crash)" << endl << endl;

    cout << "=== done — Tier 2 test sequence finished ===" << endl;
    return 0;
}