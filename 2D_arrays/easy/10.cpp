#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;
    int **b = new int*[a];
    for(int i = 0; i< a; i++)
        b[i] = new int[a];
    cout << "Matrix:\n";

    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < a; j++)
        {
            cin >> b[i][j];
        }
    }
    cout << "Anti-diagonal: ";
    
    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < a; j++)
        {
            if(i + j == a - 1){
                cout << b[i][j] << " ";
            }
        }
    }


}