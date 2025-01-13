#pragma once
#include "InputManager.h"
#include <iostream>

#include "DebugTexts.h"

namespace ETG
{
    class GameManager
    {
    private:
        //Hero class
        //UI class

    public:
        std::shared_ptr<sf::RenderWindow> window;
        sf::Clock clock;

        void Initialize()
        {
            window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML example");
            ETG::Globals::Initialize(window);
            InputManager::InitializeDebugText();
        }

        //I might delete this later on
        void LoadContent()
        {
        }

        void Update()
        {
            sf::Vector2f TestPos;

            //Update Function
            while (window->isOpen())
            {
                sf::Event event{};
                while (window->pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window->close();
                }

                //Update global elapsed time
                sf::Time elapsedTime = clock.restart();
                Globals::Update(elapsedTime);

                //Clear the screen
                window->clear(sf::Color::Black);

                //Move everything in GameManager later on. 
                InputManager::Update();
        
                //Draw using SFML
                sf::Text text("Hello, SFML!", ETG::Globals::Font, 24);

                TestPos += InputManager::GetDirection();
        
                text.setPosition(sf::Mouse::getPosition(*window).x,sf::Mouse::getPosition(*window).y);
                std::cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y  << std::endl;
        
                Draw(text);   
            }
        }

        void Draw(sf::Text& text) const
        {
            DebugText::Draw(*window);
            ETG::InputManager::Draw(*window);
            text.setFillColor(sf::Color::White);
            window->draw(text);

            //Display the frame
            window->display();


        }
    };
}
