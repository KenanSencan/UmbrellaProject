#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

// Interface for objects that can be drawn
class IDrawable
{
public:
    virtual ~IDrawable() = default;
    
    // Pure virtual method that must be implemented by derived classes
    virtual void Draw(sf::RenderWindow& window) const = 0;
};