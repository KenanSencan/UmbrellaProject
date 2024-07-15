//Time compplexity: O(log(n))
//Auxilary space O(log(n))
#include <iostream>
using namespace std;

int binarySearch(int arr[], int low, int high, int key)
{
    if (high < low) return -1;

    int mid = (low + high) / 2; /*low + (high - low)/2;*/ //It's like take the median
    if (key == arr[mid]) return mid;
    if (key > arr[mid])
        return binarySearch(arr, (mid + 1), high, key);

    //else
    return binarySearch(arr, low, (mid - 1), key);
}

int main()
{
    int arr[] = { 5, 6, 7, 8, 9, 10 };
    int size, key;

    size = sizeof(arr) / sizeof(arr[0]);
    key = 10;

    // Function call
    std::cout << "Index: " << binarySearch(arr, 0, size - 1, key) << endl;
    return 0;
}