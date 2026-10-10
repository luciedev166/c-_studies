#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  DRILL: removeFront
//
//  ABOVE the "DO NOT CHANGE" line  = YOUR AREA. Study the solved function,
//                                    then comment it out and rewrite it yourself.
//  BELOW the "DO NOT CHANGE" line  = helper functions that main.cpp relies on.
//                                    They never call the function you're
//                                    practicing, so a bug in yours can't break them.
//  (The Node struct, members, constructor and destructor are given scaffolding.
//   Keep the names head / tail / size / elem / next: the helpers use them.)
// =====================================================================

struct Node {
    int elem;
    Node *next;
    Node() : elem(0), next(nullptr) {}
    Node(int val) : elem(val), next(nullptr) {}
    Node(int val, Node *n) : elem(val), next(n) {}
};

class SinglyLinkedList {
private:
    Node *head;
    Node *tail;
    int size;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyLinkedList() {
        clear();
    }
 
    // ---------------------------------------------------------------
    //  FUNCTION UNDER PRACTICE: removeFront
    // ---------------------------------------------------------------


    int removeFront()
    {
        if(head == nullptr)return -1;
        int val =  head->elem;
        Node *toDel = head;
        head = head->next;
        if(head == nullptr)
        {
            tail = nullptr;
        }
        size--;
        delete toDel;
        return val;
    }

    //=====================================================================
    //======== DO NOT CHANGE ANYTHING UNDER THIS LINE ========
    //=====================================================================
public:

    void addFront(int val)
    {
        Node *n = new Node(val, head);
        head = n;
        if (tail == nullptr) tail = n;
        size++;
    }

    void addBack(int val)
    {
        Node *n = new Node(val);
        if (head == nullptr) head = tail = n;
        else { tail->next = n; tail = n; }
        size++;
    }

    void insertAtN(int val, int pos)
    {
        if (pos <= 0)
        {
            head = new Node(val, head);
            if (tail == nullptr) tail = head;
            size++;
            return;
        }
        if (pos >= size)
        {
            Node *n = new Node(val);
            if (head == nullptr) head = tail = n;
            else { tail->next = n; tail = n; }
            size++;
            return;
        }
        Node *prev = head;
        for (int i = 0; i < pos - 1; i++) prev = prev->next;
        prev->next = new Node(val, prev->next);
        size++;
    }

    int removeBack()
    {
        if (head == nullptr) { cout << "List empty, can't remove" << endl; return -1; }
        int val = tail->elem;
        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
        }
        else
        {
            Node *prev = head;
            while (prev->next != tail) prev = prev->next;
            delete tail;
            tail = prev;
            tail->next = nullptr;
        }
        size--;
        return val;
    }

    int removeNreturnN(int pos)
    {
        if (head == nullptr || pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }
        Node *target;
        if (pos == 0)
        {
            target = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
        }
        else
        {
            Node *prev = head;
            for (int i = 0; i < pos - 1; i++) prev = prev->next;
            target = prev->next;
            prev->next = target->next;
            if (target == tail) tail = prev;
        }
        int val = target->elem;
        delete target;
        size--;
        return val;
    }

    int removeAllVals(int val)
    {
        int count = 0;
        while (head != nullptr && head->elem == val)
        {
            Node *old = head;
            head = head->next;
            delete old;
            size--;
            count++;
        }
        if (head == nullptr) { tail = nullptr; return count; }
        Node *prev = head;
        while (prev->next != nullptr)
        {
            if (prev->next->elem == val)
            {
                Node *target = prev->next;
                prev->next = target->next;
                if (target == tail) tail = prev;
                delete target;
                size--;
                count++;
            }
            else prev = prev->next;
        }
        return count;
    }

    int getValAtN(int pos)
    {
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }
        Node *cur = head;
        for (int i = 0; i < pos; i++) cur = cur->next;
        return cur->elem;
    }

    bool getVal(int val)
    {
        for (Node *cur = head; cur != nullptr; cur = cur->next)
            if (cur->elem == val) return true;
        return false;
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

    void printFromHead()
    {
        for (Node *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->elem << " -> ";
        cout << "NULL" << endl;
    }

private:
    void printBackwardHelper(Node *cur)
    {
        if (cur == nullptr) return;
        printBackwardHelper(cur->next);
        cout << cur->elem << " -> ";
    }

public:
    void printFromTail()
    {
        printBackwardHelper(head);
        cout << "NULL" << endl;
    }

    bool isEmpty() { return head == nullptr; }

    int getSize() { return size; }

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
            if (count > 100000) return "a node points back into the list (infinite loop)";
        }
        if (head == nullptr && tail != nullptr) return "list is empty but tail is not nullptr (tail left dangling)";
        if (head != nullptr && tail == nullptr) return "list has nodes but tail is nullptr";
        if (tail != last) return "tail is not pointing at the last node";
        if (count != size) return "size says " + to_string(size) + " but the list really has " + to_string(count) + " nodes";
        return "OK";
    }
};

#endif
