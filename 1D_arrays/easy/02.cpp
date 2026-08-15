#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size of array: ";
    cin >> a;

    int *b = new int[a], sum = 0, product = 1;

    cout <<"Enter elements: ";
    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        sum += b[i];
        product *= b[i];
    }
    cout << "Sum: " << sum <<endl;
    cout << "Product: " << product << endl;

}