#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include "../../HelperClass/HelperCollection.h"

//NOTE: Global variables: 
// The player faces to the right (1,0) as the forward direction
sf::Vector2f HeroDirection(1.f, 0.f);

// Rotation speed in radians per frame when E or Q is pressed
constexpr float rotationSpeed = 0.05f;

std::string FrontOrBehindMessage;
float dot;
sf::Vector2f playerToEnemyRelative;
sf::CircleShape player;
sf::CircleShape enemy;

//This is the major calculation
void MakeFrontBehindCalculationWithDotProduct(const sf::CircleShape& player, const sf::Vector2f HeroDirection, const sf::Vector2f& enemyPos, sf::Vector2f& playerToEnemyRelative, float& dot, std::string& FrontOrBehindMes)
{
    playerToEnemyRelative = enemyPos - player.getPosition();

    dot = Math::Dot(HeroDirection, playerToEnemyRelative);

    FrontOrBehindMes = (dot >= 0.f) ? "ENEMY IS IN FRONT" : "ENEMY IS BEHIND";
}

MAIN_TEMPLATE_GAME_START

    player = Object::CreateCircleShape({400.f, 300.f}, 20.f, sf::Color::Blue);
    enemy = Object::CreateCircleShape(sf::Vector2f(sf::Mouse::getPosition(window)), 5.f, sf::Color::Red);

    // Handle key presses for rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        HeroDirection = Math::RotateVector(-rotationSpeed, HeroDirection);

        // Normalize to prevent drift
        Math::Normalize(HeroDirection);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        HeroDirection = Math::RotateVector(rotationSpeed, HeroDirection);

        // Normalize to prevent drift
        Math::Normalize(HeroDirection);
    }

    //NOTE: Main stuff happening at here. Calculate the dot product and determine if the enemy is in front or behind
    MakeFrontBehindCalculationWithDotProduct(player, HeroDirection, enemy.getPosition(), playerToEnemyRelative, dot, FrontOrBehindMessage);

    //NOTE: TEXT Drawing
    window.draw(player);
    // Draw forward direction arrow 
    sf::VertexArray forwardArrow(sf::Lines, 2);

    forwardArrow[0].position = player.getPosition(); // tail at player's center
    forwardArrow[1].position = player.getPosition() + (HeroDirection * 100.f); // 100 units ahead
    forwardArrow[0].color = sf::Color::Yellow;
    forwardArrow[1].color = sf::Color::Yellow;
    window.draw(forwardArrow);
    window.draw(enemy);

    DISPLAY_TEXT("Enemy Mouse Position: " + std::to_string(enemy.getPosition().x) + " " + std::to_string(enemy.getPosition().y));
    DISPLAY_TEXT("playerToEnemy: " + std::to_string(playerToEnemyRelative.x) + " " + std::to_string(playerToEnemyRelative.y));
    DISPLAY_TEXT("Dot Product: " + std::to_string(dot));
    DISPLAY_TEXT(FrontOrBehindMessage);
    DISPLAY_TEXT("Hero Direction: " + std::to_string(HeroDirection.x) + " " + std::to_string(HeroDirection.y));
    DISPLAY_TEXT("Press Q to rotate counter-clockwise, E to rotate clockwise");

MAIN_TEMPLATE_GAME_END
