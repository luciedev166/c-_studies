#include <iostream>
#include "List.hpp"

using namespace std;

class MyArray : public List{

    private:
    int *b = new int[100];
    int size = 0;

    public:
    void insert (int pos, int n)
    {
        size++;
        for(int i = size - 1; i > pos; i--)
        {
            b[i] = b[i - 1];
        }
        b[pos] = n;
    }

    void display()
    {
        for(int i = 0; i <size; i++)
        cout << b[i] << " ";
        cout << endl; 
    }

    void removeDuplicates()
    {
        int *map= new int[size]();
        int *c = new int[size];
        int k = 0;
        for(int i = 0; i < size; i++)
        {
            if(map[i] == 0)
            {
                for(int j = i + 1; j < size; j++)
                {
                    if(b[i] == b[j])
                    {
                        map[j] = 1;
                    }
                }
                c[k++] = b[i];
            }
        }

        size = k;
        for(int i = 0; i < size; i++)
        {
            b[i] = c[i];
        }
        delete []c;
        delete []map;
    }

    void printDuplicates()
    {
        int *map = new int[size]();
        for(int i = 0; i < size; i++)
        {
            if(map[i]  ==0)
            {
                int check = 0;
                for(int j = i + 1; j < size;j++)
                {
                    if(b[i] == b[j])
                    {
                        check = 1;
                        map[j] = 1;
                    }
                }
                if (check)
                {
                    cout << b[i] << " ";
                }
            }
        }
        delete []map;
    }

    void rotateLeft(int n)
    {
        for(int i = 0; i< n; i++)
        {
            int first;
            for(int j = 0; j < size - 1; j++)
            {
                if(j == 0)
                {
                    first = b[j];
                }
                b[j] = b[j + 1];
            }
            b[size - 1] = first;
        } 
    }

    void rotateRight(int n)
    {
        for(int i = 0; i < n; i++)
        {
            int last;
            for(int j = size - 1; j >0; j--)
            {
                if(j == size - 1)
                {
                    last = b[j];
                }
                b[j] = b[j - 1];

            }
            b[0] = last;

        }
    }

    void moveZerosToEnd()
    {
        int k = 0;
        for(int i = 0; i < size; i++)
        {
            if(b[i] != 0)
            {
                b[k++] = b[i];
            }
        }

        for(int i = k; i < size; i++)
        {
            b[i] = 0;
        }
    }

    int kthLargest(int n)
    {
        // unique first
        int *c = new int[size];
        int k = 0;
        int *map = new int[size]();
        for(int i = 0; i < size; i++)
        {
            if(map[i] == 0)
            {
                for(int j = i + 1; j < size; j++)
                {
                    if(b[i] == b[j])
                    map[j] = 1;
                }
                c[k++] = b[i];
            }
        }

        // then sort
        for(int i = 0; i < k - 1; i++)
        {
            for(int j = k - 1; j > 0; j--)
            {
                if(c[j - 1] > c[j])
                {
                    int temp = c[j  - 1];
                    c[j - 1] = c[j];
                    c[j] = temp;
                }
            }
        }

        // then retrieve
        int val = c[k - n];
        delete []c;
        delete []map;
        return val;
    }

    int firstLargest()
    {
        return kthLargest(1);
    }
    int secondLargest()
    {
        return kthLargest(2);
    }
    int thirdLargest()
    {
        return kthLargest(3);
    }
};