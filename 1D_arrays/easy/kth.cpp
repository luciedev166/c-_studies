#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    for(int i = 0; i < a; i++)
    cin >> b[i];


    int *uni = new int[a], count  = 0;
    int *c = new int[a]();
    //unique
    for(int i = 0; i < a; i++)
    {
        if(c[i] == 0)
        {
            for(int j = i + 1; j < a; j++)
            {
                if(b[i] == b[j])
                {
                    c[j] = 1;
                }
            }
            uni[count++] = b[i];
        }
    }

    for(int i = 0; i < count; i++)
    cout << uni[i] << " ";

    int k;
    cout <<"Enter k: ";
    cin >> k;

    cout <<"The " << k << "th largest: " << uni[count - k];


}