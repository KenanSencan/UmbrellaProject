#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <filesystem>

namespace ETG::Globals
{
    //Elapsed time in seconds
    extern float TotalSeconds;

    //Window and rendering resources
    extern  std::shared_ptr<sf::RenderWindow> Window;
    extern  sf::Font Font;
    extern  sf::Vector2u ScreenSize;
    // const std::string FontLoc = (std::filesystem::current_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string(); //inlined for performance

    //Function to update elapsed time
    void Update(const sf::Time& elapsedTime);

    //Initialize global variables
    void Initialize(const std::shared_ptr<sf::RenderWindow>& window);
}
