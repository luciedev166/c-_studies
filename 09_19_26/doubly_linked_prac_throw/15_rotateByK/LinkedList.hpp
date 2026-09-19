// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 15: rotateByK(k)
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
    //  THE PSET:  rotateByK(k)
    //
    //    void rotateByK(int k)
    //        rotate the list LEFT by k:  rotateByK(2) on [1,2,3,4,5] -> [3,4,5,1,2]
    //
    //    k = 0, k = size, an empty list or a 1-node list: nothing changes.
    //    k > size behaves like k % size.
    //    A negative k rotates the other way (right).
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    void rotateByK(int k) {
        if (size <= 1) return;
        k = ((k % size) + size) % size;         // k > size wraps around, negative k goes the other way
        if (k == 0) return;                     // full turn: nothing to do

        Node* newTail = head;
        for (int i = 1; i < k; i++) {
            newTail = newTail->next;            // the k-th node ends up as the new tail
        }
        Node* newHead = newTail->next;

        tail->next = head;                      // join old tail -> old head (make it a circle)
        head->prev = tail;

        head = newHead;                         // ...then cut the circle at the new spot
        tail = newTail;
        head->prev = nullptr;
        tail->next = nullptr;
    }
    // ===== END ANSWER =====
};

#endif
