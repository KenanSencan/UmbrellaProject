#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
private:
    sf::CircleShape shape;
    float lifetime = 5.f;
    
public:
    Enemy(float x, float y) 
        : GameObject(x, y)
    {
        shape.setRadius(20.f);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(20.f, 20.f);
        
        // Random velocity
        velocity = sf::Vector2f(
            (rand() % 200 - 100) / 100.f * 150.f,
            (rand() % 200 - 100) / 100.f * 150.f
        );
    }
    
    void Draw(sf::RenderWindow& window) const override
    {
        window.draw(shape);
    }
    
    sf::FloatRect GetBounds() const override
    {
        return shape.getGlobalBounds();
    }
    
    void OnCollision(ICollidable* other) override
    {
        // Enemies bounce off each other
        velocity = -velocity;
    }
    
    void Update(float deltaTime) override
    {
        Move(deltaTime);
        shape.setPosition(position);
        
        lifetime -= deltaTime;
        if (lifetime <= 0)
        {
            markedForRemoval = true;
        }
        
        // Bounce off screen edges
        if (position.x < 20 || position.x > 1900) velocity.x = -velocity.x;
        if (position.y < 20 || position.y > 1060) velocity.y = -velocity.y;
    }
};