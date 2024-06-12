//Reverse everything using Two Pointers Technique
//Time complexity O(n), Auxiliary: O(1) beause it's in place reversal
#include <iostream>

/* Function to reverse arr[] from start to end*/
void reverseArray(int arr[], int start, int end)
{
    while (start < end)
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";

    std::cout << std::endl;
}

int main()
{
    int arr[] = { 1, 2, 3, 4, 5, 6 };

    int n = sizeof(arr) / sizeof(arr[0]);

    // To print original array
    printArray(arr, n);

    // Function calling
    reverseArray(arr, 0, n - 1);

    std::cout << "Reversed array is" << std::endl;

    // To print the Reversed array
    printArray(arr, n);

    return 0;
}
