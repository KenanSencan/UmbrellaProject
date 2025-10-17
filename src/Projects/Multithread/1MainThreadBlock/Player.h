#pragma once
#include <SFML/Graphics.hpp>

// Player class - simple rectangle that moves with WASD
class Player
{
private:
    sf::RectangleShape shape;
    float speed = 300.0f;

public:
    Player(sf::Vector2f position, sf::Vector2f size)
    {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);
    }

    void Update(float deltaTime)
    {
        sf::Vector2f movement(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += speed * deltaTime;

        shape.move(movement);
    }

    void Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};