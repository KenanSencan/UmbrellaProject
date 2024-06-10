#include <iostream>

int FindMyOwnImp(int arr[], int n, int key)
{
    for (int i = 0; i < n; ++i)
    {
        if (arr[i] == key)
        {
            return arr[i];
        }
    }

    return -1;
}

int main()
{
    int arr[] = {12, 34, 10, 6, 40};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Using a last element as search element
    int key = 40;

    // Function call
    int Value = FindMyOwnImp(arr, n, key);

    std::cout << "Value is: " << Value;

    return 0;
}