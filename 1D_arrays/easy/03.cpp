#include <iostream>
#include <climits>

using namespace std;

int main()
{
    int a;
    cout << "Enter size of array: ";
    cin >> a;
    int *b = new int[a], max = INT_MIN, min = INT_MAX;

    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        if(b[i] < min)
        min = b[i];
        else if (b[i] > max)
        max = b[i];
    }

    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;
}