#include <iostream>

using namespace std;

int main()
{
    int a, c, sum;
    cout << "Enter row size: ";
    cin >> a;
    cout << "Enter col size: ";
    cin >> c;

    int **b = new int*[a];
    for(int i = 0; i < a; i++)
        b[i] = new int[c];
    
    cout << "Matrix:\n";

    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < c; j++)
            cin >> b[i][j];
    }

    cout << "Row sum: ";
    for(int i = 0; i < a; i++)
    {
        sum = 0;
        for(int j = 0; j < c; j++)
        {
            sum += b[i][j];
        }
        cout << sum << " "; 
    }
        
    cout << "\nRow sum: ";
    for(int i = 0; i < c; i++)
    {
        sum = 0;
        for(int j = 0; j < a; j++)
        {
            sum += b[j][i];
        }
        cout << sum << " "; 
    }
    
}