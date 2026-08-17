#include <iostream>
#include "dog.hpp"

using namespace std;

int main()
{
    Animal *a = new Dog();
    a->speak();
}