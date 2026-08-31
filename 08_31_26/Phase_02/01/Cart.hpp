#include "Item.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

class Cart
{
    private:
    Item *items;
    int capacity;
    int size;
    bool discountApplied;
    double discountRate;

    public:
    Cart(int initialCapacity)
    {
        capacity = initialCapacity;
        items = (Item*) calloc(capacity, sizeof(Item));
        size = 0;
        discountApplied = false;
        discountRate = 0;
    }

    ~Cart()
    {
        free(items);
    }

    bool addItem(Item it)
    {
        for(int i = 0; i < size; i++)
        {
            if(items[i].name == it.name)
            {
                items[i].qty++;
                return true;
            }
        }

        if(size == capacity)
        {
            Item* temp = (Item*) realloc(items, (capacity + 3) * sizeof(Item));
            if(temp == NULL)
            return false;

            items = temp;
            capacity = capacity + 3;
        }
        items[size++] = it;
        return true;
    }

    double getSubtotal()
    {
        double sum = 0;
        for(int i = 0; i < size; i++)
        {
            sum += items[i].price * items[i].qty;
        }
        return sum;
    }

    bool applyDiscount(double percent)
    {
        if (discountApplied) return false;
        else
        {
            discountApplied = true;
            discountRate = percent;
            return true;
        }
    }

    double getTotal()
    {
        double sum = 0;
        if(discountApplied)
        {
            return getSubtotal() * (1 - discountRate / 100);
        }
        return getSubtotal();
    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << "name: " << items[i].name << " x" << items[i].qty << " $" << items[i].price << " each"<< endl;
        }
    }

    string toString()
    {
        return "Cart: " + to_string(size) + " items (qty) " + "Subtotal: " + to_string(getSubtotal()) + " total: "+  to_string(getTotal()) + " discount applied: " + to_string(discountApplied);
    }

    int getCapacity()
    {
        return capacity;
    }

    int getItemCount()
    {
        return size;
    }

    int getTotalQuantity()
    {
        int sum = 0;
        for(int i = 0; i < size; i++)
        {
            sum += items[i].qty;
        }
        return sum;
    }
};