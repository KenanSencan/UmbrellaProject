#pragma once
#include "InputManager.h"
#include <iostream>

#include "DebugTexts.h"
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
        sf::Clock clock;
        UserInterface UI;

        void Initialize()
        {
            Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML example");
            Window->requestFocus();
            Globals::Initialize(Window);
            InputManager::InitializeDebugText();
            UI.Initialize();
        }

        //I might delete this later on
        void LoadContent()
        {
        }

        void Update()
        {
            //Update Function
            while (Window->isOpen())
            {
                sf::Event event{};
                while (Window->pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        Window->close();
                }

                //Update global elapsed time
                sf::Time elapsedTime = clock.restart();
                Globals::Update(elapsedTime);

                //Clear the screen
                Window->clear(sf::Color::Black);

                //Move everything in GameManager later on. 
                InputManager::Update();

                Draw();
            }
        }

        void Draw()
        {
            DebugText::Draw(*Window);
            UI.Draw();

            //Display the frame
            Window->display();
        }
    };
}
