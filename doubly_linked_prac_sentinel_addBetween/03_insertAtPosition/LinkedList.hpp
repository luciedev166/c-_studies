// ============================================================================
//  LinkedList.hpp  -  DOUBLY linked list of ints  (SENTINEL version)
//  PSET 03: insertAtPosition(index, num)
//
//  main.cpp includes this file and tests it. You only ever edit THIS file
//  (Node.hpp just holds the Node struct and doesn't change per exercise).
//
//    SETUP    add / clear / print ... , plus the addBetween / removeNode
//             helpers below: main.cpp (and the PSET answer) build on these.
//             They're already done - or paste in your own versions.
//    THE PSET the function(s) being tested. The ANSWER block is a finished
//             solution; comment it out and write your own to do it yourself.
//
//  SENTINELS: head and tail are two permanent dummy Node OBJECTS (not
//  pointers to heap nodes - actual members, so they're constructed and
//  destroyed automatically with the list, no new/delete needed for them).
//  They start out pointing at each other:
//
//        head <-> tail                (empty list)
//        head <-> A <-> B <-> tail    (list [A, B])
//
//  head.next is always the first REAL node (or &tail itself, if empty).
//  tail.prev is always the last  REAL node (or &head itself, if empty).
//  Every pointer walk compares against "== &head" / "== &tail", never
//  nullptr, and every insert/delete works the same at the ends as in the
//  middle (no more separate empty-list / head-only / tail-only cases).
// ============================================================================
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"
using namespace std;

class LinkedList {
private:
    Node head;      // SENTINEL object - head.next = first real node
    Node tail;      // SENTINEL object - tail.prev = last real node
    int  size;      // number of REAL nodes (sentinels don't count)

    // addBetween: builds a new node already wired between pred and succ,
    // then splices it in. Used by SETUP and by most of the PSET answers.
    void addBetween(int val, Node* pred, Node* succ) {
        Node* n = new Node(val, succ, pred);   // n->next = succ, n->prev = pred
        pred->next = n;
        succ->prev = n;
        size++;
    }

    // removeNode: unlinks target (a REAL node) and returns its value.
    // Works the same whether target is the first, last, or a middle node.
    int removeNode(Node* target) {
        Node* pred = target->prev;
        Node* succ = target->next;
        int val = target->elem;
        pred->next = succ;
        succ->prev = pred;
        delete target;
        size--;
        return val;
    }

public:
    LinkedList() : head(0), tail(0) {
        head.next = &tail;
        tail.prev = &head;
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
        addBetween(num, tail.prev, &tail);
    }

    // clear: delete every REAL node and go back to an empty list
    void clear() {
        while (head.next != &tail) {
            removeNode(head.next);
        }
    }

    // print: 10 -> 20 -> 30 -> NULL      (has to write through cout)
    void print() {
        Node* cur = head.next;
        while (cur != &tail) {
            cout << cur->elem << " -> ";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }

    // traverseBackward: 30 -> 20 -> 10 -> NULL     (walks from tail using prev)
    void traverseBackward() {
        Node* cur = tail.prev;
        while (cur != &head) {
            cout << cur->elem << " -> ";
            cur = cur->prev;
        }
        cout << "NULL" << endl;
    }

    // ------------------------------------------------------------------
    //  THE PSET:  insertAtPosition(index, num)
    //
    //     int insertAtPosition(int index, int num)
    //         insert num so that it ends up AT that 0-based index; return the index,
    //         or -1 (and leave the list alone) if the index is not 0..size
    //
    //     index 0 = front, index == size = append at the end.
    // ------------------------------------------------------------------

    // ===== ANSWER (comment this block out to solve it yourself) =====
    int insertAtPosition(int index, int num) {
        if (index < 0 || index > size) return -1;      // out of range: reject

        Node* cur = head.next;
        for (int i = 0; i < index; i++) {
            cur = cur->next;                            // cur = node currently AT index (or &tail)
        }
        addBetween(num, cur->prev, cur);                // splice in right before cur
        return index;
    }
    // ===== END ANSWER =====
};

#endif
