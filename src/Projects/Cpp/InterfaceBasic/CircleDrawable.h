#pragma once
#include "IDrawable.h"
#include <SFML/Graphics/CircleShape.hpp>

class CircleDrawable : public IDrawable
{
  public:
    CircleDrawable(float x, float y, float radius, sf::Color color)
    {
        Circle.setPosition(x, y);
        Circle.setRadius(radius);
        Circle.setFillColor(color);
    }

    void Draw(sf::RenderWindow& window) const override
    {
        window.draw(Circle);
    };

    void SetPosition(float x, float y)
    {
        Circle.setPosition(x,y);
    }

    void SetColor(sf::Color color)
    {
        Circle.setFillColor(color);
    }

  private:
    sf::CircleShape Circle;
};