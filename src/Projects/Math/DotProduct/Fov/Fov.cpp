#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include "../../HelperClass/HelperCollection.h"
#include <complex>

//NOTE: Global variables: 
// The player faces to the right (1,0) as the forward direction
sf::Vector2f HeroDirection(1.f, 0.f);

// Rotation speed in radians per frame when E or Q is pressed
constexpr float rotationSpeed = 0.05f;

// FOV angle in degrees (total angle)
float fovDegrees = 90.0f;

std::string FovStatusMessage;
float dot;
sf::Vector2f playerToEnemyRelative;
sf::CircleShape player;
sf::CircleShape enemy;
bool isInFOV = false;
constexpr float fovDistance = 250.0f; // Match this with the distance in
float TestAngle = 0.0f;

//This is the major calculation
void MakeFOVCalculationWithDotProduct(const sf::CircleShape& player, sf::Vector2f heroDirection,
                                      const sf::Vector2f& enemyPos, sf::Vector2f& playerToEnemy,
                                      float& dot, std::string& inRangeMessage, bool& isInFOV,
                                      const float fovDegrees = 90.0f, const float fovDistance = 150.0f)
{
    // Get the vector from player to enemy
    playerToEnemy = enemyPos - player.getPosition();

    // Calculate the distance between player and enemy
    const float distance = Math::Length(playerToEnemy);

    //Normalize 
    heroDirection = Math::Normalize(heroDirection);
    playerToEnemy = Math::Normalize(playerToEnemy); //Now this will instead be a direction pointing towards Player to Enemy

    // Calculate dot product with heroDirection and playerToEnemy
    //When two vectors are normalized, their dot product equals the cosine of the angle between them
    dot = Math::Dot(heroDirection, playerToEnemy);

    //This angle represents how far the enemy is from the player's direct line of sight.
    //Came closer to line's origin to get 0 and get 180 with other side.
    //NOTE: When TestAngle < 45, it's in POV
    TestAngle = Math::RadianToDegree(std::acos(dot));
    
    //This is because FOV extends equally in both directions from your center view. Half goes to the left, half to the right of your forward vector
    float halfofRadians = Math::DegreeToRadian(fovDegrees * 0.5f);
    float cosineThreshold = std::cos(halfofRadians); 
    bool isInAngle = (dot > cosineThreshold);

    // Check both angle AND distance
    isInFOV = isInAngle && (distance <= fovDistance);

    // More detailed status message
    if (!isInAngle)
    {
        inRangeMessage = "OUTSIDE FOV ANGLE";
    }
    else if (distance > fovDistance)
    {
        inRangeMessage = "OUTSIDE FOV DISTANCE";
    }
    else
    {
        inRangeMessage = "IN FOV RANGE";
    }
}

// Visualize the field of view
//TODO: Study later here more in depth
void DrawFOVVisualization(sf::RenderWindow& window, const sf::Vector2f& playerPos,
                          const sf::Vector2f& direction, const float fovDegrees, const bool enemyInFOV, const float fovLength)
{
    // Number of triangles used to draw the FOV cone
    constexpr int triangleCount = 30;


    // Create a triangle fan to represent the FOV
    sf::VertexArray fovCone(sf::TriangleFan, triangleCount + 2);

    // First vertex is the player position (cone origin)
    fovCone[0].position = playerPos;
    fovCone[0].color = sf::Color(255, 255, 100, 40); // Semi-transparent yellow

    // Calculate FOV boundary angles
    const float directionAngle = atan2(direction.y, direction.x);
    const float halfFOVRadians = (fovDegrees * 0.5f) * 3.14159f / 180.0f;
    const float startAngle = directionAngle - halfFOVRadians;
    const float endAngle = directionAngle + halfFOVRadians;

    // Fill the triangle fan with vertices along the FOV arc
    for (int i = 0; i <= triangleCount; i++)
    {
        const float ratio = static_cast<float>(i) / triangleCount;
        const float angle = startAngle + ratio * (endAngle - startAngle);
        const float x = playerPos.x + cos(angle) * fovLength;
        const float y = playerPos.y + sin(angle) * fovLength;

        fovCone[i + 1].position = sf::Vector2f(x, y);

        // Change color based on whether enemy is in FOV
        if (enemyInFOV)
        {
            fovCone[i + 1].color = sf::Color(100, 255, 100, 70); // Green if enemy detected
        }
        else
        {
            fovCone[i + 1].color = sf::Color(255, 100, 100, 70); // Red if no enemy
        }
    }

    // Draw the FOV visualization
    window.draw(fovCone);
}

MAIN_TEMPLATE_GAME_START

    player = Object::CreateCircleShape({400.f, 300.f}, 20.f, sf::Color::Blue);
    enemy = Object::CreateCircleShape(sf::Vector2f(sf::Mouse::getPosition(window)), 5.f, sf::Color::Red);

    // Handle key presses for rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        HeroDirection = Math::RotateVector(rotationSpeed, HeroDirection);
        Math::Normalize(HeroDirection);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        HeroDirection = Math::RotateVector(-rotationSpeed, HeroDirection);
        Math::Normalize(HeroDirection);
    }

// Handle key presses for FOV adjustment
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) fovDegrees+= 0.5f;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) fovDegrees-= 0.5f;

    // Calculate FOV and check if enemy is within it
    MakeFOVCalculationWithDotProduct(player, HeroDirection, enemy.getPosition(),
                                     playerToEnemyRelative, dot, FovStatusMessage,
                                     isInFOV, fovDegrees, fovDistance);

    // Set enemy color based on FOV detection
    enemy.setFillColor(isInFOV ? sf::Color::Green : sf::Color::Red);

    // Draw FOV visualization first (so it appears behind other elements)
    DrawFOVVisualization(window, player.getPosition(), HeroDirection, fovDegrees, isInFOV, fovDistance);

    // Draw other elements
    window.draw(player);

    // Draw forward direction arrow 
    sf::VertexArray forwardArrow(sf::Lines, 2);
    forwardArrow[0].position = player.getPosition();
    forwardArrow[1].position = player.getPosition() + (HeroDirection * 100.f);
    forwardArrow[0].color = sf::Color::Yellow;
    forwardArrow[1].color = sf::Color::Yellow;
    window.draw(forwardArrow);

    window.draw(enemy);

    // Display debug information
    DISPLAY_TEXT("Enemy Position: " + std::to_string(enemy.getPosition().x) + ", " + std::to_string(enemy.getPosition().y));
    DISPLAY_TEXT("Player-to-Enemy Vector: " + std::to_string(playerToEnemyRelative.x) + ", " + std::to_string(playerToEnemyRelative.y));
    DISPLAY_TEXT("Dot Product: " + std::to_string(dot));
    DISPLAY_TEXT("FOV Status: " + FovStatusMessage);
    DISPLAY_TEXT("Threshold: " + std::to_string(std::cos(fovDegrees * 0.5f * 3.14159f / 180.0f)));
    DISPLAY_TEXT("Hero Direction: " + std::to_string(HeroDirection.x) + ", " + std::to_string(HeroDirection.y));
    DISPLAY_TEXT("FOV Angle: " + std::to_string(fovDegrees) + " degrees");
    DISPLAY_TEXT("Controls: Q = rotate left, E = rotate right");
    DISPLAY_TEXT("Angle: " + std::to_string(TestAngle) + " degrees");

MAIN_TEMPLATE_GAME_END
