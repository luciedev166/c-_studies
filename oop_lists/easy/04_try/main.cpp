#include <iostream>
#include "numberlist.hpp"

int main()
{
    List *l = new NumberList();
    l->add(5);
    l->add(10);
    l->add(15);
    l->add(20);
    cout << l->get(0) << endl;
    cout << l->get(1) << endl;
    l->print();
}