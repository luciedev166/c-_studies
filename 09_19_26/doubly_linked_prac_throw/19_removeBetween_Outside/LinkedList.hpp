// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints
//  PSET 19: removeBetween(n1, n2), removeOutside(n1, n2)
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

    // helper: first node at or after "start" holding num (nullptr if none)
    Node* findFrom(Node* start, int num) {
        Node* cur = start;
        while (cur != nullptr && cur->data != num) {
            cur = cur->next;
        }
        return cur;
    }

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
    //  THE PSET:  removeBetween(n1, n2), removeOutside(n1, n2)
    //
    //    void removeBetween(int n1, int n2)
    //        remove everything STRICTLY between n1 and n2 (both stay)
    //    void removeOutside(int n1, int n2)
    //        keep only n1 .. n2 (inclusive); remove everything before n1 and after n2
    //
    //    n1 = first node holding n1; n2 = first node holding n2 from n1 onward.
    //    If either isn't found, or n2 comes before n1: nothing changes.
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    void removeBetween(int n1, int n2) {
        Node* first = findFrom(head, n1);
        if (first == nullptr) return;
        Node* last = findFrom(first, n2);       // search starts AT first, so n1 == n2 works too
        if (last == nullptr || last == first) return;

        Node* cur = first->next;                // delete everything strictly between
        while (cur != last) {
            Node* nextNode = cur->next;
            delete cur;
            size--;
            cur = nextNode;
        }
        first->next = last;                     // close the gap
        last->prev = first;
    }

    void removeOutside(int n1, int n2) {
        Node* first = findFrom(head, n1);
        if (first == nullptr) return;
        Node* last = findFrom(first, n2);
        if (last == nullptr) return;

        while (head != first) {                 // pop everything before first
            Node* old = head;
            head = head->next;
            delete old;
            size--;
        }
        head->prev = nullptr;

        while (tail != last) {                  // pop everything after last
            Node* old = tail;
            tail = tail->prev;
            delete old;
            size--;
        }
        tail->next = nullptr;
    }
    // ===== END ANSWER =====
};

#endif
