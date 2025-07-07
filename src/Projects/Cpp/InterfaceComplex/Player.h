#pragma once
#include "GameObject.h"

class Player : public GameObject
{
  private:
    sf::RectangleShape shape;
    float health = 100.f;

  public:
    Player(const float x, const float y) : GameObject(x, y)
    {
        shape.setSize({50.f, 50.f});
        shape.setFillColor(sf::Color::Green);
        shape.setOrigin(25.f, 25.f);
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
        // Player takes damage from enemies
        health -= 10.f;
        if (health <= 0)
        {
            markedForRemoval = true;
        }
    }

    void Update(const float deltaTime) override
    {
        Move(deltaTime);
        shape.setPosition(position);

        // Change color based on health
        int green = static_cast<int>(255 * (health / 100.f));
        shape.setFillColor(sf::Color(255 - green, green, 0));
    }

    float GetHealth() const
    {
        return health;
    }
};