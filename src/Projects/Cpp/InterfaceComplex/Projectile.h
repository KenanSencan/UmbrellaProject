#pragma once
#include "GameObject.h"

// Example of a class that only implements some interfaces
// This shows Interface Segregation Principle
class Projectile : public IDrawable, public IMovable, public IUpdateable
{
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime = 2.f;
    bool shouldRemove = false;
    
public:
    Projectile(float x, float y, const sf::Vector2f& vel)
        : position(x, y), velocity(vel)
    {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(5.f, 5.f);
    }
    
    // IDrawable
    void Draw(sf::RenderWindow& window) const override
    {
        window.draw(shape);
    }
    
    // IMovable
    void Move(float deltaTime) override
    {
        position += velocity * deltaTime;
    }
    
    void SetVelocity(const sf::Vector2f& vel) override { velocity = vel; }
    sf::Vector2f GetVelocity() const override { return velocity; }
    sf::Vector2f GetPosition() const override { return position; }
    
    // IUpdateable
    void Update(float deltaTime) override
    {
        Move(deltaTime);
        shape.setPosition(position);
        
        lifetime -= deltaTime;
        if (lifetime <= 0 || position.x < 0 || position.x > 1920 || 
            position.y < 0 || position.y > 1080)
        {
            shouldRemove = true;
        }
    }
    
    bool ShouldBeRemoved() const override { return shouldRemove; }
};