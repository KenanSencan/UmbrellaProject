#include <iostream>
#include "MyVector.h"
#include "Sorting.h"
#include "Benchmarking.h"

int main(int argc, char** argv) {
    // Step 1: Create an original MyVector and add some elements
    MyVector originalVector;
    originalVector.Add(10);
    originalVector.Add(20);
    originalVector.Add(30);
    originalVector.Add(40);
    originalVector.Add(50);

    originalVector.Insert(1,15);

    std::cout << "Original Vector:" << std::endl << originalVector;


    // Step 2: Use the copy constructor to create a new MyVector from the original
    MyVector copiedVector(originalVector);

    std::cout << "\nCopied Vector (using copy constructor):" << std::endl << copiedVector;

    // Modify the copied vector to show that it is independent of the original
    copiedVector.Add(40);
    copiedVector.RemoveIndex(1);

    std::cout << "\nModified Copied Vector:" << std::endl << copiedVector;

    std::cout << "\nOriginal Vector after modifying copied vector:" << std::endl << originalVector;

    // Step 3: Use the copy assignment operator to copy the contents of the original MyVector into another existing MyVector
    MyVector assignedVector = originalVector;

    std::cout << "\nAssigned Vector (using copy assignment operator):" << std::endl << assignedVector;

    // Modify the assigned vector to show that it is independent of the original
    assignedVector.Add(50);
    assignedVector.RemoveIndex(0);

    std::cout << "\nModified Assigned Vector:" << std::endl << assignedVector;

    std::cout << "\nOriginal Vector after modifying assigned vector:" << std::endl << originalVector;

    //Swapping with Index
    std::cout << "\nAssigned vector before swapping with 0 and 3:" << std::endl << assignedVector;
    assignedVector.Swap(0,3);
    std::cout << "\nAssigned vector After swapping with 0 and 3:" << std::endl << assignedVector;
    assignedVector.Swap(40,15,false);
    std::cout << "\nAssigned vector After swapping with 40 and 15 values:" << std::endl << assignedVector;


    //Swapping with value
    // assignedVector.Swap(50,1);

    //IMP: Sorting
    originalVector.Add(-15);
    std::cout << "\n Printing before sorting:" << std::endl << originalVector;

    //sorting 1
    MyVector NewSortedArr = originalVector;
    Sorting::MySorting(NewSortedArr);
    std::cout << "\n Printing After sorting with my sorting imp:" << std::endl << NewSortedArr;

    //Sorting 2
    MyVector NewSortedArr2 = originalVector;
    Sorting::SelectionSort(NewSortedArr2);
    std::cout << "\nPrinting after sorting with Selection Sort " << std::endl << NewSortedArr2;

    MyVector NewSortedArr3 = originalVector;
    Sorting::BubbleSort(NewSortedArr3);
    std::cout << "\nPrinting after bubble sort " << std::endl << NewSortedArr3;

    

    ////Benchmarking
    benchmark::Initialize(&argc, argv);
    Benchmarking::AddBenchmarks();
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    return 0;
}

