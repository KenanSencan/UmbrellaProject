//Given an array arr[] of size N. The task is to find the sum of the contiguous subarray within a arr[] with the largest sum. 
//Input: arr = {-2,-3,4,-1,-2,1,5,-3}
//Output: 7
//Explanation: The subarray {4,-1, -2, 1, 5} has the largest sum 7.

//Input: arr = {2}
//Output: 2
//Explanation: The subarray {2} has the largest sum 1.

//Input: arr = {5,4,1,7,8}
//Output: 25
//Explanation: The subarray {5,4,1,7,8} has the largest sum 25.

#include <climits>
#include <iostream>
#include <vector>

//O(n) time complexity and O(1) space complexity
void Kadane(int arr[], int size)
{
    int MaxSoFar = INT_MIN, CurrentTotal = 0, start = 0, end = 0, s = 0;

    for (int i = 0; i < size; i++)
    {
        CurrentTotal += arr[i];
        if (CurrentTotal > MaxSoFar)
        {
            MaxSoFar = CurrentTotal;
            start = s;
            end = i;
        }

        if (CurrentTotal < 0)
        {
            CurrentTotal = 0;
            s = i + 1;
        }
    }
    std::cout << "Maximum contiguous sum is " << MaxSoFar << std::endl;
    std::cout << "Starting index " << start << std::endl << "Ending index " << end << std::endl;
}



//IDK what's DYNAMIC PROGRAMMING IS. So let's skip this for now. I promise I will return here again when I handled other DSA stuffs first.
// For each index i, DP[i] stores the maximum possible Largest Sum Contiguous Subarray ending at index i, and therefore we can calculate DP[i] using the mentioned state transition:
//DP[i] = max(DP[i-1] + arr[i] , arr[i] )
void DynamicProgrammingSolution(int Arr[], const int size)
{
    std::vector<int> dp(size, 0);
    dp[0] = Arr[0];
    int ans = dp[0];
    for (int i = 1; i < size; i++)
    {
        dp[i] = std::max(Arr[i], Arr[i] + dp[i - 1]);
        ans = std::max(ans, dp[i]);
    }
    std::cout << ans;
}

int main()
{
    int arr[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int size = sizeof(arr) / sizeof(arr[0]);
    Kadane(arr, size);
    DynamicProgrammingSolution(arr, size);
}
