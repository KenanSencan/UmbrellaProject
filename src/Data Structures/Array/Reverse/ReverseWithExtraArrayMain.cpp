//O(n) for Time and Auxiliary Space
//Simplest and most slowest solution for reverse.
//I just created new array end inserted elements from behind -> front

#include <iostream>
/* Utility function to print an array */
void printArray(int arr[], int size, std::string PreMessage)
{
    std::cout << PreMessage << ": ";
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";

    std::cout << std::endl;
}

//Create a new array, from behind -> Front insert. I could also instead start the for loop from behind 
void reverseArrayExtraArray(int arr[], const int size)
{
    int* reversedArr = new int[size];
    for (int i = 0; i < size; i++)
    {
        reversedArr[i] = arr[size - i - 1];
    }

    printArray(reversedArr, size, "Reversing array with creating extra array: ");
    delete reversedArr;
}


int main()
{
    //Reverse array with creating an extra array and inserting indexes from behind 
    int originalArr[] = {1, 2, 3, 4, 5};
    int size = sizeof(originalArr) / sizeof(originalArr[0]);
    reverseArrayExtraArray(originalArr, size);
}
