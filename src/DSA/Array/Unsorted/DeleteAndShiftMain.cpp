// C++ program to implement delete operation in an unsorted array
#include <iostream>
using namespace std;

// To search a key to be deleted
int findElement(int arr[], int size, int key);

// From Delete elemnent pos -> last index, just make "i = i+1" and last index is empty.  
int deleteElement(int arr[], int size, int key)
{
    // Find position of element to be deleted
    int pos = findElement(arr, size, key);

    if (pos == -1)
    {
        cerr << "Element not found";
        return size;
    }

    // Deleting element pos = 2, size = 5
    for (int i = pos; i < size - 1; i++)
        arr[i] = arr[i + 1];

    arr[size-1] = 0xfdfdfdfd;
    return size - 1;
}

// Function to implement search operation
int findElement(int arr[], int size, int key)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == key)
            return i;

    return -1;
}

// Driver's code
int main()
{
    int* arr = new int[5]{10, 50, 30, 40, 20};
    int size = 5;
    
    int key = 30;

    cout << "Array before deletion\n";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";

    // Function call
    size = deleteElement(arr, size, key);

    cout << "\n\nArray after deletion\n";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";

    delete[] arr;
    return 0;
}
