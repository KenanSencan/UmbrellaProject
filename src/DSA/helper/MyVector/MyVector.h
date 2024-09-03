#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <ostream>

class MyVector {
public:
    MyVector();
    ~MyVector();
    MyVector(const MyVector& other); // Copy constructor
    MyVector(MyVector&& other) noexcept;
    
    MyVector& operator=(const MyVector& other); // Copy assignment operator

    //For printing easily entire array with os 
    friend std::ostream& operator<<(std::ostream& os, const MyVector& vec);
    
    /*There's no Inserting into an index or sorting. Just Add given element at the and of array and if size needs to be extended, reallocate and double the size of the array*/
    void Add(int value);
    /*Insert a given element at the given index*/
    void Insert(int index, int value);
    //What was the difference between Capacity and Size?
    [[nodiscard]] int GetCapacity() const { return Capacity; }
    
    [[nodiscard]] int GetSize() const { return CurrSize; }
    void SetSize(const int NewSize) {CurrSize = NewSize;}
    
    /*Take a value and return that value's index*/
    [[nodiscard]] int GetIndex(int value) const;
    /*Check if given index is out of bounds of the array*/
    bool Bounds(int Index) const;
    [[nodiscard]] int GetValue(int index) const;
    /*Print entire array with simple for loop*/
    void PrintEntireArray() const;
    //Remove index from the given index. Just make i = i + 1 and then remove the last index
    void RemoveIndex(int index);

    void AllocateArray(int size);

    //Get arr
    [[nodiscard]] int* GetArr() const {return Arr;}
    
    void copyFrom(const MyVector& other);

    //Swap the given value or indexes. If the bool is true (by default), it will try to take index, if IsIndex = false, it will swap values
    void Swap(int First, int Sec, bool IsIndex = true);

private:
    int* Arr;
    //CurSize only increments one when an element added,inserted and decremented when an element removed
    int CurrSize;
    int Capacity;

    //When the user tries to insert an element more than what current size can have, resize. The curr 
    void Resize(int newCapacity);
};

#endif //MYVECTOR_H
