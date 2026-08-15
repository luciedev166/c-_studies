#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a], count = 0;
    cout << "Enter element: ";

    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
    }

    int c;

    cout << "Enter threshold: ";
    cin >> c;

    for(int i = 0; i < a; i++)
    {
        if(b[i] > c)
        count++;
    }

    cout << "Count: " << count;
}