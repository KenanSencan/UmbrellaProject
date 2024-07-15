#include <iostream>

int InsertSorted(int* arr, int InsertEl, int key, int size)
{
    if (InsertEl >= size) return InsertEl;
    arr[InsertEl] = key;
    return (InsertEl + 1);
};

int main()
{
    int arr[20] = {12, 16, 20, 40, 50, 70};
    int size = sizeof(arr) / sizeof(arr[0]);
    int InsertElememt = 6;
    int key = 26;

    std::cout << "Before insertion ";
    for (int i = 0; i < InsertElememt; ++i)
    {
        std::cout << arr[i] << " ";
    }

    //Inserting a key
    std::cout << "\n";
    int sa = InsertSorted(arr,InsertElememt,key,size);
    for (int i = 0; i < sa; ++i)
    {
        std::cout << arr[i] << " ";
    }

    return 0;
}