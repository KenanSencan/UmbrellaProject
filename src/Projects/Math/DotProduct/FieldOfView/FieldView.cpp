#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <vector> // Needed for std::vector

// Define PI if not available in <cmath> for your compiler
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to calculate vector length
float vectorLength(const sf::Vector2f& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Helper function to normalize a vector
sf::Vector2f normalize(const sf::Vector2f& v)
{
    float len = vectorLength(v);
    if (len > 0)
    {
        return v / len;
    }
    return v; // Return zero vector if length is zero
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Field Of View Example");
    window.setFramerateLimit(60);

    // Font for displaying information
    sf::Font font;
    // Try loading from the current directory first, then the parent's directory as fallback
    if (!font.loadFromFile("../SegoeUI.ttf"))
    {
        if (!font.loadFromFile("../SegoeUI.ttf"))
        {
            // Original relative path attempt
            std::cerr << "Error loading font SegoeUI.ttf" << std::endl;
            return EXIT_FAILURE;
        }
    }
    sf::Text infoText("", font, 18);
    infoText.setFillColor(sf::Color::White);

    // --- Enemy Setup ---
    sf::Vector2f enemyPos(400.f, 300.f); // Center of the window
    sf::Vector2f enemySize(40.f, 30.f); // Width, Height
    sf::RectangleShape enemyRect(enemySize);
    enemyRect.setFillColor(sf::Color::Magenta);
    enemyRect.setOrigin(enemySize.x / 2.f, enemySize.y / 2.f); // Center origin
    enemyRect.setPosition(enemyPos);

    // Enemy's fixed forward direction (facing right)
    const sf::Vector2f enemyForwardDir(1.f, 0.f);

    // --- FOV Setup ---
    const float fovAngleDegrees = 45.0f;
    const float fovAngleRadians = fovAngleDegrees * M_PI / 180.0f;
    const float fovRange = 200.0f; // Max distance for FOV detection
    // Pre-calculate the cosine of half the FOV angle for dot product check
    const float cosHalfFov = std::cos(fovAngleRadians / 2.0f);

    // --- Player (Mouse) Setup ---
    sf::CircleShape playerMarker(5.f); // Small circle to show mouse position
    playerMarker.setFillColor(sf::Color::Cyan);
    playerMarker.setOrigin(5.f, 5.f);

    while (window.isOpen())
    {
        sf::Event evt{};
        while (window.pollEvent(evt))
        {
            if (evt.type == sf::Event::Closed)
                window.close();
        }

        // Get player (mouse) position
        sf::Vector2f playerPos = sf::Vector2f(sf::Mouse::getPosition(window));
        playerMarker.setPosition(playerPos); // Update marker position

        // --- FOV Detection Logic ---
        sf::Vector2f enemyToPlayer = playerPos - enemyPos;
        float distanceToPlayer = vectorLength(enemyToPlayer);

        bool inRange = false;
        std::string message = "NOT IN VIEW RANGE";

        // Only proceed with angle check if within range and not exactly at the enemy's position
        if (distanceToPlayer > 0 && distanceToPlayer <= fovRange)
        {
            // Normalize the direction vector from enemy to player
            sf::Vector2f normalizedEnemyToPlayer = normalize(enemyToPlayer);

            // Calculate the dot product between enemy's forward direction and the direction to the player
            // Both vectors must be normalized for this angle check
            float dot = (enemyForwardDir.x * normalizedEnemyToPlayer.x) + (enemyForwardDir.y * normalizedEnemyToPlayer.y);

            // If the dot product is greater than or equal to cos(FOV/2), the player is within the view angle
            if (dot >= cosHalfFov)
            {
                inRange = true;
                message = "IN RANGE";
            }
        }

        // --- Drawing ---
        window.clear(sf::Color(30, 30, 30));

        // Draw Enemy
        window.draw(enemyRect);

        // Draw FOV visualization (Yellow Cone)
        sf::VertexArray fovLines(sf::Lines, 4); // Two lines for the cone
        float halfFovRad = fovAngleRadians / 2.0f;

        // Calculate edge vectors by rotating the forward direction
        sf::Vector2f edge1Dir(
            enemyForwardDir.x * std::cos(halfFovRad) - enemyForwardDir.y * std::sin(halfFovRad),
            enemyForwardDir.x * std::sin(halfFovRad) + enemyForwardDir.y * std::cos(halfFovRad)
        );
        sf::Vector2f edge2Dir(
            enemyForwardDir.x * std::cos(-halfFovRad) - enemyForwardDir.y * std::sin(-halfFovRad),
            enemyForwardDir.x * std::sin(-halfFovRad) + enemyForwardDir.y * std::cos(-halfFovRad)
        );

        // Line 1
        fovLines[0].position = enemyPos;
        fovLines[1].position = enemyPos + edge1Dir * fovRange;
        // Line 2
        fovLines[2].position = enemyPos;
        fovLines[3].position = enemyPos + edge2Dir * fovRange;

        // Set color for both lines
        fovLines[0].color = sf::Color::Yellow;
        fovLines[1].color = sf::Color::Yellow;
        fovLines[2].color = sf::Color::Yellow;
        fovLines[3].color = sf::Color::Yellow;

        window.draw(fovLines);

        // Draw Player (Mouse Marker)
        window.draw(playerMarker);

        // Draw Info Text
        infoText.setString("Mouse Position: " + std::to_string(static_cast<int>(playerPos.x)) + ", " + std::to_string(static_cast<int>(playerPos.y)));
        infoText.setPosition(10.f, 10.f);
        window.draw(infoText);

        infoText.setString("Distance to Enemy: " + std::to_string(distanceToPlayer));
        infoText.setPosition(10.f, 35.f);
        window.draw(infoText);

        infoText.setString(message); // Display "IN RANGE" or "NOT IN VIEW RANGE"
        infoText.setPosition(10.f, 60.f);
        infoText.setFillColor(inRange ? sf::Color::Green : sf::Color::Red); // Change color based on status
        window.draw(infoText);
        infoText.setFillColor(sf::Color::White); // Reset color for other text


        window.display();
    }

    return 0;
}
