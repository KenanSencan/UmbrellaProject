#include "../../../HelperClass/HelperCollection.h"
#include "FOVVisualizer.h"
#include <SFML/Graphics.hpp>
#include <complex>
#include <filesystem>
#include <string>

// NOTE: Global variables:
//  The player faces to the right (1,0) as the forward direction
sf::Vector2f HeroDirection(1.f, 0.f);

// Rotation speed in radians per frame when E or Q is pressed
constexpr float rotationSpeed = 0.05f;

// FOV angle in degrees (total angle)
float fovDegrees = 90.0f;

std::string FovStatusMessage;
float dot;
sf::Vector2f playerToEnemy;
sf::CircleShape hero;
sf::CircleShape enemy;
bool isInFOV = false;
constexpr float fovDistance = 250.0f; // Match this with the distance in
float TestAngle = 0.0f;

// This is the major calculation
void MakeFOVCalculationWithDotProduct()
{
    // Get the vector from player to enemy
    playerToEnemy = enemy.getPosition() - hero.getPosition();

    // Calculate the distance between player and enemy
    const float distance = Math::Length(playerToEnemy);

    // Normalize
    HeroDirection = Math::Normalize(HeroDirection);
    playerToEnemy = Math::Normalize(playerToEnemy); // Now this will instead be a direction pointing towards Player to Enemy

    // NOTE: Two normalized vector's dot product equals the cosine of the angle between them
    //In obsidian look at "Math/Trigonometry/Functions/basics/Cos#Using the dot product Formula we can find cos as well" we have the formula:
    //We didn't divide with length of A * B because we already normalized them it would be just dividing with 0
    //If not rotated, this has to be >=0.7071 aka cos(45) to be in range
    dot = Math::Dot(HeroDirection, playerToEnemy);

    // This angle represents how far the enemy is from the player's direct line of sight.
    // Came closer to line's origin to get 0 and get 180 with other side.
    // NOTE: When TestAngle < 45, it's in POV
    TestAngle = Math::RadianToDegree(std::acos(dot));

    // This is because FOV extends equally in both directions from your center view. Half goes to the left, half to the right of your forward vector
    float halfOfRadians = Math::DegreeToRadian(fovDegrees * 0.5f);
    float cosineThreshold = std::cos(halfOfRadians); //will be 0.7071. cos is same in Obsidian's cos note's picture at #How to find cosine   
    const bool isInAngle = (dot > cosineThreshold);

    // Check both angle AND distance
    isInFOV = isInAngle && (distance <= fovDistance);

    // More detailed status message
    if (!isInAngle)
    {
        FovStatusMessage = "OUTSIDE FOV ANGLE";
    }
    else if (distance > fovDistance)
    {
        FovStatusMessage = "OUTSIDE FOV DISTANCE";
    }
    else
    {
        FovStatusMessage = "IN FOV RANGE";
    }
}

MAIN_TEMPLATE_GAME_START

hero = Object::CreateCircleShape({400.f, 300.f}, 20.f, sf::Color::Blue);
enemy = Object::CreateCircleShape(sf::Vector2f(sf::Mouse::getPosition(window)), 5.f, sf::Color::Red);

GAME_LOOP_START

enemy.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));

// Handle key presses for rotation
if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
{
    HeroDirection = Math::RotateVector(rotationSpeed, HeroDirection);
    HeroDirection = Math::Normalize(HeroDirection);
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
{
    HeroDirection = Math::RotateVector(-rotationSpeed, HeroDirection);
    HeroDirection = Math::Normalize(HeroDirection);
}

// Handle key presses for FOV adjustment
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    fovDegrees += 0.5f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    fovDegrees -= 0.5f;

// Calculate FOV and check if enemy is within it
MakeFOVCalculationWithDotProduct();

// Set enemy color based on FOV detection
enemy.setFillColor(isInFOV ? sf::Color::Green : sf::Color::Red);

// Draw FOV visualization first (so it appears behind other elements)
FOVVisualizer::DrawFOVVisualization(window, hero.getPosition(), HeroDirection, fovDegrees, isInFOV, fovDistance);

// Draw other elements
window.draw(hero);

window.draw(FOVVisualizer::DrawHeroForwardVec(hero, HeroDirection));
window.draw(FOVVisualizer::DrawHeroToEnArrow(hero, enemy));

// Draw the angle arc between the hero direction and hero-to-enemy vector
FOVVisualizer::DrawAngleArc(window, hero.getPosition(), HeroDirection, playerToEnemy, 40.f, sf::Color::Cyan);
FOVVisualizer::DotProductIndicator(window, dot);

window.draw(enemy);

// Display debug information
DISPLAY_TEXT("IMPORTANT! Since dot product can capture angle. I can easily check if dot > 0.7071 aka cos(45). This is the MAIN LOGIC OF ENTIRE CALCULATION!!!");
DISPLAY_TEXT("2. Focus on dot product. draw circles around blue sphere with your mouse. Look how it ranges from -1 -> 1");
DISPLAY_TEXT("3. If not rotated, it'll be in range if the angle is 45 degrees aka angle cos is >= 0.7071");
DISPLAY_TEXT("Dot Product =  HeroDirection, playerToEnemy:  " + std::to_string(dot));
DISPLAY_TEXT("cos angle: " + std::to_string(cos(Math::DegreeToRadian(TestAngle))));
DISPLAY_TEXT("Angle: " + std::to_string(TestAngle) + " degrees");
DISPLAY_TEXT( dot >= 0 ? "Mouse is in front of the Hero" : "Mouse is behind the Hero");
DISPLAY_TEXT("Logic is simple. If Dot >= 0, it's front. If it's negative, it's behind");

DISPLAY_TEXT("\n")
DISPLAY_TEXT("Enemy Position: " + std::to_string(enemy.getPosition().x) + ", " + std::to_string(enemy.getPosition().y));
DISPLAY_TEXT("Player To Enemy Normalized Dir: " + std::to_string(playerToEnemy.x) + ", " + std::to_string(playerToEnemy.y));
DISPLAY_TEXT("FOV Status: " + FovStatusMessage);
DISPLAY_TEXT("Threshold: " + std::to_string(std::cos(fovDegrees * 0.5f * 3.14159f / 180.0f)));
DISPLAY_TEXT("Hero Direction: " + std::to_string(HeroDirection.x) + ", " + std::to_string(HeroDirection.y));
DISPLAY_TEXT("FOV Angle: " + std::to_string(fovDegrees) + " degrees");
DISPLAY_TEXT("Controls: Q = rotate left, E = rotate right");

MAIN_TEMPLATE_GAME_END
