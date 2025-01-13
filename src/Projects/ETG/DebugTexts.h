#pragma once
#include "InputManager.h"

namespace ETG
{
    class DebugText
    {
    public:
        static void Draw(sf::RenderWindow& window)
        {
            // Reset textPos to starting position each frame
            InputManager::textPos = {0.f, -20.f};

            // Draw debug information
            DrawDebugText("Direction: " + std::to_string(InputManager::direction.x) + ", " + std::to_string(InputManager::direction.y), window);

            // Mouse position
            DrawDebugText("Mouse Position: " + std::to_string(InputManager::GetMouseWorldPosition(window).x) + ", " + std::to_string(InputManager::GetMouseWorldPosition(window).y), window);

            // Hero Origin
            DrawDebugText("Hero Origin: " + std::to_string(InputManager::heroOrigin.x) + ", " + std::to_string(InputManager::heroOrigin.y), window);

            // Hero Position
            DrawDebugText("Hero Position: " + std::to_string(InputManager::heroPosition.x) + ", " + std::to_string(InputManager::heroPosition.y), window);

            // Relative mouse position
            const sf::Vector2f diff = InputManager::GetMouseWorldPosition(window) - InputManager::heroPosition;
            DrawDebugText("Relative Mouse Pos: " + std::to_string(diff.x) + ", " + std::to_string(diff.y), window);

            //sf::Mouse::getPosition(window)
            DrawDebugText("sf::Mouse::getPosition X: " + std::to_string(sf::Mouse::getPosition().x) + " Y: " + std::to_string(sf::Mouse::getPosition().y), window);

            // Moving state
            DrawDebugText("Moving: " + std::string(InputManager::IsMoving() ? "true" : "false"), window);
        }

        static void DrawDebugText(const std::string& str, sf::RenderWindow& window)
        {
            InputManager::debugText.setString(str);
            InputManager::debugText.setPosition(SetDebugTextPos(InputManager::textPos));
            window.draw(InputManager::debugText);
        }

        static sf::Vector2f& SetDebugTextPos(sf::Vector2f& pos)
        {
            pos.y += 20.f; // Increment Y position for next line
            return pos;
        }
    };
}
