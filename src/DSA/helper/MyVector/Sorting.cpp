#include "Sorting.h"

#include <sstream>

MyVector Sorting::MySorting(MyVector& Vector)
{
    MyVector SortedNewVector;
    const int* Arr = Vector.GetArr();

    while (Vector.GetSize() > 0)
    {
        int MinValue = Vector.GetValue(0);
        for (int i = 1; i < Vector.GetSize(); ++i)
        {
            if (Arr[i] < MinValue)
            {
                MinValue = Arr[i];
            }
        }
        Vector.RemoveIndex(Vector.GetIndex(MinValue));
        SortedNewVector.Add(MinValue);
    }

    return Vector = SortedNewVector;
}

void Sorting::SelectionSort(const MyVector& Vector)
{
    const int Size = Vector.GetSize();
    int* Arr = Vector.GetArr();

    for (int i = 0; i < Size - 1; ++i)
    {
        int MinIndex = i;
        for (int j = i + 1; j < Size; ++j)
        {
            if (Arr[j] < Arr[MinIndex])
                MinIndex = j;
        }

        //Swap the foun dminimum element with the first element
        if (MinIndex != i)
        {
            std::swap(Arr[MinIndex], Arr[i]); //Later on Implement Swap as well. Helper.h and helper.cpp will be my STD  
        }
    }
}

void Sorting::BubbleSort(const MyVector& Vector)
{
    const int Size = Vector.GetSize();
    int* arr = Vector.GetArr();
    for (int i = 0; i < Size - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < Size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped by inner loop, then break
        if (swapped == false)
            break;
    }
}
