#pragma once
#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <complex>
#include <filesystem>
#include <string>

class FOVVisualizer
{
  public:
    // Draw angle arc between two vectors
    static void DrawAngleArc(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& vector1, const sf::Vector2f& vector2, float arcRadius, sf::Color color,
                             bool showAngleValue = true)
    {
        // Calculate angles of both vectors
        float angle1 = atan2(vector1.y, vector1.x);
        float angle2 = atan2(vector2.y, vector2.x);

        // Calculate the angle between vectors (get the shortest arc)
        float angleDiff = angle2 - angle1;

        // Ensure we get the smallest angle
        while (angleDiff > std::numbers::pi)
            angleDiff -= 2 * std::numbers::pi;
        while (angleDiff < -std::numbers::pi)
            angleDiff += 2 * std::numbers::pi;

        // Create arc visualization
        const int arcSegments = 30;
        sf::VertexArray arc(sf::LinesStrip, arcSegments + 1);

        // Draw the arc
        for (int i = 0; i <= arcSegments; i++)
        {
            float t = static_cast<float>(i) / static_cast<float>(arcSegments);
            float angle = angle1 + angleDiff * t;

            float x = position.x + arcRadius * cos(angle);
            float y = position.y + arcRadius * sin(angle);

            arc[i].position = sf::Vector2f(x, y);
            arc[i].color = color;
        }

        window.draw(arc);

        // Draw angle value text if requested
        if (showAngleValue)
        {
            float angleDegrees = fabs(angleDiff) * 180.0f / std::numbers::pi;

            // Create text for angle value
            static sf::Font font;
            static bool fontLoaded = false;

            if (!fontLoaded)
            {
                // Using the same font path from Object::CreateText
                fontLoaded = font.loadFromFile((std::filesystem::current_path().parent_path() / "SegoeUI.ttf").string());
            }

            if (fontLoaded)
            {
                sf::Text angleText;
                angleText.setFont(font);
                angleText.setString(std::to_string(static_cast<int>(angleDegrees)));
                angleText.setCharacterSize(16);
                angleText.setFillColor(sf::Color::White);

                // Position text at midpoint of arc
                float midAngle = angle1 + angleDiff * 0.5f;
                float textX = position.x + (arcRadius * 1.2f) * cos(midAngle);
                float textY = position.y + (arcRadius * 1.2f) * sin(midAngle);

                // Center the text at the calculated position
                sf::FloatRect textBounds = angleText.getLocalBounds();
                angleText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                angleText.setPosition(textX, textY);

                window.draw(angleText);
            }
        }
    }

    // Visualize the field of view
    // TODO: Study later here more in depth
    static void DrawFOVVisualization(sf::RenderWindow& window, const sf::Vector2f& playerPos, const sf::Vector2f& direction, const float fovDegrees, const bool enemyInFOV,
                                     const float fovLength)
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
        const float halfFOVRadians = Math::DegreeToRadian(fovDegrees * 0.5f);
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

        // Also draw FOV boundary lines
        sf::VertexArray fovBoundaries(sf::Lines, 4);

        // Left boundary
        fovBoundaries[0].position = playerPos;
        fovBoundaries[0].color = sf::Color(255, 255, 0, 180); // Semi-transparent yellow
        fovBoundaries[1].position = sf::Vector2f(playerPos.x + cos(startAngle) * fovLength, playerPos.y + sin(startAngle) * fovLength);
        fovBoundaries[1].color = sf::Color(255, 255, 0, 180);

        // Right boundary
        fovBoundaries[2].position = playerPos;
        fovBoundaries[2].color = sf::Color(255, 255, 0, 180);
        fovBoundaries[3].position = sf::Vector2f(playerPos.x + cos(endAngle) * fovLength, playerPos.y + sin(endAngle) * fovLength);
        fovBoundaries[3].color = sf::Color(255, 255, 0, 180);

        window.draw(fovBoundaries);
    }
    static sf::VertexArray DrawHeroForwardVec(const sf::CircleShape& hero, const sf::Vector2f HeroDirection)
    {
        // Draw forward direction arrow
        sf::VertexArray forwardArrow(sf::Lines, 2);
        forwardArrow[0].position = hero.getPosition();
        forwardArrow[1].position = hero.getPosition() + (HeroDirection * 100.f);
        forwardArrow[0].color = sf::Color::Yellow;
        forwardArrow[1].color = sf::Color::Yellow;

        return forwardArrow;
    };

    static sf::VertexArray DrawHeroToEnArrow(const sf::CircleShape& hero, const sf::CircleShape& enemy)
    {
        sf::VertexArray HeroToEnArrow(sf::Lines, 2);
        HeroToEnArrow[0].position = hero.getPosition();
        HeroToEnArrow[1].position = enemy.getPosition();
        HeroToEnArrow[0].color = sf::Color::Magenta;
        HeroToEnArrow[1].color = sf::Color::Magenta;

        return HeroToEnArrow;
    };

    static void DotProductIndicator(sf::RenderWindow& window, const float dot)
    {
        // Draw dot product visualization
        sf::CircleShape dotProductIndicator(10.f);
        dotProductIndicator.setOrigin(10.f, 10.f);
        dotProductIndicator.setPosition(800.f, 50.f);

        // Map dot product from [-1,1] to a color (red = -1, yellow = 0, green = 1)
        sf::Color dotColor;
        if (dot >= 0)
        {
            // Yellow to green (0 to 1)
            dotColor = sf::Color(255 * (1 - dot), // R decreases from 255 to 0
                                 255,             // G stays at 255
                                 0                // B stays at 0
            );
        }
        else
        {
            // Yellow to red (-1 to 0)
            dotColor = sf::Color(255,             // R stays at 255
                                 255 * (1 + dot), // G decreases from 255 to 0
                                 0                // B stays at 0
            );
        }
        dotProductIndicator.setFillColor(dotColor);

        // Draw dot product scale
        sf::RectangleShape dotProductScale(sf::Vector2f(200.f, 20.f));
        dotProductScale.setPosition(700.f, 80.f);

        // Create gradient for dot product scale
        sf::VertexArray dotProductGradient(sf::Quads, 4);
        dotProductGradient[0].position = sf::Vector2f(700.f, 80.f);
        dotProductGradient[1].position = sf::Vector2f(900.f, 80.f);
        dotProductGradient[2].position = sf::Vector2f(900.f, 100.f);
        dotProductGradient[3].position = sf::Vector2f(700.f, 100.f);

        dotProductGradient[0].color = sf::Color::Red;   // -1
        dotProductGradient[1].color = sf::Color::Green; // +1
        dotProductGradient[2].color = sf::Color::Green; // +1
        dotProductGradient[3].color = sf::Color::Red;   // -1

        window.draw(dotProductGradient);

        // Draw marker for current dot product value
        sf::RectangleShape dotMarker(sf::Vector2f(5.f, 30.f));
        dotMarker.setFillColor(sf::Color::White);
        dotMarker.setOrigin(2.5f, 15.f);
        // Map dot from [-1,1] to [700,900] (position on scale)
        float markerX = 700.f + (dot + 1.f) * 100.f;
        dotMarker.setPosition(markerX, 90.f);
        window.draw(dotMarker);

        window.draw(dotProductIndicator);
    }
};
