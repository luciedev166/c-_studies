#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    for(int i = a - 1; i >= 0; i--)
    {
        cin >>b[i];
    }
    cout << "Reversed: ";

    for(int i = 0; i < a; i++)
    cout << b[i] << " ";
}