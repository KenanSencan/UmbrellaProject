//Given an array A[] of n numbers and another number x, the task is to check whether or not there exist two elements in A[] whose sum is exactly x. 
//Input: arr[] = {0, -1, 2, -3, 1}, x= -2             1 + (-3) = -2
#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

//Simple but time complexity is: O(n*n)
bool Solution1(int A[], int size, int x)
{
    for (int i = 0; i < size - 1; ++i)
        for (int j = i + 1; j < size; ++j)
            if (A[i] + A[j] == x)
                return true;
    return false;
}

//Two pointer technique. Time Complexity: O(NlogN), Time complexity for sorting the array
bool TwoPointer(int* A, const int arrSize, int sum)
{
    int left = 0, right = arrSize - 1;
    //Before anything sort
    std::sort(A, A + arrSize);
    while (right > left || left < right)
    {
        if (A[left] + A[right] == sum) return true;
        if (A[left] + A[right] > sum) right--;
        else if (A[left] + A[right] < sum) left++;
    }
    return false;
}

void printPairs(int arr[], int arr_size, int sum)
{
    std::unordered_set<int> s;
    for (int i = 0; i < arr_size; i++)
    {
        int temp = sum - arr[i];
        
        if (s.find(temp) != s.end())
        {
            int first = temp;
            int sec = arr[i];
            int result = first + sec; 
            std::cout << "Yes" << std::endl;
            return;
        }
        s.insert(arr[i]);
    }
    std::cout << "No" << std::endl;
}


int main(int argc, char** argv)
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
    TwoPointer(A2, arrSize, x2);

    //Solution 3
    int A3[] = { 1, 4, 45, 6, 10, 8 };
    const int n = 53;
    const int arr_size = sizeof(A3) / sizeof(A[0]);

    // Function calling
    printPairs(A3, arr_size, n);
}
