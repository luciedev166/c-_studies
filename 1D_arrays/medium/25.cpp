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

    int *c = new int[a]();
    for(int i = 0; i < a; i++)
    {
        bool check = false;
        if(c[i] == 0)
        {
            for(int j = i + 1; j < a; j++)
            {
                if (b[i] == b[j])
                {
                    check = true;
                    c[j] = 1;
                }
            }
            if (check)
            cout << b[i] << " ";
        }

    }
}