#include <iostream>
#include <climits>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a], min= INT_MAX, idx;

    cout << "Enter elements: ";

    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        if(b[i] < min)
        {
            min = b[i];
            idx = i;
        }
    }
    cout << "Minimum element: " << min << endl;
    cout << "found at: " << idx << endl;

}