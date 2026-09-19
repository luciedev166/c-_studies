// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 09: deleteByValue(num)
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
    //  THE PSET:  deleteByValue(num)
    //
    //    int deleteByValue(int num)
    //        remove the FIRST node holding num; return the index it was at,
    //        or -1 (list unchanged) if num isn't in the list
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    int deleteByValue(int num) {
        Node* cur = head;
        int index = 0;
        while (cur != nullptr && cur->data != num) {   // find first match
            cur = cur->next;
            index++;
        }
        if (cur == nullptr) return -1;                 // not found (or empty list)

        // link the neighbours to each other (or move head / tail)
        if (cur->prev != nullptr) cur->prev->next = cur->next;
        else                      head = cur->next;
        if (cur->next != nullptr) cur->next->prev = cur->prev;
        else                      tail = cur->prev;

        delete cur;
        size--;
        return index;
    }
    // ===== END ANSWER =====
};

#endif
