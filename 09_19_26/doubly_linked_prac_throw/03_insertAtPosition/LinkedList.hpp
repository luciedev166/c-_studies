// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 03: insertAtPosition(index, num)
//
//  main.cpp includes this file and tests it. You only ever edit THIS file.
//
//    SETUP    add / clear / print ... : main.cpp uses these to build its test
//             lists. They're already done - or paste in your own versions.
//    THE PSET the function(s) being tested. The ANSWER block is a finished
//             solution; comment it out and write your own to do it yourself.
//
//  Every node has BOTH links:   prev <- [data] -> next
//  head / tail / size must be kept correct by anything that adds or removes.
// ============================================================================
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
using namespace std;

struct Node {
    int   data;
    Node* prev;
    Node* next;

    Node(int value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

class LinkedList {
private:
    Node* head;     // first node  (nullptr when empty)
    Node* tail;     // last node   (nullptr when empty)
    int   size;     // number of nodes

public:
    LinkedList() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~LinkedList() {
        clear();
    }

    // ------------------------------------------------------------------
    //  SETUP  (main.cpp uses these to build its test lists)
    // ------------------------------------------------------------------

    // add: append num at the end of the list
    void add(int num) {
        Node* n = new Node(num);
        if (head == nullptr) {          // empty list: n is head AND tail
            head = n;
            tail = n;
        } else {
            tail->next = n;
            n->prev = tail;
            tail = n;
        }
        size++;
    }

    // clear: delete every node and go back to an empty list
    void clear() {
        Node* cur = head;
        while (cur != nullptr) {
            Node* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // print: 10 -> 20 -> 30 -> NULL      (has to write through cout)
    void print() {
        Node* cur = head;
        while (cur != nullptr) {
            cout << cur->data << " -> ";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }

    // traverseBackward: 30 -> 20 -> 10 -> NULL     (walks from tail using prev)
    void traverseBackward() {
        Node* cur = tail;
        while (cur != nullptr) {
            cout << cur->data << " -> ";
            cur = cur->prev;
        }
        cout << "NULL" << endl;
    }

    // ------------------------------------------------------------------
    //  THE PSET:  insertAtPosition(index, num)
    //
    //    int insertAtPosition(int index, int num)
    //        insert num so that it ends up AT that 0-based index; return the index,
    //        or -1 (and leave the list alone) if the index is not 0..size
    //
    //    index 0 = front, index == size = append at the end.
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    int insertAtPosition(int index, int num) {
        if (index < 0 || index > size) return -1;      // out of range: reject

        Node* n = new Node(num);

        if (size == 0) {                               // empty list (index must be 0)
            head = n;
            tail = n;
        } else if (index == 0) {                       // new head
            n->next = head;
            head->prev = n;
            head = n;
        } else if (index == size) {                    // new tail
            tail->next = n;
            n->prev = tail;
            tail = n;
        } else {                                       // somewhere in the middle
            Node* cur = head;
            for (int i = 0; i < index; i++) {
                cur = cur->next;                       // cur = node now sitting at index
            }
            n->prev = cur->prev;
            n->next = cur;
            cur->prev->next = n;
            cur->prev = n;
        }
        size++;
        return index;
    }
    // ===== END ANSWER =====
};

#endif
