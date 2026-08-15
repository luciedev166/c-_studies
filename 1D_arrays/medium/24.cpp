#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "enter size: ";
    cin >> a;

    int *b = new int[a];

    cout << "Enter elements: ";

    for(int i =0; i < a; i++)
    {
        cin >> b[i];
    }

    int c[50] ={0};

    cout << "Remove duplicates: ";
    for(int i = 0; i < a; i++)
    {
        if(c[i] == 0)
        {
            for(int j  = 1; j < a; j++)
            {
                if(b[i] == b[j])
                    c[j] = 1;
            }
            cout << b[i] << " ";
        }
    }
}