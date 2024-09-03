#include <iostream>

#include "Test.h"

int main() {
    
    Test Object1;
    Object1.OwnName = "Object1";

    Test Object2;
    Object2.OwnName = "Object2";
    Test::SetAllVal31(Object2);
    
    //I will get error "Rvalue reference to type Test cannot bind to lvalue of type Test". Because to make asssignment I need either copy or move assignment defined.
    //I defined move but not copy assignment. Therefore in here I cannot make copy assignment (Unless I make it explicitly)  
    // AnotherHeap = HeapVar;

    //This will work. operator = will go to  "Test& Test::operator=(Test&& other) noexcept"
    //Also this doesn't make any move itself. It just casting from lvalue to rvalue
    
    Object1 = std::move(Object2);
    //This is also completely same  
    Object1 = static_cast<Test&&>(Object2);
    
    return 0;
}
 // std::cout << "least 8:  " << sizeof(std::int_least8_t) * 8 << " bits\n";
    // std::cout << "normal 8:  " << sizeof(std::uint8_t) * 8 << " bits\n";
    // std::cout << "fast 8:  " << sizeof(std::uint_fast8_t) * 8 << " bits\n";
    // std::cout << "normal int:  " << sizeof(int) * 8 << " bits\n";