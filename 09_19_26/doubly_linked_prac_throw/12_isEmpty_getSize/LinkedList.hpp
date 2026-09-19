// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 12: isEmpty(), getSize()
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

    // deleteHead: remove the first node, return its value (-1 if the list is empty)
    int deleteHead() {
        if (head == nullptr) return -1;
        Node* old = head;
        int value = old->data;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else                 tail = nullptr;
        delete old;
        size--;
        return value;
    }

    // deleteTail: remove the last node, return its value (-1 if the list is empty)
    int deleteTail() {
        if (tail == nullptr) return -1;
        Node* old = tail;
        int value = old->data;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else                 head = nullptr;
        delete old;
        size--;
        return value;
    }

    // ------------------------------------------------------------------
    //  THE PSET:  isEmpty(), getSize()
    //
    //    bool isEmpty()
    //        true when the list has no nodes
    //    int  getSize()
    //        how many nodes the list has
    //
    //    Every setup() in main.cpp calls clear() first, so a stale size counter
    //    left behind by clear() shows up as a wrong size in the next test.
    //    (deleteHead / deleteTail are in SETUP only because test 5 uses them.)
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
    // ===== END ANSWER =====
};

#endif
