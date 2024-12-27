#include <algorithm>
#include <iostream>
#include <vector>

//Iterative approach. O(n) Time and O(1) Space
void MaxIterative(const std::vector<int>& var)
{
    int MaxEl = var[0];
    for (int i = 1; i < var.size(); ++i)
    {
        if (var[i] > MaxEl) MaxEl = var[i];
    }

    std::cout << "Max: " << MaxEl;
}

//Recursive approach. O(N) Time and O(N) complexity
//The function recurses until i reaches the last index, returning that element as the base case. Each previous call compares its element to the recursive result
int MaxRecursive(const std::vector<int>& arr, const int i) noexcept
{
    //This will hit at last
    if (i == arr.size() - 1)
    {
        return arr[i];
    }

    const int MaxFound = MaxRecursive(arr, i + 1);
    return std::max(MaxFound, arr[i]);
}

int main()
{
    std::vector<int> arr{2, 3, 4, 10, 40, 31, 566, 31, 44};
    MaxIterative(arr);

    std::cout << "Largest in given array is " << MaxRecursive(arr, 0);
    int Builtin{*std::max_element(arr.begin(), arr.end())};

    return 0;
}
