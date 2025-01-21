#include "GameManager.h"
#include "DebugTexts.h"
#include "InputManager.h"

void ETG::GameManager::Initialize()
{
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML example");
    Window->requestFocus();
    // Window->setFramerateLimit(170);
    Globals::Initialize(Window);
    InputManager::InitializeDebugText();
    UI.Initialize();
}

void ETG::GameManager::LoadContent()
{
    //
}

void ETG::GameManager::Update()
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

        Globals::Update();

        //Clear the screen
        Window->clear(sf::Color::Black);

        //process input
        InputManager::Update();

        Hero.Update();
        
        //Lastly draw everything. I realized the FPS 1.5x got slower after this Draw
        Draw();
    }
}

void ETG::GameManager::Draw()
{
    DebugText::Draw(*Window);
    UI.Draw();
    Hero.Draw();
    
    //Display the frame
    Window->display();
}
