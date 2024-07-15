#ifndef MYVECTOR_H
#define MYVECTOR_H

class MyVector {
public:
    MyVector();
    ~MyVector();
    MyVector(const MyVector& other); // Copy constructor
    MyVector& operator=(const MyVector& other); // Copy assignment operator

    void Add(int value);
    void Insert(int index, int value);
    [[nodiscard]] int GetCapacity() const { return Capacity; }
    [[nodiscard]] int GetSize() const { return CurrSize; }
    [[nodiscard]] int GetIndex(int value) const;
    void Bounds(int index) const;
    [[nodiscard]] int GetValue(int index) const;
    void PrintEntireArray() const;
    void RemoveIndex(int index);

private:
    int* Arr;
    int CurrSize, Capacity;

    void Resize(int newCapacity);
    void copyFrom(const MyVector& other);
};

#endif //MYVECTOR_H
