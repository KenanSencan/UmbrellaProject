#include <iostream>
#include <ratio>
//IMP: lvalue argument  
// template <typename T, typename Arg>
// T create(Arg& a)
// {
//     return T(a);
// }
//
//IMP: Perfect Factory Method
// template <typename T, typename Arg>
// T create(const Arg& a)
// { 
//     return T(a);
// }

//IMP: rvalue only argument 
// template <typename T, typename Arg>
// T create(const Arg&& a)
// {
//     return T(a);
// }

//IMP: Both rvalue and lvalue argument though it will favor for lvalue when rvalue given so not reccommended  
// template <typename T, typename Arg>
// T create(Arg&& a)
// {
//     return T(a);
// }

//IMP: wrapper for rvalue and lvalue 
// template <class T>
// void wrapper(T&& a)
// {
//     create(std::forward<T>(a));
// }

//IMP: Even better better with variadic template
// template <typename T, typename... Args>
// T create(Args&&... args)
// {
//     return T(std::forward<Args>(args)...);
// }

//IMP: forwards arguments to constructor T, preserving value category (rvalue, lvalue) 
template <typename T, typename Arg>
T create(Arg&& a)
{
    return T(std::forward<Arg>(a));
}

class Example
{
public:
    Example(int&& x) { std::cout << "Rvalue constructor \n"; }
    Example(int& x) { std::cout << "LValue constructor \n"; }
};

int main()
{
    int a = 10;
    create<Example>(a); //lvalue
    create<Example>(10); //Rvalue
    create<Example>(std::move(a)); //Rvalue 
    return 0;
}
