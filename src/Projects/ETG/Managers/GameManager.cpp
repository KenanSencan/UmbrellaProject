#include "GameManager.h"

void ETG::GameManager::Initialize()
{
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML example");
    Window->requestFocus();
    // Window->setFramerateLimit(170);
    // Window->
    Globals::Initialize(Window);
    InputManager::InitializeDebugText();
    UI.Initialize();

    //Animation preparation: 
    const auto IdleFrontHandAnimation = Animation::CreateSpriteSheet("Resources/Player/Idle/Back","rogue_idle_back_hand_left_00","PNG", 0.12f);
    const auto IdleRightAnimation = Animation::CreateSpriteSheet("Resources/Player/Idle/Front","rogue_idle_front_hand_left_00","PNG", 0.12f);

    //Now make animation
    AnimationManager.AddAnimation<std::string>("Idle_FrontHand", IdleFrontHandAnimation);
    AnimationManager.AddAnimation<std::string>("Idle_Right", IdleRightAnimation);
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

        AnimationManager.Update<std::string>("Idle_Right");

        //Example test to change animation easily with AnimationManager
        if (Globals::ElapsedTimeSeconds >= 5.f)
        {
            AnimationManager.Update<std::string>("Idle_FrontHand");
        }

        std::cout << FrameTick << std::endl;
        

        //Lastly draw everything. I realized the FPS 1.5x got slower after this Draw 
        Draw();
    }
}

void ETG::GameManager::Draw()
{
    DebugText::Draw(*Window);
    UI.Draw();

    AnimationManager.Draw({250.f,250.f}, 0);

    //Display the frame
    Window->display();
}
