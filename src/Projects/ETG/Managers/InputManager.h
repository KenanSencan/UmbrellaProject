#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Globals.h"
#include <iostream>

namespace ETG
{
    class InputManager
    {
    private:
        inline static float MinScaleSpeed = 0.001f;
        inline static float MaxScaleSpeed = 0.008f;

        inline static float ZoomFactor = 0.01f;
        inline static float MoveFactor = 5.0f;
        inline static float MinMoveSpeed = 0.25f;
        inline static float MaxMoveSpeed = 3.f;

    public:
        inline static sf::Vector2f direction{};
        inline static sf::Vector2f heroPosition{};
        inline static sf::Vector2f heroOrigin{};
        
        inline static float ZoomScale{};

        // For drawing debug text
        inline static sf::Text debugText;
        inline static sf::Vector2f textPos{0.f, -20.f}; // Start within the window

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
            ZoomScale = GetZoomScale(Globals::MainView, *Globals::Window);

            const float adjustedZoomFactor = AdjustZoomFactor();
            const float adjustedMoveFactor = AdjustMoveFactor();

            //Calculate directions. It can only be -1 or 1 
            direction = sf::Vector2f(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y++;

            //Camera Effects:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) Globals::MainView.zoom(1.0f - adjustedZoomFactor);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) Globals::MainView.zoom(1.0f + adjustedZoomFactor);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) Globals::MainView.move(0, -adjustedMoveFactor);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) Globals::MainView.move(0, +adjustedMoveFactor);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) Globals::MainView.move(+adjustedMoveFactor, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) Globals::MainView.move(-adjustedMoveFactor, 0);
        }

        static float GetMouseAngleRelativeToHero(const sf::RenderWindow& window)
        {
            const sf::Vector2f diff = GetMouseWorldPosition(window) - heroPosition;
            return std::atan2(diff.y, diff.x);
        }

        static void SetHeroPosition(const sf::Vector2f& pos) { heroPosition = pos; }

        static void InitializeDebugText()
        {
            debugText.setFont(Globals::Font);
            debugText.setCharacterSize(16);
            debugText.setFillColor(sf::Color::Yellow);
        }

        static float GetZoomScale(const sf::View& currentView, const sf::RenderWindow& window)
        {
            //Default view size
            sf::Vector2f defSize = window.getDefaultView().getSize();

            //Get current view size
            sf::Vector2f currSize = currentView.getSize();

            return defSize.x / currSize.x;
        }

        static float AdjustMoveFactor()
        {
            const float scaleRatio = 10000.f / ZoomScale;
            float adjustedMoveFactor = ZoomFactor * std::sqrt(scaleRatio);
            
            adjustedMoveFactor = std::clamp(adjustedMoveFactor, MinMoveSpeed, MaxMoveSpeed);
            return adjustedMoveFactor;
        }

        static float AdjustZoomFactor()
        {
            const float scaleRatio = 0.1f / ZoomScale;
            float adjustedZoomFactor = ZoomFactor * std::sqrt(scaleRatio);
            adjustedZoomFactor = std::clamp(adjustedZoomFactor, MinScaleSpeed, MaxScaleSpeed);
            return adjustedZoomFactor;
        }
    };
}
