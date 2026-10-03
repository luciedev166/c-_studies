#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int elem;
    Node *next;
    Node *prev;
    Node(int val) : elem(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node *n, Node *p) : elem(val), next(n), prev(p) {}
};

class DoublyLinkedList {
private:
    Node head;
    Node tail;
    int size;

    // ================================================================
    // IGNORE THIS — infrastructure / Tier 1, already done for you.
    // You need these working in order to build/test the Tier 2 stuff
    // below. Same sentinels, same addBetween/removeNode as always.
    // ================================================================


    void addBetween(int val, Node *pred, Node *succ)
    {
        Node *n  = new Node(val, succ, pred);
        n->next = succ;
        succ->prev = n;
        pred->next = n;
        size++;
    }

    int removeNode(Node *target)
    {
        Node *pred = target->prev;
        Node *succ = target->next;
        int elem = target->elem;
        delete target;
        pred->next = succ;
        succ->prev = pred;

        size--;
        return elem;
    }

public:
    DoublyLinkedList() : head(0), tail(0)
    {
        head.next = &tail;
        tail.prev = &head;
        size= 0;
    }


    ~DoublyLinkedList() {
        clear();
    }

    bool isEmpty() {
        return head.next == &tail;
    }

    int getSize() {
        return size;
    }

    void insertAtTail(int val) {
        addBetween(val, tail.prev, &tail);
    }

    void insertAtHead(int val) {
        addBetween(val, &head, head.next);
    }

    int deleteHead() {
        if (isEmpty()) { cout << "Empty list, can't delete" << endl; return -1; }
        return removeNode(head.next);
    }

    int deleteTail() {
        if (isEmpty()) { cout << "List empty, can't delete" << endl; return -1; }
        return removeNode(tail.prev);
    }

    bool deleteByValue(int val) {
        Node *cur = head.next;
        while (cur != &tail) {
            if (cur->elem == val) { removeNode(cur); return true; }
            cur = cur->next;
        }
        cout << "value not found" << endl;
        return false;
    }

    bool contains(Node *target) {
        Node *cur = head.next;
        while (cur != &tail) {
            if (cur == target) return true;
            cur = cur->next;
        }
        return false;
    }

    void traverseForward() {
        Node *cur = head.next;
        while (cur != &tail) {
            cout << cur->elem << " -> ";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }

    void traverseBackward() {
        Node *cur = tail.prev;
        while (cur != &head) {
            cout << cur->elem << " -> ";
            cur = cur->prev;
        }
        cout << "NULL" << endl;
    }

    Node* search(int val) {
        Node *cur = head.next;
        while (cur != &tail) {
            if (cur->elem == val) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void clear() {
        Node *cur = head.next;
        while (cur != &tail) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        head.next = &tail;
        tail.prev = &head;
        size = 0;
    }

    // ================================================================
    // PRACTICE THESE — TIER 2: Very Common Lab/Exam Staples
    // Implement each function yourself. Signatures are fixed so
    // main_test_tier2.cpp compiles against them without changes.
    // Delete the "not implemented yet" line inside each one once you
    // start writing real code — it's only there so the test driver
    // runs cleanly and tells you which functions are still stubs.
    // ================================================================

    // Return a pointer to the node at index pos (0-based), or nullptr
    // if pos is out of range.
    Node* getNodeAt(int pos) {
        cout << "[getNodeAt] not implemented yet" << endl;
        return nullptr;
    }

    // Return the value stored at the front of the list.
    int getFront() {
        cout << "[getFront] not implemented yet" << endl;
        return -1;
    }

    // Return the value stored at the back of the list.
    int getBack() {
        cout << "[getBack] not implemented yet" << endl;
        return -1;
    }

    // Insert val into its correct position, ASSUMING the list is
    // already sorted ascending. If equal values already exist,
    // insert after them (keep it stable).
    void insertSorted(int val) {
        cout << "[insertSorted] not implemented yet" << endl;
    }

    // Remove duplicate values, ASSUMING the list is sorted ascending
    // (duplicates are always adjacent — one pass is enough).
    void removeDuplicatesSorted() {
        cout << "[removeDuplicatesSorted] not implemented yet" << endl;
    }

    // Remove duplicate values from an UNSORTED list. Keep the first
    // occurrence of each value; remove every later repeat.
    void removeDuplicatesUnsorted() {
        cout << "[removeDuplicatesUnsorted] not implemented yet" << endl;
    }

    // Remove EVERY node whose value == val (not just the first match).
    // Return how many nodes were removed.
    int removeAll(int val) {
        cout << "[removeAll] not implemented yet" << endl;
        return -1;
    }

    // Return the value of the middle node, using index = size/2
    // (integer division). For an even-length list this lands on the
    // SECOND of the two middle nodes.
    int findMiddle() {
        cout << "[findMiddle] not implemented yet" << endl;
        return -1;
    }

    // Structurally swap nodes a and b's POSITIONS in the list — relink
    // prev/next pointers, don't just copy their .elem values. Must
    // work even when a or b is the first or last real node.
    void swapNodes(Node* a, Node* b) {
        cout << "[swapNodes] not implemented yet" << endl;
    }

    // Simpler version: find the nodes holding val1 and val2 and swap
    // their .elem fields directly (no relinking). Print a message and
    // do nothing if either value isn't found.
    void swapByValue(int val1, int val2) {
        cout << "[swapByValue] not implemented yet" << endl;
    }

    // Move every node from `other` onto the end of this list, in
    // order. `other` must be empty when this returns.
    void concatenate(DoublyLinkedList& other) {
        cout << "[concatenate] not implemented yet" << endl;
    }

    // Split this list in half: the first ceil(size/2) nodes stay
    // here, the rest move into `secondHalf` (assume it starts empty),
    // preserving order.
    void splitList(DoublyLinkedList& secondHalf) {
        cout << "[splitList] not implemented yet" << endl;
    }

    // Return a vector containing this list's values, head to tail.
    vector<int> toArray() {
        cout << "[toArray] not implemented yet" << endl;
        return {};
    }

    // Assuming this list starts empty, insert every value in arr onto
    // the tail, in order.
    void fromArray(const vector<int>& arr) {
        cout << "[fromArray] not implemented yet" << endl;
    }

    // Return how many nodes hold the value val.
    int countOccurrences(int val) {
        cout << "[countOccurrences] not implemented yet" << endl;
        return -1;
    }

    // Find the FIRST node whose value == oldVal and change it to
    // newVal. Return true if found+updated, false if oldVal isn't
    // present.
    bool updateValue(int oldVal, int newVal) {
        cout << "[updateValue] not implemented yet" << endl;
        return false;
    }

    // Rotate the list left by k positions (k can be 0, equal to size,
    // or greater than size — handle all of those with %).
    // e.g. [1,2,3,4,5] rotated left by 2 -> [3,4,5,1,2]
    void rotateByK(int k) {
        cout << "[rotateByK] not implemented yet" << endl;
    }
};

#endif