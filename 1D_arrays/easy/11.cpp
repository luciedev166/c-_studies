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
    {
        cin >> b[i];
    }

    int c, k;

    cout << "Enter k: ";
    cin >> k;

    for(int i = 0; i < a - 1; i++)
    {
        for(int j = a - 1; j > 0; j--)
        {
            if(b[j] < b[j - 1])
            {
                int temp = b[j - 1];
                b[j - 1] = b[j];
                b[j] = temp;
            }
        }
    }

    cout << "Sorted: ";
    for(int i = 0; i < a; i++)
    {
        cout << b[i] << " ";
    }
    cout << endl;

    cout << k << " largest element: " << b[a - k];
}