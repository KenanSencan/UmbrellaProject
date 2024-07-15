#include <iostream>

//Just return the initialized variables' size of the array. 
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

//Binary Search 
int FindDesiredIndex(const int* arr, const int key, int capacity)
{
    int low = 0, high = capacity - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if ((arr[mid] < key && arr[mid + 1] > key) || arr[mid] == key) //we found it 
        {
            return mid;
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
    return -1;
}

void InsertSorted(int arr[], const int key, int capacity)
{
    int insertPos = FindDesiredIndex(arr, key, capacity);
    //Take everything from End -> mid and shift everything to right
    for (int i = capacity - 1; i >= insertPos; --i) //Go from end ->  mid
    {
        arr[i + 1] = arr[i];
    }

    //Because I shifted everything to right in above for loop, I need to shift once when inserting as well
    arr[insertPos + 1] = key;
}

void DeleteIndex(int arr[], const int key, int capacity)
{
    int DeletePos = FindDesiredIndex(arr, key, capacity);
    for (int i = DeletePos; i < capacity - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }
}

int main()
{
    int arr[20] = {12, 16, 20, 40, 50, 70};
    int size = sizeof(arr) / sizeof(arr[0]);
    constexpr int key = 26, DeleteKey = 16;

    //Calculate the Capacity of the entire array. It has 20 element but it will reutnr 6
    int Capacity = TotalInitalizedElements(arr, size);

    //Print before inserting
    std::cout << "Before inserting: ";
    for (int i = 0; i < Capacity; i++)
        std::cout<<arr[i]<<" ";
    std::cout << std::endl;

    InsertSorted(arr, key, Capacity);

    //Print after inserting
    std::cout << "After inserting: ";
    for (int i = 0; i < 6; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    DeleteIndex(arr, DeleteKey, Capacity);

    //Print after deleting
    std::cout << "After deleting: ";
    for (int i = 0; i < 6; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

}
