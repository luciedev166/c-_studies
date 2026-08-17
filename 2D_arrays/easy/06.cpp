#include <iostream>
#include <climits>
using namespace std;

int main()
{
    int a, c, min = INT_MAX, max = INT_MIN;
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
        {
            cin >> b[i][j];
            if(b[i][j] < min)
                min = b[i][j];
            if(b[i][j] > max)
                max = b[i][j];
        }
    }

    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl;
}