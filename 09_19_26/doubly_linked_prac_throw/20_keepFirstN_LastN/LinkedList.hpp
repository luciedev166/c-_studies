// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 20: keepFirstN(n), keepLastN(n)
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
    //  THE PSET:  keepFirstN(n), keepLastN(n)
    //
    //    void keepFirstN(int n)
    //        keep the first n nodes, delete the rest
    //    void keepLastN(int n)
    //        keep the last n nodes, delete the rest
    //
    //    n >= size: nothing changes.  n <= 0: the list becomes empty (and must
    //    still accept inserts afterwards, so head AND tail go back to nullptr).
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    void keepFirstN(int n) {
        if (n >= size) return;                  // keeping everything (also covers the empty list)
        if (n <= 0) {
            clear();
            return;
        }
        Node* keep = head;
        for (int i = 1; i < n; i++) {
            keep = keep->next;                  // keep = the n-th node = the new tail
        }
        Node* cur = keep->next;                 // delete everything after it
        while (cur != nullptr) {
            Node* nextNode = cur->next;
            delete cur;
            size--;
            cur = nextNode;
        }
        keep->next = nullptr;
        tail = keep;
    }

    void keepLastN(int n) {
        if (n >= size) return;                  // keeping everything (also covers the empty list)
        if (n <= 0) {
            clear();
            return;
        }
        Node* keep = tail;
        for (int i = 1; i < n; i++) {
            keep = keep->prev;                  // keep = the n-th from the end = the new head
        }
        Node* cur = keep->prev;                 // delete everything before it
        while (cur != nullptr) {
            Node* prevNode = cur->prev;
            delete cur;
            size--;
            cur = prevNode;
        }
        keep->prev = nullptr;
        head = keep;
    }
    // ===== END ANSWER =====
};

#endif
