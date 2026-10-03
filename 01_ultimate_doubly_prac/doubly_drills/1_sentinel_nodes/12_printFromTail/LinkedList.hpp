#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: printFromTail
//  STYLE: sentinels are NODES: Node Head, Tail; (objects inside the list, use &Head / &Tail)
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names Head / Tail / size / elem / prev / next: the helpers use them.)
// =====================================================================
struct Node {
    int elem;
    Node *prev;
    Node *next;
    Node() : elem(0), prev(nullptr), next(nullptr) {}
    Node(int val) : elem(val), prev(nullptr), next(nullptr) {}
    Node(int val, Node *p, Node *n) : elem(val), prev(p), next(n) {}
};

class DoublyLinkedList {
private:
    Node Head;        // SENTINEL (guard) #1: a real Node object living inside the list. NOT a pointer.
    Node Tail;        // SENTINEL (guard) #2: same. The real nodes always sit BETWEEN these two.
    int size;         // counts real nodes only (never the sentinels)

public:
    DoublyLinkedList() : size(0)
    {
        Head.next = &Tail;    // empty list = the two sentinels pointing at each other
        Tail.prev = &Head;
    }

    ~DoublyLinkedList()
    {
        clear();                   // nothing else to free: the sentinels die with the list object
    }

    // The sentinels point at each other's ADDRESS, so a copy would point into the original.
    DoublyLinkedList(const DoublyLinkedList &) = delete;
    DoublyLinkedList &operator=(const DoublyLinkedList &) = delete;

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: printFromTail
    // ---------------------------------------------------------------

public:
    void printFromTail()
    {
        for (Node *cur = Tail.prev; cur != &Head; cur = cur->prev)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:
    void addHead(int val)
    {
        Node *first = Head.next;
        Node *n = new Node(val, &Head, first);
        first->prev = n;
        Head.next = n;
        size++;
    }

    void addTail(int val)
    {
        Node *last = Tail.prev;
        Node *n = new Node(val, last, &Tail);
        last->next = n;
        Tail.prev = n;
        size++;
    }

    int removeHead()
    {
        if (size == 0) { cout << "List empty, can't remove" << endl; return -1; }
        Node *old = Head.next;
        int val = old->elem;
        Head.next = old->next;
        old->next->prev = &Head;
        delete old;
        size--;
        return val;
    }

    int removeTail()
    {
        if (size == 0) { cout << "List empty, can't remove" << endl; return -1; }
        Node *old = Tail.prev;
        int val = old->elem;
        Tail.prev = old->prev;
        old->prev->next = &Tail;
        delete old;
        size--;
        return val;
    }

    int get(int pos)
    {
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }
        Node *cur = Head.next;
        for (int i = 0; i < pos; i++) cur = cur->next;
        return cur->elem;
    }

    int remove(int num)
    {
        for (Node *cur = Head.next; cur != &Tail; cur = cur->next)
            if (cur->elem == num)
            {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                delete cur;
                size--;
                return num;
            }
        cout << "Value not found" << endl;
        return -1;
    }

    void addAt(int val, int pos)
    {
        if (pos < 0) pos = 0;
        if (pos > size) pos = size;
        Node *succ = Head.next;
        for (int i = 0; i < pos; i++) succ = succ->next;
        Node *pred = succ->prev;
        Node *n = new Node(val, pred, succ);
        pred->next = n;
        succ->prev = n;
        size++;
    }

    int removePosn(int pos)
    {
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }
        Node *cur = Head.next;
        for (int i = 0; i < pos; i++) cur = cur->next;
        int val = cur->elem;
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
        size--;
        return val;
    }

    int removeAllVals(int val)
    {
        int count = 0;
        Node *cur = Head.next;
        while (cur != &Tail)
        {
            Node *nxt = cur->next;
            if (cur->elem == val)
            {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                delete cur;
                size--;
                count++;
            }
            cur = nxt;
        }
        return count;
    }

    int getVal(int val)
    {
        int idx = 0;
        for (Node *cur = Head.next; cur != &Tail; cur = cur->next, idx++)
            if (cur->elem == val) return idx;
        return -1;
    }

    void printFromHead()
    {
        for (Node *cur = Head.next; cur != &Tail; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    void clear()
    {
        Node *cur = Head.next;
        while (cur != &Tail)
        {
            Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        Head.next = &Tail;
        Tail.prev = &Head;
        size = 0;
    }

    bool isEmpty() { return size == 0; }

    int getSize() { return size; }

    // ---- TEST-ONLY helpers (not part of the usual assignment) ----------------

    // The list read BACKWARD through the prev pointers, e.g. "30 -> 20 -> 10 -> NULL".
    // (Separate from printFromTail on purpose, so the tests can check prev pointers
    //  even while you are busy rewriting printFromTail.)
    string backwardText()
    {
        string s;
        int guard = 0;
        for (Node *cur = Tail.prev; cur != &Head && cur != nullptr && guard < 100000; cur = cur->prev, guard++)
            s += to_string(cur->elem) + " -> ";
        return s + "NULL";
    }

    // Checks that next pointers, prev pointers and size all agree with each other.
    // Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        int count = 0;
        Node *before = &Head;                           // the node we just came from
        Node *cur = Head.next;
        while (cur != &Tail)
        {
            if (cur == nullptr) return "a next pointer is nullptr before the Tail was reached (chain is cut)";
            if (cur->prev != before) return "the node holding " + to_string(cur->elem) + " has a wrong prev pointer";
            before = cur;
            cur = cur->next;
            count++;
            if (count > 100000) return "a node points back into the list (infinite loop)";
        }
        if (Tail.prev != before) return "Tail's prev is not pointing at the last node";
        if (count != size) return "size says " + to_string(size) + " but the list really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
