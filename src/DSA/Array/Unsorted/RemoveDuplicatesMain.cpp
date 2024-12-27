#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <set>

void RemoveDuplicateUnorderedMine(std::vector<int>& vec)
{
    std::unordered_set<int> seen;
    std::vector<int> IndexToRemove;
    for (int i = 0; i < vec.size(); i++)
    {
        if (seen.contains(vec[i]))
        {
            IndexToRemove.push_back(i);
            continue; // Duplicate found
        }
        seen.insert(vec[i]);
    }

    int shifter = 0;
    for (int i = (int)IndexToRemove.size() - 1; i >= 0; i--)
    {
        vec.erase((vec.begin() + IndexToRemove[i]));
    }
}

//Retains first occurrences in original order.
void RemoveDuplicates_UnorderedSet(std::vector<int>& vec)
{
    std::unordered_set<int> set;
    int WriteCounter = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        if (!set.contains(vec[i]))
        {
            set.insert(vec[i]);
            vec[WriteCounter] = vec[i];
            WriteCounter++;
        }
    }
    vec.resize(WriteCounter);
}

//I didn't understand any shit on how this works. I need to study how iterators works. 
void RemoveDuplicatesSortUnique(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end());
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}

//set will ignore adding duplicates by default 
void RemoveDuplicatesSet(std::vector<int>& vec)
{
    std::set<int> uniqueSet(vec.begin(), vec.end());
    vec.assign(uniqueSet.begin(), uniqueSet.end());
}

void RemoveDuplicates_SortManual(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end()); // Sort
    // Manually remove consecutive duplicates:
    auto it = vec.begin();
    while (it != vec.end())
    {
        auto nextIt = std::next(it);
        if (nextIt != vec.end() && *it == *nextIt)
        {
            vec.erase(nextIt);
        }
        else
        {
            ++it;
        }
    }
}

void RemoveDuplicates_Copy(std::vector<int>& vec)
{
    std::unordered_set<int> seen;
    std::vector<int> result;
    result.reserve(vec.size());

    for (int value : vec)
    {
        if (seen.insert(value).second)
        {
            // If insert was successful (i.e., not a duplicate)
            result.push_back(value);
        }
    }
    vec.swap(result); // Replace original with filtered
}


int main()
{
    std::vector<int> vector{1, 2, 3, 1, 2, 3, 4, 5, 6, 4, 3, 2, 1};
    std::cout << "Before duplication removal" << std::endl;
    std::for_each(vector.begin(), vector.end(), [](const int i)
    {
        std::cout << i << std::endl;
    });

    RemoveDuplicatesSortUnique(vector);

    std::cout << "After" << std::endl;
    std::for_each(vector.begin(), vector.end(), [](const int i)
    {
        std::cout << i << std::endl;
    });
    return 0;
}
