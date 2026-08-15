#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
    cin >> b[i];

    int c;
    cout << "Enter number of rotations: ";
    cin >> c;

    for(int i = 0; i < c; i++)
    {
        int last;
        for(int j = a - 1; j > 0; j--)
        {
            if(j == a - 1)
            last = b[j];

            b[j] = b[j - 1];
        }
        b[0] = last;
    }

    cout << "Rotated: ";
    for(int i = 0; i < a; i++)
    cout << b[i] << " ";
}