#include "Test.h"

#include <algorithm>
#include <iostream>

Test::Test() : CurrSize(10), OwnNameUntouched("Untouched")
{
    std::cout << "Test Constructed \n";
    SelfPtr = new int[CurrSize];
    for (int i = 0; i < CurrSize; ++i)
    {
        SelfPtr[i] = i;
    }
}

Test::~Test()
{
    std::cout << "Test Destructed \n";
    delete[] SelfPtr;
}

Test::Test(const Test& other) : CurrSize(0)
{
    std::cout << "Copy Constructor Constructed \n";
    SelfPtr = nullptr;
    CopyFrom(other);
}

//Move constructor
Test::Test(Test&& other) noexcept : SelfPtr(other.SelfPtr), CurrSize(other.CurrSize),OwnName(std::move(other.OwnName))
{
    std::cout << "Move Test Constructed \n";
    other.SelfPtr = nullptr;
    other.CurrSize = 0;
}


Test& Test::operator=(Test&& other) noexcept
{
    std::cout << "Move = assignment called \n";
    if (this != &other)
    {
        delete[] SelfPtr;
        this->SelfPtr = other.SelfPtr;
        this->OwnName = std::move(other.OwnName);
        this->CurrSize = other.CurrSize;

        other.SelfPtr = nullptr;
        other.CurrSize = 0;
    }
    return *this;
}

void Test::SetAllVal31(const Test& Test)
{
    for (int i = 0; i < Test.CurrSize; ++i)
    {
        Test.SelfPtr[i] = 31;
    }
}


void Test::CopyFrom(const Test& Test)
{
    CurrSize = Test.CurrSize;
    SelfPtr = new int[CurrSize];
    std::copy_n(Test.SelfPtr, CurrSize, SelfPtr);
}

void Test::PrintAll() const
{
    std::cout << "Ptr values are: ";
    for (int i = 0; i < CurrSize; ++i)
    {
        std::cout << SelfPtr[i] << " ";
    }

    std::cout << "Own name is: " << this->OwnName;
    std::cout.flush();
}
