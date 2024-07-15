#ifndef HELPER_H
#define HELPER_H
#include <fstream>
#include <random>
#include <sstream>
#include <iostream>

static std::string TEXTLOC = R"(C:\Users\Selviniah\Desktop\cmake-sfml-project\src\input.txt)";

class Helper
{
    
public:
    static void WriteRandomValToPath(const std::string& filePath, const int count)
    {
        std::ofstream ofs(filePath);
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

    static bool ReadFromPathAssignToArr(int* arr, const std::string& path, const int size)
    {
        std::ifstream ifs(path);
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
