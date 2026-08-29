#include <iostream>
#include <cmath>
#include "Entry.hpp"

class Leaderboard 
{
    private:
    Entry board[5];
    int size = 0;

    public:
    bool add(Entry c){

        // lets try a new algorithm which sorts everytime a user adds, but not the entire thing.
        if(size < 5)
        {
            for(int i = 0; i < size; i++)
            {
                // if we start from front, naturally we encounter the biggest ones so we won't need to do the max loop
                if(c.score > board[i].score)
                {
                    int idx = i;
                    for(int j = size; j > idx; j--)
                    {
                        board[j] = board[j - 1]; 
                    }
                    board[idx] = c;
                    size++;
                    return true;
                }
            }
            board[size++] = c;
            return true;
        }
        else
        {
            for(int i = 0; i < size; i++)
            {
                // if we start from front, naturally we encounter the biggest ones so we won't need to do the max loop
                if(c.score > board[i].score)
                {
                    int idx = i;
                    for(int j = size - 1; j > idx; j--)
                    {
                        board[j] = board[j - 1]; 
                    }
                    board[idx] = c;
                    return true;
                }
            }
            return false;
        }


    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << board[i].name << " (" << board[i].score << ") ";
        }
        cout << endl;
    }



    double getAverageScore()
    {
        double sum = 0;
        for(int i = 0; i < size; i++)
        {
            sum += board[i].score;
        }
        return round((sum / size) * pow(10, 2)) / pow(10, 2);
    }

    bool isFull()
    {
        if(size == 5)
        return true;
        else return false;
    }

    bool isEmpty()
    {
        if(size == 0)
        return true; else return false;
    }

    int getSize()
    {
        return size;
    }

    Entry getStrongest()
    {
        Entry max = board[0];
        for(int i = 1; i < size; i++)
        {
            if(board[i].score > max.score)
            {
                max = board[i];
            }
        }
        return max;
    }
    Entry getWeakest()
    {
        Entry min = board[0];
        for(int i = 1; i < size; i++)
        {
            if(board[i].score < min.score)
            {
                min = board[i];
            }
        }
        return min;
    }

    void clear()
    {
        size = 0;
    }

    bool contains(string name)
    {
        for(int i = 0; i < size; i++)
        {
            if(name == board[i].name)
            return true;
        }
        return false;
    }

    int indexOf(string name)
    {
        for(int i = 0; i < size; i++)
        {
            if(name == board[i].name)
            return i;
        }
        return -1;    
    }

    bool removeByName(string name)
    {
        for(int i = 0; i < size; i++)
        {
            if(name == board[i].name)
            {
                for(int j = i; j < size - 1; j++)
                {
                    board[j] = board[j + 1];
                }
                size--;
                return true;
            }
        }
        return false;
    }


};