#include <filesystem>

#include "MyString.h"
#include "../Helper.h"
#include "benchmark/benchmark.h"


int main(int argc, char** argv)
{
    std::cout << std::unitbuf;
    MyString Str{"Hello"};

    bool sa = Str == "ErbakanHoca";

    std::cout << "Searching the index that first contains letter 'o': " << Str.Search("o") << std::endl; //Return the index of found string
    std::cout << "Before insertion: " << Str << std::endl; //Print directly the string with operator overload

    Str.Insert("Market", 2); //Insert given string to given index 
    std::cout << "After Insertion: " << Str << std::endl; //and print again the array to see the difference

    //Test assignment operator
    std::cout << "Before assignment operator: " << Str << std::endl;
    Str = "Hello";
    std::cout << "After assignment operator: " << Str << std::endl;

    std::cout << "After Removal index 3: ";
    Str.RemoveIdx(2);
    std::cout << Str << std::endl;

    std::cout << "Before inserting odd indexes: " << Str << std::endl;

    size_t j = 0;
    for (size_t i = 0; i < Str.Size(); ++i)
    {
        if (i + j + 1 > Str.Size()) break;
        Str.Insert("*", i + ++j);
    }
    std::cout << "After insert ing odd indexes: " << Str << std::endl;


    //Keyword removal
    Str = {"antartikaantar"};
    std::cout << "Str is: " << Str << std::endl;
    Str.RemoveAllPattern("antar");
    std::cout << "After every 'antar' keyword removed from Str: " << Str << std::endl;

    //Concationation
    Str = "Hello";
    MyString Str2 = "Neco";
    const auto test = Str + Str2;
    std::cout << "After concatenation of " << Str << " and " << Str2 << " is: " << test << std::endl;

    //+= operator
    std::cout << "Before += operator: " << Str;
    Str += "Truck";
    std::cout << "After += operator: " << Str;

    MyString<char> Test{Str.Size()};

    Helper::Reverse(Str.begin(), Str.end(),Test.begin());
    
    //Benchmarking
    constexpr int size = 1000000;
    MyString<char> Bm{size};

    // Helper::WriteRandomValToPath(size);
    Helper::ReadFromPathAssignToArr(Bm,size);
    benchmark::Initialize(&argc, argv);

    benchmark::RegisterBenchmark("BruteForce", [&Bm, &size](benchmark::State& state)
    {
        auto BmCopy = Bm;
        for (auto _ : state)
            BmCopy.RemoveAllCharNaive("2"); 
    })->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("RemoveAllCharInPlace", [&Bm, &size](benchmark::State& state)
    {
        auto BmCopy = Bm;
        for (auto _ : state)
            BmCopy.RemoveAllCharInPlace('2'); 
    })->Unit(benchmark::kMillisecond);
    
    
    // Str.RemoveAllCharNaive("a");
    // Str.RemoveAllCharInPlace('a');

    // benchmark::RunSpecifiedBenchmarks();
    // benchmark::Shutdown();
}
