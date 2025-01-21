#include "Globals.h"

#include <complex>
#include <filesystem>

namespace ETG::Globals
{
    
    float FrameTick = 0.0f;
    float ElapsedTimeSeconds = 0.0f;
    std::shared_ptr<sf::RenderWindow> Window = nullptr;
    sf::Font Font;
    sf::Vector2u ScreenSize;
    sf::Clock clock;
    sf::Clock tickClock;

    void Update()
    {
        //Counter starts the beginning in runtime and never stops
        ElapsedTimeSeconds = clock.getElapsedTime().asSeconds();

        //Calculate tick. In 60fps it should be: 0.016
        //It was hard for me to understand. restart sets 0 and returns total elapsed time since the last time.
        //Every loop call this will be called. So in every call it will return the total time passed in seconds.   
        const sf::Time elapsedTime = tickClock.restart();
        FrameTick = elapsedTime.asSeconds();
    }

    void Initialize(const std::shared_ptr<sf::RenderWindow>& window)
    {
        Window = window;
        ScreenSize = window->getSize();

        //Load font
        if (!Font.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string()))
        {
            throw std::runtime_error("Failed to load font");
        }
    }

    sf::Vector2f Normalize(const sf::Vector2f& vector)
    {
        const float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (length != 0)
            return vector / length;
        return {0.f, 0.f}; // Return zero vector if the length is 0
    }

    void Renderer::SimpleDraw(const sf::Texture& tex, const sf::Vector2f& pos)
    {
        sf::Sprite frame;
        frame.setTexture(tex);
        frame.setScale(25.f, 25.f); // Scale to make it visually larger
        frame.setPosition(pos); // Position it at the specified location

        Globals::Window->draw(frame);
    }

   
}
