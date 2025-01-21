#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <filesystem>

namespace ETG::Globals
{
    //Elapsed time in seconds
    extern float FrameTick;
    extern float ElapsedTimeSeconds;

    //Window and rendering resources
    extern  std::shared_ptr<sf::RenderWindow> Window;
    extern  sf::Font Font;
    extern  sf::Vector2u ScreenSize;

    //Function to update elapsed time
    void Update();

    //Initialize global variables
    void Initialize(const std::shared_ptr<sf::RenderWindow>& window);

    //length: Compute the magnitude of the vector using the formula sqrt(x^2 + y^2)
    // Division: Divide the vector components by the magnitude to scale it to a unit vector (length 1).
    sf::Vector2f Normalize(const sf::Vector2f& vector);

    class Renderer
    {
    public:
        static void SimpleDraw(const sf::Texture& tex, const sf::Vector2f& pos);
    };
}
