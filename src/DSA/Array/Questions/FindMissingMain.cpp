//     Input: arr[] = {1, 2, 4, 6, 3, 7, 8} , N = 8
// Output: 5
// Explanation: Here the size of the array is 8, so the range will be [1, 8]. The missing number between 1 to 8 is 5
//
// Input: arr[] = {1, 2, 3, 5}, N = 5
// Output: 4
// Explanation: Here the size of the array is 4, so the range will be [1, 5]. The missing number between 1 to 5 is 4

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

//My solution. It's not perfect I guess lots of foor loops in it but overall it's O(N) and O(N)
int Hashing(int max, vector<int> arr)
{
    std::unordered_set<int> Ranges;
    auto min = *std::min_element(arr.begin(), arr.end(), [](int a, int b){return a < b;});
    
    //From 0-n insert all to USet
    for (int i = min; i <= max; ++i)
        Ranges.insert(i);

    //Remove USet els found in arr so only the one left will be the missing number
    for (int i : arr)
        Ranges.erase(i);

    if (!Ranges.empty())
        return *Ranges.begin();
    return -1;
}

//Time: O(N) Aux: O(1)
int GaussSummation(const int start, const int end, const vector<int>& arr)
{
    int ArraySum = 0;
    // Sum everything inside the array
    for (int i : arr) {
        ArraySum += i;
    }

    // Calculate the expected sum for the range [start, end]
    int expectedSum = ((((end + 1) * end) / 2) - (((start + 1) * start) / 2)) + start;

    // Subtract from ArrSum - RangeSum which will result the missing number 
    int result = expectedSum - ArraySum;
     return result;
}

//Time complexity: O(N) Auxiliary: O(1)
int XOR(int start, int end, vector<int>& arr)
{ 
    int xor1 = 0, xor2 = 0;

    // XOR all array elements
    for (const int i : arr) {
        xor2 ^= i;
    }

    // XOR all numbers from 1 to n
    for (int i = start; i <= end; i++) {
        xor1 ^= i;
    }

    // Missing number is the XOR of xor1 and xor2
    return xor1 ^ xor2;
}

int main()
{
    vector<int> arr = {4,5,6,7,9};
    int n = 9;
    int start = 4;
    int end = 9;
    cout << Hashing(n, arr);
    cout << GaussSummation(start, end, arr);// Output should be 8}
    int missing = XOR(start,end, arr);
    return 0;
} 
