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

void MySolution(int arr[], int size)
{
    int max = 0;
    int current = 0;
    for (int i = 0; i < size; ++i)
    {
         current += arr[i] + arr[i + 1];
        if (current > max)
            max = current;
        else
            current = 0;
    }
}

int main()
{
    int arr[] = {-2,-3,4,-1,-2,1,5,-3};
    int size = sizeof(arr) / sizeof(arr[0]);
    MySolution(arr,size);
    
}
