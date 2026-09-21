/*
 * TEST DRIVER for DoublyLinkedList — TIER 1 (Core/Foundational) ONLY
 * -------------------------------------------------------------------
 * Paste your implemented DoublyLinkedList class above this file's contents
 * (or #include it if you split it into a separate header), then run.
 * Check each printed result against the "expected" comment beside it.
 *
 *   g++ -std=c++17 -Wall -o test main_test.cpp
 *   ./test
 *
 * NOTE ON SIGNATURES: this script assumes:
 *   - search(value)      returns a Node* (nullptr if not found)
 *   - insertAfter/insertBefore/deleteNode take a Node* target
 * If your actual signatures differ (e.g. search returns an index, or
 * insertAfter takes a value instead of a pointer), adjust the calls below
 * accordingly — the test SEQUENCE and expected results still hold.
 */

#include <iostream>
#include "LinkedList.hpp"
using namespace std;

// ==================== PASTE YOUR DoublyLinkedList CLASS HERE ====================
// (struct Node with elem/next/prev + class DoublyLinkedList with Tier 1 functions)





// ==================================================================================

int main() {
    DoublyLinkedList list;

    cout << "=== isEmpty() on fresh list ===" << endl;
    cout << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl << endl;

    cout << "=== insertAtTail 10,20,30,40 ===" << endl;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.insertAtTail(40);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 4)" << endl << endl;

    cout << "=== insertAtHead 5 ===" << endl;
    list.insertAtHead(5);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== traverseBackward() (DLL-specific: confirms prev pointers) ===" << endl;
    list.traverseBackward();
    cout << "expected: 40 -> 30 -> 20 -> 10 -> 5 -> NULL" << endl << endl;

    cout << "=== insertAtPosition(99, 2) ===" << endl;
    list.insertAtPosition(99, 2);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== search(99) / search(1000) ===" << endl;
    Node* found = list.search(99);
    cout << (found != nullptr ? "found" : "not found") << "  (expected: found)" << endl;
    Node* notFound = list.search(1000);
    cout << (notFound != nullptr ? "found" : "not found") << "  (expected: not found)" << endl << endl;

    cout << "=== insertAfter(target=99, value=77) ===" << endl;
    list.insertAfter(found, 77);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 99 -> 77 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== insertBefore(target=20, value=88) ===" << endl;
    Node* twenty = list.search(20);
    list.insertBefore(twenty, 88);
    list.traverseForward();
    cout << "expected: 5 -> 10 -> 99 -> 77 -> 88 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== deleteHead() ===" << endl;
    list.deleteHead();
    list.traverseForward();
    cout << "expected: 10 -> 99 -> 77 -> 88 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== deleteTail() ===" << endl;
    list.deleteTail();
    list.traverseForward();
    cout << "expected: 10 -> 99 -> 77 -> 88 -> 20 -> 30 -> NULL" << endl << endl;

    cout << "=== deleteAtPosition(1) ===" << endl;
    list.deleteAtPosition(1);
    list.traverseForward();
    cout << "expected: 10 -> 77 -> 88 -> 20 -> 30 -> NULL" << endl << endl;

    cout << "=== deleteByValue(88) ===" << endl;
    list.deleteByValue(88);
    list.traverseForward();
    cout << "expected: 10 -> 77 -> 20 -> 30 -> NULL" << endl << endl;

    cout << "=== deleteNode(target=77) [delete via direct pointer] ===" << endl;
    Node* seventySeven = list.search(77);
    list.deleteNode(seventySeven);
    list.traverseForward();
    cout << "expected: 10 -> 20 -> 30 -> NULL" << endl << endl;

    cout << "=== EDGE CASES: single-node list ===" << endl;
    list.clear();
    list.insertAtHead(999);
    list.traverseForward();
    cout << "expected: 999 -> NULL" << endl;
    list.traverseBackward();
    cout << "expected: 999 -> NULL" << endl;
    list.deleteHead();
    list.traverseForward();
    cout << "expected:  -> NULL  (list should now be empty)" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl << endl;

    cout << "=== EDGE CASES: operations on EMPTY list (should not crash) ===" << endl;
    list.deleteHead();
    list.deleteTail();
    list.deleteByValue(5);
    list.deleteAtPosition(0);
    Node* nothing = list.search(5);
    cout << (nothing != nullptr ? "found" : "not found") << "  (expected: not found)" << endl;
    list.traverseForward();
    cout << "expected:  -> NULL  (still empty, no crash)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl << endl;

    cout << "=== rebuild + clear() ===" << endl;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.clear();
    list.traverseForward();
    cout << "expected:  -> NULL" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl << endl;

    cout << "=== destructor test (goes out of scope here, no explicit output) ===" << endl;
    cout << "Run this file through valgrind/ASan to confirm no leaks:" << endl;
    cout << "  g++ -std=c++17 -g -fsanitize=address -o test main_test.cpp && ./test" << endl;

    return 0;
}