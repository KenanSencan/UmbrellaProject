#include <iostream>

template<typename T>
void PrintTypeName()
{
    std::cerr << typeid(T).name();
}
//int

int main()
{
    PrintTypeName<int>();
}