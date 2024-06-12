#include <iostream>

int main()
{

int originalArray[] = { 1, 2, 3, 4, 5 };
int length = sizeof(originalArray) / sizeof(originalArray[0]);

// Using inbuilt method in C++
std::reverse(&originalArray[0], &originalArray[length]);

// Print the reversed array
for (int i = 0; i < length; i++) {
    std::cout << originalArray[i] << " ";
}
}