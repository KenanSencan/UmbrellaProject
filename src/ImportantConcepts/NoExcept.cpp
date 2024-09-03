// h/t to reader yellowEmu for the first draft of this program
#include <iostream>

class Doomed
{
public:
    Doomed() { std::cout << "Doomed construced"; }
    ~Doomed() { std::cout << "Doomed destructed\n"; }
    int i = 31;
};

void thrower()
{
    std::cout << "Throwing exception\n";

    throw 1;
}

void pt()
{
    std::cout << "pt (potentally throwing) called\n";
    //This object will be destroyed during stack unwinding (if it occurs)
    Doomed doomed{};
    thrower();
    std::cout << "This never prints\n";
}

void nt() noexcept
{
    std::cout << "nt (noexcept) called\n";
    //This object will be destroyed during stack unwinding (if it occurs)
    Doomed doomed{};
    thrower();
    std::cout << "this never prints\n";
}

void tester(int c) noexcept
{
    std::cout << "tester (noexcept) case " << c << " called\n";
    try
    {
        (c == 1) ? pt() : nt();
    }
    catch (...)
    {
        std::cout << "tester caught exception\n";
    }
}

//Noexcept specifier with a Boolean parameter
//Call "frame info" in LLDB to get the template's constant value 
template <bool Noexcept>
void TemplateFunction() noexcept(Noexcept)
{
    std::cout << "templateFunction called with noexcept(" << std::boolalpha << Noexcept << ")\n";
    Doomed doomed;
    if constexpr (!Noexcept)
    {
        std::cout << "Throwing exception 1 \n";
        throw 1;
    }
    else
    {
        
    }
    std::cout << "This may or may not print depending on noexcept \n";
}

int main()
{
    std::cout << std::unitbuf; // flush buffer after each insertion
    std::cout << std::boolalpha; // print boolean as true/false
    tester(1);
    std::cout << "Test successful\n\n";
    // tester(2); //This will give error on MSVC when "throw 1" is called
    std::cout << "Test with 2 successful\n";

    //Template function calling
    try { TemplateFunction<true>(); }
    catch (...) { std::cout << "caught exception from TemplateFunction<true>\n"; }

    try { TemplateFunction<false>(); }
    catch (...) { std::cout << "Caught exception from templateFunction<false>\n"; }

    constexpr bool b1{noexcept(5+3)}; //true ints are non-throwing
    constexpr bool b2{noexcept(TemplateFunction<true>())}; //True. throws exception if the given varaible to template argument is "true"
    constexpr bool b3{noexcept(TemplateFunction<false>())}; ////False. not throws exception if the given varaible to template argument is "false"
    

    return 0;
}
