#include <iostream>
#include "circle.hpp"
using namespace std;

int main()
{
    Shape *l = new Circle();
    l->draw();
}