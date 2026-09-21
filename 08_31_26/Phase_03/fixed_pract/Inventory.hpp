#include <iostream>
#include <cstdlib>
#include <string>
#include "Product.hpp"

using namespace std;

class Inventory
{
    private:
    Product fixedProducts[2];
    int capacity;
    int size;
    Product *products;
    bool usingHeap;

    public:
    Inventory()
    {
        capacity = 2;
        size = 0;
        usingHeap = false;
        products = nullptr;
    }

    ~Inventory()
    {
        if(usingHeap)
            delete [] products;
    }

    void addProduct(Product p)
    {
        if(!usingHeap && size < capacity)
        {
            fixedProducts[size++] = p;
            return;
        }
        else if(!usingHeap && size == capacity)
        {
            capacity = capacity * 2;
            products = new Product[capacity];
            for(int i = 0; i < size; i++)
            {
                products[i] = fixedProducts[i];
            }
            usingHeap = true;
        }

        if(usingHeap && capacity == size)
        {
            capacity = capacity * 2;
            Product *temp = new Product[capacity];

            for(int i = 0; i < size; i++)
            {
                temp[i] = products[i];
            }
            delete []products;
            products = temp;
        }
        products[size++] = p;

    }

    // calloc & realloc version
    // void addProduct(Product p)
    // {
    //     if(!usingHeap && size < capacity)
    //     {
    //         fixedProducts[size++] = p;
    //         return;
    //     }
    //     else if(!usingHeap && size == capacity)
    //     {
    //         capacity = capacity * 2;
    //         products = (Product*) calloc(capacity, sizeof(Product));
    //         for(int i = 0; i < size; i++)
    //         {
    //             products[i] = fixedProducts[i];
    //         }
    //         usingHeap = true;
    //     }

    //     if(usingHeap && size == capacity)
    //     {
    //         capacity = capacity * 2;
    //         Product *temp = (Product*) realloc(products, capacity * sizeof(Product));
    //         if(temp == NULL)
    //             return;
    //         products = temp;
    //     }

    //     products[size++] = p;
    // }

};