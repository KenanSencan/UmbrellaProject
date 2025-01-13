#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include "Globals.h"

namespace ETG
{
    class InputManager
    {
    public:
        inline static sf::Vector2f direction{};
        inline static sf::Vector2f heroPosition{};
        inline static sf::Vector2f heroOrigin{};

        // For drawing debug text
        inline static sf::Text debugText;
        inline static sf::Vector2f textPos{0.f, -20.f}; // Start within the window

    public:
        static sf::Vector2f GetDirection() { return direction; }
        static bool IsMoving() { return direction != sf::Vector2f(0.f, 0.f); }

        static sf::Vector2f GetMouseWorldPosition(const sf::RenderWindow& window)
        {
            const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            return window.mapPixelToCoords(pixelPos);
        }

        static void Update()
        {
            heroOrigin += heroPosition;
            direction = sf::Vector2f(0.f, 0.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y++;
        }

        static float GetMouseAngleRelativeToHero(const sf::RenderWindow& window)
        {
            sf::Vector2f diff = GetMouseWorldPosition(window) - heroPosition;
            return std::atan2(diff.y, diff.x);
        }

        static void SetHeroPosition(const sf::Vector2f& pos) { heroPosition = pos; }

        static void InitializeDebugText()
        {
            debugText.setFont(Globals::Font);
            debugText.setCharacterSize(16);
            debugText.setFillColor(sf::Color::Yellow);
        }

        //TODO: Later on create a new class for only DebugTexts. It's pointless to have so many Debug texts in InputManager
        static void Draw(sf::RenderWindow& window)
        {
        }
    };
}
