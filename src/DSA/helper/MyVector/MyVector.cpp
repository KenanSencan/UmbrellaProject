// ReSharper disable CppDFAConstantConditions
#include "MyVector.h"
#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::copy


MyVector::MyVector() : Arr(nullptr), CurrSize(0), Capacity(0)
{
}

MyVector::~MyVector()
{
    delete[] Arr;
}

MyVector::MyVector(const MyVector& other) : Arr(nullptr), CurrSize(0), Capacity(0)
{
    copyFrom(other);
}

MyVector::MyVector(MyVector&& other) noexcept : Arr(other.Arr), CurrSize(other.CurrSize), Capacity(other.Capacity)
{
}


MyVector& MyVector::operator=(const MyVector& other)
{
    if (this != &other)
    {
        delete[] Arr;
        copyFrom(other);
    }
    return *this;
}

void MyVector::copyFrom(const MyVector& other)
{
    CurrSize = other.CurrSize;
    Capacity = other.Capacity;
    Arr = new int[Capacity];
    std::copy(other.Arr, other.Arr + CurrSize, Arr);
}

void MyVector::Swap(int First, int Sec, bool IsIndex)
{
    int IdxFirst;
    int IdxSec;
    
    if (IsIndex && Bounds(First) && Bounds(Sec))
    {
        IdxFirst = First;
        IdxSec = Sec;
    }
    else
    {
        IdxFirst = GetIndex(First);
        IdxSec = GetIndex(Sec);
    }
    
    const int temp = Arr[IdxFirst];
    Arr[IdxFirst] = (Arr[IdxSec]);
    Arr[IdxSec] = temp;
}

void MyVector::Resize(int newCapacity)
{
    int* tempArr = new int[newCapacity];
    std::copy(Arr, Arr + CurrSize, tempArr);
    delete[] Arr;
    Arr = tempArr;
    Capacity = newCapacity;
}

int MyVector::GetIndex(const int value) const
{
    for (int i = 0; i < CurrSize; ++i)
    {
        if (Arr[i] == value)
        {
            return i;
        }
    }
    throw std::out_of_range("Index is not found");
}

bool MyVector::Bounds(const int Index) const
{
    if (Index < 0 || Index >= CurrSize)
    {
        throw std::out_of_range("Index is out of range");
        return false;
    }
    return true;
}

int MyVector::GetValue(int index) const
{
    Bounds(index);
    return Arr[index];
}

void MyVector::Add(int value)
{
    if (Capacity == CurrSize)
    {
        Resize(Capacity == 0 ? 1 : Capacity * 2);
    }
    Arr[CurrSize++] = value;
}

void MyVector::Insert(const int index, int value)
{
    if (index < 0 || index > CurrSize)
    {
        throw std::out_of_range("Index is out of range");
    }

    //First Resize with +1 size. Then From Index -> End move everything to the
    Resize(CurrSize + 1);
    for (int i = CurrSize - 2; i >= index; --i)
    {
        Arr[i + 1] = Arr[i];
    }
    Arr[index] = value;
}

void MyVector::PrintEntireArray() const
{
    for (int i = 0; i < CurrSize; ++i)
    {
        std::cout << "Index: " << i << " " << Arr[i] << std::endl;
    }
    std::cout.flush();
}

void MyVector::RemoveIndex(int index)
{
    Bounds(index);
    for (int i = index; i < CurrSize - 1; ++i)
    {
        Arr[i] = Arr[i + 1];
    }
    Arr[CurrSize - 1] = 0; //clear the last element
    --CurrSize;
    if (CurrSize > 0 && CurrSize == Capacity / 4)
    {
        Resize(Capacity / 2);
    }
}

void MyVector::AllocateArray(int size)
{
    delete[] Arr;
    Arr = new int[size];
    CurrSize = size;
    Capacity = CurrSize;
}

std::ostream& operator<<(std::ostream& os, const MyVector& vec)
{
    for (int i = 0; i < vec.CurrSize; ++i)
    {
        os << "Index: " << i << " " << vec.Arr[i] << std::endl;
    }
    return os;
}
