#pragma once
#include "stack.hpp"

class ArrayStack : public Stack {
private:
    char* arr;
    int cap;
    int sz;

    // Copy everything into a new array of the given capacity
    void resize(int newCap) {
        char* newArr = new char[newCap];
        for (int i = 0; i < sz; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        cap = newCap;
    }

public:
    ArrayStack() {
        cap = 5;
        sz = 0;
        arr = new char[cap];
    }

    ~ArrayStack() {
        delete[] arr;
    }

    void push(char c) override {
        // Full: grow by 50%, rounded up (5 -> 8)
        if (sz == cap) {
            resize((cap * 3 + 1) / 2);
        }
        arr[sz++] = c;
    }

    char pop() override {
        if (sz == 0) {
            return ' ';
        }
        char removed = arr[--sz];

        // At 60% of capacity or less: shrink to 80% rounded down, min 5
        if (sz * 5 <= cap * 3) {
            int newCap = (cap * 4) / 5;
            if (newCap < 5) {
                newCap = 5;
            }
            if (newCap != cap) {
                resize(newCap);
            }
        }
        return removed;
    }

    char top() override {
        if (sz == 0) {
            return ' ';
        }
        return arr[sz - 1];
    }

    int size() override {
        return sz;
    }

    bool isEmpty() override {
        return sz == 0;
    }
};
