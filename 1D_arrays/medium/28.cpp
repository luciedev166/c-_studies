#include <iostream>
using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];
    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
    }

    int k;
    cout << "Enter k: ";
    cin >> k;

    for(int i = 0; i < k; i++)
    {
        int first;
        for(int j = 0; j < a - 1; j++)
        {
            if(j == 0)
            first = b[j];

            b[j] = b[j + 1];
        }
        b[a - 1] = first;
    }

    for(int i = 0; i < a;i++)
    cout << b[i] << " ";
}