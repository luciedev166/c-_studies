#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <iostream>
using namespace std;

// ---------------------------------------------------------------
// SINGLY LINKED LIST -- no sentinels.
// head and tail are plain Node* that are nullptr when the list is empty.
// That means every operation has to think about:
//    1) empty list       (head == nullptr)
//    2) one-node list    (head == tail)
//    3) first / last position
//    4) out-of-range position
// ---------------------------------------------------------------

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

    // Helper for printFromTail: recurse to the end first, print on the way back.
    void printBackwardHelper(Node *cur)
    {
        if (cur == nullptr) return;          // base case: ran past the last node
        printBackwardHelper(cur->next);      // go deeper first...
        cout << cur->elem << " -> ";         // ...print while unwinding
    }

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    bool isEmpty() {
        return head == nullptr;              // (size == 0 also works)
    }

    int getSize() {
        return size;
    }

    // ---------------- ADD ----------------

    void addFront(int val)
    {
        Node *n = new Node(val, head);       // new node points at the old head
        head = n;
        if (tail == nullptr) tail = n;       // EDGE CASE: list was empty, so the new node is also the tail
        size++;
    }

    void addBack(int val)
    {
        Node *n = new Node(val);             // next is already nullptr
        if (isEmpty())
        {
            head = tail = n;                 // EDGE CASE: empty list, new node is both head and tail
        }
        else
        {
            tail->next = n;                  // link old tail to new node
            tail = n;                        // then move tail forward
        }
        size++;
    }

    // Insert val so that it ends up at index pos (0-based).
    // pos <= 0   -> front
    // pos >= size -> back
    // otherwise  -> walk to the node BEFORE pos and splice in
    void insertAtN(int val, int pos)
    {
        if (pos <= 0)    { addFront(val); return; }
        if (pos >= size) { addBack(val);  return; }

        Node *prev = head;
        for (int i = 0; i < pos - 1; i++)    // stop at index pos-1
        {
            prev = prev->next;
        }
        Node *n = new Node(val, prev->next); // new node points at what prev used to point at
        prev->next = n;                      // THEN prev points at new node (order matters!)
        size++;
    }

    // ---------------- REMOVE ----------------

    int removeFront()
    {
        if (isEmpty()) { cout << "List empty, can't remove" << endl; return -1; }

        Node *old = head;
        int val = old->elem;
        head = head->next;
        if (head == nullptr) tail = nullptr; // EDGE CASE: removed the only node, tail would dangle otherwise
        delete old;
        size--;
        return val;
    }

    int removeBack()
    {
        if (isEmpty()) { cout << "List empty, can't remove" << endl; return -1; }
        if (head == tail) return removeFront();   // EDGE CASE: one node, same as removeFront

        // Singly list can't go backwards, so walk to the node BEFORE the tail.
        Node *prev = head;
        while (prev->next != tail)
        {
            prev = prev->next;
        }
        int val = tail->elem;
        delete tail;
        tail = prev;
        tail->next = nullptr;                // the new last node must end the list
        size--;
        return val;
    }

    // Remove the node at index pos and return its value. -1 if pos is invalid.
    int removeNreturnN(int pos)
    {
        if (isEmpty())            { cout << "List empty, can't remove" << endl; return -1; }
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl;       return -1; }

        if (pos == 0)        return removeFront();
        if (pos == size - 1) return removeBack();

        Node *prev = head;
        for (int i = 0; i < pos - 1; i++)    // stop at index pos-1
        {
            prev = prev->next;
        }
        Node *target = prev->next;
        int val = target->elem;
        prev->next = target->next;           // skip over target
        delete target;
        size--;
        return val;
    }

    // Remove EVERY node whose elem == val. Return how many nodes were removed (0 if none).
    int removeAllVals(int val)
    {
        int count = 0;

        // Step 1: strip matches off the FRONT. A while (not an if), because the
        // new head can match too: 7 -> 7 -> 7 -> 1 has to lose all three 7s.
        while (head != nullptr && head->elem == val)
        {
            Node *old = head;
            head = head->next;
            delete old;
            size--;
            count++;
        }

        // EDGE CASE: list was empty, or every node matched. head is nullptr now,
        // so tail must be reset too or it dangles.
        if (head == nullptr)
        {
            tail = nullptr;
            return count;
        }

        // Step 2: head is now guaranteed NOT to match, so walk with prev and
        // look one node ahead (prev->next) at the candidate to delete.
        Node *prev = head;
        while (prev->next != nullptr)
        {
            if (prev->next->elem == val)
            {
                Node *target = prev->next;
                prev->next = target->next;   // skip over the match
                if (target == tail) tail = prev;   // EDGE CASE: deleted the last node, move tail back
                delete target;
                size--;
                count++;
                // Do NOT advance prev here: the new prev->next might match too (5 -> 6 -> 6 -> 6).
            }
            else
            {
                prev = prev->next;           // only advance when nothing was removed
            }
        }
        return count;
    }

    void clear()
    {
        Node *cur = head;
        while (cur != nullptr)
        {
            Node *nxt = cur->next;           // save next BEFORE deleting
            delete cur;
            cur = nxt;
        }
        head = nullptr;                      // reset everything, or the list holds dangling pointers
        tail = nullptr;
        size = 0;
    }

    // ---------------- GET ----------------

    // Return the value stored at index pos (0-based). -1 if pos is invalid.
    // Empty list is covered too: size is 0, so every pos is out of range.
    int getValAtN(int pos)
    {
        if (pos < 0 || pos >= size) { cout << "Invalid position" << endl; return -1; }

        Node *cur = head;
        for (int i = 0; i < pos; i++)        // walk pos steps from head
        {
            cur = cur->next;
        }
        return cur->elem;
    }

    // Return true if val is somewhere in the list, false otherwise.
    // Empty list: the loop never runs, so it falls through to false.
    bool getVal(int val)
    {
        Node *cur = head;
        while (cur != nullptr)
        {
            if (cur->elem == val) return true;
            cur = cur->next;
        }
        return false;
    }

    // ---------------- PRINT ----------------

    void printFromHead()
    {
        Node *cur = head;
        while (cur != nullptr)
        {
            cout << cur->elem << " -> ";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }

    void printFromTail()
    {
        printBackwardHelper(head);           // recursion does the "backwards" for us
        cout << "NULL" << endl;
    }
};

#endif