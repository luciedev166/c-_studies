#include <iostream>
#include "arraystack.hpp"
using namespace std;

int main() {
    Stack* stack = new ArrayStack();
    char input;
    char op;
    do {
        cout << "Enter op: ";
        cin >> op;
        switch (op) {
            case 'a' :
                cin >> input;
                stack->push(input);
                break;
            case 'r':
                cout << "Removed " << stack->pop() << endl;
                break;
            case 't':
                cout << "Last element: " << stack->top() << endl;
                break;
            case 's':
                cout << "Size: " << stack->size() << endl;
                break;
            case '?':
                cout << "Empty? " << stack->isEmpty() << endl;
                break;
            case 'x':
                cout << "Exiting";
                break;
            default:
                cout << "Unknown operation." << endl;
        }
    } while (op != 'x');
    return 0;
}
