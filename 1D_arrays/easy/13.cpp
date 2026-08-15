#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    cout << "Enter elements: ";

    for(int i = 0; i < a;  i++)
    {
        cin >> b[i];

    }

    cout << "Elements at even indices: ";
    
    for(int i = 0; i < a; i++)
    {
        if(i % 2 == 0)
        cout << b[i] << " ";
    }
}