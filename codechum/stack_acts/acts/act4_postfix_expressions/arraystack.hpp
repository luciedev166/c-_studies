// NOTE: In CodeChum this file is provided (hidden) by the instructor.
// This is the old ArrayStack adapted to int + print() so the act compiles locally.
#pragma once
#include <iostream>
#include "stack.hpp"

class ArrayStack : public Stack {
private:
    int* arr;
    int cap;
    int sz;

    void resize(int newCap) {
        int* newArr = new int[newCap];
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
        arr = new int[cap];
    }

    ~ArrayStack() {
        delete[] arr;
    }

    void push(int c) override {
        if (sz == cap) {
            resize((cap * 3 + 1) / 2);
        }
        arr[sz++] = c;
    }

    int pop() override {
        if (sz == 0) {
            return 0;
        }
        int removed = arr[--sz];

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

    int top() override {
        if (sz == 0) {
            return 0;
        }
        return arr[sz - 1];
    }

    int size() override {
        return sz;
    }

    bool isEmpty() override {
        return sz == 0;
    }

    void print() override {
        for (int i = 0; i < sz; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
};
