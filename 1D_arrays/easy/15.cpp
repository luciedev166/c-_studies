#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a];
    int *c = new int[a];

    cout <<"Enter elements: ";
    for(int i = 0; i < a; i++)
    cin >> b[i];

    cout << "Copied array: ";
    for(int i = 0; i < a; i++)
    {
        c[i] = b[i];
        cout << c[i] << " ";
    }
  

}