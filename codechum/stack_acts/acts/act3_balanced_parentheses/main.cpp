#include<iostream>
#include<string>
#include "arraystack.hpp" // Already implemented! Do not recreate!
using namespace std;

// TODO implement this method.
// Use the ArrayStack to perform the stack operations needed.
int is_balanced_parentheses(string str){
    Stack* stack = new ArrayStack();

    for(int i = 0; i < (int)str.length(); i++)
    {
        char c = str[i];

        if(c == '(' || c == '{' || c == '[')
        {
            stack->push(c);
        }
        else
        {
            if(stack->isEmpty())
            {
                return 3;
            }

            char open = stack->pop();
            if((c == ')' && open != '(') || (c == ']' && open != '[') ||
            (c == '}' && open != '{') )
            {
                return 1;
            }

        }
    }

    if(!stack->isEmpty())
    {
        return 2;
    }
    return 0;
}

// WARNING! Do not modify this method.
// Doing so will nullify your score for this problem.
int main(){
    string str;
    cout << "Enter a string with parentheses: ";
    cin >> str;
    int res = is_balanced_parentheses(str);
    switch(res) {
        case 0:
            cout << "The parentheses are balanced!" << endl;
            break;
        case 1:
            cout << "The parentheses are not balanced due to mismatch!" << endl;
            break;
        case 2:
            cout << "The parentheses are not balanced due to missing closing parentheses!" << endl;
            break;
        case 3:
            cout << "The parentheses are not balanced due to missing opening parentheses!" << endl;
            break;
    }
    return 0;
}
