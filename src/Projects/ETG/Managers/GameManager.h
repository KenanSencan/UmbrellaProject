#pragma once
#include "InputManager.h"
#include <iostream>

#include "DebugTexts.h"
#include "../Animation/Animation.h"
#include "../Animation/AnimationManager.h"
#include "../UI/UserInterface.h"

namespace ETG
{
    using namespace Globals;

    class GameManager
    {
    private:
        //Hero class
        //UI class

    public:
        UserInterface UI;

        void Initialize();

        //I might delete this later on 
        void LoadContent();
        void Update();
        void Draw();

        // sf::Texture IdleTex;
        AnimationManager AnimationManager;
    };
}
