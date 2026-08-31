#include <cstdlib>
#include <iostream>

using namespace std;
class DynamicArray{
    private:
    int *data;
    int capacity;
    int size;

    public:

    DynamicArray(int initialCapacity)
    {
        data = (int*) calloc(initialCapacity, sizeof(int));
        capacity = initialCapacity;
        size = 0;
    }

    ~DynamicArray()
    {
        free(data);
    }

    bool add(int value)
    {
        // learning how to grow??
        if(size == capacity)
        {
            int newCap = (int)(capacity * 1.5);
            int *temp = (int*)realloc(data, newCap * sizeof(int));
            if(temp == NULL)
            {
                return false;
            }
            data = temp;
            capacity = capacity + 3;
        }
        data[size++] = value;
        return true;
    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    int getSize()
    {
        return size;
    }

    void clear()
    {
        size = 0;
    }

    double getAverage()
    {
        double sum = 0;
        for(int i = 0; i < size; i++)
        {
            sum += data[i];
        }
        return sum / size;
    }

    int getMax()
    {
        int max = data[0];
        for(int i = 1; i < size; i++)
        {
            if(data[i] > max)
            {
                max = data[i];
            }
        }
        return max;
    }

    int getMin()
    {
        int min = data[0];
        for(int i = 1; i < size; i++)
        {
            if(data[i] < min)
            {
                min = data[i];
            }
        }
        return min;
    }

    int getCapacity()
    {
        return capacity;
    }

};