//Find majority element in an array. It has to be minimum more than n/2 times
//I think this is not that important or worth to learn algorithm. The logic is so weird that it has to always provide n/2 + 1

//A[] = {3,4,2,4,2,4,4} Output: 4. Frequency of 4, is greater than the half of the size of the array size.
//A[] = {3,3,4,2,4,4,2,4} Output: None. None of the elements are more frequent than the half of the size of the array
//Being equal to size of the array is not acceptable it has to be minimum "(n / 2) + 1"  

#include <iostream>

//The algorithm BoyerMoore
// Time: O(N) Auxilary: O(1)  
int BoyerMooreMethod(const int Arr[], int size)
{
    int maj_index = 0, count = 1;
    for (int i = 1; i < size; i++)
    {
        if (Arr[maj_index] == Arr[i])
            count++;
        else
            count--;
        if (count == 0)
        {
            maj_index = i;
            count = 1;
        }
    }

    //maj_index is determined. The found element is mostly repetitive element but still it's not sure it's > n/2. So final check really if it's majority element in an array
    count = 0;
    for (int i = 0; i < size; i++)

        if (Arr[i] == Arr[maj_index])
            count++;

    if (count > size / 2)
        // state.counters["ReturnVal"] = 0;
        return Arr[maj_index];
    else
    return -1;
        // state.counters["ReturnVal"] = 1;
}

int main(int argc, char** argv)
{
    int arr[] = { 1, 2, 3, 1, 2, 3, 4, 1,1,1,1,1 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int majority = BoyerMooreMethod(arr, n);
    std::cout << " The majority element is : " << majority;
}