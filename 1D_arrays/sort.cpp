#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];
    for(int i = 0; i < a; i++)
    cin >>b[i];

    // sort
    for(int i = 0; i < a - 1; i++)
    {
        for(int j = a - 1; j > 0; j--)
        {
            if(b[j] < b[j - 1])
            {
                int temp = b[j];
                b[j] = b[j - 1];
                b[j - 1] =  temp;
            }
        }
    }

    for(int i = 0; i < a; i++)
    {
        cout << b[i] << " ";
    }
}