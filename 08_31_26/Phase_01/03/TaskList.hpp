#include "Task.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;

class TaskList
{
    private:
    int size;
    int capacity;
    Task *tasks;

    public:
    TaskList(int initialCap)
    {
        capacity = initialCap;
        tasks = (Task*) calloc(capacity, sizeof(Task));
        size = 0;
    }

    ~TaskList()
    {
        free(tasks);
    }

    bool add(Task n)
    {
        if(size == capacity)
        {
            Task *temp = (Task*) realloc(tasks, (capacity + 3) * sizeof(Task));
            if(temp == NULL)
            {
                return false;
            }
            tasks = temp;
            capacity = capacity + 3;
        }
        tasks[size++] = n;
        return true;
    }

    bool removeById(int id)
    {
        for(int i = 0; i < size; i++)
        {
            if(tasks[i].id == id)
            {
                for(int j = i; j < size - 1; j++)
                {
                    tasks[j] = tasks[j + 1];
                }
                size--;
                return true;
            }
        }
         return false;
    }

     int  indexOf(int id)
     {
        for(int i = 0; i < size; i++)
        {
            if(tasks[i].id == id)
            return i;
        }
        return -1;
     }
    int  getSize()
    {
        return size;
    }
    int  getCapacity()
    {
        return capacity;
    }
    bool isEmpty()
    {
        if(size == 0) return true;
        else return false;
    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << "ID: " << tasks[i].id << endl;
        }
    }

    void clear(){
        size = 0;
    }

};