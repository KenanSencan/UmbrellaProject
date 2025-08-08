#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <ostream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <utility>  // Added for std::swap

template <typename T>
class MyVector
{
public:
    //NOTE: RULE OF FIVE HERE. WHENEVER YOU NEED FOR CONSTRUCTORS REFERENCE HERE
    //NOTE:  1. default(normal) constructor,
    //NOTE:  2. destructor 
    //NOTE:  3. Copy constructor + copy assignment operator. Arg: (const SelfT& other) 
    //NOTE:  4. Move constructor + move assignment operator. Arg: (selfT&& other)    
  
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
    void swap(MyVector& other) noexcept; // Added for copy-and-swap idiom
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
MyVector<T>::MyVector(const MyVector& other) : Arr(nullptr), CurrSize(0), Capacity(0)
{
    std::cout << "Copy constructor called" << std::endl;
    copyFrom(other);
}

//Move constructor
template <typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept
    : Arr(other.Arr), CurrSize(other.CurrSize), Capacity(other.Capacity)
{
    std::cout << "Move constructor called" << std::endl;
    // Leave 'other' in a valid, empty state
    other.Arr = nullptr;
    other.CurrSize = 0;
    other.Capacity = 0;
}

//Copy assignment operator - Using copy-and-swap idiom for exception safety
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &other)
    {
        MyVector temp(other); // Copy constructor creates a temporary
        swap(temp); // Swap contents with temporary
        // temp's destructor will clean up our old data
    }
    return *this;
}

//Move assignment operator
template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept
{
    std::cout << "Move assignment operator called" << std::endl;
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
        os << "Index: " << i << " Value: " << vec.Arr[i] << std::endl;
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

// Fixed Insert method - now allows insertion at the end (index == CurrSize)
template <typename T>
void MyVector<T>::Insert(int index, const T& value)
{
    // Allow insertion at index == CurrSize (end of vector)
    if (index < 0 || index > CurrSize)
    {
        throw std::out_of_range("Index is out of range for insertion");
    }
    
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
        std::cout << "Index: " << i << " Value: " << Arr[i] << std::endl;
    }
    std::cout.flush();
}

template <typename T>
void MyVector<T>::RemoveIndex(const int& index)
{
    CheckBounds(index);
    
    // Use move assignment for better performance with complex types
    for (int i = index; i < CurrSize - 1; ++i)
    {
        Arr[i] = std::move(Arr[i + 1]);
    }
    --CurrSize;

    // Optional shrink strategy: if size is 1/4 capacity AND capacity > 1, halve capacity
    // Added minimum capacity check to avoid excessive resizing
    if (CurrSize > 0 && CurrSize <= Capacity / 4 && Capacity > 1)
    {
        Resize(std::max(Capacity / 2, 1));
    }
}

// Fixed AllocateArray - now properly initializes elements
template <typename T>
void MyVector<T>::AllocateArray(int size)
{
    if (size < 0)
    {
        throw std::invalid_argument("Size cannot be negative");
    }
    
    delete[] Arr;
    if (size > 0)
    {
        Arr = new T[size](); // Value-initialize all elements
    }
    else
    {
        Arr = nullptr;
    }
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
    if (newCapacity < CurrSize)
    {
        throw std::invalid_argument("New capacity cannot be less than current size");
    }
    
    T* tempArr = new T[newCapacity](); // Value-initialize new array
    
    // Use std::move for better performance with move-constructible types
    for (int i = 0; i < CurrSize; ++i)
    {
        tempArr[i] = std::move(Arr[i]);
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
    if (Capacity > 0)
    {
        Arr = new T[Capacity];
        std::copy_n(other.Arr, CurrSize, Arr);
    }
    else
    {
        Arr = nullptr;
    }
}

// Added swap method for copy-and-swap idiom
template <typename T>
void MyVector<T>::swap(MyVector& other) noexcept
{
    std::swap(Arr, other.Arr);
    std::swap(CurrSize, other.CurrSize);
    std::swap(Capacity, other.Capacity);
}

#endif // MYVECTOR_H
