//Given an array A[] of n numbers and another number x, the task is to check whether or not there exist two elements in A[] whose sum is exactly x. 
//Input: arr[] = {0, -1, 2, -3, 1}, x= -2             1 + (-3) = -2

#include <algorithm>
#include <iostream>

//Simple but time complexity is: O(n*n) unfortunately
bool Solution1(int A[], int size, int x)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            if (A[i] + A[j] == x)
            {
                return true;
            }
        }
    }
    return false;
}

//Two pointer technique. Time Complexity: O(NlogN), Time complexity for sorting the array
bool HasArrayTwoCandidates(int* A, const int arrSize, int sum)
{
    int l, r;

    /* Sort the elements */
    std::sort(A, A + arrSize);

    /* Now look for the two candidates in the sorted array*/
    l = 0; // first element
    r = arrSize - 1; //last element
    while (l < r)
    {
        if (A[l] + A[r] == sum)
            return 1;
        else if (A[l] + A[r] < sum)
            l++;
        else // A[l] + A[r] > sum
            r--;
    }
    return 0;
}

int main()
{
    //Solution 1 
    int A[] = {0, -1, 2, -3, 1};
    int x = -2;
    int size = sizeof(A) / sizeof(A[0]);
    Solution1(A, size, x);

    //Solution 2
    int A2[] = {1, 4, 45, 6, 10, -8};
    int x2 = 16;
    int arrSize = sizeof(A2) / sizeof(A2[0]);
    HasArrayTwoCandidates(A, arrSize, x2);
}
