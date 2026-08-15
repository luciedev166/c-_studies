#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    for(int i =0; i < a; i++)
        cin >> b[i];

    int c[100] = {0};
    cout << "duplicate values: ";
    for(int i = 0; i < a; i++)
    {
        bool dup = false;
        if(c[i] == 0)
        {
            for(int j = i + 1; j < a; j++)
            {
                if(b[i] == b[j])
                {
                    dup = true;
                    c[j] = 1;
                }

            }
            if(dup)
                cout << b[i] << " ";
        }
    }

}