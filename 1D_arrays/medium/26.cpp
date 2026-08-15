#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    cout <<"Enter elements: ";

    for(int i = 0; i < a; i++)
    cin >> b[i];

    int c;
    cout << "Enter num of rotations: ";
    cin >> c;

    for(int i = 0; i < c; i++)
    {
        int first;
        for(int j = 0; j < a - 1; j++)
        {
            if(j == 0) first = b[j];

            b[j] = b[j + 1];
        }
        b[a - 1] = first;
    }

    cout << "Rotated: ";

    for(int i = 0; i < a; i++)
    cout << b[i]<< " ";
}