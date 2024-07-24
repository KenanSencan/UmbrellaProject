#ifndef HELPER_H
#define HELPER_H
#include <fstream>
#include <random>
#include <sstream>
#include <iostream>
#include <filesystem>

// static std::string TEXTLOC = R"(C:\Users\Selviniah\Desktop\cmake-sfml-project\src\input.txt)";
const std::string TEXTLOC = (std::filesystem::current_path().parent_path().parent_path() / "src" / "input.txt").string();
class Helper
{
    
public:
    static void WriteRandomValToPath(const int count)
    {
        std::ofstream ofs(TEXTLOC);
        if (!ofs)
        {
            std::cerr << "couldn't open" << std::endl;
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(-50000, 50000);

        for (int i = 0; i < count; ++i)
        {
            ofs << dis(gen) << " "; // Separate values with space
        }
        ofs.close();
    }

    static bool ReadFromPathAssignToArr(int* arr, const int size)
    {
        std::ifstream ifs(TEXTLOC);
        if (!ifs)
        {
            std::cerr << "Failed to open file." << std::endl;
            return false;
        }

        int i = 0;
        int j = 0;
        while (j < 3)
        {
            j++;
        }
        
        while (ifs >> arr[i] && i < size - 1)
        {
            i++;
        }
        
        // Handle the case where the last value needs to be read
        if (i < size)
        {
            ifs >> arr[i];
        }
        
        ifs.close();
        return true;
    }
};


#endif //HELPER_H
