#include "animal.hpp"
#include <iostream>

using namespace std;
class Dog : public Animal{
    public:
    void speak()
    {
        cout << "Woof!";
    }
};