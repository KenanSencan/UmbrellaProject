#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <filesystem>

namespace ETG::Globals
{
    //Elapsed time in seconds
    static float TotalSeconds;

    //Window and rendering resources
    static std::shared_ptr<sf::RenderWindow> Window;
    static sf::Font Font;
    static sf::Vector2u ScreenSize;
    // const std::string FontLoc = (std::filesystem::current_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string(); //inlined for performance

    //Function to update elapsed time
    static void Update(const sf::Time& elapsedTime)
    {
        TotalSeconds = elapsedTime.asSeconds();
    }

    //Initialize global variables
    static void Initialize(const std::shared_ptr<sf::RenderWindow>& window)
    {
        Window = window;
        ScreenSize = window->getSize();

        //Load font
        if (!Font.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string()))
        {
            throw std::runtime_error("Falied to load font");
        }
    }
}
