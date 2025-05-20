// --- Unit Struct ---
#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include "../../../Math/HelperClass/HelperCollection.h"

class Unit
{
  public:
    sf::CircleShape body;
    sf::VertexArray forwardLine; // To show direction
    sf::Vector2f position;
    sf::Vector2f forwardVector;
    float currentAngleRad; // Radians

    Unit(const sf::Vector2f startPos, const sf::Color color, bool isLeader = false, float unitRadius = 15.f)
        : position(startPos), forwardVector(1.f, 0.f), // Default facing right
          currentAngleRad(0.f)
    {
        body.setRadius(unitRadius);
        body.setOrigin(unitRadius, unitRadius);
        body.setFillColor(color);
        body.setPosition(position);

        forwardLine.setPrimitiveType(sf::Lines);
        forwardLine.resize(2);
        updateForwardLine();
    }

    void setPosition(const sf::Vector2f& newPos)
    {
        position = newPos;
        body.setPosition(position);
        updateForwardLine();
    }

    void setForwardVector(const sf::Vector2f& newForward)
    {
        forwardVector = Math::Normalize(newForward);

        // A single 2D vector’s angle is always measured relative to the origin’s positive x-axis (1, 0)
        currentAngleRad = Math::getAngle(forwardVector);
        float TestAtan2 = Math::RadianToDegree(Math::getAngle({4.02, 5.83}));
        updateForwardLine();
    }

    // Just visualisation
    void updateForwardLine(float unitRadius = 15.f)
    {
        forwardLine[0].position = position;
        forwardLine[1].position = position + forwardVector * (unitRadius * 1.5f);
        forwardLine[0].color = sf::Color::White;
        forwardLine[1].color = sf::Color::White;
    }
    void rotate(float angleOffsetRad)
    {
        currentAngleRad += angleOffsetRad;
        // Normalize angle to keep it within -PI to PI or 0 to 2PI if desired (optional)
        forwardVector = Math::vectorFromAngle(currentAngleRad);
        updateForwardLine();
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(body);
        window.draw(forwardLine);
    }
};