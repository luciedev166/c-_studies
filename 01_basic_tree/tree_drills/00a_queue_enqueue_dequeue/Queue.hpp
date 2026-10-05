#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: enqueue, dequeue
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function(s) you're
//                                    practicing, so a bug in yours can't break them.
//  (The QNode struct, members, constructor and destructor are given scaffolding.
//   Keep the names head / tail / size / elem / next: the helpers use them.)
// =====================================================================

struct QNode {
    int elem;
    QNode *next;
    QNode() : elem(0), next(nullptr) {}
    QNode(int val) : elem(val), next(nullptr) {}
};

// A queue is a singly linked list that you only touch at TWO places:
//   head = the FRONT of the line (people leave here    -> dequeue)
//   tail = the BACK of the line  (people join here     -> enqueue)
class Queue {
private:
    QNode *head;   // front
    QNode *tail;   // back
    int size;

public:
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    ~Queue() {
        clear();
    }

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: enqueue, dequeue
    // ---------------------------------------------------------------

    void enqueue(int val)
    {
        QNode *n = new QNode(val);
        if (head == nullptr) head = tail = n;     // EDGE CASE: empty queue, the new node is front AND back
        else { tail->next = n; tail = n; }        // otherwise it joins the BACK (same as addBack)
        size++;
    }

    int dequeue()
    {
        if (head == nullptr) { cout << "Queue empty, can't dequeue" << endl; return -1; }
        QNode *old = head;
        int val = old->elem;
        head = head->next;                        // the front leaves (same as removeFront)
        if (head == nullptr) tail = nullptr;      // EDGE CASE: queue just became empty, tail must not dangle
        delete old;
        size--;
        return val;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:

    int front()
    {
        if (head == nullptr) { cout << "Queue empty, no front" << endl; return -1; }
        return head->elem;                        // look at the front WITHOUT removing it
    }

    bool isEmpty()
    {
        return head == nullptr;
    }

    int getSize()
    {
        return size;
    }

    void clear()
    {
        QNode *cur = head;
        while (cur != nullptr)
        {
            QNode *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // front to back, e.g.  10 -> 20 -> 30 -> NULL   (empty queue prints just NULL)
    void printQueue()
    {
        for (QNode *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    // ---- TEST-ONLY helper (not part of the usual assignment): checks that
    //      head / tail / size agree with each other.
    //      Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        int count = 0;
        QNode *cur = head;
        QNode *last = nullptr;
        while (cur != nullptr)
        {
            last = cur;
            cur = cur->next;
            count++;
            if (count > 100000) return "a node points back into the queue (infinite loop)";
        }
        if (head == nullptr && tail != nullptr) return "queue is empty but tail is not nullptr (tail left dangling)";
        if (head != nullptr && tail == nullptr) return "queue has nodes but tail is nullptr";
        if (tail != last) return "tail is not pointing at the last node";
        if (count != size) return "size says " + to_string(size) + " but the queue really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
