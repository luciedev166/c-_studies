#include <iostream>

using namespace std;

int main()
{
    int a, c, odd = 0, even = 0;
    cout << "Enter row size: ";
    cin >> a;
    cout << "Enter col size: ";
    cin >> c;

    int **b = new int*[a];
    for(int i = 0; i< a; i++)
        b[i] = new int[c];

    cout << "Matrix: " << endl;
    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < c; j++)
        {
            cin >> b[i][j];
            if(b[i][j] % 2 == 0)
                even++;
            else
                odd++;
        }
            
    }
    cout << "Even count: " << even << endl;
    cout << "Odd count: " << odd << endl;

}