#include <iostream>
#include "MyVector.h"

int main() {
    // Step 1: Create an original MyVector and add some elements
    MyVector originalVector;
    originalVector.Add(10);
    originalVector.Add(20);
    originalVector.Add(30);
    originalVector.Add(40);

    originalVector.Insert(1,15);

    std::cout << "Original Vector:" << std::endl;
    originalVector.PrintEntireArray();


    // Step 2: Use the copy constructor to create a new MyVector from the original
    MyVector copiedVector(originalVector);

    std::cout << "\nCopied Vector (using copy constructor):" << std::endl;
    copiedVector.PrintEntireArray();

    // Modify the copied vector to show that it is independent of the original
    copiedVector.Add(40);
    copiedVector.RemoveIndex(1);

    std::cout << "\nModified Copied Vector:" << std::endl;
    copiedVector.PrintEntireArray();

    std::cout << "\nOriginal Vector after modifying copied vector:" << std::endl;
    originalVector.PrintEntireArray();

    // Step 3: Use the copy assignment operator to copy the contents of the original MyVector into another existing MyVector
    MyVector assignedVector = originalVector;

    std::cout << "\nAssigned Vector (using copy assignment operator):" << std::endl;
    assignedVector.PrintEntireArray();

    // Modify the assigned vector to show that it is independent of the original
    assignedVector.Add(50);
    assignedVector.RemoveIndex(0);

    std::cout << "\nModified Assigned Vector:" << std::endl;
    assignedVector.PrintEntireArray();

    std::cout << "\nOriginal Vector after modifying assigned vector:" << std::endl;
    originalVector.PrintEntireArray();

    return 0;
}
