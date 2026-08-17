#include <iostream>
#include "shape.hpp"

using namespace std;

class Circle : public Shape{
    public:
    void draw(){
        cout << "Circle!";
    }
};