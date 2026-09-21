#ifndef NODE_HPP
#define NODE_HPP

// Every node has BOTH links: prev <- [elem] -> next
struct Node {
    int   elem;
    Node* next;
    Node* prev;

    Node(int val) : elem(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node* n, Node* p) : elem(val), next(n), prev(p) {}
};

#endif
