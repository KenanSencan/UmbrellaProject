#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>

// Helper function to find the src directory and construct path to the font
inline std::string findFontPath()
{
    std::filesystem::path currentPath = std::filesystem::current_path();

    // Go up until we find the "src" directory or reach the root
    while (currentPath.has_parent_path() && currentPath.filename() != "src")
    {
        currentPath = currentPath.parent_path();
    }

    // Check if we found the src directory
    if (currentPath.filename() == "src")
    {
        return (currentPath / "Projects" / "Math" / "HelperClass" / "SegoeUI.ttf").string();
    }

    std::cerr << "Warning: Could not locate src directory" << std::endl;
    return "";
}

#define TEXT_POSITION_INIT float g_TextPosition = 0.0f;
#define TEXT_POSITION_RESET g_TextPosition = 0.0f;

// Create window and text without starting the game loop
#define MAIN_TEMPLATE_GAME_START                                                                                                                                                   \
    int main()                                                                                                                                                                     \
    {                                                                                                                                                                              \
        sf::RenderWindow window(sf::VideoMode(1920, 1080), "Dot Product - Front/Behind Example");                                                                                  \
        window.setFramerateLimit(170);                                                                                                                                             \
        TEXT_POSITION_INIT                                                                                                                                                         \
        sf::Clock DeltaClock;                                                                                                                                                      \
        sf::Text infoText = Object::CreateText("", findFontPath(), 16, sf::Color::White);

// Rest of the file remains unchanged
#define GAME_LOOP_START                                                                                                                                                            \
    while (window.isOpen())                                                                                                                                                        \
    {                                                                                                                                                                              \
        window.clear(sf::Color(30, 30, 30));                                                                                                                                       \
        sf::Event evt{};                                                                                                                                                           \
        while (window.pollEvent(evt))                                                                                                                                              \
            if (evt.type == sf::Event::Closed)                                                                                                                                     \
                window.close();                                                                                                                                                    \
                                                                                                                                                                                   \
        TEXT_POSITION_RESET                                                                                                                                                        \
        sf::Time dt = DeltaClock.restart();                                                                                                                                        \
        float deltaTime = dt.asSeconds();

#define MAIN_TEMPLATE_GAME_END                                                                                                                                                     \
    window.display();                                                                                                                                                              \
    }                                                                                                                                                                              \
    return 0;                                                                                                                                                                      \
    }

#define LEGACY_MAIN_TEMPLATE_GAME_START                                                                                                                                            \
    MAIN_TEMPLATE_GAME_START                                                                                                                                                       \
    GAME_LOOP_START

#define DISPLAY_TEXT(message)                                                                                                                                                      \
    infoText.setPosition(0.f, g_TextPosition);                                                                                                                                     \
    infoText.setString(message);                                                                                                                                                   \
    window.draw(infoText);                                                                                                                                                         \
    g_TextPosition += 20.f;