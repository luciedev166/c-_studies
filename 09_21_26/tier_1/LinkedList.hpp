#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
using namespace std;

struct Node{
    int elem;
    Node *next;
    Node *prev;
    Node() : elem(0), next(nullptr), prev(nullptr) {}
    Node(int val) : elem(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node *n, Node *p) : elem(val), next(n), prev(p) {}
};

struct Node{
    int elem;
    Node *next;
    Node *prev;
    Node(int val): elem(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node *n, Node *p): elem(val), next(n), prev(p) {}
};

class DoublyLinkedList {
private:
    Node head;
    Node tail;
    int size;

    void addBetween(int val, Node *pred, Node *succ)
    {
        Node *n = new Node(val, succ, pred);
        n->next = succ;
        pred->next = n;
        succ->prev = n;
        size++;
    }

    int removeNode(Node *target)
    {
        Node *pred =  target->prev;
        Node *succ =  target->next;

        int elem = target->elem;
        pred->next = succ;
        succ->prev = pred;

        delete target;
        size--;
        return elem;
    }

public:
    DoublyLinkedList() : head(0), tail(0) {
        head.next = &tail;
        tail.prev = &head;
        size = 0;
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

    void insertAtTail(int val)
    {
        addBetween(val, tail.prev, &tail);
    }


    void insertAtHead(int val)
    {
        addBetween(val, &head, head.next);
    }

    void insertAtPosition(int val, int pos){
        if(pos <= 0)
        {
            addBetween(val, &head, head.next);
            return;
        }
        if(pos >= size)
        {
            addBetween(val, tail.prev, &tail);
            return;
        }
        Node *cur = head.next;
        for(int i = 0; i < pos; i++)
        {
            cur = cur ->next;
        }
        addBetween(val, cur->prev, cur);

    }
    void insertAfter(Node *target, int val)
    {
        if(target == nullptr || target == &tail)
        {
            cout << "Target not found, cannot insert." << endl;
            return;
        }
        addBetween(val, target, target->next);
    }

    void insertBefore(Node* target, int val) {
        if (target == nullptr || target == &head) {
            cout << "Target not found, cannot insert." << endl;
            return;
        }
        addBetween(val, target->prev, target);
    }

    int deleteHead()
    {
        if(isEmpty()) {cout <<"Empty list, can't delete" << endl; return - 1;}

        return removeNode(head.next);
    }

    int deleteTail()
    {
        if(isEmpty()) {cout << "List empty, can't delete" << endl; return - 1;}
        return removeNode(tail.prev);
    }

    int deleteAtPosition(int pos)
    {
        if(pos < 0 || pos >= size) {cout << "invalid position" << endl; return -1;}
        if(isEmpty()) {cout << "Empty list" << endl; return -1;}

        Node *cur = head.next;
        for(int i = 0; i < pos; i++) cur = cur -> next;
        
        return removeNode(cur);
    }

    bool deleteByValue(int val)
    {
        Node *cur = head.next;
        while(cur != &tail)
        {
            if(cur->elem == val) {removeNode(cur); return true;}
            cur = cur ->next;
        }

        cout << "value not found" << endl;
        return false;
    }

    bool deleteNode(Node *target)
    {
        if(!contains(target))
        {
            cout << "Not in list" << endl;
            return false;
        }

        removeNode(target);
        return true;
    }

    bool contains(Node * target)
    {
        Node *cur = head.next;
        while(cur != &tail)
        {
            if(cur == target) return true;
            cur = cur -> next;
        }
        return false;
    }


    void traverseForward()
    {
        Node *cur = head.next;
        while(cur != &tail)
        {
            cout << cur -> elem<< "->";
            cur = cur -> next;
        }
        cout << "NULL" << endl;
    }

    void traverseBackward() {
        Node* curr = tail.prev;
        while (curr != &head) { cout << curr->elem << " -> "; curr = curr->prev; }
        cout << "NULL" << endl;
    }

    Node *search(int val)
    {
        Node *cur = head.next;
        while(cur != &tail)
        {
            if(cur->elem == val)
            {
                return cur;
            }
            cur = cur -> next;
        }
        return nullptr;
    }

    Node* getNodeAt(int pos) {
        if (pos < 0 || pos >= size) return nullptr;
        Node* curr = head.next;
        for (int i = 0; i < pos; i++) curr = curr->next;
        return curr;
    }

    // void clear() {
    //     Node* curr = head.next;
    //     while (curr != &tail) { Node* next = curr->next; delete curr; curr = next; }
    //     head.next = &tail;
    //     tail.prev = &head;
    //     size = 0;
    // }

    void clear()
    {
        Node *cur = head.next;
        while(cur != &tail)
        {
            Node *next = cur -> next;
            delete cur;
            cur = next;
        }
        head.next = &tail;
        tail.prev = &head;
        size = 0;
    }
};

#endif