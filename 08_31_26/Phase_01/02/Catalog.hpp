#include "Product.hpp"
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;

class Catalog{
    private:
    Product *items;
    int capacity;
    int size;

    public:
    Catalog(int initialCap)
    {
        capacity = initialCap;
        items = (Product*) calloc(capacity, sizeof(Product));
        size = 0;
    }

    ~Catalog()
    {
        free(items);
    }

    bool add(Product n)
    {
        if(size == capacity)
        {
            Product *temp = (Product*) realloc(items, (capacity + 3) * sizeof(Product));
            if(temp == NULL)
            {
                return false;
            }

            items = temp;
            capacity = capacity + 3;
        }
        items[size++] = n;
        return true;
    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << "name: " << items[i].name << " price: " << fixed << setprecision(2) << items[i].price << endl;
        }
    }

    int getCapacity()
    {
        return capacity;
    }

    bool isEmpty()
    {
        if (size == 0) return true;
        else return true;
    }


};