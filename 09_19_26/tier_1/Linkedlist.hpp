#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>

struct Node {
    int data;
    Node* next;
    Node* prev;
    Node() : elem(0), next(nullptr), prev(nullptr) {}
    Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

struct Node{
    int 
}

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    // ---------- Tier 1: status ----------

    bool isEmpty() {
        return head == nullptr;
    }

    int getSize() {
        return size;
    }

    // ---------- Tier 1: insert ----------

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

    // pos is clamped: pos <= 0 behaves like insertAtHead,
    // pos >= size behaves like insertAtTail
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
        // insert new node right before curr
        Node* newNode = new Node(val);
        Node* prevNode = curr->prev;
        newNode->next = curr;
        newNode->prev = prevNode;
        prevNode->next = newNode;
        curr->prev = newNode;
        size++;
    }

    // insert val immediately AFTER target node
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

    // insert val immediately BEFORE target node
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

    // ---------- Tier 1: delete ----------

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
            tail = nullptr; // list is now empty
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
            head = nullptr; // list is now empty
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

    // removes FIRST occurrence only
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

    // delete via a direct pointer (already have the node, no search needed)
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

    // ---------- Tier 1: traverse / search ----------

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

    // returns pointer to first matching node, or nullptr if not found
    Node* search(int val) {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // returns pointer to node at given index, or nullptr if out of bounds
    Node* getNodeAt(int pos) {
        if (pos < 0 || pos >= size) return nullptr;
        Node* curr = head;
        for (int i = 0; i < pos; i++) {
            curr = curr->next;
        }
        return curr;
    }

    // ---------- Tier 1: clear / destructor ----------

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
};

#endif