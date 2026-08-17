#include <iostream>

using namespace std;

void printMatrix(int **b, int a, int c){
    for(int i = 0; i < a;i++)
    {
        for(int j = 0; j < c; j++)
        {
            cout << b[i][j] << " ";
        }
        cout << endl;
    }
}

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
        {
            cin >> b[i][j];
        }
    }

    printMatrix(b, a, c);

}