#include <algorithm>
#include <iostream>
#include <limits.h>
#include <vector>

//Time O((n*logn) + n) //Aux: O(1) 
int getSecondLargest(std::vector<int>& arr)
{
    const int size = static_cast<int>(arr.size());

    // Sort the array in non-decreasing order
    std::sort(arr.begin(), arr.end()); //O(n log n) 
    return arr[size - 2];
}

//O(2 * n) time O(1) space
int getSecondLargest2(std::vector<int>& arr)
{
    int size = static_cast<int>(arr.size());
    int Max = arr[0], SecondMax = arr[0];

    //Get Max array first.
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] > Max) Max = arr[i];
    }

    //Get second aray
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] > SecondMax && arr[i] != Max) SecondMax = arr[i];
    }
    return SecondMax;
}

//O(n) time O(1)
int getSecondLargest3(const std::vector<int>& arr)
{
    const int size = arr.size();
    int Largest = INT_MIN, LargestSec = INT_MIN;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] > Largest)
        {
            LargestSec = Largest;
            Largest = arr[i];
        }

        else if (arr[i] < Largest && arr[i] > LargestSec)
        {
            LargestSec = arr[i];
        }
    }
    
    return LargestSec;
}

int main()
{
    std::vector<int> arr{12, 44, 35, 1, 10, 34, 1}; //Expected: 34
    std::cout << getSecondLargest3(arr);
}
