#include "../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <complex>
#include <filesystem>
#include <iostream>
#include <string>

// Global variables
sf::Vector2f Direction(1.f, 0.f);      // Forward direction
sf::Vector2f Velocity(0.f, 0.f);     // Player's current velocity
sf::Vector2f Position(400.f, 300.f); // Starting position
sf::Vector2f ProjectedVector(0.f, 0.f); // Starting position
float ProjectedSpeed = 0.0f; // Speed in the forward direction

// Movement settings
constexpr float MoveSpeed = 1.1f;      // Base movement speed. It'll increase %10
constexpr float RotationSpeed = 0.05f; // Rotation speed in radians
constexpr float Drag = 0.9f;           // Velocity drag (slows down over time)

// Projection variables
float LateralSpeed = 0.0f;   // Speed perpendicular to the forward direction
sf::Vector2f LateralVector(0.f, 0.f); // Starting position

// Function to calculate velocity projection
void CalculateMovementProjection()
{
    // Normalize the direction vector just in case
    Direction = Math::Normalize(Direction);

    // Calculate how much of the velocity is in the forward direction
    // This is the core of the movement projection - using dot product
    //In short projectedSpeed means calculated speed in a specific direction. 
    ProjectedSpeed = Math::Dot(Velocity, Direction);
    ProjectedVector = Direction * ProjectedSpeed;

    // Calculate the lateral component (perpendicular to direction)
    // We can create a perpendicular vector by swapping x/y and negating one
    const sf::Vector2f lateralDirection(-Direction.y, Direction.x);
    LateralSpeed = Math::Dot(Velocity, lateralDirection);
}

// Draw velocity and projection visualization
void DrawVelocityProjection(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Vector2f& direction)
{
    // Draw direction arrow
    sf::VertexArray directionArrow(sf::Lines, 2);
    directionArrow[0].position = Position;
    directionArrow[1].position = Position + (Direction * 50.f);
    directionArrow[0].color = sf::Color::Yellow;
    directionArrow[1].color = sf::Color::Yellow;
    window.draw(directionArrow);
    
    // Draw the full velocity vector (red)
    sf::VertexArray velocityLine(sf::Lines, 2);
    velocityLine[0].position = position;
    velocityLine[1].position = position + (velocity * 15.f); // Scaled for visibility
    velocityLine[0].color = sf::Color::Red;
    velocityLine[1].color = sf::Color::Red;
    window.draw(velocityLine);

    // Draw the projected component (green)
    sf::VertexArray projectedLine(sf::Lines, 2);
    projectedLine[0].position = position;
    projectedLine[1].position = position + (ProjectedVector * 15.f); // Scaled for visibility
    projectedLine[0].color = sf::Color::Green;
    projectedLine[1].color = sf::Color::Green;
    window.draw(projectedLine);

    // Draw the lateral component (blue)
    sf::VertexArray lateralLine(sf::Lines, 2);
    lateralLine[0].position = position + (ProjectedVector * 15.f);
    lateralLine[1].position = position + (velocity * 15.f);
    lateralLine[0].color = sf::Color::Blue;
    lateralLine[1].color = sf::Color::Blue;
    window.draw(lateralLine);

    // Draw a point at the projection's endpoint
    sf::CircleShape projPoint(4);
    projPoint.setFillColor(sf::Color::Yellow);
    projPoint.setOrigin(4, 4);
    projPoint.setPosition(position + (ProjectedVector * 15.f));
    window.draw(projPoint);
}

MAIN_TEMPLATE_GAME_START

sf::CircleShape hero = Object::CreateCircleShape(Position, 20.f, sf::Color::Blue);

// NOTE: Handle rotation
if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
{
    Direction = Math::RotateVector(RotationSpeed, Direction);
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
{
    Direction = Math::RotateVector(-RotationSpeed, Direction);
}

// NOTE: Handle movement input
sf::Vector2f inputDirection(0.f, 0.f);

if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    inputDirection.y -= 1.f;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    inputDirection.y += 1.f;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    inputDirection.x -= 1.f;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    inputDirection.x += 1.f;

// NOTE: Normalize input direction and apply movement speed
if (Math::Length(inputDirection) > 0)
{
    // In case it's going diagonal with W E keys, it'll be: 1.41421 we need to normalize it.
    inputDirection = Math::Normalize(inputDirection);

    // Apply movement speed
    Velocity += inputDirection * MoveSpeed;
}

// NOTE: Apply drag to slow down
Velocity *= Drag;

// NOTE: Update position based on velocity
Position += Velocity;
hero.setPosition(Position);

// NOTE: Calculate movement projection
CalculateMovementProjection();

// Draw the player
window.draw(hero);

// Draw velocity projections
DrawVelocityProjection(window, Position, Velocity, Direction);

// Display debug information
DISPLAY_TEXT("Position: " + std::to_string(Position.x) + ", " + std::to_string(Position.y));
DISPLAY_TEXT("Velocity: " + std::to_string(Velocity.x) + ", " + std::to_string(Velocity.y) + " Magnitude: " + std::to_string(Math::Length(Velocity)) +"");
DISPLAY_TEXT("input Direction: " + std::to_string(inputDirection.x) + ", " + std::to_string(inputDirection.y));
DISPLAY_TEXT("Hero Direction: " + std::to_string(Direction.x) + ", " + std::to_string(Direction.y));
DISPLAY_TEXT("Forward (projected) Speed: " + std::to_string(ProjectedSpeed));
DISPLAY_TEXT("Lateral Speed: " + std::to_string(LateralSpeed));

DISPLAY_TEXT("Controls: WASD = move, Q/E = rotate");
DISPLAY_TEXT("Lengths of the arrows are just for representation purposes.");
DISPLAY_TEXT("Rotate to see exact difference with Velocity (red) and Projected Vector (green) lines.");
DISPLAY_TEXT("Yellow arrow = forward direction");
DISPLAY_TEXT("Red arrow = (HeroPos + Velocity) AKA normal velocity");
DISPLAY_TEXT("Green arrow = forward projected velocity ProjectedVector");
DISPLAY_TEXT("Blue arrow = Lateral projected velocity Vector");

// Movement analysis text
std::string movementStatus;
if (std::abs(ProjectedSpeed) < 0.1f)
{
    movementStatus = "STATIONARY";
}
else if (ProjectedSpeed > 0)
{
    movementStatus = "MOVING FORWARD";
}
else
{
    movementStatus = "MOVING BACKWARD";
}

if (std::abs(LateralSpeed) > 0.5f)
{
    if (LateralSpeed > 0)
        movementStatus += " + Sliding RIGHT";
    else
        movementStatus += " + Sliding LEFT";
}

DISPLAY_TEXT("Movement: " + movementStatus);

MAIN_TEMPLATE_GAME_END
