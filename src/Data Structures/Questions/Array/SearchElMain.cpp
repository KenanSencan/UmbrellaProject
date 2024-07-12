//Given a sorted and rotated array arr[] of n distinct elements, the task is to find the index of given target element in the array. If target is not present in the array, return -1.
// Input : arr[] = {30, 40, 50, 10, 20}, target = 10   
// Output : 3

#include <algorithm>
#include <iostream>

//Naive and simple approach O(N) Time and O(1) space 
int NaiveApproach(int arr[], int target, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int BinarySearch(int arr[], int target, int size)
{
    std::sort(arr,arr + size);
    int left = 0, right = size - 1;

    while (right >= left)
    {
        int mid = arr[(left + right) /2]; 
        if (mid == target)
            return mid;

        if (mid > target) right--;
        else left++;
    }
}

int main()
{
    int arr[] = {30, 40, 50, 10, 20};
    int target = 10;
    int size = sizeof(arr) / sizeof(arr[1]); 
    int Index = NaiveApproach(arr, target, size); //return 3
    int Index2 = BinarySearch(arr,target,size);
}
