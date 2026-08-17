#include <iostream>

using namespace std;

void printrev(int b[], int a)
{
    for(int i = a - 1; i >= 0; i--)
        cout << b[i] << " ";
}
int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;
    int *b = new int[a];

    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
        cin >> b[i];
    
    printrev(b, a);
}