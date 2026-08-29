#include <iostream>
#include <string>
using namespace std;

struct Student{
    int age;
    string name;
    double grades[3];
};
int main()
{
    Student s;
    cout << "Enter name: ";
    cin >> s.name;

    cout << "Enter age: ";
    cin >> s.age;

    cout << "Enter grades: " << endl;

    for(int i = 0; i < 3; i++)
    {
        cin >> s.grades[i];
    }

    cout << "Display:" << endl;

    cout << s.name << endl;
    cout << s.age << endl;
    for(int i = 0; i < 3; i++)
    {
        cout <<  s.grades[i] << " ";
    }
}