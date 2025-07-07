#pragma once
#include <SFML/Graphics/Rect.hpp>

// Interface for objects that can collide
class ICollidable
{
public:
    virtual ~ICollidable() = default;
    
    virtual sf::FloatRect GetBounds() const = 0;
    virtual void OnCollision(ICollidable* other) = 0;
    virtual bool IsCollidingWith(const ICollidable* other) const = 0;
};