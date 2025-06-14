// --- Unit Struct ---
#pragma once
#include "../../../Math/HelperClass/HelperCollection.h"
#include <SFML/Graphics/CircleShape.hpp>

class Unit
{
  public:
    sf::CircleShape body{};
    sf::VertexArray forwardLine{};
    sf::Vector2f position{};
    sf::Vector2f forwardVector{};
    float currentAngleRad{};
    sf::Vector2f velocity; // Added for prediction
    float unitRadius;

    // Added for formation recovery
    sf::Vector2f idealPosition;
    float formationDeviation;

    Unit() = default;

    Unit(const sf::Vector2f startPos, const sf::Color color, const float unitRadius = 15.f)
        : position(startPos), forwardVector(1.f, 0.f), currentAngleRad(0.f), velocity(0.f, 0.f), unitRadius(unitRadius), idealPosition(startPos), formationDeviation(0.f)
    {
        body.setRadius(unitRadius);
        body.setFillColor(color);
        body.setOrigin(unitRadius, unitRadius);
        body.setPosition(position);

        forwardLine.setPrimitiveType(sf::Lines);
        forwardLine.resize(2);
        UpdateForwardLine();
    }

    void SetVelocity(const sf::Vector2f& newVelocity)
    {
        velocity = newVelocity;
    }

    void SetPosition(const sf::Vector2f& newPos)
    {
        position = newPos;
        body.setPosition(position);
        UpdateForwardLine();
    }

    void UpdatePosition(float deltaTime)
    {
        position += velocity * deltaTime;
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

    void Rotate(float angleOffsetRad)
    {
        currentAngleRad += angleOffsetRad;
        forwardVector = Math::vectorFromAngle(currentAngleRad);
        UpdateForwardLine();
    }

    // calculates a repulsion force to prevent units from colliding
    sf::Vector2f GetAvoidanceVector(const Unit& other, float maxAvoidDistance) const
    {
        const sf::Vector2f direction = position - other.position;
        float distance = Math::VectorLength(direction);

        //Calculate minimum distance needed between centers
        float minSeparation = unitRadius + other.unitRadius;

        //if already at safe distance or further, no avoidance needed
        if (distance >= maxAvoidDistance) return {0.f, 0.f};

        // normalize direction
        const sf::Vector2f normalizedDir = Math::Normalize(direction);

        const float repulsionStrength = Math::smoothstep(maxAvoidDistance, minSeparation, distance);
        return normalizedDir * repulsionStrength;
    }

    //timeHorizon represents the maximum time window (in seconds) that the function looks ahead to predict collisions.
    //It's like saying "I only care about potential collisions that might happen within the next X seconds."
    //If the closest approach between units would occur after this time window, the function ignores it.
    //If units are closer than minDistance, they are considered to be colliding.
    bool WillCollideWith(const Unit& other, const float timeHorizon, const float minAllowedDistance) const
    {
        // Current Positions
        const sf::Vector2f OtherToSelfPos = other.position - position;
        const float currDistance = Math::VectorLength(OtherToSelfPos);

        //If already colliding, return true
        if (currDistance < minAllowedDistance) return true;

        // Relative velocity
        const sf::Vector2f OtherToSelfVel = other.velocity - velocity;

        // If moving apart, no collision will occur NOTE: Study how this works exactly. Study somewhere where I explained non normalzied dot products are useful
        const float dot = Math::Dot(OtherToSelfPos, OtherToSelfVel);
        if (dot <= 0) return false;

        //Project relative velocity onto relative position to find closest approach
        float relPosSquared = Math::Dot(OtherToSelfPos, OtherToSelfPos);
        float relVelSquared = Math::Dot(OtherToSelfVel, OtherToSelfVel);

        //Calculate the time to closest approach
        float timeToClosest = std::max(0.0f, dot / relVelSquared);

        //If closest approach is beyond our time horizon, ignore
        if (timeToClosest > timeHorizon) return false; //NOTE: This part looks important. Study how all squared stuff works 

        //Calculate position at closest approach
        sf::Vector2f closestPos = OtherToSelfPos + OtherToSelfVel * timeToClosest; //NOTE: This looks quite quite advanced
        float closestDistance = Math::VectorLength(closestPos);

        //Return true if we'll be closer than minimum distance
        return closestDistance < minAllowedDistance;
    }

    void SetIdealPosition(const sf::Vector2f& newIdealPos)
    {
        idealPosition = newIdealPos;
        formationDeviation = Math::VectorLength(position - idealPosition);
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(body);
        window.draw(forwardLine);
    }
};