#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: removeHead
//  STYLE: sentinels are POINTERS: Node *header, *trailer; (allocated with new in the constructor)
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names header / trailer / size / elem / prev / next: the helpers use them.)
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
    Node *header;     // SENTINEL (guard) #1: a pointer to a dummy Node allocated with new
    Node *trailer;    // SENTINEL (guard) #2: same. The real nodes always sit BETWEEN these two.
    int size;         // counts real nodes only (never the sentinels)

public:
    DoublyLinkedList() : size(0)
    {
        header = new Node();       // dummy nodes: their elem is never used
        trailer = new Node();
        header->next = trailer;    // empty list = the two sentinels pointing at each other
        trailer->prev = header;
    }

    ~DoublyLinkedList()
    {
        clear();                   // free the real nodes ...
        delete header;             // ... and then the two sentinels (they came from new)
        delete trailer;
    }

    // Copying would make two lists share the same sentinels (double delete).
    DoublyLinkedList(const DoublyLinkedList &) = delete;
    DoublyLinkedList &operator=(const DoublyLinkedList &) = delete;

    // ---------------------------------------------------------------
    //  PRIMITIVE USED BY removeHead: removeNode
    //  (rewrite it too if you want. Only your removeHead calls it;
    //   the helpers under the DO NOT CHANGE line don't, so they stay safe.)
    // ---------------------------------------------------------------

private:
    // PRIMITIVE: unlink one REAL node and return its value. (node must not be a sentinel.)
    // Thanks to the sentinels, node->prev and node->next ALWAYS exist: no nullptr cases.
    int removeNode(Node *node)
    {
        int val = node->elem;
        node->prev->next = node->next;                  // left neighbor skips over node
        node->next->prev = node->prev;                  // right neighbor skips back over node
        delete node;
        size--;
        return val;
    }

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: removeHead
    // ---------------------------------------------------------------

public:
    int removeHead()
    {
        if (size == 0) { cout << "List empty, can't remove" << endl; return -1; }
        return removeNode(header->next);                // the first real node is the header's next
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:
    void addHead(int val)
    {
        Node *first = header->next;
        Node *n = new Node(val, header, first);
        first->prev = n;
        header->next = n;
        size++;
    }

    void addTail(int val)
    {
        Node *last = trailer->prev;
        Node *n = new Node(val, last, trailer);
        last->next = n;
        trailer->prev = n;
        size++;
    }

    int removeTail()
    {
        if (size == 0) { cout << "List empty, can't remove" << endl; return -1; }
        Node *old = trailer->prev;
        int val = old->elem;
        trailer->prev = old->prev;
        old->prev->next = trailer;
        delete old;
        size--;
        return val;
    }

    int get(int pos)
    {
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }
        Node *cur = header->next;
        for (int i = 0; i < pos; i++) cur = cur->next;
        return cur->elem;
    }

    int remove(int num)
    {
        for (Node *cur = header->next; cur != trailer; cur = cur->next)
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
        Node *succ = header->next;
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
        Node *cur = header->next;
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
        Node *cur = header->next;
        while (cur != trailer)
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
        for (Node *cur = header->next; cur != trailer; cur = cur->next, idx++)
            if (cur->elem == val) return idx;
        return -1;
    }

    void printFromHead()
    {
        for (Node *cur = header->next; cur != trailer; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    void printFromTail()
    {
        for (Node *cur = trailer->prev; cur != header; cur = cur->prev)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    void clear()
    {
        Node *cur = header->next;
        while (cur != trailer)
        {
            Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        header->next = trailer;
        trailer->prev = header;
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
        for (Node *cur = trailer->prev; cur != header && cur != nullptr && guard < 100000; cur = cur->prev, guard++)
            s += to_string(cur->elem) + " -> ";
        return s + "NULL";
    }

    // Checks that next pointers, prev pointers and size all agree with each other.
    // Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        int count = 0;
        Node *before = header;                          // the node we just came from
        Node *cur = header->next;
        while (cur != trailer)
        {
            if (cur == nullptr) return "a next pointer is nullptr before the trailer was reached (chain is cut)";
            if (cur->prev != before) return "the node holding " + to_string(cur->elem) + " has a wrong prev pointer";
            before = cur;
            cur = cur->next;
            count++;
            if (count > 100000) return "a node points back into the list (infinite loop)";
        }
        if (trailer->prev != before) return "trailer's prev is not pointing at the last node";
        if (count != size) return "size says " + to_string(size) + " but the list really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
