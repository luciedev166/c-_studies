#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main(){
    stack<int> numstack;      // repeat counts
    stack<string> strstack;   // strings built before each '['
    string current = "";      // string currently being built
    string encoded;

    cout << "Enter encoded string: ";
    cin >> encoded;

    for (int i = 0; i < (int)encoded.length(); i++) {
        char c = encoded[i];

        if (c >= '0' && c <= '9') {
            // single-digit repeat count
            numstack.push(c - '0');
        } else if (c == '[') {
            // save what we have so far, start a fresh string for the inside
            strstack.push(current);
            current = "";
        } else if (c == ']') {
            int n = numstack.top();
            numstack.pop();

            string previous = strstack.top();
            strstack.pop();

            string repeated = "";
            for (int j = 0; j < n; j++) {
                repeated += current;
            }
            current = previous + repeated;
        } else {
            // normal letter
            current += c;
        }
    }

    cout << "Result: " << current << endl;
    return 0;
}
