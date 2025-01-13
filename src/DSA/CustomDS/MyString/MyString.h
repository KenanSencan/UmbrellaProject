#ifndef MYSTRING_H
#define MYSTRING_H
#include <iostream>
#include <cstring>

template <typename T>
class MyString
{
public:
    MyString() : Str(nullptr), CurrentSize(0), Capacity(0)
    {
        std::cout << "Normal constructor called" << std::endl;
    };

    ~MyString()
    {
        delete[] Str;
    }

    MyString(const T* value) : Str(nullptr), CurrentSize(0), Capacity(0)
    {
        const size_t size = strlen(value);
        CurrentSize = size;
        Capacity = CurrentSize;
        Str = new T[CurrentSize]; //I don't know how wchar or other char types works. But it should be relatively easy to implement
        CopyStr(value);
        Str[size] = '\0';

        // std::cout << "Normal constructor with value argument called" << std::endl;
    }
    

    MyString(const size_t size) : Str(nullptr), CurrentSize(0), Capacity(0)
    {
        CurrentSize = size;
        Capacity = CurrentSize;
        Str = new T[CurrentSize]; //I don't know how wchar or other char types works. But it should be relatively easy to implement
        Str[size] = '\0';

        std::cout << "Normal constructor with size argument called" << std::endl;
    }


    //Copy constructor

    MyString(const MyString& other) : Str(nullptr), CurrentSize(other.CurrentSize), Capacity(other.Capacity)
    {
        Str = new T[Capacity];
        for (size_t i = 0; i < CurrentSize; ++i)
        {
            Str[i] = other.Str[i];
        }
        Str[CurrentSize] = '\0';
        // std::cout << "copy constructor with other argument called" << std::endl;
    }


    //Copy assignment

    MyString& operator=(const MyString& other)
    {
        if (this != &other)
        {
            delete[] Str;
            CurrentSize = other.CurrentSize;
            Capacity = other.Capacity;
            Str = new T[Capacity];
            for (size_t i = 0; i < CurrentSize; ++i)
            {
                Str[i] = other.Str[i];
            }
            Str[CurrentSize] = '\0';
        }
        return *this;
    }


    //Raw string == comparison

    bool operator==(const T* other)
    {
        size_t otherLen = strlen(other);
        //If sizes are different, no need to check individually each element anyway 
        if (CurrentSize != otherLen && Str[otherLen] != '\0') return false;

        for (size_t i = 0; i < otherLen; ++i)
        {
            if (Str[i] != other[i])
                return false;
        }

        return true;
    }

    bool operator==(const MyString& other)
    {
        //If sizes are different, no need to check individually each element anyway 
        if (CurrentSize != other.CurrentSize) return false;

        for (size_t i = 0; i < CurrentSize; ++i)
        {
            if (Str[i] != other.Str[i])
                return false;
        }

        return true;
    }

    T& operator[](size_t idx)
    {
        return Str[idx];
    }

    const T& operator[](size_t idx) const
    {
        return Str[idx];
    }


    MyString& operator=(const T* str)
    {
        AssignNewValue(str);
        return *this;
    }

    MyString operator+(const MyString& str)
    {
        MyString result(*this);

        //Make concat
        EnsureCapacity(result.Size() + str.Size());
        for (int i = 0; i < str.Size(); ++i)
        {
            result[i + result.Size()] = str[i];
        }
        result.CurrentSize += str.Size();
        result[result.Size()] = '\0';
        return result;
    }

    MyString& operator+=(const T* str)
    {
        size_t strSize = strlen(str);
        EnsureCapacity(CurrentSize + strSize);
        for (int i = 0; i < strSize; ++i)
        {
            Str[i + CurrentSize] = str[i];
        }

        CurrentSize += strSize;
        Str[CurrentSize] = '\0';
        return *this;
    }


    //strlen implementation
    [[nodiscard]] size_t Size() const
    {
        return CurrentSize;
    }

    //Search a String. Return first occurrence. If not found, return -1
    //Ex: "Hello" str = lo. Return 3
    //However it doesn't return the start and end indexes of found substring. 
    //O(n * n) time. O(1) Space.
    [[nodiscard]] int Search(const T* str) const
    {
        const size_t ArgSize = strlen(str);
        for (int i = 0; i < Size(); ++i)
        {
            if (Str[i] == str[0])
            {
                for (int j = ArgSize >= 2 ? 1 : 0; j < ArgSize; ++j)
                {
                    if (Str[i + j] != str[j])
                        break;

                    if (j == ArgSize - 1)
                        return i;
                }
            }
        }
        return -1;
    }

    //For easier printing the array
    friend std::ostream& operator<<(std::ostream& lhs, const MyString& str)
    {
        lhs << str.Str;
        return lhs;
    }

    void Insert(const T* chr, const size_t idx)
    {
        BoundsCheck(idx);
        const size_t chrSize = strlen(chr);
        const size_t oldSize = Size() - 1;
        const size_t NewSize = chrSize + oldSize;

        // Resize(NewSize);
        EnsureCapacity(NewSize);

        //Resize idx times. Start from last index element. Move backwards and move it at the last index of newly resized array.
        //I need to static cast because once it gets 0, it wil be decremented and instead of being -1, it will be max size_t
        const int IntIdx = static_cast<int>(idx);
        const int chrSizeS = static_cast<int>(chrSize);

        for (int i = static_cast<int>(oldSize); i >= IntIdx; --i)
        {
            Str[i + chrSizeS] = Str[i];
        }

        for (size_t i = 0; i < chrSize; ++i)
        {
            Str[i + idx] = chr[i];
        }

        CurrentSize += chrSize;
        Str[CurrentSize] = '\0';
    }


    void RemoveIdx(const size_t idx)
    {
        BoundsCheck(idx);
        for (size_t i = idx + 1; i < Size(); ++i)
        {
            Str[i - 1] = Str[i];
        }

        CurrentSize--;
        Str[CurrentSize] = '\0'; // Null-terminate for safety
    }

    //Remove index: Time: Slow way to remove //Time: O(n * n * n * n)
    void RemoveAllCharNaive(const char* c)
    {
        for (size_t i = 0; i < Size(); ++i)
        {
            if (Str[i] == '\0') break;
            const int foundIndex = Search(c);
            if (foundIndex == -1) return;
            RemoveIdx(foundIndex);
        }
    }

    //O(N). Way faster than above. 
    void RemoveAllCharInPlace(char c)
    {
        size_t writeCounter = 0;
        for (size_t i = 0; i < Size(); ++i)
        {
            if (Str[i] != c)
            {
                Str[writeCounter++] = Str[i];
            }
        }

        CurrentSize = writeCounter;
        if (CurrentSize > 0 && CurrentSize <= Capacity)
        {
            Str[CurrentSize] = '\0';
        }
    }

    void RemoveAllPattern(const T* pattern)
    {
        const size_t patLen = strlen(pattern);
        if (patLen == 0) return;
        size_t writeCounter = 0;
        size_t readCounter = 0;

        while (writeCounter < CurrentSize)
        {
            bool match = true;
            for (int i = 0; i < patLen; ++i)
            {
                if (Str[i + readCounter] != pattern[i])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                readCounter += patLen;
            }
            else
            {
                Str[writeCounter++] = Str[readCounter++];
            }
        }

        CurrentSize = writeCounter;
        if (CurrentSize > 0 && CurrentSize <= Capacity)
        {
            Str[CurrentSize] = '\0'; // null-terminate
        }
    }

    [[nodiscard]] T* begin() { return Str; }
    [[nodiscard]] T* end() { return Str + CurrentSize; }

private:
    T* Str;

    size_t CurrentSize;

    size_t Capacity;

    //Size of the value and member array needs to be same.  

    void CopyStr(const T* value)
    {
        size_t counter = 0;
        while (value[counter] != '\0')
        {
            Str[counter] = value[counter];
            counter++;
        }

        //Add null terminator at th end of array
        Str[counter] = '\0';
        CurrentSize = counter;
    }

    void AssignNewValue(const T* value)
    {
        EnsureCapacity(strlen(value));
        CopyStr(value);
    }

    void EnsureCapacity(const size_t newSize)
    {
        if (newSize <= Capacity) return; //Enough space. No need to resize
        Capacity = (Capacity == 0) ? newSize : Capacity * 2;
        if (Capacity < newSize) Capacity = newSize;

        //Copy
        if (Str)
        {
            CurrentSize = newSize + 1;
            auto Temp = Str;
            Str = new T[Capacity];
            CopyStr(Temp);
            delete[] Temp;
        }
    }

    void BoundsCheck(const size_t idx) const
    {
        if (idx > CurrentSize)
            throw std::out_of_range("Insert position out of range");
    }
};

#endif
