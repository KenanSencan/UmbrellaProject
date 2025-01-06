#include <iostream>
#include <vector>

#include "MyVector.h"

MyVector<std::string> CreateTempVector()
{
    MyVector<std::string> temp;
    temp.Add("Apple");
    temp.Add("Banana");
    return std::move(temp);
}

int main(int argc, char** argv)
{
    // Step 1: Create an original MyVector and add some elements
    MyVector<std::string> originalVector;
    originalVector.Add("sa1");
    originalVector.Add("sokrates");
    originalVector.Add("pekmez");
    originalVector.Add("tetanoz");
    originalVector.Add("forma");

    originalVector.Insert(1, "feristah");

    std::cout << "Original Vector:" << std::endl << originalVector;

    // Step 2: Use the copy constructor to create a new MyVector from the original
    MyVector copiedVector{originalVector};
    std::cout << "\nCopied Vector (using copy constructor):" << std::endl << copiedVector;

    // Modify the copied vector to show that it is independent of the original
    copiedVector.Add("fokurdayan");
    copiedVector.RemoveIndex(1);

    std::cout << "\nModified Copied Vector:" << std::endl << copiedVector;
    std::cout << "\nOriginal Vector after modifying copied vector:" << std::endl << originalVector;

    // Step 3: Use the copy assignment operator to copy the contents of the original MyVector into another existing MyVector
    MyVector assignedVector = originalVector;
    std::cout << "\nAssigned Vector (using copy assignment operator):" << std::endl << assignedVector;

    // Modify the assigned vector to show that it is independent of the original
    assignedVector.Add("fermente");
    assignedVector.RemoveIndex(0);
    std::cout << "\nModified Assigned Vector:" << std::endl << assignedVector;
    std::cout << "\nOriginal Vector after modifying assigned vector:" << std::endl << originalVector;

    //Swapping with Index
    std::cout << "\nAssigned vector before swapping with 0 and 3:" << std::endl << assignedVector;
    assignedVector.SwapIdx(0, 3);
    std::cout << "\nAssigned vector After swapping with 0 and 3:" << std::endl << assignedVector;

    //Use move constructor
    auto MoveMyVector{std::move(originalVector)};
    auto newVector = CreateTempVector();
    
    return 0;
}
