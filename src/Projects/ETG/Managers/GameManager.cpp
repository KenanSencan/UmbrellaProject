#include "GameManager.h"

void ETG::GameManager::Initialize()
{
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML example");
    Window->requestFocus();
    Globals::Initialize(Window);
    InputManager::InitializeDebugText();
    UI.Initialize();

    //Animation: 
    const auto rogueFirePath = (std::filesystem::current_path().parent_path() / "Resources" / "Guns" / "RogueSpecial_Fire.png").string();
    if (!rogueFireTexture.loadFromFile(rogueFirePath))
    {
        throw std::runtime_error("Failed to load: " + rogueFirePath);
    }

    testAnimation = Animation(rogueFireTexture, 0.1f, 5, 1);

}

void ETG::GameManager::LoadContent()
{
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

        //Update global elapsed time
        sf::Time elapsedTime = clock.restart();
        Globals::Update(elapsedTime);

        //Clear the screen
        Window->clear(sf::Color::Black);

        //process input
        InputManager::Update();

        testAnimation.Update();

        //Lastly draw everything
        Draw();
    }
}

void ETG::GameManager::Draw()
{
    DebugText::Draw(*Window);
    UI.Draw();

    testAnimation.Draw(rogueFireTexture,sf::Vector2f{350.f,350.f},sf::Color::White,0.f,sf::Vector2f{0.f,0.f},35.f);
    // testAnimation.Draw(sf::Vector2f{350.f,350.f}, 0);

    //Display the frame
    Window->display();
}
