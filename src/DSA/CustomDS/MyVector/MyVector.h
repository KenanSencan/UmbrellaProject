#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <ostream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

template <typename T>
class MyVector
{
public:
    //NOTE: RULE OF FIVE HERE. WHENEVER YOU NEED FOR CONSTRUCTORS REFERENCE HERE
    //NOTE:  1. default(normal) constructor,
    //NOTE:  2. destructor 
    //NOTE:  3. Copy constructor + copy assignment operator. Arg: (const SelfT& other) 
    //NOTE:  4. Move constructor + move assignment operaqtor. Arg: (selfT&& other)    
  
    MyVector(); //normal constructor
    ~MyVector(); //destructor
    MyVector(const MyVector& other); // Copy constructor
    MyVector(MyVector&& other) noexcept; // Move constructor
    // Assignment Operators
    MyVector& operator=(const MyVector& other); // Copy assignment
    MyVector& operator=(MyVector&& other) noexcept; // Move assignment
    //NOTE: REFERENCE PART for constructor is over
    
    // Friend Function for Output
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const MyVector<U>& vec);

    // Member Functions
    void Add(const T& value);
    void Insert(int index, const T& value);

    [[nodiscard]] int GetCapacity() const { return Capacity; }
    [[nodiscard]] int GetSize() const { return CurrSize; }
    void SetSize(const int NewSize) { CurrSize = NewSize; }

    /*Take a value and return that value's index*/
    [[nodiscard]] int GetIndex(const T& value) const;

    /*Check if given index is out of bounds of the array*/
    void CheckBounds(int Index) const;

    [[nodiscard]] T GetValue(int index) const;
    void PrintEntireArray() const;
    void RemoveIndex(const int& index);
    void AllocateArray(int size);

    // Get arr
    [[nodiscard]] T* GetArr() const { return Arr; }

    // Swap the given values or indexes. If IsIndex=true, swaps by index; if false, by value
    void SwapIdx(const int& First, const int& Sec);

private:
    T* Arr{};
    int CurrSize{};
    int Capacity{};

    // Helper Functions
    void Resize(int newCapacity);
    void copyFrom(const MyVector& other);
};

//Normal constructor 
template <typename T>
MyVector<T>::MyVector() : Arr(nullptr), CurrSize(0), Capacity(0)
{
    std::cout << "Normal constructor called" << std::endl;
}

template <typename T>
MyVector<T>::~MyVector()
{
    std::cout << "Destructor called" << std::endl;
    delete[] Arr;
}

//Copy constructor
template <typename T>
MyVector<T>::MyVector(const MyVector& other)
{
    std::cout << "Copy constructor called" << std::endl;
    copyFrom(other);
}

//Move constructor
template <typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept
    : Arr(other.Arr), CurrSize(other.CurrSize), Capacity(other.Capacity)
{
    std::cout << "Move constructor" << std::endl;
    // Leave 'other' in a valid, empty state
    other.Arr = nullptr;
    other.CurrSize = 0;
    other.Capacity = 0;
}

//Copy assignment operator 
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other)
{
    if (this != &other)
    {
        delete[] Arr;
        copyFrom(other);
    }
    return *this;
}

//Move assignment operator
template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept
{
    if (this != &other)
    {
        delete[] Arr;

        Arr = other.Arr;
        CurrSize = other.CurrSize;
        Capacity = other.Capacity;

        other.Arr = nullptr;
        other.CurrSize = 0;
        other.Capacity = 0;
    }
    return *this;
}

// Friend Function for cout
template <typename T>
std::ostream& operator<<(std::ostream& os, const MyVector<T>& vec)
{
    for (int i = 0; i < vec.CurrSize; ++i)
    {
        os << "Index: " << i << " " << vec.Arr[i] << std::endl;
    }
    return os;
}

// Member Functions
template <typename T>
void MyVector<T>::Add(const T& value)
{
    if (Capacity == CurrSize)
    {
        Resize(Capacity == 0 ? 1 : Capacity * 2);
    }

    Arr[CurrSize++] = value;
}

template <typename T>
void MyVector<T>::Insert(int index, const T& value)
{
    CheckBounds(index);
    // Ensure capacity
    if (CurrSize == Capacity)
    {
        Resize(Capacity == 0 ? 1 : Capacity * 2);
    }

    // Shift elements to the right from the index
    for (int i = CurrSize; i > index; --i)
    {
        Arr[i] = Arr[i - 1];
    }
    Arr[index] = value;
    ++CurrSize;
}

template <typename T>
int MyVector<T>::GetIndex(const T& value) const
{
    for (int i = 0; i < CurrSize; ++i)
    {
        if (Arr[i] == value)
        {
            return i;
        }
    }
    throw std::out_of_range("Value not found");
}

template <typename T>
void MyVector<T>::CheckBounds(int Index) const
{
    if (Index < 0 || Index >= CurrSize)
        throw std::out_of_range("Index is out of range");
}

template <typename T>
T MyVector<T>::GetValue(int index) const
{
    CheckBounds(index);
    return Arr[index];
}

template <typename T>
void MyVector<T>::PrintEntireArray() const
{
    for (int i = 0; i < CurrSize; ++i)
    {
        std::cout << "Index: " << i << " " << Arr[i] << std::endl;
    }
    std::cout.flush();
}

template <typename T>
void MyVector<T>::RemoveIndex(const int& index)
{
    CheckBounds(index);
    for (int i = index; i < CurrSize - 1; ++i)
    {
        Arr[i] = Arr[i + 1];
    }
    --CurrSize;

    // Optional shrink strategy: if size is 1/4 capacity, halve capacity
    if (CurrSize > 0 && CurrSize == Capacity / 4)
    {
        Resize(Capacity / 2);
    }
}

template <typename T>
void MyVector<T>::AllocateArray(int size)
{
    delete[] Arr;
    Arr = new T[size];
    CurrSize = size;
    Capacity = size;
}

template <typename T>
void MyVector<T>::SwapIdx(const int& First, const int& Sec)
{
    CheckBounds(First);
    CheckBounds(Sec);

    std::swap(Arr[First], Arr[Sec]);
}

// Helper Functions

template <typename T>
void MyVector<T>::Resize(int newCapacity)
{
    T* tempArr = new T[newCapacity];
    for (int i = 0; i < CurrSize; ++i)
    {
        tempArr[i] = Arr[i];
    }
    delete[] Arr;
    Arr = tempArr;
    Capacity = newCapacity;
}

template <typename T>
void MyVector<T>::copyFrom(const MyVector& other)
{
    CurrSize = other.CurrSize;
    Capacity = other.Capacity;
    Arr = new T[Capacity];
    std::copy_n(other.Arr, CurrSize, Arr);
}

// Friend Function for Output

#endif // MYVECTOR_H
