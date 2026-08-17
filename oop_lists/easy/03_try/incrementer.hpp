#include "counter.hpp"
#include <iostream>

using namespace std;

class Incrementer : public Counter{
    private:
    int count = 0;
    public:
    void increment(){
        count++;
    }

    int Count(){
        return count;
    }
};