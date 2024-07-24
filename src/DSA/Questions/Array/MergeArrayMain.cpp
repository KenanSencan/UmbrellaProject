// There are two sorted arrays. First one is of size m+n containing only m elements. Another one is of size n and contains n elements.
// Merge these two arrays into the first array of size m+n such that the output is sorted. 
// Input: array with m+n elements (mPlusN[]).

//MPlusN[] = {1,5,8,None,None,None}
//N[] = {2,3,7}
//Result: MPlusN[] = {1,2,3,5,7,8}

// We need to merge the elements of N[] into mPlusN[] while maintaining the sorted order.

//Just randomly insert random data into the array. Then insert randomly nullptr. Do this for 2 arrays. Then merge scarping nullptr and sort 

// ReSharper disable CppUseAuto
#include <climits>
#include <iostream>
#include "Helper.h"
//last test

void Prepare(int* Arr1, int* Arr2, const int Size1, const int Size2)
{
    Helper::WriteRandomValToPath(500);
    Helper::ReadFromPathAssignToArr(Arr1,Size1);
    Helper::ReadFromPathAssignToArr(Arr2, Size2);

    //Randomly assign half of the array INT_MIN to first array 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, Size1 - 1);
    for (int i = 0; i < Size1 / 2; ++i)
    {
        int rdnIndex = dis(gen);
        Arr1[rdnIndex] = INT_MIN;
    }

    std::cout << "Printing the first one";
    for (int i = 0; i < Size1; ++i)
    {
        std::cout << Arr1[i] << std::endl;
    }

    std::cout << "Printing the second one";
    for (int i = 0; i < Size2; ++i)
    {
        std::cout << Arr2[i] << std::endl;
    }
}

int main()
{
    constexpr int Size1 = 8;
    constexpr int Size2 = 4;

    int* Arr1 = new int[Size1];
    int* Arr2 = new int[Size2];
    Prepare(Arr1,Arr2,Size1,Size2);

    //Built in easy method
    std::sort(Arr1, Arr1 + Size1);
    std::sort(Arr2, Arr2 + Size2);
    int* Arr3 = new int[Size1 + Size2];
    std::merge(Arr1,Arr1+Size1,Arr2,Arr2+Size2,Arr3);
    std::cout << "Printing the first one";
    for (int i = 0; i < Size1+Size2; ++i)
    {
        if (Arr3[i] == INT_MIN) //remove and shrink the array
        std::cout << Arr3[i] << std::endl;
    }
    
    
    delete[] Arr1;
    delete[] Arr2;
    delete[] Arr3;

    
}