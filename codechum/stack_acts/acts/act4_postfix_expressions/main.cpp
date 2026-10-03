#include <iostream>
#include <string>
#include "arraystack.hpp" // ALREADY IMPLEMENTED
using namespace std;

int main() {
    Stack* stack = new ArrayStack();
    string input;
    do {
        cout << "Enter: ";
        cin >> input;

        if (input == "+" || input == "-" || input == "*" || input == "/") {
            int a = stack->pop();
            int b = stack->pop();
            int result = 0;

            if (input == "+") {
                result = b + a;
            } else if (input == "-") {
                result = b - a;
            } else if (input == "*") {
                result = b * a;
            } else {
                result = b / a;
            }
            stack->push(result);
        } else if (input == "x") {
            cout << "Answer is " << stack->top() << endl;
        } else {
            stack->push(stoi(input));
        }

    } while (input != "x");
    return 0;
}
