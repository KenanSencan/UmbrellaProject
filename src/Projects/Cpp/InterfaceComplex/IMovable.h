#pragma once
#include <SFML/System/Vector2.hpp>

// Interface for objects that can move
class IMovable
{
public:
    virtual ~IMovable() = default;
    
    virtual void Move(float deltaTime) = 0;
    virtual void SetVelocity(const sf::Vector2f& velocity) = 0;
    virtual sf::Vector2f GetVelocity() const = 0;
    virtual sf::Vector2f GetPosition() const = 0;
};