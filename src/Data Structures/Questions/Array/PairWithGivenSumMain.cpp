//Given an array A[] of n numbers and another number x, the task is to check whether or not there exist two elements in A[] whose sum is exactly x. 
//Input: arr[] = {0, -1, 2, -3, 1}, x= -2             1 + (-3) = -2

#include <algorithm>
#include <iostream>

//Simple but time complexity is: O(n*n)
bool Solution1(int A[], int size, int x)
{
    int ComplexCounter = 0;
    for (int i = 0; i < size - 1; ++i)
    {
        ComplexCounter++;
        for (int j = i + 1; j < size; ++j)
        {
            ComplexCounter++;
            if (A[i] + A[j] == x)
            {
                ComplexCounter++;
                std::cout << "Complexity counter is: " << ComplexCounter;
                return true;
            }
            ComplexCounter++;
        }
        ComplexCounter++;
    }
    std::cout << "Complexity counter is: " << ComplexCounter;
    return false;
}

//Two pointer technique. Time Complexity: O(NlogN), Time complexity for sorting the array
bool HasArrayTwoCandidates(int* A, const int arrSize, int sum)
{
    int left = 0, right = arrSize - 1;

    //sort the array first
    std::sort(&A[0], &A[arrSize]);
    while (left < right)
    {
        if (A[left] + A[right] == sum)
            return true;

        if (A[left] + A[right] < sum) //need to go from left -> right
            left++;
        else
            right--;
    };
    return false;
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
    HasArrayTwoCandidates(A2, arrSize, x2);
}
