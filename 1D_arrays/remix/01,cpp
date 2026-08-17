#include <iostream>

using namespace std;

void func(int b[], int a)
{
    int *c = new int[a]();
    
    for(int i = 0; i < a; i++)
    {
        int check = 0;
        if(c[i] == 0)
        {
            for(int j = i + 1; j < a; j++)
            {
                if(b[i] == b[j])
                {
                    check = 1;
                    c[j] = 1;
                }
            }
            if(check)
            cout << b[i] << " ";
        }
    }
}
int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];
    cout << "Enter array: ";
    for(int i = 0; i < a; i++)
    cin >> b[i];

    func(b, a);
}