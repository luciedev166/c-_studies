/*
 * TEST DRIVER for SinglyLinkedList (NO sentinels: head/tail are nullptr when empty)
 * -------------------------------------------------------------------
 * Write your own SinglyLinkedList.hpp, then run:
 *
 *   g++ -std=c++17 -Wall -o test singlymain.cpp
 *   ./test
 *
 * Check each printed result against the "expected" comment beside it.
 *
 * FUNCTIONS YOUR CLASS MUST HAVE:
 *   isEmpty(), getSize()
 *   addFront(val), addBack(val), insertAtN(val, pos)
 *   removeFront(), removeBack(), removeNreturnN(pos)   <- all three return the removed value, or -1 if invalid
 *   removeAllVals(val) -> int, removes EVERY node equal to val, returns how many were removed (0 if none)
 *   getValAtN(pos)  -> int, value at index pos, or -1 if invalid
 *   getVal(val)     -> bool, true if val is in the list, false otherwise
 *   clear()
 *   printFromHead(), printFromTail()
 *
 * RULES THE TEST ASSUMES:
 *   insertAtN: pos <= 0 goes to the front, pos >= size goes to the back
 *   removeNreturnN: invalid pos (negative or >= size) prints a message, returns -1, list unchanged
 *   getValAtN: invalid pos (negative or >= size) prints a message, returns -1, list unchanged
 */

#include <iostream>
#include "SinglyLinkedList.hpp"
using namespace std;

int main() {
    SinglyLinkedList list;
    int r;

    cout << "=== fresh list ===" << endl;
    cout << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl;
    list.printFromHead();
    cout << "expected: NULL" << endl << endl;

    cout << "=== addBack 10,20,30,40 ===" << endl;
    list.addBack(10);
    list.addBack(20);
    list.addBack(30);
    list.addBack(40);
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 4)" << endl << endl;

    cout << "=== addFront 5 ===" << endl;
    list.addFront(5);
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== printFromTail() (singly-specific: no prev pointers, so recursion/stack) ===" << endl;
    list.printFromTail();
    cout << "expected: 40 -> 30 -> 20 -> 10 -> 5 -> NULL" << endl << endl;

    cout << "=== insertAtN(99, 2)  [middle] ===" << endl;
    list.insertAtN(99, 2);
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== insertAtN(0, 0)  [front] ===" << endl;
    list.insertAtN(0, 0);
    list.printFromHead();
    cout << "expected: 0 -> 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== insertAtN(100, 7)  [pos == size, back] ===" << endl;
    list.insertAtN(100, 7);
    list.printFromHead();
    cout << "expected: 0 -> 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> 100 -> NULL" << endl << endl;

    cout << "=== insertAtN(200, 50)  [pos past the end, back] ===" << endl;
    list.insertAtN(200, 50);
    list.printFromHead();
    cout << "expected: 0 -> 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> 100 -> 200 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 9)" << endl << endl;

    cout << "=== printFromTail() again (checks tail is correct after inserts) ===" << endl;
    list.printFromTail();
    cout << "expected: 200 -> 100 -> 40 -> 30 -> 20 -> 99 -> 10 -> 5 -> 0 -> NULL" << endl << endl;

    cout << "=== removeFront() ===" << endl;
    r = list.removeFront();
    cout << "returned: " << r << "  (expected: 0)" << endl;
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> 100 -> 200 -> NULL" << endl << endl;

    cout << "=== removeBack() ===" << endl;
    r = list.removeBack();
    cout << "returned: " << r << "  (expected: 200)" << endl;
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> 100 -> NULL" << endl << endl;

    cout << "=== removeBack() again (tail must have moved correctly) ===" << endl;
    r = list.removeBack();
    cout << "returned: " << r << "  (expected: 100)" << endl;
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> NULL" << endl;
    list.addBack(41);
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 99 -> 20 -> 30 -> 40 -> 41 -> NULL" << endl;
    r = list.removeBack();
    cout << "returned: " << r << "  (expected: 41)" << endl << endl;

    cout << "=== removeNreturnN(2)  [middle] ===" << endl;
    r = list.removeNreturnN(2);
    cout << "returned: " << r << "  (expected: 99)" << endl;
    list.printFromHead();
    cout << "expected: 5 -> 10 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== removeNreturnN(0)  [head] ===" << endl;
    r = list.removeNreturnN(0);
    cout << "returned: " << r << "  (expected: 5)" << endl;
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> 40 -> NULL" << endl << endl;

    cout << "=== removeNreturnN(3)  [last index] ===" << endl;
    r = list.removeNreturnN(3);
    cout << "returned: " << r << "  (expected: 40)" << endl;
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> NULL" << endl;
    list.printFromTail();
    cout << "expected: 30 -> 20 -> 10 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 3)" << endl << endl;

    cout << "=== removeNreturnN with INVALID positions (list must stay unchanged) ===" << endl;
    r = list.removeNreturnN(3);
    cout << "returned: " << r << "  (expected: -1, pos == size)" << endl;
    r = list.removeNreturnN(10);
    cout << "returned: " << r << "  (expected: -1, way out of range)" << endl;
    r = list.removeNreturnN(-1);
    cout << "returned: " << r << "  (expected: -1, negative)" << endl;
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> NULL" << endl << endl;

    cout << "=== getValAtN (list is 10 -> 20 -> 30) ===" << endl;
    r = list.getValAtN(0);
    cout << "getValAtN(0): " << r << "  (expected: 10, head)" << endl;
    r = list.getValAtN(1);
    cout << "getValAtN(1): " << r << "  (expected: 20, middle)" << endl;
    r = list.getValAtN(2);
    cout << "getValAtN(2): " << r << "  (expected: 30, last index)" << endl;
    r = list.getValAtN(3);
    cout << "getValAtN(3): " << r << "  (expected: -1, pos == size)" << endl;
    r = list.getValAtN(-1);
    cout << "getValAtN(-1): " << r << "  (expected: -1, negative)" << endl;
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> NULL  (getValAtN must NOT remove anything)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 3)" << endl << endl;

    cout << "=== getVal (bool: is it in the list?) ===" << endl;
    cout << (list.getVal(10) ? "true" : "false") << "  (expected: true, head)" << endl;
    cout << (list.getVal(20) ? "true" : "false") << "  (expected: true, middle)" << endl;
    cout << (list.getVal(30) ? "true" : "false") << "  (expected: true, tail)" << endl;
    cout << (list.getVal(99) ? "true" : "false") << "  (expected: false)" << endl;
    cout << (list.getVal(-5) ? "true" : "false") << "  (expected: false)" << endl;
    list.printFromHead();
    cout << "expected: 10 -> 20 -> 30 -> NULL  (getVal must NOT change the list)" << endl << endl;

    cout << "=== removeAllVals: matches at head, middle AND tail ===" << endl;
    list.clear();
    list.addBack(1);
    list.addBack(2);
    list.addBack(1);
    list.addBack(3);
    list.addBack(1);
    list.printFromHead();
    cout << "expected: 1 -> 2 -> 1 -> 3 -> 1 -> NULL" << endl;
    r = list.removeAllVals(1);
    cout << "removed: " << r << "  (expected: 3)" << endl;
    list.printFromHead();
    cout << "expected: 2 -> 3 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 2)" << endl;
    list.addBack(9);
    list.printFromHead();
    cout << "expected: 2 -> 3 -> 9 -> NULL  (tail must have been fixed after removing the old tail)" << endl;
    list.printFromTail();
    cout << "expected: 9 -> 3 -> 2 -> NULL" << endl << endl;

    cout << "=== removeAllVals: value NOT in list ===" << endl;
    r = list.removeAllVals(99);
    cout << "removed: " << r << "  (expected: 0)" << endl;
    list.printFromHead();
    cout << "expected: 2 -> 3 -> 9 -> NULL  (unchanged)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 3)" << endl << endl;

    cout << "=== removeAllVals: consecutive matches at the BACK ===" << endl;
    list.clear();
    list.addBack(5);
    list.addBack(6);
    list.addBack(6);
    list.addBack(6);
    r = list.removeAllVals(6);
    cout << "removed: " << r << "  (expected: 3)" << endl;
    list.printFromHead();
    cout << "expected: 5 -> NULL" << endl;
    list.addBack(8);
    list.printFromHead();
    cout << "expected: 5 -> 8 -> NULL" << endl;
    list.printFromTail();
    cout << "expected: 8 -> 5 -> NULL" << endl << endl;

    cout << "=== removeAllVals: consecutive matches in the MIDDLE ===" << endl;
    list.clear();
    list.addBack(1);
    list.addBack(5);
    list.addBack(5);
    list.addBack(2);
    r = list.removeAllVals(5);
    cout << "removed: " << r << "  (expected: 2)" << endl;
    list.printFromHead();
    cout << "expected: 1 -> 2 -> NULL" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 2)" << endl << endl;

    cout << "=== removeAllVals: consecutive matches at the FRONT ===" << endl;
    list.clear();
    list.addBack(7);
    list.addBack(7);
    list.addBack(7);
    list.addBack(1);
    r = list.removeAllVals(7);
    cout << "removed: " << r << "  (expected: 3)" << endl;
    list.printFromHead();
    cout << "expected: 1 -> NULL" << endl;
    r = list.getValAtN(0);
    cout << "getValAtN(0): " << r << "  (expected: 1, head must have moved)" << endl << endl;

    cout << "=== removeAllVals: EVERY node matches (list empties, tail must reset) ===" << endl;
    list.clear();
    list.addBack(7);
    list.addBack(7);
    list.addBack(7);
    r = list.removeAllVals(7);
    cout << "removed: " << r << "  (expected: 3)" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl;
    list.printFromHead();
    cout << "expected: NULL" << endl;
    list.addBack(4);
    list.addBack(5);
    list.printFromHead();
    cout << "expected: 4 -> 5 -> NULL  (list is reusable, tail was reset)" << endl;
    list.printFromTail();
    cout << "expected: 5 -> 4 -> NULL" << endl << endl;

    cout << "=== removeAllVals: single-node list ===" << endl;
    list.clear();
    list.addBack(5);
    r = list.removeAllVals(5);
    cout << "removed: " << r << "  (expected: 1)" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    list.addBack(6);
    list.printFromHead();
    cout << "expected: 6 -> NULL" << endl << endl;

    cout << "=== EDGE CASES: single-node list ===" << endl;
    list.clear();
    list.addFront(999);
    list.printFromHead();
    cout << "expected: 999 -> NULL" << endl;
    list.printFromTail();
    cout << "expected: 999 -> NULL" << endl;
    r = list.getValAtN(0);
    cout << "getValAtN(0): " << r << "  (expected: 999)" << endl;
    cout << (list.getVal(999) ? "true" : "false") << "  (expected: true)" << endl;
    r = list.removeBack();
    cout << "returned: " << r << "  (expected: 999)" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    list.printFromHead();
    cout << "expected: NULL" << endl << endl;

    cout << "=== EDGE CASE: tail must reset when list empties (addBack right after) ===" << endl;
    list.addBack(1);
    r = list.removeFront();
    cout << "returned: " << r << "  (expected: 1)" << endl;
    list.addBack(2);
    list.addBack(3);
    list.printFromHead();
    cout << "expected: 2 -> 3 -> NULL" << endl;
    list.printFromTail();
    cout << "expected: 3 -> 2 -> NULL" << endl << endl;

    cout << "=== EDGE CASE: removeNreturnN(0) on single-node list ===" << endl;
    list.clear();
    list.addBack(7);
    r = list.removeNreturnN(0);
    cout << "returned: " << r << "  (expected: 7)" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl << endl;

    cout << "=== EDGE CASES: operations on EMPTY list (should not crash) ===" << endl;
    r = list.removeFront();
    cout << "returned: " << r << "  (expected: -1)" << endl;
    r = list.removeBack();
    cout << "returned: " << r << "  (expected: -1)" << endl;
    r = list.removeNreturnN(0);
    cout << "returned: " << r << "  (expected: -1)" << endl;
    r = list.getValAtN(0);
    cout << "getValAtN(0): " << r << "  (expected: -1)" << endl;
    cout << (list.getVal(5) ? "true" : "false") << "  (expected: false)" << endl;
    r = list.removeAllVals(5);
    cout << "removeAllVals(5): " << r << "  (expected: 0)" << endl;
    list.printFromHead();
    cout << "expected: NULL  (still empty, no crash)" << endl;
    list.printFromTail();
    cout << "expected: NULL  (still empty, no crash)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl << endl;

    cout << "=== EDGE CASE: insertAtN on empty list ===" << endl;
    list.insertAtN(55, 3);
    list.printFromHead();
    cout << "expected: 55 -> NULL" << endl;
    list.insertAtN(44, 0);
    list.printFromHead();
    cout << "expected: 44 -> 55 -> NULL" << endl << endl;

    cout << "=== rebuild + clear() ===" << endl;
    list.addBack(1);
    list.addBack(2);
    list.addBack(3);
    list.clear();
    list.printFromHead();
    cout << "expected: NULL" << endl;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << "  (expected: true)" << endl;
    cout << "getSize(): " << list.getSize() << "  (expected: 0)" << endl;
    list.addBack(8);
    list.printFromHead();
    cout << "expected: 8 -> NULL  (list is reusable after clear)" << endl << endl;

    cout << "=== destructor test (goes out of scope here, no explicit output) ===" << endl;
    cout << "Run this file through valgrind/ASan to confirm no leaks:" << endl;
    cout << "  g++ -std=c++17 -g -fsanitize=address -o test singlymain.cpp && ./test" << endl;

    return 0;
}