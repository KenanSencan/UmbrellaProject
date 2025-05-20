#pragma once
#include "IDrawable.h"
#include <SFML/Graphics/RectangleShape.hpp>

class RectangleDrawable : public IDrawable
{
  public:
    RectangleDrawable(float x, float y, float width, float height, sf::Color color)
    {
        Rectangle.setPosition(x, y);
        Rectangle.setSize(sf::Vector2f(width, height));
        Rectangle.setFillColor(color);
    }

    void Draw(sf::RenderWindow& window) const override
    {
        window.draw(Rectangle);
    };

  private:
    sf::RectangleShape Rectangle;
};