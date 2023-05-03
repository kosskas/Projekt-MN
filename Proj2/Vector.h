#pragma once
#include <iostream>

using namespace std;

template <typename T>
class Vector {
public:
    int size;
    int cap;
    T* ptr;

    Vector() {
        size = 0;
        cap = 1;
        ptr = new T[1];
    }

    Vector(const Vector& cpy) {
        size = cpy.size;
        cap = cpy.size;
        ptr = new T[cap];
        for (int i = 0; i < size; i++)
            ptr[i] = cpy.ptr[i];
    }

    Vector(Vector&& right) {
        size = right.size;
        cap = right.size;
        ptr = right.ptr;

        right.ptr = nullptr;
        right.size = 0;
        right.cap = 0;

    }

    Vector& operator=(Vector&& right) {
        swap(ptr, right.ptr);
        swap(size, right.size);
        swap(cap, right.cap);
        return *this;
    }

    Vector& operator=(const Vector& right) {
        Vector tmp(right);
        swap(ptr, tmp.ptr);
        swap(size, tmp.size);
        swap(cap, tmp.cap);
        return *this;

    }

    void resize() {
        cap *= 2;
        T* tmp = new T[cap];
        for (int i = 0; i < size; i++)
            tmp[i] = ptr[i];
        delete[] ptr;
        ptr = tmp;
    }

    void pushBack(T c) {
        if (size < cap) {
            ptr[size++] = c;
        }
        else {
            resize();
            pushBack(c);
        }
    }

    void pushFront(T c) {
        if (size < cap) {
            T* tmp = new T[cap];
            tmp[0] = c;
            for (int i = 0; i < size; i++)
                tmp[i + 1] = ptr[i];
            delete[] ptr;
            ptr = tmp;
            size++;
        }
        else {
            resize();
            pushFront(c);
        }
    }

    void popFront() {
        if (size > 0) {
            T* tmp = new T[cap];
            for (int i = 1; i < size; i++)
                tmp[i - 1] = ptr[i];
            delete[] ptr;
            ptr = tmp;
            size--;
        }
    }

    void Print() {
        for (int i = 0; i < size; i++) {
            cout << ptr[i] << " ";
        }
        cout << endl;
    }
    const int GetSize() const {
        return size;
    }

    const T& operator[](int i) const {
        return ptr[i];
    }

    T& operator[](int i) {
        return ptr[i];
    }

    ~Vector() {
        if (ptr != nullptr)
        {
            delete[] ptr;
            ptr = nullptr;
        }
    }
};