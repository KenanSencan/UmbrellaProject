#include <iostream>
#include <valarray>
#include <vector>
int main()
{
    std::vector<int>Test {1,2,3,4,5};
    int* p = Test.data();
    std::cout << *p << std::endl;
    std::cout << p[1] << std::endl;
}