#include <iostream>
#include <iomanip>

using namespace std;
int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a], sum = 0;

    for (int i = 0; i < a; i++)
    cin >> b[i];

    cout << "Enter elements: ";

    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        sum += b[i];
    } 
     cout << "Mean: " << fixed << setprecision(2) << float(sum) / a;

}