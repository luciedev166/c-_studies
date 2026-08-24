#include <iostream>
#include "List.hpp"
#include <climits>

using namespace std;
class MyArray : public List{
    private:
    int size = 0;
    int *b = new int[100];

    public:

    void insert(int pos, int n)
    {
        size++;
        for(int i = size - 1; i > pos; i--)
        {
            b[i] = b[i - 1];
        }    
        b[pos] = n;
        return;
    }

    void display()
    {
        for(int i = 0; i<size; i++)
        {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    void addFront(int n)
    {
        size++;
        for(int i = size - 1; i > 0; i--)
        {
            b[i] = b[i - 1];
        }
        b[0] = n;
    }

    void addBack(int n)
    {
        size++;
        b[size - 1]= n;
    }

    void removeOccurrences(int n)
    {
        int k = 0;
        int *c= new int[size];
        for(int i = 0; i < size; i++)
        {
            if(b[i] != n)
            {
                c[k++] = b[i];
            }
        }

        size = k;
        for(int i = 0; i < size; i++)
        {
            b[i] = c[i];
        }

        delete []c;
    }

    int removeAt(int n)
    {
        int temp = b[n];
        for(int i = n; i < size - 1; i++)
        {
            b[i] = b[i + 1];
        }
        b[size - 1] = 0;
        size--;

        return temp;
    }

    void clear()
    {
        for(int i = 0; i < size; i++)
        {
            b[i] = 0;
        }
        size = 0;
    }

    int getSize()
    {
        return size;
    }

    bool isEmpty()
    {
        if (size == 0)
        return true;
        else
        return false;
    }
    int search(int n)
    {
        for(int i = 0; i < size; i++)
        {
            if(n == b[i])
            return i;
        }
        return -1;
    }

    bool contains(int n)
    {
        for(int i = 0; i < size; i++)
        {
            if(b[i] == n)
            return true;
        }
        return false;
    }
    int count(int n)
    {
        int count = 0;
        for(int i = 0; i < size; i++)
        {
            if(b[i] == n)
            count++;
        }
        return count;
    }
    int sum()
    {
        int sum = 0;
        for(int i  = 0; i < size; i++)
        {
            sum += b[i];
        }
        return sum;
    }

    int maxValue()
    {
        int max = INT_MIN;
        for(int i = 0; i < size; i++)
        {
            if(b[i] > max)
            max = b[i];
        }
        return max;
    }
    int minValue()
    {
        int min = INT_MAX;
        for(int i = 0; i < size; i++)
        {
            if(b[i] < min)
            min = b[i];
        }
        return min;
    }

    void reverse()
    {
        int k =0;
        int *c = new int[size];
        for(int i = size - 1; i >=0; i--)
        {
            c[k++] = b[i];
        }
        for(int i = 0; i < size; i++)
        {
            b[i] = c[i];
        }
        delete[]c;
    }
    
    int replace(int n, int val)
    {
        if(n < 0 || n >= size)
        return -1;

        int temp = b[n];
        b[n] = val;
        return temp;
    }
};