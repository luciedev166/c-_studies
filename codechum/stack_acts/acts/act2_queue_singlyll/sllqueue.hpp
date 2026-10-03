#pragma once
#include "queue.hpp"
#include "node.hpp"

class SLLQueue : public Queue {
private:
    node* head;
    node* tail;
    int sz;

public:
    SLLQueue() {
        head = nullptr;
        tail = nullptr;
        sz = 0;
    }

    // Free any nodes still in the queue
    ~SLLQueue() {
        while (head != nullptr) {
            node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // O(1): add at the tail
    void enqueue(int value) override {
        node* newNode = new node;
        newNode->elem = value;
        newNode->next = nullptr;

        if (tail == nullptr) {
            head = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;
        sz++;
    }

    // O(1): remove from the head
    int dequeue() override {
        if (head == nullptr) {
            return 0;
        }
        node* temp = head;
        int value = temp->elem;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        sz--;
        return value;
    }

    int first() override {
        if (head == nullptr) {
            return 0;
        }
        return head->elem;
    }

    int size() override {
        return sz;
    }

    bool isEmpty() override {
        return sz == 0;
    }
};
