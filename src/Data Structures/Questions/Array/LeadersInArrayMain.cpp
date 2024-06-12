//Write a program to print all the LEADERS in the array. An element is a leader if it is greater than all the elements to its right side. And the rightmost element is always a leader.
//Input: arr[] = {16, 17, 4, 3, 5, 2}, 
// Output: 17,5,2

#include<iostream>
#include <stack>
#include <vector>
using namespace std;

//Works but extremely slow. Time cmplexity is: O(N * N) because there's 2 nested loops
void printLeaders(int arr[], int size)
{
    std::cout << "First approach: ";
    for (int i = 0; i < size; i++)
    {
        int j;
        for (j = i + 1; j < size; j++)
        {
            if (arr[i] <= arr[j])
                break;
        }
        if (j == size) // the loop didn't break
            cout << arr[i] << " ";
    }
    std::cout << "\n";
}

//Time complexity O(n)
void printLeaders2(int arr[], int size)
{
    std::vector<int> Leaders;
    //Determine the biggest number.
    //When moving from right to left, if the current index not bigger than the previous one, count that in
    //and assign that as biggest number
    int biggestNumber = arr[size - 1];
    std::cout << " second approach: " << arr[size - 1] << " ";
    for (int i = size - 2; i >= 0; --i) //move backwards
    {
        if (arr[i] > biggestNumber)
        {
            std::cout << arr[i] << " ";
            biggestNumber = arr[i];
        }
    }
    std::cout << "\n";\
}

//Roughly same as the last method. Instead stack is used
void printLeaders3(int arr[], int size)
{
    /* create stack to store leaders*/
    stack<int> sk;
    sk.push(arr[size - 1]);

    for (int i = size - 2; i >= 0; i--)
    {
        if (arr[i] >= sk.top())
        {
            sk.push(arr[i]);
        }
    }

    /* print stack elements*/ /* run loop till stack is not empty*/
    while (!sk.empty())
    {
        cout << sk.top() << " ";
        sk.pop();
    }
}

int main()
{
    int arr[] = {16, 17, 4, 3, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    printLeaders(arr, n);
    printLeaders2(arr, n);
    printLeaders3(arr, n);
    return 0;
}
