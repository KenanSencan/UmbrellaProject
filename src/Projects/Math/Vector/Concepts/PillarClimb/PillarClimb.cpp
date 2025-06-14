#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Global variables
sf::Vector2f Position(400.f, 500.f);   // Player position
sf::Vector2f Velocity(0.f, 0.f);       // Player velocity
bool OnLeftPillar = false;             // Is player on left pillar?
bool OnRightPillar = false;            // Is player on right pillar?
bool IsHolding = false;                // Is player holding?
float TimeSinceJump = 0.f;             // Time since last jump (for jump cooldown)

// Movement settings
constexpr float MoveSpeed = 0.5f;      // Base movement speed
constexpr float JumpForce = 8.0f;     // Jump force
constexpr float Drag = 0.95f;          // Velocity drag
constexpr float Gravity = 0.15f;       // Downward force
constexpr float SlideSpeed = 0.3f;     // Sliding speed when not holding
constexpr float HoldingSlideSpeedMult = 0.99f; // Sliding speed when holding
constexpr float JumpCooldown = 0.3f;   // Time before player can jump again

// Pillar variables
sf::RectangleShape LeftPillar;         // Left pillar shape
sf::RectangleShape RightPillar;        // Right pillar shape
constexpr float PillarWidth = 40.f;    // Width of pillars
constexpr float PillarHeight = 500.f;  // Height of pillars
constexpr float LeftPillarX = 250.f;   // X position of left pillar
constexpr float RightPillarX = 550.f;  // X position of right pillar
constexpr float PillarY = 100.f;       // Y position (top) of pillars
constexpr float PlayerRadius = 20.f;   // Player radius

// Initialize pillars
void InitializePillars()
{
    // Set up left pillar
    LeftPillar.setSize(sf::Vector2f(PillarWidth, PillarHeight));
    LeftPillar.setPosition(LeftPillarX, PillarY);
    LeftPillar.setFillColor(sf::Color(150, 100, 50));
    
    // Set up right pillar
    RightPillar.setSize(sf::Vector2f(PillarWidth, PillarHeight));
    RightPillar.setPosition(RightPillarX, PillarY);
    RightPillar.setFillColor(sf::Color(150, 100, 50));
}

// Check if player is touching the left pillar
bool CheckPillarCollision(const sf::RectangleShape& pillar)
{
   //Get pil;lar corner
    const sf::Vector2f pillarPos = pillar.getPosition();
    const sf::Vector2f pillarSize = pillar.getSize();
    auto [TopLeft, TopRight, BottomLeft, BottomRight] = OtherHelper::CalculateFourCorner(pillarPos, pillarSize, {0, 0});

    //Check collision with left edge
    const bool touchingLeft = std::abs(Position.x - TopLeft.x) <= PlayerRadius &&
                       Position.y >= TopLeft.y - PlayerRadius &&
                       Position.y <= BottomLeft.y + PlayerRadius;

    // Check collision with right edge
    const bool touchingRight = std::abs(Position.x - TopRight.x) <= PlayerRadius &&
                        Position.y >= TopRight.y - PlayerRadius &&
                        Position.y <= BottomRight.y + PlayerRadius;

    return touchingLeft || touchingRight;
}

// Handle pillar movement and sliding
void HandlePillarMovement(const float deltaTime)
{
    // Check for pillar collisions
    OnLeftPillar = CheckPillarCollision(LeftPillar);
    OnRightPillar = CheckPillarCollision(RightPillar);
    
    // If on any pillar, handle sliding
    if (OnLeftPillar || OnRightPillar)
    {
        // Apply sliding effect
        if (IsHolding)
        {
            // Slower slide when holding
            Velocity.y *= std::pow(HoldingSlideSpeedMult, deltaTime);
        }
        else
        {
            // Normal slide when not holding
            Velocity.y = SlideSpeed;
        }
        
        // Cancel horizontal velocity to stick to pillar
        Velocity.x = 0.f;
    }
    else
    {
        // Normal gravity when not on a pillar
        Velocity.y += Gravity;
    }
}

// Handle jumping between pillars
void HandleJump(const float deltaTime)
{
    // Update jump cooldown
    TimeSinceJump += deltaTime;
    
    // Check for jump input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && TimeSinceJump >= JumpCooldown)
    {
        // Can only jump if on a pillar
        if (OnLeftPillar || OnRightPillar)
        {
            // Reset jump cooldown
            TimeSinceJump = 0.f;
            
            // Apply upward force
            
            // Apply horizontal force based on direction
            if (OnLeftPillar && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                Velocity.y = -JumpForce;
                
                // Jump from left pillar to right
                Velocity.x = JumpForce * 1.5f;
            }
            else if (OnRightPillar && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                Velocity.y = -JumpForce;
                
                // Jump from right pillar to left
                Velocity.x = -JumpForce * 1.5f;
            }
            
            // Detach from pillar
            OnLeftPillar = false;
            OnRightPillar = false;
        }
    }
}

MAIN_TEMPLATE_GAME_START

// Initialize pillars
InitializePillars();

// Create player circle
sf::CircleShape player = Object::CreateCircleShape(Position, PlayerRadius, sf::Color::Blue);

GAME_LOOP_START

// Handle input
sf::Vector2f inputDirection(0.f, 0.f);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) inputDirection.x -= 1.f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) inputDirection.x += 1.f;

// Check if player is holding
IsHolding = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

// Apply input forces (only when not on a pillar)
if (!OnLeftPillar && !OnRightPillar && Math::Length(inputDirection) > 0)
{
    inputDirection = Math::Normalize(inputDirection);
    Velocity += inputDirection * MoveSpeed;
}

// Handle pillar collision and sliding
HandlePillarMovement(deltaTime);

// Handle jumping
HandleJump(deltaTime);

// Apply drag (in air only)
if (!OnLeftPillar && !OnRightPillar)
{
    Velocity *= Drag;
}

// Update position
Position += Velocity;

// Keep player within window bounds
if (Position.x < PlayerRadius) Position.x = PlayerRadius;
if (Position.x > 800.f - PlayerRadius) Position.x = 800.f - PlayerRadius;
if (Position.y < PlayerRadius) Position.y = PlayerRadius;
if (Position.y > 600.f - PlayerRadius) Position.y = 600.f - PlayerRadius;

// Ground collision
if (Position.y > 600.f - PlayerRadius)
{
    Position.y = 600.f - PlayerRadius;
    Velocity.y = 0;
}

// Update player position
player.setPosition(Position);

// Draw everything
window.draw(LeftPillar);
window.draw(RightPillar);
window.draw(player);

//TEMP
auto temp = Object::CreateCircleShape(Position - LeftPillar.getPosition() , 3, sf::Color::Green);
auto temp2 = Object::CreateCircleShape( LeftPillar.getPosition() , 3, sf::Color::Cyan);
window.draw(temp);
window.draw(temp2);

// Display debug information using DISPLAY_TEXT macro
DISPLAY_TEXT("Position: " + std::to_string(Position.x) + ", " + std::to_string(Position.y))
DISPLAY_TEXT("Velocity: " + std::to_string(Velocity.x) + ", " + std::to_string(Velocity.y))
DISPLAY_TEXT("On Left Pillar: " + std::string(OnLeftPillar ? "YES" : "NO"))
DISPLAY_TEXT("On Right Pillar: " + std::string(OnRightPillar ? "YES" : "NO"))
DISPLAY_TEXT("Holding: " + std::string(IsHolding ? "YES" : "NO"))
DISPLAY_TEXT("")
DISPLAY_TEXT("Mouse Position: " + std::to_string(sf::Mouse::getPosition(window).x) + ", " + std::to_string(sf::Mouse::getPosition(window).y))

// Display controls information
DISPLAY_TEXT("Controls: A/D to move, SPACE to jump, SHIFT to hold")
DISPLAY_TEXT("Jump from left pillar: Hold D + SPACE")
DISPLAY_TEXT("Jump from right pillar: Hold A + SPACE")

MAIN_TEMPLATE_GAME_END