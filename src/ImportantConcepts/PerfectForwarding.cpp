#include <iostream>
#include <ratio>
//NOTE: lvalue argument
//Only accepts lvalues
//Rejects rvalues (like literals or temporaries)
template <typename T, typename Arg>
T create(Arg& a)
{
    return T(a);
}

//NOTE: Perfect Factory Method
//Works with both lvalues and rvalues
//But everything gets treated as const lvalue
template <typename T, typename Arg>
T create(const Arg& a)
{
    return T(a);
}

//NOTE: rvalue only argument
//Only accepts rvalues
//The a inside becomes an lvalue (named value)
template <typename T, typename Arg>
T create(const Arg&& a)
{
    return T(a);
}

//NOTE: Both rvalue and lvalue argument though it will favor for lvalue when rvalue given so not reccommended
//NOTE: This is the most dangerous one
//Takes both lvalues and rvalues
// But a is always treated as lvalue inside function
// template <typename T, typename Arg>
// T create(Arg&& a)
// {
//     return T(a);
// }

// IMP: Even better better with variadic template
//  template <typename T, typename... Args>
//  T create(Args&&... args)
//  {
//      return T(std::forward<Args>(args)...);
//  }

//---------------------------------CORRECT PERFECT FORWARDING!!!!!!
//NOTE: forwards arguments to constructor T, preserving value category (rvalue, lvalue)
//This is the correct implementation using perfect forwarding
//Preserves value category using std::forward
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
