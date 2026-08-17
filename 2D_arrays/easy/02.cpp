#include <iostream>

using namespace std;

int main()
{
    int a, c;
    cout << "Enter row: ";
    cin >> a;
    cout << "Enter col: ";
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

    cout << "Output:\n";

    for(int i = 0; i < c ;i++)
    {
        for(int j = 0; j < a; j++)
            cout << b[j][i] << " ";
        cout << endl;
    }
}