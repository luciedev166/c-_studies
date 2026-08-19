#include <iostream>

using namespace std;

void count(int n)
{
    if (n == 0)
    return 0;
    return count(n - 1);
    
}
int main()
{
    count(5);
}