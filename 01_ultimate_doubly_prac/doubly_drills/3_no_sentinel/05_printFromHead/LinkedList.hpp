#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: printFromHead
//  STYLE: NO sentinels: plain Node *head, *tail (every function handles the nullptr edge cases itself)
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function(s),
//                                    then comment them out and rewrite them yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names head / tail / size / elem / prev / next: the helpers use them.)
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
    Node *head;
    Node *tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList()
    {
        clear();
    }

    // Copying would make two lists share the same nodes (double delete).
    DoublyLinkedList(const DoublyLinkedList &) = delete;
    DoublyLinkedList &operator=(const DoublyLinkedList &) = delete;

    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: printFromHead
    // ---------------------------------------------------------------

public:
    void printFromHead()
    {
        for (Node *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:
    void addHead(int val)
    {
        Node *n = new Node(val, nullptr, head);         // nothing before it, the old head after it
        if (head != nullptr) head->prev = n;            // the old head must point BACK at the new node
        else tail = n;                                  // EDGE CASE: list was empty, so the new node is also the tail
        head = n;
        size++;
    }

    void addTail(int val)
    {
        Node *n = new Node(val, tail, nullptr);         // the old tail before it, nothing after it
        if (tail != nullptr) tail->next = n;            // the old tail must point FORWARD at the new node
        else head = n;                                  // EDGE CASE: list was empty, so the new node is also the head
        tail = n;
        size++;
    }

    int removeHead()
    {
        if (head == nullptr) { cout << "List empty, can't remove" << endl; return -1; }
        Node *old = head;
        int val = old->elem;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;      // the new head has nothing before it
        else tail = nullptr;                            // EDGE CASE: that was the only node, list is now empty
        delete old;
        size--;
        return val;
    }

    int removeTail()
    {
        if (tail == nullptr) { cout << "List empty, can't remove" << endl; return -1; }
        Node *old = tail;
        int val = old->elem;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;      // the new tail has nothing after it
        else head = nullptr;                            // EDGE CASE: that was the only node, list is now empty
        delete old;
        size--;
        return val;
    }

    void printFromTail()
    {
        for (Node *cur = tail; cur != nullptr; cur = cur->prev)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
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

    bool isEmpty() { return head == nullptr; }

    int getSize() { return size; }

    // ---- TEST-ONLY helpers (not part of the usual assignment) ----------------

    // The list read BACKWARD through the prev pointers, e.g. "30 -> 20 -> 10 -> NULL".
    // (Separate from printFromTail on purpose, so the tests can check prev pointers
    //  even while you are busy rewriting printFromTail.)
    string backwardText()
    {
        string s;
        int guard = 0;
        for (Node *cur = tail; cur != nullptr && guard < 100000; cur = cur->prev, guard++)
            s += to_string(cur->elem) + " -> ";
        return s + "NULL";
    }

    // Checks that head / tail / next pointers / prev pointers / size all agree.
    // Returns "OK", or a sentence describing what is broken.
    string integrity()
    {
        if (head == nullptr && tail != nullptr) return "list is empty but tail is not nullptr (tail left dangling)";
        if (head != nullptr && tail == nullptr) return "list has nodes but tail is nullptr";
        int count = 0;
        Node *before = nullptr;
        Node *cur = head;
        while (cur != nullptr)
        {
            if (cur->prev != before)
                return before == nullptr ? "head's prev is not nullptr"
                                         : "the node holding " + to_string(cur->elem) + " has a wrong prev pointer";
            before = cur;
            cur = cur->next;
            count++;
            if (count > 100000) return "a node points back into the list (infinite loop)";
        }
        if (tail != before) return "tail is not pointing at the last node";
        if (count != size) return "size says " + to_string(size) + " but the list really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
