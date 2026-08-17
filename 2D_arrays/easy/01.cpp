#include <iostream>

using namespace std;

int main()
{
    int a, c;
    cout << "Enter size for rows: ";
    cin >> a;
    cout << "Enter size for cols: ";
    cin >> c;

    int **b = new int*[a];
    
    for(int i = 0; i < a; i++)
        b[i] = new int[c];
    
    cout << "Matrix: " << endl;

    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < c; j++)
        {
            cin >> b[i][j];
        }
        
    }
    cout << "Output: " << endl;
    
    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < c; j++)
        {
            cout << b[i][j] << " ";
        }
        cout << endl;
        
    }

}