#pragma once

#include "MyVector.h"

class Sorting
{
public:
    //Simplest and possibly the slowest one. If second bool is true, it will apply sorting to arr, if not it will return the copy of arr which is sorted.
    static MyVector MySorting(MyVector& Vector);

    //The implementation is same as MySorting. This is only swapping with the current index and found smallest value. This way it makes auxiliary complexity performant too 
    static void SelectionSort(const MyVector& Vector);

    //Similiar to selection sort. Instead do, if (Arr[j] > Arr[j + 1]) swap(Arr[j] > Arr[j + 1])
    //
    static void BubbleSort(const MyVector& Vector); 
};
