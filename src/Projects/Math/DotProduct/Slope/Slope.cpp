#include "../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Global variables
sf::Vector2f Position(400.f, 300.f); // Player position
sf::Vector2f Velocity(0.f, 0.f);     // Player velocity

// Movement settings
constexpr float MoveSpeed = 1.0f; // Base movement speed
constexpr float Drag = 0.95f;     // Velocity drag
constexpr float Gravity = 0.15f;  // Downward force

// Slope variables
sf::Vector2f SlopeNormal(0.f, -1.f);   // Normal vector of the slope (default: pointing up)
sf::Vector2f SlopeStart(200.f, 500.f); // Start point of the slope
sf::Vector2f SlopeEnd(800.f, 350.f);   // End point of the slope
sf::Vector2f ClosestSlopePointToHero{}; // Closest point on the slope to the player
sf::Vector2f slopeDir{};
bool OnSlope = false; // Is player on the slope?

// Initialize slope parameters
void InitializeSlope()
{
    // Calculate slope direction
    slopeDir = Math::Normalize(SlopeEnd - SlopeStart);

    // Normal is perpendicular to slope (90° rotation)
    SlopeNormal = sf::Vector2f(-slopeDir.y, slopeDir.x);
}

// Check if player is on the slope
bool CheckOnSlope()
{
    // Vector from slope start to player
    sf::Vector2f toPlayer = Position - SlopeStart;

    // Calculate slope direction
    slopeDir = Math::Normalize(SlopeEnd - SlopeStart);

    // Project player position onto slope line
    //We need to calculate how close the player is to the slope line. This line will ensure this  
    float projectedDistance = Math::Dot(toPlayer, slopeDir);

    // Total slope length
    float slopeLength = Math::Length(SlopeEnd - SlopeStart);

    // Check if projection is within slope segment
    if (projectedDistance >= 0 && projectedDistance <= slopeLength)
    {
        // Find the closest point on slope
        ClosestSlopePointToHero = SlopeStart + (slopeDir * projectedDistance);

        // Distance from player to closest point
        float distance = Math::Length(Position - ClosestSlopePointToHero);

        // If player is close enough to slope
        return distance < 25.0f;
    }

    return false;
}

// Handle movement on slopes
void HandleSlopeMovement()
{
    if (OnSlope)
    {
        // Get slope direction
        slopeDir = Math::Normalize(SlopeEnd - SlopeStart);

        // Project velocity onto slope
        float projectedVelOnSlop = Math::Dot(Velocity, slopeDir);

        // Replace velocity with only the component along the slope
        // This constrains movement to the slope
        Velocity = slopeDir * projectedVelOnSlop;

        // Apply gravity along the slope (parallel component)
        float downwardForce = Gravity * Math::Dot(sf::Vector2f(0.f, 1.f), slopeDir);
        Velocity += slopeDir * downwardForce;
    }
    else
    {
        // Normal gravity when not on slope
        Velocity.y += Gravity;
    }
}

// Draw the slope and player
void DrawSlope(sf::RenderWindow& window, sf::Text& text)
{
    // Draw the slope line
    sf::VertexArray slopeLine(sf::Lines, 2);
    slopeLine[0].position = SlopeStart;
    slopeLine[1].position = SlopeEnd;
    slopeLine[0].color = sf::Color::White;
    slopeLine[1].color = sf::Color::White;
    window.draw(slopeLine);

    text.setString("Slope Start: " + std::to_string(SlopeStart.x) + ", " + std::to_string(SlopeStart.y));
    text.setPosition(SlopeStart);
    text.setCharacterSize(15);
    window.draw(text);

    text.setString("Slope End: " + std::to_string(SlopeEnd.x) + ", " + std::to_string(SlopeEnd.y));
    text.setPosition(SlopeEnd);
    window.draw(text);

    // Draw the slope normal at the middle
    sf::Vector2f midpoint = (SlopeStart + SlopeEnd) / 2.f;
    sf::VertexArray normalSlopeLine(sf::Lines, 2);
    normalSlopeLine[0].position = midpoint;
    normalSlopeLine[1].position = midpoint + (SlopeNormal * 40.f);
    normalSlopeLine[0].color = sf::Color::Green;
    normalSlopeLine[1].color = sf::Color::Green;
    window.draw(normalSlopeLine);

    text.setString("Slope Normal: " + std::to_string(SlopeNormal.x) + ", " + std::to_string(SlopeNormal.y));
    text.setPosition(midpoint);
    window.draw(text);

    //Slope direction
    sf::VertexArray SlopeDirLine(sf::Lines, 2);
    SlopeDirLine[0].position = midpoint + sf::Vector2f{0,200}; //this sec vector is all arbitrary
    SlopeDirLine[1].position = midpoint +  sf::Vector2f{0,200} + (slopeDir * 50.f);
    SlopeDirLine[0].color = sf::Color::Red;
    SlopeDirLine[1].color = sf::Color::Red;
    window.draw(SlopeDirLine);

    //Velocity direction
    sf::VertexArray VelocityDirLine(sf::Lines, 2);
    auto velocityDir = Math::Normalize(Velocity);
    VelocityDirLine[0].position = midpoint + sf::Vector2f{0,300}; //this sec vector is all arbitrary
    VelocityDirLine[1].position = midpoint +  sf::Vector2f{0,300} + (Math::Normalize(velocityDir) * 50.f);
    VelocityDirLine[0].color = sf::Color::Cyan;
    VelocityDirLine[1].color = sf::Color::Cyan;
    window.draw(VelocityDirLine);

    text.setString("Slope Direction Line");
    text.setPosition(midpoint + sf::Vector2f{0,200});
    window.draw(text);

    sf::CircleShape closestPointCircle(5.f);
    closestPointCircle.setFillColor(sf::Color::Yellow);
    closestPointCircle.setOrigin(5.f, 5.f);
    closestPointCircle.setPosition(ClosestSlopePointToHero);
    window.draw(closestPointCircle);

    text.setString("closestPointCircle: " + std::to_string(ClosestSlopePointToHero.x) + ", " + std::to_string(ClosestSlopePointToHero.y));
    text.setPosition(ClosestSlopePointToHero );
    text.setCharacterSize(12);
    window.draw(text);
    
    text.setCharacterSize(18);
}

MAIN_TEMPLATE_GAME_START

// Initialize the slope
InitializeSlope();

// Create player circle
sf::CircleShape player = Object::CreateCircleShape(Position, 20.f, sf::Color::Blue);

GAME_LOOP_START

// Handle input
sf::Vector2f inputDirection(0.f, 0.f);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    inputDirection.y -= 1.f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    inputDirection.y += 1.f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    inputDirection.x -= 1.f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    inputDirection.x += 1.f;

// Apply input forces
if (Math::Length(inputDirection) > 0)
{
    inputDirection = Math::Normalize(inputDirection);
    Velocity += inputDirection * MoveSpeed;
}

// Check if player is on the slope
OnSlope = CheckOnSlope();

// Handle slope physics
HandleSlopeMovement();

// Apply drag
Velocity *= Drag;

// Update position
Position += Velocity;
player.setPosition(Position);

// Draw everything
DrawSlope(window, infoText);
window.draw(player);

// Display debug info
DISPLAY_TEXT("Position: " + std::to_string(Position.x) + ", " + std::to_string(Position.y));
DISPLAY_TEXT("Slope normal: " + std::to_string(SlopeNormal.x) + ", " + std::to_string(SlopeNormal.y));
DISPLAY_TEXT("Slope slopeDirection: " + std::to_string(slopeDir.x) + ", " + std::to_string(slopeDir.y));
DISPLAY_TEXT("On Slope: " + std::string(OnSlope ? "YES" : "NO"));
DISPLAY_TEXT("Velocity: " + std::to_string(Velocity.x) + ", " + std::to_string(Velocity.y));

// Slope movement analysis
if (OnSlope)
{
    sf::Vector2f slopeDir = Math::Normalize(SlopeEnd - SlopeStart);
    float velocityAlongSlope = Math::Dot(Velocity, slopeDir);

    std::string slopeStatus;
    if (std::abs(velocityAlongSlope) < 0.1f)
        slopeStatus = "STATIONARY ON SLOPE";
    else if (velocityAlongSlope > 0)
        slopeStatus = "SLIDING DOWNHILL";
    else
        slopeStatus = "CLIMBING UPHILL";

    DISPLAY_TEXT("Slope Movement: " + slopeStatus);
    DISPLAY_TEXT("Speed Along Slope: " + std::to_string(velocityAlongSlope));
}
else
{
    DISPLAY_TEXT("No Slope");
}

DISPLAY_TEXT("Controls: WASD = move");
DISPLAY_TEXT("Green line starts at center of the slope and represents SlopeNormal direction");
DISPLAY_TEXT("Red line is slope direciton ");

MAIN_TEMPLATE_GAME_END