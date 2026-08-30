#include <iostream>
#include <string>
#include "Entry.hpp"

using namespace std;

class LRUCache{
    private:
    Entry cache[5];
    int size = 0;


// * put(key, value) RULES:
// *   - If key already exists anywhere in the cache: update its value
// *     IN PLACE, then promote it to the front (index 0). Promotion means
// *     shifting every entry that was AHEAD of it one slot to the RIGHT
// *     to open up index 0 — do not touch anything behind its old position.
// *   - If key does not exist and the cache isn't full: insert a brand
// *     new entry at the front, shifting the whole current contents one
// *     slot to the right first.
// *   - If key does not exist and the cache IS full: the entry at the
// *     back (index size-1, the least recently used) is evicted first —
// *     you should never explicitly clear it, it's simply overwritten as
// *     part of the same rightward shift used for a normal insert.
// *   - put() always returns true. Unlike Leaderboard's add(), there is
// *     no rejection case here — something is always evicted to make room.
    public:

    //shifts to front
    void toFront(int idx)
    {
        Entry temp = cache[idx];
        for(int i = idx; i > 0; i--)
        {
            cache[i] = cache[i - 1];
        }
        cache[0] = temp; 
    }
    bool put(string key, int val)
    {
        // check if key exists
        for(int i = 0; i < size; i++)
        {
            if(key == cache[i].key)
            {
                cache[i].value = val;
                toFront(i);
                return true;
            }
        }
        // key not found
        if(size < 5)
        {
            int n = size++;
            cache[n].key = key;
            cache[n].value = val;

            toFront(n);
            return true;   
        }
        else
        {
            cache[size - 1].key = key;
            cache[size - 1].value = val;
            toFront(size - 1);
            return true;
        }
        return false;
    }

    void display()
    {
        for(int i = 0; i < size; i++)
        {
            cout << cache[i].key << ":" << cache[i].value << " ";
        }
        cout << endl;
    }

//  * get(key, valueOut) RULES:
//  *   - If found: write the value into valueOut (out-param), promote that
//  *     entry to the front exactly like put() does for an existing key,
//  *     and return true.
//  *   - If NOT found: valueOut is left untouched, nothing in the cache
//  *     moves, and the function returns false. A miss must not shift
//  *     anything — that's the trap: it's tempting to run your promotion
//  *     loop unconditionally.
//  *
    bool get(string key, int &valOut)
    {
        // check if key exists
        for(int i = 0; i < size; i++)
        {
            if(key == cache[i].key)
            {
                valOut = cache[i].value;
                toFront(i);
                return true;
            }
        } 
        return false;
    }

    bool isFull()
    {
        if(size ==5)
        return true;
        else return false;
    }

    int getSize()
    {
        return size;
    }

    bool contains(string key)
    {
        for(int i = 0; i < size; i++)
        {
            if(cache[i].key == key)
            return true;
        }
        return false;
    }

    Entry getMostRecentlyUsed()
    {
        return cache[0];
    }
    Entry getLeastRecentlyUsed()
    {
        return cache[size - 1];
    }

    bool remove(string key)
    {
        for(int i = 0; i < size; i++)
        {
            if(cache[i].key == key)
            {
                for(int j = i; j < size - 1; j++)
                {
                    cache[j] = cache[j + 1];
                }
                return true;
            }
        }
        return false;
        size--;
    }

    bool clear()
    {
        size = 0;
        return true;
    }

    bool isEmpty()
    {
        if(size == 0) return true;
        else return false;
    }
};