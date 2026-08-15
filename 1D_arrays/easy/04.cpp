#include <iostream>

using namespace std;

int main()
{
    int a;
    cout <<"Enter size: ";
    cin >> a;

    int *b = new int[a], odd = 0, even = 0;
    
    cout << "Enter elements: ";

    for(int i = 0; i < a; i++)
    {
        cin >> b[i];
        if (b[i] % 2  == 0)
        even += 1;
        else
        odd += 1;
    }

    cout << "Odd: " << odd << endl;
     cout << "Even: " << even << endl;
}