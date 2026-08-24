#include "Matrix.hpp"
#include <iostream>
#include <climits>

using namespace std;

class MyMatrix : public Matrix{
    private:
    int b[100][100];
    int r = 0;
    int c = 0;

    public:
    MyMatrix(int r, int c)
    {
        this->r = r;
        this->c = c;
    }

    void set(int r,int c,int val)
    {
        b[r][c] = val;
    }

    void displayRowByRow()
    {
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                cout << b[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void displayColByCol()
    {
        for(int i = 0; i < c; i++)
        {
            for(int j = 0; j < r; j++)
            {
                cout << b[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void displayReverseRows()
    {
        for(int i = r - 1; i >= 0; i--)
        {
            for(int j = 0; j < c; j++)
            {
                cout << b[i][j] << " ";
            }
        cout << endl;
        }
        cout << endl;
    }

    void displayRowsReversed()
    {
        for(int i = 0; i < r; i++)
        {
            for(int j = c - 1; j >= 0; j--)
            {
                cout << b[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    int sum()
    {
        int sum = 0;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                sum+= b[i][j];
            }
        }
        return sum;
    }

    int maxValue()
    {
        int max = INT_MIN;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(b[i][j] > max)
                max = b[i][j];
            }
        }
        return max;
    }

    int minValue()
    {
        int min = INT_MAX;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(b[i][j] < min)
                min = b[i][j];
            }
        }
        return min;
    }

    int countEven()
    {
        int count = 0;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(b[i][j] % 2 == 0)
                count++;
            }
        }
        return count;
    }
    int countOdd()
    {
        int count = 0;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(b[i][j] % 2 != 0)
                count++;
            }
        }
        return count;
    }

    void displayRowSums()
    {
        int count;
        for(int i = 0; i < r; i++)
        {
            count = 0;
            for(int j = 0; j < c; j++)
            {
                count += b[i][j];
            }
            cout << count << " ";
        }
        cout << endl;
    }
    void displayColSums()
    {
        int count;
        for(int i = 0; i < c; i++)
        {
            count = 0;
            for(int j = 0; j < r; j++)
            {
                count += b[j][i];
            }
            cout << count << " ";
        }
        cout << endl;
    }
    bool isSquare()
    {
        return c == r;
    }
    MyMatrix transpose()
    {
        MyMatrix results(c, r);
        for(int i = 0; i < c; i++)
        {
            for(int j = 0; j < r; j++)
            {
                results.set(i,j,b[j][i]);
            }
        }
        return results;
    }
    void swapRows(int x, int y)
    {
        for(int i = 0; i < c; i++)
        {
            int temp = b[x][i];
            b[x][i] = b[y][i];
            b[y][i] = temp;
        }
    }
    void swapCols(int x, int y)
    {
        for(int i = 0; i < r; i++)
        {
            int temp = b[i][x];
            b[i][x] = b[i][y];
            b[i][y] = temp;
        }     
    }
    void displayMainDiagonal()
    {
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(i == j)
                {
                    cout << b[i][j] << " ";
                }
            }
        }
        cout << endl;
    }

    void displayAntiDiagonal()
    {
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(i + j == r - 1)
                {
                    cout << b[i][j] << " ";
                }
            }
        }
        cout << endl;
    }
   int sumMainDiagonal()
    {
        int sum = 0;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(i == j)
                {
                    sum += b[i][j];
                }
            }
        }
        return sum;
    }

    int sumAntiDiagonal()
    {
        int sum = 0;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(i + j == r - 1)
                {
                    sum += b[i][j];
                }
            }
        }
        return sum;
    }
};