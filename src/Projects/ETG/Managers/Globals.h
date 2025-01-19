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
}
