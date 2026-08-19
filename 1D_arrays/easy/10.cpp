#include <iostream>
#include <climits>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];

    int first = INT_MIN,  sec = INT_MIN, third = INT_MIN;

    for(int i = 0; i < a; i++)
    {
        cin >> b[i];

        if(b[i] > first)
        {
            third = sec;
            sec = first;
            first = b[i];
        }
        else if (b[i] > sec)
        {
            third = sec;
            sec = b[i];
        }
        else if(b[i] > third)
        {
            third = b[i];
        }
    }

    cout << first << sec << third;
}