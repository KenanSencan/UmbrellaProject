#include <algorithm>
#include <iostream>
#include <vector>

//Time O(n * count)  SpaceO(1)
void RotateArright1(std::vector<int>& arr, const int& count)
{
    for (int i = 1; i <= count; ++i)
    {
        int LastEl = arr[arr.size() - 1];
        for (int i = arr.size() - 1; i >= 0; --i)
        {
            arr[i] = arr[i - 1];
        }
        arr[0] = LastEl;
    }
}

//Time O(2N) Space O(N) 
void rotateArr2(std::vector<int>& arr, int d)
{
    const int size = arr.size();
    std::vector<int> TempArr(size);
    int counter = 0;

    // Handle case when d > n
    d %= size;

    for (int i = size - d; i < size; ++i)
    {
        TempArr[counter] = arr[i];
        counter++;
    }

    for (int i = 0; i < size - d; ++i)
    {
        TempArr[i + d] = arr[i];
    }

    arr = TempArr;
}

void rotateArr3(std::vector<int>& arr, int d)
{
    //firstly reverse the array
    std::reverse(arr.begin(),arr.end());

    //Sec reverse first d elements
    std::reverse(arr.begin(), arr.begin() + d);

    //Third reverse remaning
    std::reverse(arr.begin() + d, arr.end());
    
}

int main()
{
    std::cout.setf(std::ios::unitbuf);

    std::vector<int> arr{1, 2, 3, 4, 5, 6};
    int d = 2;

    rotateArr3(arr, d);

    for (const int i : arr)
    {
        std::cout << i << ' ';
    }
}
