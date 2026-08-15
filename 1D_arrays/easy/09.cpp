#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    bool asc = true;

    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        if(i > 0 && b[i] < b[i - 1])
        asc = false;
    }

    if(asc)
    cout << "Ascending";
    else
    cout << "Not";
}