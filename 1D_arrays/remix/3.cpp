#include <iostream>

using namespace std;

void func(int b[], int a, int k)
{
    int *c = new int[a]();
    for(int i = 0; i < a; i++)
    {
        int count = 1;
        if(c[i] == 0)
        {
            for(int j = i + 1; j < a; j++)
            {
                if(b[i] == b[j])
                {
                    c[j] = 1;
                    count++;
                }
            }
            if(count == k)
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

    for(int i = 0; i < a; i++)
    cin >>b[i];

    int k;
    cout << "Enter k: ";
    cin >> k;
    func(b, a, k);
}