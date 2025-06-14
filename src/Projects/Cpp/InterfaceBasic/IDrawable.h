#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

// Simple interface for objects that can be drawn
class IDrawable
{
public:
    // Virtual destructor is important for interfaces
    virtual ~IDrawable() = default;
    
    // Pure virtual method that must be implemented by derived classes
    virtual void Draw(sf::RenderWindow& window) const = 0;
};