#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Enter size: ";
    cin >> a;

    int *b = new int[a], neg = 0, pos = 0;

    for(int i = 0; i < a; i++)
    {
        cin >>b[i];
        if(b[i] < 0)
        neg+=b[i];
        else if(b[i] > 0)
        pos+=b[i];
    }

    cout << "Sum of positives: " << pos << endl;
    cout << "Sum of negatives: " << neg << endl;

}