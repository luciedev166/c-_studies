#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: front, isEmpty, getSize
// 
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function(s) you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names head / tail / size / elem / next: the helpers use them.)
// =====================================================================

struct Node {
    int elem;
    Node *next;
    Node() : elem(0), next(nullptr) {}
    Node(int val) : elem(val), next(nullptr) {}
};

// A queue is a singly linked list that you only touch at TWO places:
//   head = the FRONT of the line (people leave here    -> dequeue)
//   tail = the BACK of the line  (people join here     -> enqueue)
class Queue {
private:
    Node *head;   // front
    Node *tail;   // back
    int size;

public:
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    ~Queue() {
        clear();
    }

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: front, isEmpty, getSize
    // ---------------------------------------------------------------

    int front()
    {
        if(size == 0){cout<<"List is empty"<<endl; return -1;}
        return head->elem;
    }

    bool isEmpty()
    {
        return size == 0;
    }
    int getSize()
    {
        return size;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:

    void enqueue(int val)
    {
        Node *n = new Node(val);
        if (head == nullptr) head = tail = n;     // EDGE CASE: empty queue, the new node is front AND back
        else { tail->next = n; tail = n; }        // otherwise it joins the BACK (same as addBack)
        size++;
    }

    int dequeue()
    {
        if (head == nullptr) { cout << "Queue empty, can't dequeue" << endl; return -1; }
        Node *old = head;
        int val = old->elem;
        head = head->next;                        // the front leaves (same as removeFront)
        if (head == nullptr) tail = nullptr;      // EDGE CASE: queue just became empty, tail must not dangle
        delete old;
        size--;
        return val;
    }

    void clear()
    {
        Node *cur = head;
        while (cur != nullptr)
        {
            Node *nxt = cur->next;
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
        for (Node *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    // ---- TEST-ONLY helper (not part of the usual assignment): checks that
    //      head / tail / size agree with each other.
    //      Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        int count = 0;
        Node *cur = head;
        Node *last = nullptr;
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
