// TODO implement DoublyDeque as a subclass of Deque
#include <iostream>
#include "deque.h"

using namespace std;
class DoublyDeque : public Deque
{
    private:
    struct Node {
        int elem;
        Node *prev;
        Node *next;
    };

    Node *head;
    Node *tail;

    int s;

    void addBetween(int val, Node* pred, Node *succ)
    {
        Node *n = new Node;
        n->elem = val;
        n->prev = pred;
        n->next = succ;
        pred->next = n;
        succ->prev = n;
        s++;
    }

    int removeNode(Node *n)
    {
        int val = n->elem;
        n->prev->next = n->next;
        n->next->prev = n->prev;
        delete n;
        s--;
        return val;
    }

    public:
    DoublyDeque()
    {
        head = new Node;
        tail = new Node;
        head->elem = 0;
        head->prev = nullptr;
        head->next = tail;
        tail->elem = 0;
        tail->prev = head;
        tail->next = nullptr;
        s = 0;

    }

    ~DoublyDeque()
    {
        Node *cur = head;
        while(cur != nullptr)
        {
            Node *n = cur->next;
            delete cur;
            cur = n;
        }

    }

    void addFirst(int val)
    {
        addBetween(val, head, head->next);
    }

    void addLast(int val)
    {
        addBetween(val, tail->prev, tail);
    }

    int removeFirst()
    {
        if(s == 0) return 0;
        return removeNode(head->next);
    }
    int removeLast()
    {

        if(s == 0) return 0;
        return removeNode(tail->prev);
    }

    int size()
    {
        return s;
    }

    bool isEmpty()
    {
        return s == 0;
    }

    void print()
    {
        if(s == 0)
        {
            cout << "Empty" << endl;
            return;
        }

        cout << "From HEAD: ";
        for(Node *cur = head->next; cur != tail; cur = cur->next)
        {
            if(cur != head->next)
            {
                cout << " -> ";
            }
            cout << cur->elem;
        }
        cout << endl;

        cout << "From TAIL: ";
        for(Node *cur = tail->prev; cur != head; cur = cur->prev)
        {
            if(cur != tail->prev)
            cout << " <- ";
            cout << cur->elem;
        }
        cout << endl;
    }
};
