#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

//1. sort, remove duplicates and then get 3th max
//O(n log n)
int ThirdLargest(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end());
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());

    return vec[vec.size() - 3];
}

//2. loop
int ThirdLargest2(const std::vector<int>& vec)
{
    int MaxFirst = INT_MIN, MaxSec = INT_MIN, MaxThird = INT_MIN;
    for (const int i : vec)
    {
        if (i > MaxFirst)
        {
            MaxSec = MaxFirst;
            MaxFirst = i;
        }

        if (i < MaxFirst && i > MaxSec)
        {
            MaxThird = MaxSec;
            MaxSec = i;
        }

        if (i < MaxSec && i > MaxThird)
        {
            MaxThird = i;
        }
    }

    return MaxThird;
}

int main()
{
    std::vector<int> arr{12, 35, 35, 35, 1, 10, 34, 1}; //Expected: 34
    std::cout << ThirdLargest2(arr);
}
