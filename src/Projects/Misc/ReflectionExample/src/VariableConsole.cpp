#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/json.hpp>
#include <string_view>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

// get variable
template <class Scope>
boost::json::value get(Scope& scope, std::string_view name)
{
    using Md = boost::describe::describe_members<Scope, boost::describe::mod_public>;

    bool found = false;
    boost::json::value result;

    boost::mp11::mp_for_each<Md>([&](auto D)
    {
        auto sa = D.name;
        if (!found && name == D.name)
        {
            result = boost::json::value_from(scope.*D.pointer);
            found = true;
        }
    });

    if (!found)
    {
        throw std::invalid_argument(
            std::string("'") + std::string(name) + "': no such variable");
    }

    return result;
}

// set variable

template <class T>
void set_impl(T& t, std::string_view /*name*/, boost::json::value const& value)
{
    t = boost::json::value_to<T>(value);
}

template <class T>
void set_impl(T const& /*t*/, std::string_view name, boost::json::value const& /*value*/)
{
    throw std::invalid_argument(
        std::string("'") + std::string(name) + "': variable cannot be modified");
}

template <class Scope>
void set(Scope& scope, std::string_view name, boost::json::value const& value)
{
    using Md = boost::describe::describe_members<Scope, boost::describe::mod_public>;

    bool found = false;

    boost::mp11::mp_for_each<Md>([&](auto D)
    {
        if (!found && name == D.name)
        {
            set_impl(scope.*D.pointer, name, value);
            found = true;
        }
    });

    if (!found)
    {
        throw std::invalid_argument(
            std::string("'") + std::string(name) + "': no such variable");
    }
}

//

struct globals
{
    std::string const help = "Enter a variable name ('x', 'y', 'v', or 'm') to print its value; enter variable=value to assign a new value to a variable. Values are in JSON format.";
    int x = 1;
    double y = 3.14;
    std::vector<int> v{1, 2, 3};
    std::map<std::string, double> m{{"BTC", 44898.68}, {"ETH", 1386.57}};
};

BOOST_DESCRIBE_STRUCT(globals, (), (help, x, y, v, m))

int main()
{
    globals g_;

    while (true)
    {
        std::cout << "\n> ";

        std::string input;
        std::getline(std::cin, input);

        try
        {
            std::size_t i = input.find('=');

            if (i != std::string::npos)
            {
                set(g_, input.substr(0, i), boost::json::parse(input.substr(i + 1)));
            }
            else
            {
                std::cout << get(g_, input) << std::endl;
            }
        }
        catch (std::exception const& x)
        {
            std::cout << "Error: " << x.what() << std::endl;
        }
    }
}
