#include "Managers/GameManager.h"

int main()
{
    ETG::GameManager GM;
    GM.Initialize();

    while (ETG::GameManager::IsRunning())
    {
        GM.Update();
        GM.Draw();        
    }
    
    return 0;
}
