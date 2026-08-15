#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b =  new int[a];

    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
    cin >> b[i];

    int c;
    cout << "Enter value to find: ";
    cin >> c;

    int count = 0;
    for(int i = 0; i < a; i++)
    {
        if( c == b[i])
        count++;
    }

    cout << "Occurences: " << count;
}