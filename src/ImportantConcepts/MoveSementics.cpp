#include <iostream>
#include <string>
#include <utility> // for std::move
#include <algorithm> // for std::copy_n

/**
 * Example class demonstrating move semantics in C++
 * 
 * This class contains a dynamically allocated resource (int array) to show
 * proper resource management with move operations
 */

class Test
{
private:
    int* data; // Dynamically allocated array
    int size; // Size of the array
    std::string name; // Name for tracking objects

public:
    // Default constructor
    Test() : size(10), name("unnamed")
    {
        std::cout << "Default constructor: Creating '" << name << "'\n";
        data = new int[size];
        for (int i = 0; i < size; ++i)
        {
            data[i] = i;
        }
    }

    // Constructor with name
    explicit Test(const std::string& objectName) : size(10), name(objectName)
    {
        std::cout << "Named constructor: Creating '" << name << "'\n";
        data = new int[size];
        for (int i = 0; i < size; ++i)
        {
            data[i] = i;
        }
    }

    // Destructor
    ~Test()
    {
        std::cout << "Destructor: Destroying '" << name << "'\n";
        delete[] data;
    }

    // Copy constructor
    Test(const Test& other) : size(other.size), name(other.name + " (copy)")
    {
        std::cout << "Copy constructor: Creating copy of '" << other.name << "'\n";
        data = new int[size];
        std::copy_n(other.data, size, data);
    }

    // Move constructor
    Test(Test&& other) noexcept :
        data(other.data),
        size(other.size),
        name(std::move(other.name))
    {
        std::cout << "Move constructor: Moving from '" << name << "'\n";

        // Reset source object (leave in valid but empty state)
        other.data = nullptr;
        other.size = 0;
        other.name = "moved-from";
    }

    // Copy assignment operator (deleted to focus on move semantics)
    Test& operator=(const Test& other) = delete;

    // Move assignment operator
    Test& operator=(Test&& other) noexcept
    {
        std::cout << "Move assignment: Moving '" << other.name << "' into '" << name << "'\n";

        if (this != &other)
        {
            // Free existing resources
            delete[] data;

            // Transfer ownership
            data = other.data;
            size = other.size;
            name = std::move(other.name);

            // Reset source object
            other.data = nullptr;
            other.size = 0;
            other.name = "moved-from";
        }
        return *this;
    }

    // Utility to change all values in the array
    void fillWith(int value)
    {
        for (int i = 0; i < size; ++i)
        {
            data[i] = value;
        }
        std::cout << "Filled '" << name << "' with value " << value << "\n";
    }

    // Print array contents
    void print() const
    {
        std::cout << "Object '" << name << "' contains: [";
        for (int i = 0; i < size; ++i)
        {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    // Check if object has been moved from
    [[nodiscard]] bool isMovedFrom() const
    {
        return data == nullptr;
    }
};

int main()
{
    std::cout << "=== MOVE SEMANTICS DEMONSTRATION ===\n\n";

    // Create first object
    Test obj1("Original");
    obj1.print();

    std::cout << "\n--- MOVE CONSTRUCTOR EXAMPLE ---\n";
    // Use move constructor to create obj2 from obj1
    Test obj2(std::move(obj1)); //Directly transfer of ownership
    obj2.print();

    // Check state of obj1 after being moved from
    std::cout << "After move, original object state: "
        << (obj1.isMovedFrom() ? "moved-from (empty)" : "still has data") << "\n";

    std::cout << "\n--- MOVE ASSIGNMENT EXAMPLE ---\n";
    // Create a third object
    Test obj3("Another");
    obj3.fillWith(42);
    obj3.print();

    // Perform move assignment
    obj2 = std::move(obj3);
    obj2.print();

    std::cout << "\n--- EQUIVALENT std::move FORMS ---\n";
    // Create a new object
    Test obj4("Final");
    obj4.print();

    // Demonstrate that std::move is just a cast
    std::cout << "Using std::move:\n";
    obj4 = std::move(obj2);

    // Create another object to show static_cast equivalence
    Test obj5("StaticCast");
    std::cout << "Using static_cast<Test&&>:\n";
    // This is equivalent to std::move - both just cast to an rvalue reference
    obj5 = static_cast<Test&&>(obj4);

    std::cout << "\n=== PROGRAM ENDING - WATCH DESTRUCTORS CALLED ===\n";
    return 0;
}
