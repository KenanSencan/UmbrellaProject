#include "MyVector.h"
#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::copy

MyVector::MyVector() : Arr(nullptr), Capacity(0), CurrSize(0) {
}

MyVector::~MyVector() {
    delete[] Arr;
}

MyVector::MyVector(const MyVector& other) : Arr(nullptr), CurrSize(0), Capacity(0) {
    copyFrom(other);
}

MyVector& MyVector::operator=(const MyVector& other) {
    if (this != &other) {
        delete[] Arr;
        copyFrom(other);
    }
    return *this;
}

void MyVector::copyFrom(const MyVector& other) {
    CurrSize = other.CurrSize;
    Capacity = other.Capacity;
    Arr = new int[Capacity];
    std::copy(other.Arr, other.Arr + CurrSize, Arr);
}

void MyVector::Resize(int newCapacity) {
    int* tempArr = new int[newCapacity];
    std::copy(Arr, Arr + CurrSize, tempArr);
    delete[] Arr;
    Arr = tempArr;
    Capacity = newCapacity;
}

int MyVector::GetIndex(int value) const {
    for (int i = 0; i < CurrSize; ++i) {
        if (Arr[i] == value) {
            return i;
        }
    }
    return -1;
}

void MyVector::Bounds(int index) const {
    if (index < 0 || index >= CurrSize) {
        throw std::out_of_range("Index is out of range");
    }
}

int MyVector::GetValue(int index) const {
    Bounds(index);
    return Arr[index];
}

void MyVector::Add(int value) {
    if (Capacity == CurrSize) {
        Resize(Capacity == 0 ? 1 : Capacity * 2);
    }
    Arr[CurrSize++] = value;
}

void MyVector::Insert(const int index, int value)
{
    if (index < 0 || index > CurrSize) {
        throw std::out_of_range("Index is out of range");
    }
    
    //First Resize with +1 size. Then From Index -> End move everything to the
    Resize(++CurrSize);
    for (int i = CurrSize-2; i >= index; --i)
    {
        Arr[i + 1] = Arr[i];
    }
    Arr[index] = value;
}

void MyVector::PrintEntireArray() const {
    for (int i = 0; i < CurrSize; ++i) {
        std::cout << "Index: " << i << " " << Arr[i] << std::endl;
    }
    std::cout.flush();
}

void MyVector::RemoveIndex(int index) {
    Bounds(index);
    for (int i = index; i < CurrSize - 1; ++i) {
        Arr[i] = Arr[i + 1];
    }
    --CurrSize;
    if (CurrSize > 0 && CurrSize == Capacity / 4) {
        Resize(Capacity / 2);
    }
}
