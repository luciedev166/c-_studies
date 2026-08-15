#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];
    int *c = new int[a];

    cout << "Array 1: ";
    for(int i = 0; i < a; i++)
        cin >> b[i];

    cout << "Array 2: ";
    for(int i = 0; i < a; i++)
        cin >> c[i];

    for(int i = 0; i <a; i++)
    {
        if(b[i] != c[i])
        {
            cout << "Not identical";
            return 0;
        }
    }

    cout << "Identical";
    
}