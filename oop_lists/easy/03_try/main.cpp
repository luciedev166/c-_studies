#include <iostream>
#include "incrementer.hpp"
using namespace std;

int main()
{
    Counter *l = new Incrementer();
    l->increment();
    l->increment();
    l->increment();
    l->increment();
    int count = l->Count();
    cout << count;
}