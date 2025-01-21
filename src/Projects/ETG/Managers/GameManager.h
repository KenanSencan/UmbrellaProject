#pragma once

#include "../Animation/AnimationManager.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"

namespace ETG
{
    using namespace Globals;

    class GameManager
    {
    private:
        //Hero class
        Hero Hero{sf::Vector2f(150,150)};
        //UI class

    public:
        UserInterface UI;

        void Initialize();

        //I might delete this later on 
        void LoadContent();
        void Update();
        void Draw();
    };
}
