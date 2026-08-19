#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "enter size: ";
    cin >> a;

    int *b = new int[a];

    for(int i = 0; i < a; i++)
    cin >> b[i];

    int *c = new int[a];

    int k;
    cout <<"Enter k: ";
    cin >> k;

    for(int i = 0; i < k; i++)
    {
        int last;
        for(int j = a - 1; j > 0; j--)
        {
            if (j == a - 1)
            last = b[j];

            b[j] = b[j- 1];
        }
        b[0] = last;
    }

    for(int i = 0; i < a; i++)
    cout << b[i] << " ";
}