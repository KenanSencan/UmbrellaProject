#pragma once
#include "IDrawable.h"
#include "IMovable.h"
#include "ICollidable.h"
#include "IUpdateable.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Abstract base class that implements common functionality
// Note: This demonstrates multiple interface inheritance
class GameObject : public IDrawable, 
                   public IMovable, 
                   public ICollidable, 
                   public IUpdateable
{
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool markedForRemoval = false;
    
public:
    GameObject(float x, float y) : position(x, y), velocity(0.f, 0.f) {}
    
    // IMovable implementation
    void Move(float deltaTime) override
    {
        position += velocity * deltaTime;
    }
    
    void SetVelocity(const sf::Vector2f& vel) override 
    {
        velocity = vel;
    }
    
    sf::Vector2f GetVelocity() const override 
    {
        return velocity;
    }
    
    sf::Vector2f GetPosition() const override
    {
        return position;
    }
    
    // IUpdateable partial implementation
    bool ShouldBeRemoved() const override
    {
        return markedForRemoval;
    }
    
    // ICollidable helper
    bool IsCollidingWith(const ICollidable* other) const override
    {
        return GetBounds().intersects(other->GetBounds());
    }
    
    // Pure virtual methods that derived classes must implement
    //NOTE: We have override because we actually overriding from the interfaces but not implementing because it's even further derived classes' responsibilities.
    void Draw(sf::RenderWindow& window) const override = 0;
    sf::FloatRect GetBounds() const override = 0;
    void OnCollision(ICollidable* other) override = 0;
    void Update(float deltaTime) override = 0;
};