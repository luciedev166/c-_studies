#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size of array: ";
    cin >> a;

    int *b = new int[a];
    cout << "Enter elements: ";
    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
    }

    cout <<"Output: " << endl;

    for(int i = 0; i < a; i++)
    {
        cout << b[i] << " ";
    }

}