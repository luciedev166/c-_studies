#include <iostream>
#include "list.hpp"

using namespace std;

class NumberList : public List{
    private:
    int *arr = new int[100]();
    int size = 0;

    public:
    void add(int n)
    {
        arr[size++] = n;
    }

    int get(int idx){
        if(idx >= size)
        return 0;

        return arr[idx];
    }

    void print(){
        for(int i = 0; i < size; i++)
        cout << arr[i] << " ";
    }
};