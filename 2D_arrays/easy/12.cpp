#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;
    int **b = new int*[a];
    for(int i = 0; i < a; i++)
        b[i] = new int[a];

    cout << "Matrix:\n";
    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < a; j++)
            cin >> b[i][j];
    }

    int c, d;
    cout<< "Row 1: ";
    cin >> c;
    cout<< "Row 2: ";
    cin >> d;

    for(int i = 0; i < a; i++)
    {
        int temp = b[c][i];
        b[c][i] = b[d][i];
        b[d][i] = temp;
    }
    
    for(int i = 0; i < a; i++)
    {
        for(int j = 0; j < a; j++)
            cout << b[i][j] << " ";
        cout << endl;
}
}