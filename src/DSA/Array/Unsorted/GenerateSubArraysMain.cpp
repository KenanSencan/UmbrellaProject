#include <iostream>
#include <vector>

//O(3 * n) time and O(1) space 
void PrtSubArrays(const std::vector<int>& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        for (int j = 0; j < vec.size(); ++j)
        {
            for (int z = i; z <= j; ++z)
            {
                std::cout << vec[z] << ' ';
            }

            if (i <= j) std::cout << std::endl;
        }
    }
}

void printSubArrays2(std::vector<int>& arr, const int start, const int end)
{
    // Stop if we have reached the end of the array
    if (end == arr.size())
        return;

    // Increment the end point and reset the start to 0
    if (start > end)
        printSubArrays2(arr, 0, end + 1);

    // Print the subarray and increment the starting point
    else
    {
        for (int i = start; i <= end; i++)
            std::cout << arr[i] << " ";
        std::cout << std::endl;
        printSubArrays2(arr, start + 1, end);
    }
}

void Test(const std::vector<int>& arr, const int start, const int end)
{
    if (end == arr.size()) return;
    if (start > end)
    {
        Test(arr, 0, end + 1);
    }
    else
    {
        for (int i = start; i <= end; ++i)
        {
            std::cout << arr[i] << ' ';
        }
        std::cout << std::endl;

        Test(arr,start + 1, end);    
    }
}

int main()
{
    // std::vector<int> arr{12, 35, 1, 10, 34, 1};
    std::cout.setf(std::ios::unitbuf);

    std::vector<int> arr{1, 2, 3};
    Test(arr, 0, 0);
}
