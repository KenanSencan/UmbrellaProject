#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class Object
{
public:
    static sf::CircleShape CreateCircleShape(const sf::Vector2f playerPos, const float radius, const sf::Color color)
    {
        sf::CircleShape circle(radius);
        circle.setFillColor(color);
        circle.setOrigin(radius, radius);
        circle.setPosition(playerPos);
        return circle;
    }

    static sf::Text CreateText(const std::string& text, const std::string& fontPath, const unsigned int size, const sf::Color color)
    {
        static sf::Font font;
        if (!font.loadFromFile(fontPath))
            throw std::runtime_error("Failed to load font from " + fontPath);
            
        sf::Text textObj;
        textObj.setFont(font);
        textObj.setString(text);
        textObj.setCharacterSize(size);
        textObj.setFillColor(color);
        return textObj;
    }
};
