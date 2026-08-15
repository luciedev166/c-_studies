#include <iostream>
#include <climits>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a], min = INT_MAX, max = INT_MIN;

    cout << "Enter elements: ";

    for(int i =0; i < a; i++)
    {
        cin >> b[i];
        if (b[i] < min)
        min = b[i];
        else if (b[i] > max)
        max = b[i];
    }

    cout << "Range: " << max - min;
}