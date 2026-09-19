#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>

struct Node {
    int data;
    Node* next;
    Node* prev;
    Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

    // deep-copies another list's nodes into this one (used by copy ctor/assign)
    void copyFrom(const DoublyLinkedList& other) {
        head = nullptr;
        tail = nullptr;
        size = 0;
        Node* curr = other.head;
        while (curr != nullptr) {
            insertAtTail(curr->data);
            curr = curr->next;
        }
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // ---------- Rule of Three (needed since splitList() returns by value) ----------

    DoublyLinkedList(const DoublyLinkedList& other) {
        copyFrom(other);
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~DoublyLinkedList() {
        clear();
    }

    // ================= TIER 1 =================

    bool isEmpty() {
        return head == nullptr;
    }

    int getSize() {
        return size;
    }

    void insertAtHead(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void insertAtTail(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void insertAtPosition(int val, int pos) {
        if (pos <= 0 || isEmpty()) {
            insertAtHead(val);
            return;
        }
        if (pos >= size) {
            insertAtTail(val);
            return;
        }
        Node* curr = head;
        for (int i = 0; i < pos; i++) {
            curr = curr->next;
        }
        Node* newNode = new Node(val);
        Node* prevNode = curr->prev;
        newNode->next = curr;
        newNode->prev = prevNode;
        prevNode->next = newNode;
        curr->prev = newNode;
        size++;
    }

    void insertAfter(Node* target, int val) {
        if (target == nullptr) {
            std::cout << "Target not found, cannot insert." << std::endl;
            return;
        }
        if (target == tail) {
            insertAtTail(val);
            return;
        }
        Node* newNode = new Node(val);
        Node* nextNode = target->next;
        newNode->prev = target;
        newNode->next = nextNode;
        target->next = newNode;
        nextNode->prev = newNode;
        size++;
    }

    void insertBefore(Node* target, int val) {
        if (target == nullptr) {
            std::cout << "Target not found, cannot insert." << std::endl;
            return;
        }
        if (target == head) {
            insertAtHead(val);
            return;
        }
        Node* newNode = new Node(val);
        Node* prevNode = target->prev;
        newNode->next = target;
        newNode->prev = prevNode;
        prevNode->next = newNode;
        target->prev = newNode;
        size++;
    }

    int deleteHead() {
        if (isEmpty()) {
            std::cout << "List is empty, cannot delete." << std::endl;
            return -1;
        }
        Node* temp = head;
        int val = temp->data;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        size--;
        return val;
    }

    int deleteTail() {
        if (isEmpty()) {
            std::cout << "List is empty, cannot delete." << std::endl;
            return -1;
        }
        Node* temp = tail;
        int val = temp->data;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        size--;
        return val;
    }

    int deleteAtPosition(int pos) {
        if (isEmpty() || pos < 0 || pos >= size) {
            std::cout << "Invalid position or empty list." << std::endl;
            return -1;
        }
        if (pos == 0) return deleteHead();
        if (pos == size - 1) return deleteTail();

        Node* curr = head;
        for (int i = 0; i < pos; i++) {
            curr = curr->next;
        }
        int val = curr->data;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        size--;
        return val;
    }

    bool deleteByValue(int val) {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) {
                if (curr == head) { deleteHead(); return true; }
                if (curr == tail) { deleteTail(); return true; }
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
                size--;
                return true;
            }
            curr = curr->next;
        }
        std::cout << "Value not found." << std::endl;
        return false;
    }

    bool deleteNode(Node* target) {
        if (target == nullptr) return false;
        if (target == head) { deleteHead(); return true; }
        if (target == tail) { deleteTail(); return true; }
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
        size--;
        return true;
    }

    void traverseForward() {
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << curr->data << " -> ";
            curr = curr->next;
        }
        std::cout << "NULL" << std::endl;
    }

    void traverseBackward() {
        Node* curr = tail;
        while (curr != nullptr) {
            std::cout << curr->data << " -> ";
            curr = curr->prev;
        }
        std::cout << "NULL" << std::endl;
    }

    Node* search(int val) {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    Node* getNodeAt(int pos) {
        if (pos < 0 || pos >= size) return nullptr;
        Node* curr = head;
        for (int i = 0; i < pos; i++) {
            curr = curr->next;
        }
        return curr;
    }

    void clear() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // ================= TIER 2 =================

    int getFront() {
        if (isEmpty()) {
            std::cout << "List is empty." << std::endl;
            return -1;
        }
        return head->data;
    }

    int getBack() {
        if (isEmpty()) {
            std::cout << "List is empty." << std::endl;
            return -1;
        }
        return tail->data;
    }

    int countOccurrences(int val) {
        int count = 0;
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) count++;
            curr = curr->next;
        }
        return count;
    }

    // updates the FIRST matching node's value; returns false if not found
    bool updateValue(int oldVal, int newVal) {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == oldVal) {
                curr->data = newVal;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // removes EVERY node matching val (contrast with deleteByValue's first-only)
    void removeAll(int val) {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next; // cache BEFORE possible deletion
            if (curr->data == val) {
                deleteNode(curr); // handles head/tail/middle + size--
            }
            curr = next;
        }
    }

    // ASSUMES the list is already sorted ascending; inserts val in place
    void insertSorted(int val) {
        if (isEmpty() || val <= head->data) {
            insertAtHead(val);
            return;
        }
        if (val >= tail->data) {
            insertAtTail(val);
            return;
        }
        Node* curr = head;
        while (curr->next != nullptr && curr->next->data < val) {
            curr = curr->next;
        }
        insertAfter(curr, val);
    }

    // returns the UPPER-middle value (slow/fast pointer technique)
    int findMiddle() {
        if (isEmpty()) {
            std::cout << "List is empty." << std::endl;
            return -1;
        }
        Node* slow = head;
        Node* fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->data;
    }

    // rotates the list LEFT by k (elements shift toward head, wrap to tail)
    // handles k == 0, k >= size, and negative k
    void rotateByK(int k) {
        if (isEmpty() || size <= 1) return;
        k = ((k % size) + size) % size; // normalize negatives and overshoot
        if (k == 0) return;

        Node* newHead = head;
        for (int i = 0; i < k; i++) {
            newHead = newHead->next;
        }
        Node* newTail = newHead->prev;

        // stitch old tail to old head, then cut at the new boundary
        tail->next = head;
        head->prev = tail;

        newHead->prev = nullptr;
        newTail->next = nullptr;

        head = newHead;
        tail = newTail;
    }

    // swaps the DATA of the two nodes matching v1 and v2 (not the links)
    void swapByValue(int v1, int v2) {
        Node* n1 = search(v1);
        Node* n2 = search(v2);
        if (n1 == nullptr || n2 == nullptr) {
            std::cout << "One or both values not found." << std::endl;
            return;
        }
        int temp = n1->data;
        n1->data = n2->data;
        n2->data = temp;
    }

    // caller is responsible for delete[]-ing the returned array
    int* toArray() {
        int* arr = new int[size > 0 ? size : 1];
        Node* curr = head;
        int i = 0;
        while (curr != nullptr) {
            arr[i++] = curr->data;
            curr = curr->next;
        }
        return arr;
    }

    // wipes this list and rebuilds it from a raw array
    void fromArray(int arr[], int n) {
        clear();
        for (int i = 0; i < n; i++) {
            insertAtTail(arr[i]);
        }
    }

    // appends other's nodes onto the end of this list;
    // other is left EMPTY afterward (ownership transfer, no copying)
    void concatenate(DoublyLinkedList& other) {
        if (other.isEmpty()) return;
        if (isEmpty()) {
            head = other.head;
            tail = other.tail;
            size = other.size;
        } else {
            tail->next = other.head;
            other.head->prev = tail;
            tail = other.tail;
            size += other.size;
        }
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    // splits the list in half: keeps the FIRST half (extra node stays here
    // on odd-length lists), returns a NEW list containing the second half
    DoublyLinkedList splitList() {
        DoublyLinkedList secondHalf;
        if (size < 2) return secondHalf; // nothing meaningful to split

        int firstHalfSize = (size + 1) / 2; // ceil -> first half keeps the odd node
        Node* curr = head;
        for (int i = 0; i < firstHalfSize - 1; i++) {
            curr = curr->next;
        }
        // curr is now the last node of the first half
        Node* secondHead = curr->next;
        curr->next = nullptr;
        if (secondHead != nullptr) {
            secondHead->prev = nullptr;
        }

        secondHalf.head = secondHead;
        secondHalf.tail = tail;
        secondHalf.size = size - firstHalfSize;

        tail = curr;
        size = firstHalfSize;

        return secondHalf;
    }
};

#endif