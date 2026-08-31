#include <string>
#include <iostream>
#include <cstdlib>
#include "Student.hpp"

class Roster{
    private:
    Student* students;
    int capacity;
    int size;
    bool locked;

    public:
    Roster(int initialCapacity)
    {
        capacity = initialCapacity;
        students = (Student*) calloc(capacity, sizeof(Student));
        size =0;
        locked = false;
    }


    ~Roster()
    {
        free(students);
    }

    bool addStudent(Student s)
    {
        if (locked) return false;

        if(s.gender != 'm' && s.gender != 'f')
        {
            s.gender = 'm';
        }
        if(size == capacity)
        {
            Student *temp = (Student*) realloc(students, (capacity + 2) * sizeof(Student));
            if(temp == NULL) return false;

            students = temp;
            capacity += 2;
        }
        students[size++] = s;
        return true;
    }

    void finalizeRoster()
    {
        locked = true;
    }

    bool isLocked() 
    {
        return locked;
    }
    int getSize() 
    {
        return size;
    }
    int getCapacity()
    {
        return capacity;
    }
    int countMinors() 
    {
        int count = 0;
        for(int i = 0; i < size; i++)
        {
            if(students[i].age < 18)
            count++;
        }
        return count;
    }     
    double getAverageAge()
    {
        // to do, will shift to phase 3 as phase 2 has gotten repetitive
    }

};
