#include <iostream>
#include <string>
#include <utility>

class Test
{
public:
    explicit Test(std::string SelfName);
    ~Test();
    Test(const Test& other);
    Test(Test&& other) noexcept;
    // Test& operator=(Test&& other) noexcept;
    int* ArrPtr;
    std::string OwnName;
    int ArrSize;

    static Test DeleteGivenAndAssignSomethingElse(const Test& HeapVar);
    void CopyFrom(const Test& Test);
    void PrintAll() const;

    //iterators
    [[nodiscard]] int* begin() const { return ArrPtr; }
    [[nodiscard]] int* end() const { return ArrPtr + ArrSize; }
};

Test::Test(std::string SelfName) : OwnName(std::move(SelfName)), ArrSize(10)
{
    ArrPtr = new int[ArrSize];
    for (int i = 0; i < ArrSize; ++i)
    {
        ArrPtr[i] = i;
    }
}

Test::~Test()
{
    std::cout << "Destructor invoked for: " << this->OwnName << std::endl;
    delete[] ArrPtr;
}

Test::Test(const Test& other) : ArrSize(0)
{
    std::cout << "Copy constructor called";
    ArrPtr = nullptr;
    CopyFrom(other);
}

//Move constructor. Self will be assigned with other
Test::Test(Test&& other) noexcept : ArrPtr(other.ArrPtr), OwnName(std::move(other.OwnName)), ArrSize(other.ArrSize)
{
    std::cout << "Move invoked" << std::endl;
    other.ArrPtr = nullptr;
    other.ArrSize = 0;
}

Test Test::DeleteGivenAndAssignSomethingElse(const Test& HeapVar)
{
    Test StackVar("StackVar");

    for (int i = 0; i < StackVar.ArrSize; ++i)
        StackVar.ArrPtr[i] = 31;

    HeapVar.PrintAll();
    delete[] HeapVar.ArrPtr;
    return std::move(StackVar);
}

void Test::CopyFrom(const Test& Test)
{
    ArrSize = Test.ArrSize;
    ArrPtr = new int[ArrSize];
    std::copy<int*, int*>(Test.begin(), Test.end(), ArrPtr);
}

void Test::PrintAll() const
{
    std::cout << "Object: " << OwnName;
    for (int i = 0; i < ArrSize; ++i)
    {
        std::cout << ArrPtr[i] << " ";
    }
    std::cout << std::endl;
    std::cout.flush();
}

int main()
{
    const Test HeapVar("HeapVar");
    const Test NewObject = Test::DeleteGivenAndAssignSomethingElse(HeapVar);
    HeapVar.PrintAll();
    NewObject.PrintAll();

    return 0;
}