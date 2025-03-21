#include <iostream>
#include <variant>
#include <string>
#include <boost/describe.hpp>

int main()
{
    //------------------------------Variant with mp11----------------------------------
    std::variant<int, double, std::string> myVariant = 42;

    using variant_types = boost::mp11::mp_list<int, double, std::string>;

    // Process all possible types
    boost::mp11::mp_for_each<variant_types>([&]<typename T0>(const T0&)
    {
        // Get the type represented by the dummy parameter
        using T = std::decay_t<T0>;

        // Check if variant currently holds this type
        if (std::holds_alternative<T>(myVariant))
        {
            T& value = std::get<T>(myVariant);
            if constexpr (std::is_same_v<T, int>) std::cout << "It's int: " << value << std::endl;
            else if constexpr (std::is_same_v<T, double>) std::cout << "It's double: " << value << std::endl;
            else if constexpr (std::is_same_v<T, std::string>) std::cout << "It's string: " << value << std::endl;
        }
    });
    
    if (std::holds_alternative<int>(myVariant))
    {
        std::cout << std::get<int>(myVariant) << std::endl;
    }
    
    myVariant = 3.14;
    if (std::holds_alternative<double>(myVariant))
    {
        std::cout << std::get<double>(myVariant) << std::endl;
    }
    
    myVariant = std::string{"HelloWorld"};
    if (std::holds_alternative<std::string>(myVariant))
    {
        std::cout << std::get<std::string>(myVariant) << std::endl;
    }
    
    //-----------------------------Variant with std::visit----------------------------
    myVariant = std::string{"HelloWorld"};

    auto visitor = []<typename T0>(T0&& arg) -> std::string
    {
        using T = std::decay_t<T0>;
    
        if constexpr (std::is_same_v<T, std::string>) return "It's string";
        if constexpr (std::is_same_v<T, double>) return "It's double";
        if constexpr (std::is_same_v<T, int>) return "It's int";
    
        return "it's nothing";
    };
    
    std::cout << std::visit(visitor, myVariant) << std::endl;
   

    return 0;
}
