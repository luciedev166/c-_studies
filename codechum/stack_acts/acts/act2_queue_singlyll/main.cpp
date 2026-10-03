#include <iostream>
#include "sllqueue.hpp"
using namespace std;

int main() {
    Queue* queue = new SLLQueue();
    int input;
    char op;
    do {
        cout << "Enter op: ";
        cin >> op;
        switch (op) {
            case 'e' :
                cin >> input;
                queue->enqueue(input);
                break;
            case 'd':
                cout << "Removed " << queue->dequeue() << endl;
                break;
            case 'f':
                cout << "First element: " << queue->first() << endl;
                break;
            case 's':
                cout << "Size: " << queue->size() << endl;
                break;
            case '?':
                cout << "Empty? " << queue->isEmpty() << endl;
                break;
            case 'x':
                cout << "Exiting";
                break;
            default:
                cout << "Unknown operation" << endl;
                break;
        }
    } while (op != 'x');
    return 0;
}
