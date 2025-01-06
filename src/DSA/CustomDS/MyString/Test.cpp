// File: TestMyString.cpp
#include <cassert>
#include "MyString.h"

int main()
{
    // 1. Basic constructor + Search test
    {
        MyString<char> str{"Hello"};
        assert(str.Search("o") == 4);
        assert(str.Search("H") == 0);
        assert(str.Size() == 5);  // 'Hello' => 5 characters
    }

    //2. Check string equals with raw CStyle string
    {
        MyString<char> str{"Hello"};
        assert(str == "Hello");
    }

    //3. Check copy assignment and == operator 
    {
        MyString<char> strFirst{"Hello"};
        MyString<char> strSec{"Sec"};
        strFirst = strSec;
        assert(strFirst == strSec);
    }

    //4. Insert and search
    {
        MyString<char> str{"Hello"};
        str.Insert("X",0);
        assert(str == "XHello");
        assert(str.Search("X") == 0);
    }

    //5. Remove index
    {
        MyString<char> str{"Hello"};
        str.RemoveIdx(0);
        assert(str == "ello");
    }

    //6 access single element
    {
        MyString<char> str{"Hello"};
        assert(str[0] == 'H');
    }

    //7. Ensure CurrentSize always correct
    {
        MyString<char> str{"Hello"};
        assert(str.Size() == 5);

        str = "Practice";
        assert(str.Size() == 8);
    }

    //8. Remove substring
    {
        MyString<char> str{"antartikaantar"};
        str.RemoveAllPattern("antar");
        assert(str == "tika");
    }
    
    //9. Concatenate with + operator
    {
        MyString<char> str{"Hello"};
        const MyString<char> str2{"Neco"};
        auto Test = str + str2;
        assert(Test == "HelloNeco");
    }

    //9. Concatenate with += operator
    {
        MyString<char> str{"Hello"};
        str+= "Neco";
        assert(str == "HelloNeco");
    }

    
    return 0; // Return 0 => test success
}
