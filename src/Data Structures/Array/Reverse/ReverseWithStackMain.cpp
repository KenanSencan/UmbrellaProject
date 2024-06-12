#include <iostream>
#include <stack>

void reverseArrayUsingStack(int arr[], int size)
{
    std::stack<int> stack;

    // Push elements onto the stack
    for (int i = 0; i < size; i++) {
        stack.push(arr[i]);
    }

    // Pop elements from the stack to reverse the array
    for (int i = 0; i < size; i++) {
        arr[i] = stack.top();
        stack.pop();
    }
}

int main()
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int size = sizeof(arr) / sizeof(arr[0]);

    reverseArrayUsingStack(arr, size);

    std::cout << "Reversed Array: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }

    return 0;
}
