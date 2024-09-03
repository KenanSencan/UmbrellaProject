#pragma once
#include <string>


class Test {
public:
    
    int* SelfPtr;
    int CurrSize;
    //Just a name to track the current object easier 
    std::string OwnName;
    std::string OwnNameUntouched;

    Test();
    ~Test();
    //Copy constructor
    Test(const Test& other);
    //Move constructor
    Test(Test&& other) noexcept;
    //Move assignment 
    Test& operator=(Test&& other) noexcept;
    static void SetAllVal31(const Test& Test);
    void CopyFrom(const Test& Test);
    void PrintAll() const;

    //IMP: Do not allow copy constructor
    // Test(const Test& other) = delete;

    //IMP: Do not allow copy assignment
    Test& operator=(Test& other) = delete;
};



