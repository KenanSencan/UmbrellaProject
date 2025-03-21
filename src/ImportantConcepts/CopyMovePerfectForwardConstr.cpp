#include <iostream>
#include <string> // for std::string
#include <vector> // for std::vector
#include <utility> // for std::pair

template <typename T, typename U>
struct MyPair
{
    // Constructor for general initialization
    template <typename T1, typename U1>
    MyPair(T1&& a, U1&& b) : first(std::forward<T1>(a)), second(std::forward<U1>(b))
    {
        std::cout << "general constructor with perfect forwarding\n";
    }
    
    //Copy constructor. Copy from other to self 
    MyPair(const MyPair& other) : first(other.first), second(other.second)
    {
        std::cout << "copy constructor \n";
    }

    // Move constructor. Move from other to self 
    MyPair(MyPair&& other) noexcept : first(std::move(other.first)), second(std::move(other.second))
    {
        std::cout << "move constructor \n";
    };

    //Default constructors. It will do what I already did above 
    // MyPair(const MyPair& other) = default;
    // MyPair(MyPair&& other) noexcept = default;

    T first{};
    U second{};
};


int main()
{
    int a = 1, b = 3;
    
    MyPair<int, int> p6(3, 5); //perfect forward
    MyPair<int, std::string> p7(3, "answer"); //perfect forward 
    MyPair<int, int> p8(a, b); //perfect forward

    MyPair<int,int> p9 = p6; //copy constructor
    MyPair<int,int> g10 = std::move(p6); //move constructor
    

    std::cout << p7.first << std::endl;
    std::cout << p7.second << std::endl;
}
