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
    sf::Vector2f velocity;       // Added for prediction
    float currentAngleRad;       // Radians
    float unitRadius;            // Track radius for collision calculations
    
    // Added for formation recovery
    sf::Vector2f idealPosition;
    float formationDeviation;

    Unit(const sf::Vector2f startPos, const sf::Color color, bool isLeader = false, float unitRadius = 15.f)
        : position(startPos), forwardVector(1.f, 0.f), // Default facing right
          currentAngleRad(0.f), velocity(0.f, 0.f), unitRadius(unitRadius),
          idealPosition(startPos), formationDeviation(0.f)
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

    void setVelocity(const sf::Vector2f& newVelocity)
    {
        velocity = newVelocity;
    }

    void updatePosition(float deltaTime)
    {
        position += velocity * deltaTime;
        body.setPosition(position);
        updateForwardLine();
    }

    void setForwardVector(const sf::Vector2f& newForward)
    {
        forwardVector = Math::Normalize(newForward);
        currentAngleRad = Math::getAngle(forwardVector);
        updateForwardLine();
    }

    // Just visualisation
    void updateForwardLine(float lineLength = 1.5f)
    {
        forwardLine[0].position = position;
        forwardLine[1].position = position + forwardVector * (unitRadius * lineLength);
        forwardLine[0].color = sf::Color::White;
        forwardLine[1].color = sf::Color::White;
    }

    void rotate(float angleOffsetRad)
    {
        currentAngleRad += angleOffsetRad;
        forwardVector = Math::vectorFromAngle(currentAngleRad);
        updateForwardLine();
    }

    // Smooth avoidance with falloff
    sf::Vector2f getAvoidanceVector(const Unit& other, float maxAvoidDistance) const
    {
        sf::Vector2f direction = position - other.position;
        float distance = Math::VectorLength(direction);
        
        // Calculate the minimum distance needed between centers
        float minSeparation = unitRadius + other.unitRadius;
        
        // If already at safe distance or further, no avoidance needed
        if (distance >= maxAvoidDistance) return sf::Vector2f(0.f, 0.f);
        
        // Normalize direction
        sf::Vector2f normalizedDir = Math::Normalize(direction);
        
        // Calculate smooth falloff (stronger as we get closer)
        // This creates a smooth curve from 0 (at maxAvoidDistance) to 1 (at minSeparation)
        float t = 1.0f - ((distance - minSeparation) / (maxAvoidDistance - minSeparation));
        t = std::max(0.0f, std::min(1.0f, t)); // Clamp between 0 and 1
        
        // Apply curve for smoother falloff (cubic falloff feels natural)
        float repulsionStrength = t * t * (3.0f - 2.0f * t);
        
        return normalizedDir * repulsionStrength;
    }
    
    // Predictive collision detection
    bool willCollideWith(const Unit& other, float timeHorizon, float minDistance) const
    {
        // Current positions
        sf::Vector2f relativePos = other.position - position;
        float currentDistance = Math::VectorLength(relativePos);
        
        // If already colliding, return true
        if (currentDistance < minDistance) return true;
        
        // Relative velocity
        sf::Vector2f relativeVel = other.velocity - velocity;
        
        // If moving apart, no collision will occur
        float dot = Math::Dot(relativePos, relativeVel);
        if (dot <= 0) return false;
        
        // Project relative velocity onto relative position to find closest approach
        float relPosSquared = Math::Dot(relativePos, relativePos);
        float relVelSquared = Math::Dot(relativeVel, relativeVel);
        
        // Calculate time to closest approach
        float timeToClosest = std::max(0.0f, dot / relVelSquared);
        
        // If closest approach is beyond our time horizon, ignore
        if (timeToClosest > timeHorizon) return false;
        
        // Calculate position at closest approach
        sf::Vector2f closestPos = relativePos + relativeVel * timeToClosest;
        float closestDistance = Math::VectorLength(closestPos);
        
        // Return true if we'll be closer than minimum distance
        return closestDistance < minDistance;
    }
    
    // Get predicted position after time
    sf::Vector2f getPredictedPosition(float time) const
    {
        return position + velocity * time;
    }

    void setIdealPosition(const sf::Vector2f& newIdealPos)
    {
        idealPosition = newIdealPos;
        formationDeviation = Math::VectorLength(position - idealPosition);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(body);
        window.draw(forwardLine);
    }
};