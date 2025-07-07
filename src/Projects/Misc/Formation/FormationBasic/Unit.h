// --- Unit Struct ---
#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include "../../../Math/HelperClass/HelperCollection.h"

class Unit
{
public:
    sf::CircleShape body{};
    sf::VertexArray forwardLine{};
    sf::Vector2f position{};
    sf::Vector2f forwardVector{};
    float currentAngleRad{};

    Unit() = default;

    Unit(const sf::Vector2f startPos, const sf::Color color, float unitRadius = 15.f)
        : position(startPos), forwardVector(1.f, 0.f), currentAngleRad(0.f)
    {
        body.setRadius(unitRadius);
        body.setFillColor(color);
        body.setOrigin(unitRadius, unitRadius);
        body.setPosition(position);

        forwardLine.setPrimitiveType(sf::Lines);
        forwardLine.resize(2);
        forwardLine[0].color = sf::Color::Red;
        forwardLine[1].color = sf::Color::Red;
    };

    void SetPosition(const sf::Vector2f& newPos)
    {
        position = newPos;
        body.setPosition(position);
        UpdateForwardLine();
    }

    void SetForwardVector(const sf::Vector2f& newForward)
    {
        forwardVector = Math::Normalize(newForward);

        // A single 2D vector’s angle is always measured with positive x-axis (1, 0)
        currentAngleRad = Math::getAngle(forwardVector);
        UpdateForwardLine();
    }

    void UpdateForwardLine(float UnitRadius = 15.f)
    {
        forwardLine[0].position = position;
        forwardLine[1].position = position + forwardVector * (UnitRadius * 1.5f);
        forwardLine[0].color = sf::Color::White;
        forwardLine[1].color = sf::Color::White;
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(body);
        window.draw(forwardLine);
    }
};