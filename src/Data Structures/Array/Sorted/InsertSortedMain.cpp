#include <iostream>

int TotalInitalizedElements(const int* arr, const int size)
{
    int InitElements = 0;
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] != 0)
        {
            InitElements++;
        }
    }
    return InitElements;
}

void InsertSorted(int arr[] , const int key, int capacity)
{
    //Find the size of the array
    int low = 0, high = capacity - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (arr[mid] < key && arr[mid + 1] > key) //we found it 
        {
            //Take everything from End -> mid and shift everything to right
            for (int i = capacity - 1; i >= mid; --i) //Go from end ->  mid
            {
                arr[i + 1] = arr[i];
            }

            //Because I shifted everything to right in above for loop, I need to shift once when inserting as well
            arr[mid + 1] = key; 
            return;
        }

        if (key > arr[mid])
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
}

int main()
{
    int arr[20] = {12,16,20,40,50,70};
    int size = sizeof(arr) / sizeof(arr[0]);
    int Capacity = TotalInitalizedElements(arr,size);
    const int key = 26;
    InsertSorted(arr,key, Capacity);

    //Print everything afterwards
    for (auto arr1 : arr)
    {
        std::cout << arr1 << std::endl;
    }
    
}